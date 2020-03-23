//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Yangen/YangenManager.h"

#include "OgreHlmsCompute.h"
#include "OgreHlmsComputeJob.h"
#include "OgreHlmsManager.h"
#include "OgreRoot.h"
#include "OgreTextureGpuManager.h"

#include "Vao/OgreConstBufferPacked.h"
#include "Vao/OgreVaoManager.h"

#include "Compositor/OgreCompositorManager2.h"
#include "Compositor/OgreCompositorWorkspace.h"

namespace Ogre
{
	struct HeightmapToNormalParams
	{
		uint32 heightMapResolution[2];
		uint32 heightMapArrayIdx;
		float depthScale;
	};

	YangenManager::YangenManager( const String &texName, HlmsManager *hlmsManager,
								  CompositorManager2 *compositorManager,
								  TextureGpuManager *textureGpuManager, SceneManager *sceneManager ) :
		m_texName( texName ),
		m_waitableTexture( 0 ),
		m_heightMap( 0 ),
		m_normalMap( 0 ),
		m_workspace( 0 ),
		m_compositorManager( compositorManager ),
		m_textureGpuManager( textureGpuManager ),
		m_sceneManager( sceneManager ),
		m_dummyCamera( 0 )
	{
		m_heightMapToNormalMapDepthScale[0] = 0.5f;
		m_heightMapToNormalMapDepthScale[1] = 20.0f;
		m_heightMapToNormalMapDepthScale[2] = 0.0f;

		const size_t numParams =
			sizeof( m_heightmapToNormalParams ) / sizeof( m_heightmapToNormalParams[0] );
		for( size_t i = 0u; i < numParams; ++i )
			m_heightmapToNormalParams[i] = 0;

		m_heightmapToNormalJob =
			hlmsManager->getComputeHlms()->findComputeJob( "Yangen/HeightToNormalMap" );
	};
	//-------------------------------------------------------------------------
	YangenManager::~YangenManager() { unloadTextures(); }
	//-------------------------------------------------------------------------
	void YangenManager::unloadTextures()
	{
		unloadGenerationResources();

		if( m_heightMap )
		{
			m_textureGpuManager->destroyTexture( m_heightMap );
			m_heightMap = 0;
		}

		if( m_normalMap )
		{
			m_textureGpuManager->destroyTexture( m_normalMap );
			m_normalMap = 0;
		}

		m_waitableTexture = 0;
	}
	//-------------------------------------------------------------------------
	void YangenManager::loadFromHeightmap( const String &filename, const String &resourceGroup )
	{
		m_heightMap = m_textureGpuManager->createOrRetrieveTexture(
			filename, m_texName + "/Heightmap", GpuPageOutStrategy::Discard,
			CommonTextureTypes::Monochrome, resourceGroup );
		m_heightMap->scheduleTransitionTo( GpuResidency::Resident );

		m_normalMap = m_textureGpuManager->createOrRetrieveTexture(
			m_texName + "/Normalmap", GpuPageOutStrategy::Discard, TextureFlags::Uav,
			TextureTypes::Type2DArray, resourceGroup );

		m_waitableTexture = m_heightMap;

		m_heightMap->waitForMetadata();

		m_normalMap->copyParametersFrom( m_heightMap );
		m_normalMap->setPixelFormat( PFG_RGBA8_SNORM );
		m_normalMap->setNumMipmaps( 1u );

		m_normalMap->scheduleTransitionTo( GpuResidency::Resident );
	}
	//-------------------------------------------------------------------------
	void YangenManager::loadGenerationResources()
	{
		unloadGenerationResources();

		VaoManager *vaoManager = m_textureGpuManager->getVaoManager();
		const size_t numParams =
			sizeof( m_heightmapToNormalParams ) / sizeof( m_heightmapToNormalParams[0] );
		for( size_t i = 0u; i < numParams; ++i )
		{
			m_heightmapToNormalParams[i] =
				vaoManager->createConstBuffer( sizeof( HeightmapToNormalParams ), BT_DEFAULT, 0, false );
		}

		// Tell Ogre it's a shadow casting camera to avoid penalizing performance
		m_dummyCamera = m_sceneManager->createCamera( m_texName + "/DummyCam", false );

		CompositorChannelVec channels;
		channels.reserve( 2u );

		channels.push_back( m_heightMap );
		channels.push_back( m_normalMap );

		m_workspace = m_compositorManager->addWorkspace( m_sceneManager, channels, m_dummyCamera,
														 "Yangen/Gen", false );
		m_workspace->addListener( this );
	}
	//-------------------------------------------------------------------------
	void YangenManager::unloadGenerationResources()
	{
		if( m_dummyCamera )
		{
			m_sceneManager->destroyCamera( m_dummyCamera );
			m_dummyCamera = 0;
		}

		if( m_workspace )
		{
			m_compositorManager->removeWorkspace( m_workspace );
			m_workspace = 0;
		}
	}
	//-------------------------------------------------------------------------
	/**
	@brief YangenManager::uploadParams
		Uploads all C++ params that the GPU shaders need to be aware of in VRAM
	*/
	void YangenManager::uploadParams()
	{
		const size_t numParams =
			sizeof( m_heightmapToNormalParams ) / sizeof( m_heightmapToNormalParams[0] );
		for( size_t i = 0u; i < numParams; ++i )
		{
			HeightmapToNormalParams heightmapToNormalParams;
			heightmapToNormalParams.heightMapResolution[0] = m_heightMap->getWidth();
			heightmapToNormalParams.heightMapResolution[1] = m_heightMap->getHeight();
			heightmapToNormalParams.heightMapArrayIdx = m_heightMap->getInternalSliceStart();
			heightmapToNormalParams.depthScale = m_heightMapToNormalMapDepthScale[i];

			m_heightmapToNormalParams[i]->upload( &heightmapToNormalParams, 0u,
												  sizeof( heightmapToNormalParams ) );
		}
		m_heightmapToNormalJob->setConstBuffer( 0u, m_heightmapToNormalParams[0] );

		HlmsCompute *hlmsCompute = static_cast<HlmsCompute *>( m_heightmapToNormalJob->getCreator() );
		HlmsComputeJob *gaussJob = 0;

		gaussJob = hlmsCompute->findComputeJob( "Yangen/GaussianBlurH" );
		setGaussianFilterParams( gaussJob, 8u, 0.5f );

		gaussJob = hlmsCompute->findComputeJob( "Yangen/GaussianBlurV" );
		setGaussianFilterParams( gaussJob, 8u, 0.5f );
	}
	//-------------------------------------------------------------------------
	void YangenManager::setGaussianFilterParams( Ogre::HlmsComputeJob *job, Ogre::uint8 kernelRadius,
												 float gaussianDeviationFactor )
	{
		assert( !( kernelRadius & 0x01 ) && "kernelRadius must be even!" );

		if( job->getProperty( "kernel_radius" ) != kernelRadius )
			job->setProperty( "kernel_radius", kernelRadius );
		ShaderParams &shaderParams = job->getShaderParams( "default" );

		std::vector<float> weights( kernelRadius + 1u );

		const float fKernelRadius = kernelRadius;
		const float gaussianDeviation = fKernelRadius * gaussianDeviationFactor;

		// It's 2.0f if using the approximate filter (sampling between two pixels to
		// get the bilinear interpolated result and cut the number of samples in half)
		const float stepSize = 1.0f;

		// Calculate the weights
		float fWeightSum = 0;
		for( uint32 i = 0; i < kernelRadius + 1u; ++i )
		{
			const float _X = i - fKernelRadius + ( 1.0f - 1.0f / stepSize );
			float fWeight = 1.0f / sqrt( 2.0f * Math::PI * gaussianDeviation * gaussianDeviation );
			fWeight *= exp( -( _X * _X ) / ( 2.0f * gaussianDeviation * gaussianDeviation ) );

			fWeightSum += fWeight;
			weights[i] = fWeight;
		}

		fWeightSum = fWeightSum * 2.0f - weights[kernelRadius];

		// Normalize the weights
		for( uint32 i = 0; i < kernelRadius + 1u; ++i )
			weights[i] /= fWeightSum;

		// Remove shader constants from previous calls (needed in case we've reduced the radius size)
		ShaderParams::ParamVec::iterator itor = shaderParams.mParams.begin();
		ShaderParams::ParamVec::iterator end = shaderParams.mParams.end();

		while( itor != end )
		{
			String::size_type pos = itor->name.find( "c_weights[" );

			if( pos != String::npos )
			{
				itor = shaderParams.mParams.erase( itor );
				end = shaderParams.mParams.end();
			}
			else
			{
				++itor;
			}
		}

		// Set the shader constants, 16 at a time (since that's the limit of what ManualParam can hold)
		char tmp[32];
		LwString weightsString( LwString::FromEmptyPointer( tmp, sizeof( tmp ) ) );
		const uint32 floatsPerParam = sizeof( ShaderParams::ManualParam().dataBytes ) / sizeof( float );
		for( uint32 i = 0; i < kernelRadius + 1u; i += floatsPerParam )
		{
			weightsString.clear();
			weightsString.a( "c_weights[", i, "]" );

			ShaderParams::Param p;
			p.isAutomatic = false;
			p.isDirty = true;
			p.name = weightsString.c_str();
			shaderParams.mParams.push_back( p );
			ShaderParams::Param *param = &shaderParams.mParams.back();

			param->setManualValue(
				&weights[i],
				std::min<uint32>( floatsPerParam, static_cast<uint32>( weights.size() - i ) ) );
		}

		shaderParams.setDirty();
	}
	//-------------------------------------------------------------------------
	void YangenManager::process()
	{
		if( !m_workspace )
			loadGenerationResources();
		uploadParams();

		m_waitableTexture->waitForData();

		m_workspace->_beginUpdate( false );
		m_workspace->_update();
		m_workspace->_endUpdate( false );
	}
	//-------------------------------------------------------------------------
	void YangenManager::setHeightMapToNormalMapDepthScale( float depth, uint8 detailIdx )
	{
		assert( detailIdx < 3u );
		m_heightMapToNormalMapDepthScale[detailIdx] = depth;
	}
	//-------------------------------------------------------------------------
	void YangenManager::passPreExecute( CompositorPass *pass )
	{
		const uint32 identifier = pass->getDefinition()->mIdentifier;
		if( identifier >= 10u && identifier <= 11u )
			m_heightmapToNormalJob->setConstBuffer( 0u, m_heightmapToNormalParams[identifier - 10u] );
	}
}  // namespace Ogre

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

	struct DiffuseToRoughnessParams
	{
		uint32 diffuseMapResolution[2];
		uint32 diffuseMapArrayIdx;
		uint32 padding0;

		float midPoint;
		float scale;
		float exponent;
		float padding1;
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
		m_dummyCamera( 0 ),
		m_heightmapToNormalJob( 0 ),
		m_mergeNormalMapsJob( 0 ),
		m_mergeNormalMapsParams( 0 ),
		m_mergeNormalMapsSteepness( 0 ),
		m_diffusemapToRoughnessParams( 0 ),
		m_diffusemapToRoughnessJob( 0 ),
		m_diffuseToRoughnessMidpoint( 0.5f ),
		m_diffuseToRoughnessScale( 0.5f ),
		m_diffuseToRoughnessExponent( 1.0f ),
		m_roughnessBlurOffset( 0u ),
		m_roughnessBlurAmplitude( 2u ),
		m_defaultGenerateAllConstantDefinitionArrayEntries( false )
	{
		m_heightMapToNormalMapStrength[0] = 0.5f;
		m_heightMapToNormalMapStrength[1] = 20.0f;
		m_heightMapToNormalMapStrength[2] = 0.0f;

		m_heightMapToNormalMapBlurRadius[0] = 8u;
		m_heightMapToNormalMapBlurRadius[1] = 32u;

		const size_t numSteepnessParams =
			sizeof( m_normalMapSteepness ) / sizeof( m_normalMapSteepness[0] );
		for( size_t i = 0u; i < numSteepnessParams; ++i )
			m_normalMapSteepness[i] = 0.0f;

		const size_t numParams =
			sizeof( m_heightmapToNormalParams ) / sizeof( m_heightmapToNormalParams[0] );
		for( size_t i = 0u; i < numParams; ++i )
			m_heightmapToNormalParams[i] = 0;

		m_heightmapToNormalJob =
			hlmsManager->getComputeHlms()->findComputeJob( "Yangen/HeightToNormalMap" );
		m_diffusemapToRoughnessJob =
			hlmsManager->getComputeHlms()->findComputeJob( "Yangen/DiffuseToRoughness" );

		m_mergeNormalMapsJob = hlmsManager->getComputeHlms()->findComputeJob( "Yangen/MergeNormalMaps" );

		m_mergeNormalMapsParams = &m_mergeNormalMapsJob->getShaderParams( "default" );
		m_mergeNormalMapsSteepness = m_mergeNormalMapsParams->findParameter( "steepness" );
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

		if( m_roughnessMap )
		{
			m_textureGpuManager->destroyTexture( m_roughnessMap );
			m_roughnessMap = 0;
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

		m_roughnessMap = m_textureGpuManager->createOrRetrieveTexture(
			m_texName + "/Roughness", GpuPageOutStrategy::Discard, TextureFlags::Uav,
			TextureTypes::Type2DArray, resourceGroup );

		m_waitableTexture = m_heightMap;

		m_heightMap->waitForMetadata();

		m_normalMap->copyParametersFrom( m_heightMap );
		m_normalMap->setTextureType( TextureTypes::Type2DArray );
		m_normalMap->setPixelFormat( PFG_RGBA8_SNORM );
		m_normalMap->setNumMipmaps( 1u );

		m_roughnessMap->copyParametersFrom( m_normalMap );
		m_roughnessMap->setPixelFormat( PFG_R8_UNORM );

		m_normalMap->scheduleTransitionTo( GpuResidency::Resident );
		m_roughnessMap->scheduleTransitionTo( GpuResidency::Resident );
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

		m_diffusemapToRoughnessParams =
			vaoManager->createConstBuffer( sizeof( DiffuseToRoughnessParams ), BT_DEFAULT, 0, false );

		// Tell Ogre it's a shadow casting camera to avoid penalizing performance
		m_dummyCamera = m_sceneManager->createCamera( m_texName + "/DummyCam", false );

		CompositorChannelVec channels;
		channels.reserve( 3u );

		channels.push_back( m_heightMap );
		channels.push_back( m_normalMap );
		channels.push_back( m_roughnessMap );

		m_workspace = m_compositorManager->addWorkspace( m_sceneManager, channels, m_dummyCamera,
														 "Yangen/Gen", false );
		m_workspace->addListener( this );
	}
	//-------------------------------------------------------------------------
	void YangenManager::unloadGenerationResources()
	{
		VaoManager *vaoManager = m_textureGpuManager->getVaoManager();

		const size_t numParams =
			sizeof( m_heightmapToNormalParams ) / sizeof( m_heightmapToNormalParams[0] );
		for( size_t i = 0u; i < numParams; ++i )
		{
			if( m_heightmapToNormalParams[i] )
			{
				vaoManager->destroyConstBuffer( m_heightmapToNormalParams[i] );
				m_heightmapToNormalParams[i] = 0;
			}
		}

		if( m_diffusemapToRoughnessParams )
		{
			vaoManager->destroyConstBuffer( m_diffusemapToRoughnessParams );
			m_diffusemapToRoughnessParams = 0;
		}

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
			heightmapToNormalParams.depthScale = m_heightMapToNormalMapStrength[i];

			if( i > 0u )
				heightmapToNormalParams.depthScale *= m_heightMapToNormalMapBlurRadius[i - 1u];

			m_heightmapToNormalParams[i]->upload( &heightmapToNormalParams, 0u,
												  sizeof( heightmapToNormalParams ) );
		}
		m_heightmapToNormalJob->setConstBuffer( 0u, m_heightmapToNormalParams[0] );

		const size_t numSteepnessParams =
			sizeof( m_normalMapSteepness ) / sizeof( m_normalMapSteepness[0] );
		float steepnessVal[numSteepnessParams];
		for( size_t i = 0u; i < numSteepnessParams; ++i )
		{
			if( m_normalMapSteepness[i] >= 0.0f )
				steepnessVal[i] = m_normalMapSteepness[i] + 1.0f;
			else
				steepnessVal[i] = -1.0f / ( m_normalMapSteepness[i] - 1.0f );
		}

		m_mergeNormalMapsSteepness->setManualValue( steepnessVal, numSteepnessParams );
		m_mergeNormalMapsParams->setDirty();

		DiffuseToRoughnessParams diffuseToRoughnessParams;
		diffuseToRoughnessParams.diffuseMapResolution[0] = m_heightMap->getWidth();
		diffuseToRoughnessParams.diffuseMapResolution[1] = m_heightMap->getHeight();
		diffuseToRoughnessParams.diffuseMapArrayIdx = m_heightMap->getInternalSliceStart();
		diffuseToRoughnessParams.padding0 = 0u;

		diffuseToRoughnessParams.midPoint = m_diffuseToRoughnessMidpoint;
		diffuseToRoughnessParams.scale = m_diffuseToRoughnessScale;
		diffuseToRoughnessParams.exponent = m_diffuseToRoughnessExponent;
		diffuseToRoughnessParams.padding1 = 0.0f;
		m_diffusemapToRoughnessParams->upload( &diffuseToRoughnessParams, 0u,
											   sizeof( diffuseToRoughnessParams ) );
		m_diffusemapToRoughnessJob->setConstBuffer( 0u, m_diffusemapToRoughnessParams );
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
		ShaderParams::ParamVec::iterator endt = shaderParams.mParams.end();

		while( itor != endt )
		{
			String::size_type pos = itor->name.find( "c_weights[" );

			if( pos != String::npos )
			{
				itor = shaderParams.mParams.erase( itor );
				endt = shaderParams.mParams.end();
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

		m_defaultGenerateAllConstantDefinitionArrayEntries =
			Ogre::GpuNamedConstants::getGenerateAllConstantDefinitionArrayEntries();

		m_workspace->_beginUpdate( false );
		m_workspace->_update();
		m_workspace->_endUpdate( false );
	}
	//-------------------------------------------------------------------------
	void YangenManager::setHeightMapToNormalMapStrength( float strength, uint8 detailIdx )
	{
		assert( detailIdx < 3u );
		m_heightMapToNormalMapStrength[detailIdx] = strength;
	}
	//-------------------------------------------------------------------------
	void YangenManager::setHeightMapToNormalMapRadius( uint8 radius, uint8 detailIdx )
	{
		assert( detailIdx > 0u && detailIdx < 3u );
		assert( radius > 1u && radius <= 65u );
		assert( !( radius & 0x01u ) && "Radius must be even!" );
		m_heightMapToNormalMapBlurRadius[detailIdx - 1u] = radius;
	}
	//-------------------------------------------------------------------------
	void YangenManager::setNormalMapSteepness( float steepness, uint8 detailIdx )
	{
		assert( detailIdx < 3u );
		m_normalMapSteepness[detailIdx] = steepness;
	}
	//-------------------------------------------------------------------------
	void YangenManager::setRoughnessMidpoint( float midpoint )
	{
		m_diffuseToRoughnessMidpoint = midpoint;
	}
	//-------------------------------------------------------------------------
	float YangenManager::getRoughnessMidpoint() const { return m_diffuseToRoughnessMidpoint; }
	//-------------------------------------------------------------------------
	void YangenManager::setRoughnessScale( float scale ) { m_diffuseToRoughnessScale = scale; }
	//-------------------------------------------------------------------------
	float YangenManager::getRoughnessScale() const { return m_diffuseToRoughnessScale; }
	//-------------------------------------------------------------------------
	void YangenManager::setRoughnessExponent( float exponent )
	{
		m_diffuseToRoughnessExponent = exponent;
	}
	//-------------------------------------------------------------------------
	float YangenManager::getRoughnessExponent() const { return m_diffuseToRoughnessExponent; }
	//-------------------------------------------------------------------------
	void YangenManager::setRoughnessBlurOffset( uint8 offset ) { m_roughnessBlurOffset = offset; }
	//-------------------------------------------------------------------------
	uint8 YangenManager::getRoughnessBlurOffset() const { return m_roughnessBlurOffset; }
	//-------------------------------------------------------------------------
	void YangenManager::setRoughnessBlurAmplitude( uint8 amplitude )
	{
		m_roughnessBlurAmplitude = amplitude;
	}
	//-------------------------------------------------------------------------
	uint8 YangenManager::getRoughnessBlurAmplitude() const { return m_roughnessBlurAmplitude; }
	//-------------------------------------------------------------------------
	void YangenManager::passPreExecute( CompositorPass *pass )
	{
		const uint32 identifier = pass->getDefinition()->mIdentifier;
		if( identifier >= 10u && identifier <= 12u )
			m_heightmapToNormalJob->setConstBuffer( 0u, m_heightmapToNormalParams[identifier - 10u] );
		else if( identifier >= 21u && identifier <= 22u )
		{
			HlmsCompute *hlmsCompute =
				static_cast<HlmsCompute *>( m_heightmapToNormalJob->getCreator() );
			HlmsComputeJob *gaussJob = 0;

			// We need this set so that large filter radiuses actually work
			Ogre::GpuNamedConstants::setGenerateAllConstantDefinitionArrayEntries( true );

			gaussJob = hlmsCompute->findComputeJob( "Yangen/GaussianBlurH" );
			setGaussianFilterParams( gaussJob, m_heightMapToNormalMapBlurRadius[identifier - 21u],
									 0.5f );

			gaussJob = hlmsCompute->findComputeJob( "Yangen/GaussianBlurV" );
			setGaussianFilterParams( gaussJob, m_heightMapToNormalMapBlurRadius[identifier - 21u],
									 0.5f );
		}
		else if( identifier >= 121u && identifier <= 122u )
		{
			HlmsCompute *hlmsCompute =
				static_cast<HlmsCompute *>( m_heightmapToNormalJob->getCreator() );
			HlmsComputeJob *gaussJob = 0;

			uint8 blurRadius = 0u;
			if( identifier == 121u )
				blurRadius = m_roughnessBlurOffset;
			else
				blurRadius = m_roughnessBlurOffset + m_roughnessBlurAmplitude;

			// We need this set so that large filter radiuses actually work
			Ogre::GpuNamedConstants::setGenerateAllConstantDefinitionArrayEntries( true );

			gaussJob = hlmsCompute->findComputeJob( "Yangen/GaussianBlurH" );
			setGaussianFilterParams( gaussJob, blurRadius, 0.5f );

			gaussJob = hlmsCompute->findComputeJob( "Yangen/GaussianBlurV" );
			setGaussianFilterParams( gaussJob, blurRadius, 0.5f );
		}
	}
	//-------------------------------------------------------------------------
	void YangenManager::passPosExecute( CompositorPass *pass )
	{
		const uint32 identifier = pass->getDefinition()->mIdentifier;
		if( identifier == 32u )
		{
			// Restore this setting after the compute shaders have been generated,
			// since it affects all shaders globally
			Ogre::GpuNamedConstants::setGenerateAllConstantDefinitionArrayEntries(
				m_defaultGenerateAllConstantDefinitionArrayEntries );
		}
	}
}  // namespace Ogre

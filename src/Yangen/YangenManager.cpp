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
		m_dummyCamera( 0 ),
		m_heightmapToNormalParams( 0 ),
		m_heightMapToNormalMapDepthScale( 20.0f )
	{
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
		m_heightmapToNormalParams =
			vaoManager->createConstBuffer( sizeof( HeightmapToNormalParams ), BT_DEFAULT, 0, false );

		// Tell Ogre it's a shadow casting camera to avoid penalizing performance
		m_dummyCamera = m_sceneManager->createCamera( m_texName + "/DummyCam", false );

		CompositorChannelVec channels;
		channels.reserve( 2u );

		channels.push_back( m_heightMap );
		channels.push_back( m_normalMap );

		m_workspace = m_compositorManager->addWorkspace( m_sceneManager, channels, m_dummyCamera,
														 "Yangen/Gen", false );
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
		HeightmapToNormalParams heightmapToNormalParams;
		heightmapToNormalParams.heightMapResolution[0] = m_heightMap->getWidth();
		heightmapToNormalParams.heightMapResolution[1] = m_heightMap->getHeight();
		heightmapToNormalParams.heightMapArrayIdx = m_heightMap->getInternalSliceStart();
		heightmapToNormalParams.depthScale = m_heightMapToNormalMapDepthScale;

		m_heightmapToNormalParams->upload( &heightmapToNormalParams, 0u,
										   sizeof( heightmapToNormalParams ) );

		m_heightmapToNormalJob->setConstBuffer( 0u, m_heightmapToNormalParams );
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
	void YangenManager::setHeightMapToNormalMapDepthScale( float depth )
	{
		m_heightMapToNormalMapDepthScale = depth;
	}
}  // namespace Ogre

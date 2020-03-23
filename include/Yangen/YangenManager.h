//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

#include "OgrePrerequisites.h"

#include "Compositor/OgreCompositorWorkspaceListener.h"

#include "OgreTextureGpu.h"

namespace Ogre
{
	class YangenManager : public CompositorWorkspaceListener
	{
		String m_texName;

		TextureGpu *m_waitableTexture;

		TextureGpu *m_heightMap;
		TextureGpu *m_normalMap;

		CompositorWorkspace *m_workspace;

		CompositorManager2 *m_compositorManager;
		TextureGpuManager * m_textureGpuManager;
		SceneManager *      m_sceneManager;

		Camera *m_dummyCamera;

		ConstBufferPacked *m_heightmapToNormalParams[3];
		HlmsComputeJob *   m_heightmapToNormalJob;

		float m_heightMapToNormalMapDepthScale[3];

		/// Unloads everything. Implies calling YangenManager::YangenManagerunloadGenerationResources
		void unloadTextures();

		/// Creates all resources which are only needed to generate the textures.
		void loadGenerationResources();

		/// Unloads all resources which are only needed to generate the textures.
		/// Useful if you want to save memory while keeping the generated textures
		void unloadGenerationResources();

		void uploadParams();

		static void setGaussianFilterParams( Ogre::HlmsComputeJob *job, Ogre::uint8 kernelRadius,
											 float gaussianDeviationFactor );

	public:
		YangenManager( const String &texName, HlmsManager *hlmsManager,
					   CompositorManager2 *compositorManager, TextureGpuManager *textureGpuManager,
					   SceneManager *sceneManager );
		virtual ~YangenManager();

		void loadFromHeightmap( const String &filename, const String &resourceGroup );

		void process();

		void setHeightMapToNormalMapDepthScale( float depth, uint8 detailIdx );

		virtual void passPreExecute( CompositorPass *pass );

		TextureGpu *getNormalMap() const { return m_normalMap; }
	};
}  // namespace Ogre

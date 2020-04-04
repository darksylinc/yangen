//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

#include "OgrePrerequisites.h"

#include "Compositor/OgreCompositorWorkspaceListener.h"
#include "OgreShaderParams.h"

#include "OgreTextureGpu.h"

namespace Ogre
{
	class YangenManager : public CompositorWorkspaceListener
	{
		String m_texName;

		TextureGpu *m_waitableTexture;

		TextureGpu *m_heightMap;
		TextureGpu *m_normalMap;
		TextureGpu *m_roughnessMap;

		CompositorWorkspace *m_workspace;

		CompositorManager2 *m_compositorManager;
		TextureGpuManager * m_textureGpuManager;
		SceneManager *      m_sceneManager;

		Camera *m_dummyCamera;

		ConstBufferPacked *  m_heightmapToNormalParams[3];
		HlmsComputeJob *     m_heightmapToNormalJob;
		HlmsComputeJob *     m_mergeNormalMapsJob;
		ShaderParams *       m_mergeNormalMapsParams;
		ShaderParams::Param *m_mergeNormalMapsSteepness;

		ConstBufferPacked *m_diffusemapToRoughnessParams;
		HlmsComputeJob *   m_diffusemapToRoughnessJob;

		float m_heightMapToNormalMapStrength[3];
		uint8 m_heightMapToNormalMapBlurRadius[2];
		float m_normalMapSteepness[3];

		float m_diffuseToRoughnessMidpoint;
		float m_diffuseToRoughnessScale;
		float m_diffuseToRoughnessExponent;

		uint8 m_roughnessBlurOffset;
		uint8 m_roughnessBlurAmplitude;

		bool m_defaultGenerateAllConstantDefinitionArrayEntries;

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

		void setHeightMapToNormalMapStrength( float strength, uint8 detailIdx );

		/** Sets the radius of the gaussian blur we apply to each detail

			With a high radius, the broad details are considered i.e. large changes in height
			With a low radius, very fine details are considered, i.e. small variations in height
			are turned into normal map
		@param radius
			Value in range [2; 64] must be even
		@param detailIdx
			Value in range [1; 3)
		*/
		void setHeightMapToNormalMapRadius( uint8 radius, uint8 detailIdx );

		/** Sets the steepness of each detail.
			A positive steepness means the normal is bent away and usually
			works better for broad details.

			A negative steepness means the normal makes the normals noisy and usually works
			better for fine details.
		@param steepness
			Value in range (-inf; inf)
			Default: 0.0 (disabled)
		@param detailIdx
			Value in range [0; 3)
		 */
		void setNormalMapSteepness( float steepness, uint8 detailIdx );

		virtual void passPreExecute( CompositorPass *pass );
		virtual void passPosExecute( CompositorPass *pass );

		TextureGpu *getDiffuseMap() const { return m_heightMap; }
		TextureGpu *getHeightMap() const { return m_heightMap; }
		TextureGpu *getNormalMap() const { return m_normalMap; }
		TextureGpu *getRoughnessMap() const { return m_roughnessMap; }
	};
}  // namespace Ogre

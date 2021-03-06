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
	public:
		struct Preset
		{
			float nmStrength[3];
			float nmSteepness[3];
			uint8 nmRadius[2];

			uint8 roughnessBlurOffset;
			uint8 roughnessBlurAmplitude;
			float roughnessMidpoint;
			float roughnessScale;
			float roughnessExponent;
		};

	protected:
		String m_texName;

		TextureGpu *m_waitableTexture;

		/** There are three possiblities:
				1. m_diffuseMap was provided.
					a. We generate m_greyscaleMap out of the diffuse map
					b. m_heightMap = m_greyscaleMap (same pointer)
				2. m_heightMap was provided.
					a. There's no diffuse.
					b. m_diffuseMap = nullptr
					c. m_heightMap = m_greyscaleMap (same pointer)
				3. m_diffuseMap & m_heightMap were provided
					a. We generate m_greyscaleMap out of the diffuse map
					b. None of the 3 variables are nullptr, they're all different memory regions
		*/
		TextureGpu *m_diffuseMap;
		TextureGpu *m_greyscaleMap;
		TextureGpu *m_heightMap;
		TextureGpu *m_normalMap;
		TextureGpu *m_roughnessMap;

		CompositorWorkspace *m_workspace;
		CompositorWorkspace *m_greyscaleWorkspace;

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

		float m_normalMapStrength[3];
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

		/** D3D11 can take a long time to compile the Gaussian Blur's compute shader.
			This makes the UI/UX horribly slow when the user changes the blur slider since
			a new shader needs to be recompiled

			This function populates the shader cache with all possible variants of our
			gaussian blur so that compilation happens at startup time, not when
			the user moves the slider

			Ogre::GpuProgramManager::getSingleton().setSaveMicrocodesToCache must be set
			to true for this to work

			Saving the microcode cache to disk speeds up this enormously.
		@param maxBlurRadius
			Maximum blur radius to cache, inclusive.
			The bigger this value, the longer it will take
		*/
		void populateShaderCache( uint8 maxBlurRadius );

		/** Loads from a diffuse. Generates the heightmap from the diffuse map
		@param filename
		@param resourceGroup
		*/
		void loadFromDiffusemap( const String &filename, const String &resourceGroup );

		/** Loads from a heightmap.
			Assumes there is no diffuse map, and the heightmap
			is also used to create the roughness map
		@param filename
		@param resourceGroup
		*/
		void loadFromHeightmap( const String &filename, const String &resourceGroup );

		/** Loads from a diffuse and heightmap.
			Roughness is calculated from diffuse. Normals from heightmap
		@param diffuseName
		@param diffuseResourceGroup
		@param heightName
		@param heightResourceGroup
		*/
		void loadFromDiffuseAndHeightmap( const String &diffuseName, const String &diffuseResourceGroup,
										  const String &heightName, const String &heightResourceGroup );

		void process();

		void  setNormalMapStrength( float strength, uint8 detailIdx );
		float getNormalMapStrength( uint8 detailIdx ) const;

		/** Sets the radius of the gaussian blur we apply to each detail

			With a high radius, the broad details are considered i.e. large changes in height
			With a low radius, very fine details are considered, i.e. small variations in height
			are turned into normal map
		@param radius
			Value in range [2; 64] must be even
		@param detailIdx
			Value in range [1; 3)
		*/
		void  setHeightMapToNormalMapRadius( uint8 radius, uint8 detailIdx );
		uint8 getHeightMapToNormalMapRadius( uint8 detailIdx ) const;

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
		void  setNormalMapSteepness( float steepness, uint8 detailIdx );
		float getNormalMapSteepness( uint8 detailIdx ) const;

		void  setRoughnessMidpoint( float midpoint );
		float getRoughnessMidpoint() const;

		void  setRoughnessScale( float scale );
		float getRoughnessScale() const;

		void  setRoughnessExponent( float exponent );
		float getRoughnessExponent() const;

		void  setRoughnessBlurOffset( uint8 offset );
		uint8 getRoughnessBlurOffset() const;

		void  setRoughnessBlurAmplitude( uint8 amplitude );
		uint8 getRoughnessBlurAmplitude() const;

		void setPreset( const Preset &preset );

		bool isHeightmapAutogenerated() const;

		virtual void passPreExecute( CompositorPass *pass );
		virtual void passPosExecute( CompositorPass *pass );

		TextureGpu *getDiffuseMap() const { return m_diffuseMap; }
		TextureGpu *getHeightMap() const { return m_heightMap; }
		TextureGpu *getNormalMap() const { return m_normalMap; }
		TextureGpu *getRoughnessMap() const { return m_roughnessMap; }
	};
}  // namespace Ogre

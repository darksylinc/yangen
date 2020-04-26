//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Yangen/MaterialSwitcher.h"

#include "Yangen/YangenManager.h"

#include "OgreHlmsManager.h"

#include "OgreHlmsPbs.h"
#include "OgreHlmsPbsDatablock.h"

#include "OgreHlmsUnlit.h"
#include "OgreHlmsUnlitDatablock.h"
#include "OgreTextureBox.h"
#include "OgreTextureGpuManager.h"

namespace Ogre
{
	MaterialSwitcher::MaterialSwitcher( HlmsManager *hlmsManager, TextureGpuManager *textureGpuManager,
										YangenManager *yangenManager ) :
		m_yangenManager( yangenManager ),
		m_grey50( 0 )
	{
		{
			m_grey50 = textureGpuManager->createTexture( "Yangen/Grey50", GpuPageOutStrategy::Discard,
														 TextureFlags::ManualTexture,
														 TextureTypes::Type2DArray );
			m_grey50->setResolution( 1u, 1u );
			m_grey50->setPixelFormat( PFG_RGBA8_UNORM );
			m_grey50->scheduleTransitionTo( GpuResidency::Resident );

			Ogre::Image2 greyImg;
			greyImg.createEmptyImageLike( m_grey50 );
			greyImg.getData( 0u ).setColourAt( ColourValue( 0.5f, 0.5f, 0.5f ), 0u, 0u, 0u,
											   greyImg.getPixelFormat() );
			greyImg.uploadTo( m_grey50, 0u, 0u );
		}

		assert( dynamic_cast<Ogre::HlmsPbs *>( hlmsManager->getHlms( Ogre::HLMS_PBS ) ) );
		Ogre::HlmsPbs *hlmsPbs = static_cast<Ogre::HlmsPbs *>( hlmsManager->getHlms( HLMS_PBS ) );

		assert( dynamic_cast<Ogre::HlmsUnlit *>( hlmsManager->getHlms( Ogre::HLMS_UNLIT ) ) );
		Ogre::HlmsUnlit *hlmsUnlit =
			static_cast<Ogre::HlmsUnlit *>( hlmsManager->getHlms( HLMS_UNLIT ) );

		int numMaterials = 0;

		Ogre::String baseName = "Yangen MaterialSwitcher/" +
								StringConverter::toString( Id::generateNewId<MaterialSwitcher>() );
		Ogre::String datablockName;

		datablockName = baseName + "/" + StringConverter::toString( numMaterials++ );
		Ogre::HlmsPbsDatablock *pbsDatablock = static_cast<Ogre::HlmsPbsDatablock *>(
			hlmsPbs->createDatablock( datablockName, datablockName, Ogre::HlmsMacroblock(),
									  Ogre::HlmsBlendblock(), Ogre::HlmsParamVec() ) );

		m_datablocks[YangenVisualizationModes::FinalRender] = pbsDatablock;

		Ogre::HlmsUnlitDatablock *unlitDatablock = 0;

		datablockName = baseName + "/" + StringConverter::toString( numMaterials++ );
		unlitDatablock = static_cast<Ogre::HlmsUnlitDatablock *>(
			hlmsUnlit->createDatablock( datablockName, datablockName, Ogre::HlmsMacroblock(),
										Ogre::HlmsBlendblock(), Ogre::HlmsParamVec() ) );
		m_datablocks[YangenVisualizationModes::Normal] = unlitDatablock;
		unlitDatablock->setTexture( 1u, m_grey50 );
		unlitDatablock->setBlendMode( 1u, UNLIT_BLEND_MULTIPLY );
		unlitDatablock->setTexture( 2u, m_grey50 );
		unlitDatablock->setBlendMode( 2u, UNLIT_BLEND_ADD );

		datablockName = baseName + "/" + StringConverter::toString( numMaterials++ );
		unlitDatablock = static_cast<Ogre::HlmsUnlitDatablock *>(
			hlmsUnlit->createDatablock( datablockName, datablockName, Ogre::HlmsMacroblock(),
										Ogre::HlmsBlendblock(), Ogre::HlmsParamVec() ) );
		m_datablocks[YangenVisualizationModes::Roughness] = unlitDatablock;
	}
	//-------------------------------------------------------------------------
	MaterialSwitcher::~MaterialSwitcher()
	{
		if( m_grey50 )
		{
			m_grey50->getTextureManager()->destroyTexture( m_grey50 );
			m_grey50 = 0;
		}
	}
	//-------------------------------------------------------------------------
	void MaterialSwitcher::prepareMaterials()
	{
		Ogre::HlmsPbsDatablock *pbsDatablock =
			static_cast<Ogre::HlmsPbsDatablock *>( m_datablocks[YangenVisualizationModes::FinalRender] );

		pbsDatablock->setTexture( PBSM_DIFFUSE, m_yangenManager->getDiffuseMap() );
		pbsDatablock->setTexture( PBSM_NORMAL, m_yangenManager->getNormalMap() );
		pbsDatablock->setTexture( PBSM_ROUGHNESS, m_yangenManager->getRoughnessMap() );

		Ogre::HlmsUnlitDatablock *unlitDatablock = 0;

		unlitDatablock =
			static_cast<Ogre::HlmsUnlitDatablock *>( m_datablocks[YangenVisualizationModes::Normal] );
		unlitDatablock->setTexture( 0, m_yangenManager->getNormalMap() );

		unlitDatablock =
			static_cast<Ogre::HlmsUnlitDatablock *>( m_datablocks[YangenVisualizationModes::Roughness] );
		unlitDatablock->setTexture( 0, m_yangenManager->getRoughnessMap() );
	}
	//-------------------------------------------------------------------------
	HlmsDatablock *MaterialSwitcher::getDatablock(
		YangenVisualizationModes::YangenVisualizationModes mode ) const
	{
		return m_datablocks[mode];
	}
	//-------------------------------------------------------------------------
	HlmsPbsDatablock *MaterialSwitcher::getFinalRenderDatablock() const
	{
		return static_cast<Ogre::HlmsPbsDatablock *>(
			m_datablocks[YangenVisualizationModes::FinalRender] );
	}
}  // namespace Ogre

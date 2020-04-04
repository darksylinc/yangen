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

namespace Ogre
{
	MaterialSwitcher::MaterialSwitcher( HlmsManager *hlmsManager, YangenManager *yangenManager ) :
		m_yangenManager( yangenManager )
	{
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

		datablockName = baseName + "/" + StringConverter::toString( numMaterials++ );
		unlitDatablock = static_cast<Ogre::HlmsUnlitDatablock *>(
			hlmsUnlit->createDatablock( datablockName, datablockName, Ogre::HlmsMacroblock(),
										Ogre::HlmsBlendblock(), Ogre::HlmsParamVec() ) );
		m_datablocks[YangenVisualizationModes::Roughness] = unlitDatablock;
	}
	//-------------------------------------------------------------------------
	MaterialSwitcher::~MaterialSwitcher() {}
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
}  // namespace Ogre

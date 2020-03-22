//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

#include "OgrePrerequisites.h"

namespace Ogre
{
	class YangenManager;

	namespace YangenVisualizationModes
	{
		enum YangenVisualizationModes
		{
			FinalRender,
			Normal,
			NumYangenVisualizationModes
		};
	}

	class MaterialSwitcher
	{
		HlmsDatablock *m_datablocks[YangenVisualizationModes::NumYangenVisualizationModes];

		YangenManager *m_yangenManager;

	public:
		MaterialSwitcher( HlmsManager *hlmsManager, YangenManager *yangenManager );
		~MaterialSwitcher();

		void prepareMaterials();

		HlmsDatablock *getDatablock( YangenVisualizationModes::YangenVisualizationModes mode ) const;
	};
}  // namespace Ogre

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
	class HlmsPbsDatablock;

	namespace YangenVisualizationModes
	{
		enum YangenVisualizationModes
		{
			FinalRender,
			Normal,
			Roughness,
			NumYangenVisualizationModes
		};
	}

	class MaterialSwitcher
	{
		HlmsDatablock *m_datablocks[YangenVisualizationModes::NumYangenVisualizationModes];

		YangenManager *m_yangenManager;

		/// 50% grey. For normal map display
		TextureGpu *m_grey50;

	public:
		MaterialSwitcher( HlmsManager *hlmsManager, TextureGpuManager *textureGpuManager,
						  YangenManager *yangenManager );
		~MaterialSwitcher();

		void prepareMaterials();

		HlmsDatablock *   getDatablock( YangenVisualizationModes::YangenVisualizationModes mode ) const;
		HlmsPbsDatablock *getFinalRenderDatablock() const;
	};
}  // namespace Ogre

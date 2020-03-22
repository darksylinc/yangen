//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

#include "LinkedWidgets.h"

#include <map>

class wxCommandEvent;
class wxScrollEvent;

/** @ingroup Helpers
@class LinkedWidgetsPanel
	Implements base class to link a text control to a slider.
	The text control can have arbitrary values outside the range of the slider, for maximum flexibility
*/
class LinkedWidgetsPanel
{
protected:
	std::map<wxSlider*,		ConversionBase*> m_sliderToTextCtrlMap;
	std::map<wxTextCtrl*,	ConversionBase*> m_textCtrlToSliderMap;

	void mapSliderAndTextCtrl( ConversionBase *converter );
	void mapSliderAndTextCtrl( ConversionBase *converter, float newDefault, bool bSetDefault=true );

	/// Must be called by derived class when receiving text ctrl events!
	void OnTextCtrl( wxCommandEvent& event );
	/// Must be called by derived class when receiving text ctrl events!
	void OnSliderScroll( wxScrollEvent& event );

	void refreshAllLinkedControls();

	virtual void valueUpdated( wxTextCtrl *textCtrl ) = 0;

public:
	static float getValueFrom( wxTextCtrl *textCtrl, float defaultValue );
};

/** @ingroup Helpers
@class LinkedWidgetsPairPanel
	Extends LinkedWidgetsPanel with two pairs of text/slider, where one acts as a minimum value
	and other as a maximum; thus we always ensure min <= max
*/
class LinkedWidgetsPairPanel : public LinkedWidgetsPanel
{
protected:
	std::map<wxSlider*,		ConvertPair*> m_pairSlidersToTextCtrls;
	std::map<wxTextCtrl*,	ConvertPair*> m_pairTextCtrlsToSliders;

	void mapSliderAndTextCtrl( ConvertPair *pair );
	void mapSliderAndTextCtrl( ConvertPair *pair, float minDefault,
							   float maxDefault, bool bSetDefault=true );

	/// Must be called by derived class when receiving text ctrl events!
	void OnTextCtrl( wxCommandEvent& event );
	/// Must be called by derived class when receiving text ctrl events!
	void OnSliderScroll( wxScrollEvent& event );

	void refreshAllLinkedControls();
};

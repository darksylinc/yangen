//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

/**
@file
Subclass of PcgPanel, which is generated by wxFormBuilder.
*/

#include "OgrePrerequisites.h"

#include "OgreVector2.h"

#include "../../src/Autogenerated/wxYangen.h"

#include "Helpers/LinkedWidgets.h"

#include <map>

//// end generated include

namespace Ogre
{
	class YangenManager;
}  // namespace Ogre

class YangenWindowImpl;
class ConfigReader;

/** Implementing PcgPanel */
class TexturePanelImpl : public TexturePanel
{
protected:
	bool mInitializing;

	std::map<wxSlider *, ConversionBase *>   m_sliderToTextCtrlMap;
	std::map<wxTextCtrl *, ConversionBase *> m_textCtrlToSliderMap;
	std::map<wxSlider *, ConvertPair *>      m_pairSlidersToTextCtrls;
	std::map<wxTextCtrl *, ConvertPair *>    m_pairTextCtrlsToSliders;

	Ogre::YangenManager *m_yangenManager;

	void mapSliderAndTextCtrl( ConversionBase *converter );
	void mapSliderAndTextCtrl( ConversionBase *converter, float newDefault, bool bSetDefault = true );
	void mapSliderAndTextCtrl( ConvertPair *pair );
	void mapSliderAndTextCtrl( ConvertPair *pair, float minDefault, float maxDefault,
							   bool bSetDefault = true );

	float getValueFrom( wxTextCtrl *textCtrl, float defaultValue ) const;

	// Handlers for PcgPanel events.
	void OnScroll( wxScrollEvent &event );
	void OnCheckBox( wxCommandEvent &event );
	void OnText( wxCommandEvent &event );

	void valueUpdated( wxTextCtrl *textCtrl );

public:
	/** Constructor */
	TexturePanelImpl( wxWindow *parent, Ogre::YangenManager *yangenManager );
	~TexturePanelImpl();
	//// end generated class members

	wxScrolledWindow *getScrolledWindow() const { return m_scrolledWindow2; }
};

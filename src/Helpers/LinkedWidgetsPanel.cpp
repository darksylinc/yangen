//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Helpers/LinkedWidgetsPanel.h"

#include <wx/slider.h>
#include <wx/textctrl.h>

void LinkedWidgetsPanel::mapSliderAndTextCtrl( ConversionBase *converter )
{
	mapSliderAndTextCtrl( converter, 0, false );
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPanel::mapSliderAndTextCtrl( ConversionBase *converter,
											   float newDefault, bool bSetDefault )
{
	m_sliderToTextCtrlMap[converter->m_slider]		= converter;
	m_textCtrlToSliderMap[converter->m_textCtrl]	= converter;

	if( !bSetDefault )
		converter->toTextCtrl();
	else
	{
		converter->m_textCtrl->SetValue( wxString::Format( wxT("%.05lf"), newDefault ) );
		converter->toSlider();
	}
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPanel::OnTextCtrl( wxCommandEvent& event )
{
	wxTextCtrl *textCtrl = static_cast<wxTextCtrl*>( event.GetEventObject() );

	std::map<wxTextCtrl*, ConversionBase*>::const_iterator itor = m_textCtrlToSliderMap.find( textCtrl );
	if( itor != m_textCtrlToSliderMap.end() )
	{
		itor->second->toSlider();
		valueUpdated( itor->second->m_textCtrl );
	}
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPanel::OnSliderScroll( wxScrollEvent& event )
{
	wxSlider *slider = static_cast<wxSlider*>( event.GetEventObject() );

	std::map<wxSlider*, ConversionBase*>::const_iterator itor = m_sliderToTextCtrlMap.find( slider );
	if( itor != m_sliderToTextCtrlMap.end() )
		itor->second->toTextCtrl();
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPanel::refreshAllLinkedControls()
{
	std::map<wxTextCtrl*, ConversionBase*>::const_iterator itor = m_textCtrlToSliderMap.begin();
	std::map<wxTextCtrl*, ConversionBase*>::const_iterator end  = m_textCtrlToSliderMap.end();

	while( itor != end )
	{
		ConversionBase *converter = itor->second;
		converter->toSlider();
		valueUpdated( converter->m_textCtrl );
		++itor;
	}
}
//-----------------------------------------------------------------------------
float LinkedWidgetsPanel::getValueFrom( wxTextCtrl *textCtrl, float defaultValue )
{
	wxString value = textCtrl->GetValue();
	double fValue = defaultValue;
	value.ToDouble( &fValue );
	return static_cast<float>( fValue );
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void LinkedWidgetsPairPanel::mapSliderAndTextCtrl( ConvertPair *pair )
{
	mapSliderAndTextCtrl( pair, 0, false );
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPairPanel::mapSliderAndTextCtrl( ConvertPair *pair, float minDefault,
												   float maxDefault, bool bSetDefault )
{
	m_pairSlidersToTextCtrls[pair->m_min->m_slider]	= pair;
	m_pairSlidersToTextCtrls[pair->m_max->m_slider]	= pair;

	m_pairTextCtrlsToSliders[pair->m_min->m_textCtrl]= pair;
	m_pairTextCtrlsToSliders[pair->m_max->m_textCtrl]= pair;

	if( !bSetDefault )
	{
		pair->m_min->toTextCtrl();
		pair->m_max->toTextCtrl();
	}
	else
	{
		pair->m_min->m_textCtrl->SetValue( wxString::Format( wxT("%.05lf"), minDefault ) );
		pair->m_max->m_textCtrl->SetValue( wxString::Format( wxT("%.05lf"), maxDefault ) );
		pair->m_min->toSlider();
		pair->m_max->toSlider();
	}
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPairPanel::OnTextCtrl( wxCommandEvent& event )
{
	wxTextCtrl *textCtrl = static_cast<wxTextCtrl*>( event.GetEventObject() );
	std::map<wxTextCtrl*, ConvertPair*>::const_iterator itPair =
			m_pairTextCtrlsToSliders.find( textCtrl );
	if( itPair != m_pairTextCtrlsToSliders.end() )
	{
		itPair->second->textCtrlChanged( textCtrl );
		valueUpdated( textCtrl );
	}

	LinkedWidgetsPanel::OnTextCtrl( event );
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPairPanel::OnSliderScroll( wxScrollEvent& event )
{
	wxSlider *slider = static_cast<wxSlider*>( event.GetEventObject() );
	std::map<wxSlider*, ConvertPair*>::const_iterator itPair =
			m_pairSlidersToTextCtrls.find( slider );
	if( itPair != m_pairSlidersToTextCtrls.end() )
		itPair->second->sliderChanged( slider );

	LinkedWidgetsPanel::OnSliderScroll( event );
}
//-----------------------------------------------------------------------------
void LinkedWidgetsPairPanel::refreshAllLinkedControls()
{
	LinkedWidgetsPanel::refreshAllLinkedControls();

	std::map<wxTextCtrl*, ConvertPair*>::const_iterator itor = m_pairTextCtrlsToSliders.begin();
	std::map<wxTextCtrl*, ConvertPair*>::const_iterator end  = m_pairTextCtrlsToSliders.end();

	while( itor != end )
	{
		ConvertPair *converter = itor->second;
		converter->m_min->toSlider();
		converter->m_max->toSlider();
		valueUpdated( converter->m_min->m_textCtrl );
		valueUpdated( converter->m_max->m_textCtrl );
		++itor;
	}
}

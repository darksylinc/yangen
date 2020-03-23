//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Core/TexturePanelImpl.h"

#include "Core/YangenWindowImpl.h"

#include "Yangen/YangenManager.h"

#include "OgreGpuProgramManager.h"
#include "OgreHlms.h"
#include "OgreHlmsManager.h"
#include "OgreRoot.h"

#include "OgreLwString.h"

#include <wx/colordlg.h>

TexturePanelImpl::TexturePanelImpl( wxWindow *parent, Ogre::YangenManager *yangenManager ) :
	TexturePanel( parent ),
	mInitializing( true ),
	m_yangenManager( yangenManager )
{
	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapDepth0Slider, m_normalMapDepth0TextCtrl, -20.0f, 20.0f ) );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapDepth1Slider, m_normalMapDepth1TextCtrl, -20.0f, 20.0f ) );

	mInitializing = false;
}
//-----------------------------------------------------------------------------
TexturePanelImpl::~TexturePanelImpl()
{
	{
		std::map<wxSlider *, ConversionBase *>::const_iterator itor = m_sliderToTextCtrlMap.begin();
		std::map<wxSlider *, ConversionBase *>::const_iterator endt = m_sliderToTextCtrlMap.end();

		while( itor != endt )
		{
			delete itor->second;
			++itor;
		}

		m_sliderToTextCtrlMap.clear();
		m_textCtrlToSliderMap.clear();
	}

	{
		std::set<ConvertPair *> uniquePairs;
		std::map<wxSlider *, ConvertPair *>::const_iterator itor = m_pairSlidersToTextCtrls.begin();
		std::map<wxSlider *, ConvertPair *>::const_iterator endt = m_pairSlidersToTextCtrls.end();

		while( itor != endt )
		{
			uniquePairs.insert( itor->second );
			++itor;
		}
		m_pairSlidersToTextCtrls.clear();
		m_pairTextCtrlsToSliders.clear();

		std::set<ConvertPair *>::const_iterator itPair = uniquePairs.begin();
		std::set<ConvertPair *>::const_iterator enPair = uniquePairs.end();

		while( itPair != enPair )
			delete *itPair++;
	}
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::mapSliderAndTextCtrl( ConversionBase *converter )
{
	mapSliderAndTextCtrl( converter, 0, false );
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::mapSliderAndTextCtrl( ConversionBase *converter, float newDefault,
											 bool bSetDefault )
{
	m_sliderToTextCtrlMap[converter->m_slider] = converter;
	m_textCtrlToSliderMap[converter->m_textCtrl] = converter;

	if( !bSetDefault )
		converter->toTextCtrl();
	else
	{
		converter->m_textCtrl->SetValue( wxString::Format( wxT( "%.05lf" ), newDefault ) );
		converter->toSlider();
	}
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::mapSliderAndTextCtrl( ConvertPair *pair )
{
	mapSliderAndTextCtrl( pair, 0, false );
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::mapSliderAndTextCtrl( ConvertPair *pair, float minDefault, float maxDefault,
											 bool bSetDefault )
{
	m_pairSlidersToTextCtrls[pair->m_min->m_slider] = pair;
	m_pairSlidersToTextCtrls[pair->m_max->m_slider] = pair;

	m_pairTextCtrlsToSliders[pair->m_min->m_textCtrl] = pair;
	m_pairTextCtrlsToSliders[pair->m_max->m_textCtrl] = pair;

	if( !bSetDefault )
	{
		pair->m_min->toTextCtrl();
		pair->m_max->toTextCtrl();
	}
	else
	{
		pair->m_min->m_textCtrl->SetValue( wxString::Format( wxT( "%.05lf" ), minDefault ) );
		pair->m_max->m_textCtrl->SetValue( wxString::Format( wxT( "%.05lf" ), maxDefault ) );
		pair->m_min->toSlider();
		pair->m_max->toSlider();
	}
}
//-----------------------------------------------------------------------------
float TexturePanelImpl::getValueFrom( wxTextCtrl *textCtrl, float defaultValue ) const
{
	wxString value = textCtrl->GetValue();
	double fValue = defaultValue;
	value.ToDouble( &fValue );
	return static_cast<float>( fValue );
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::OnScroll( wxScrollEvent &event )
{
	wxSlider *slider = static_cast<wxSlider *>( event.GetEventObject() );
	std::map<wxSlider *, ConvertPair *>::const_iterator itPair = m_pairSlidersToTextCtrls.find( slider );
	if( itPair != m_pairSlidersToTextCtrls.end() )
		itPair->second->sliderChanged( slider );

	std::map<wxSlider *, ConversionBase *>::const_iterator itor = m_sliderToTextCtrlMap.find( slider );
	if( itor != m_sliderToTextCtrlMap.end() )
		itor->second->toTextCtrl();
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::OnCheckBox( wxCommandEvent &event )
{
	// wxCheckBox *checkBox = static_cast<wxCheckBox *>( event.GetEventObject() );
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::OnText( wxCommandEvent &event )
{
	wxTextCtrl *textCtrl = static_cast<wxTextCtrl *>( event.GetEventObject() );
	std::map<wxTextCtrl *, ConvertPair *>::const_iterator itPair =
		m_pairTextCtrlsToSliders.find( textCtrl );
	if( itPair != m_pairTextCtrlsToSliders.end() )
	{
		itPair->second->textCtrlChanged( textCtrl );
		valueUpdated( textCtrl );
	}

	std::map<wxTextCtrl *, ConversionBase *>::const_iterator itor =
		m_textCtrlToSliderMap.find( textCtrl );
	if( itor != m_textCtrlToSliderMap.end() )
	{
		itor->second->toSlider();
		valueUpdated( itor->second->m_textCtrl );
	}
}
//-----------------------------------------------------------------------------
void TexturePanelImpl::valueUpdated( wxTextCtrl *textCtrl )
{
	if( mInitializing )
		return;

	if( textCtrl == m_normalMapDepth0TextCtrl )
	{
		m_yangenManager->setHeightMapToNormalMapDepthScale(
			getValueFrom( m_normalMapDepth0TextCtrl, 1.0f ), 0u );
		m_yangenManager->process();
	}
	else if( textCtrl == m_normalMapDepth1TextCtrl )
	{
		m_yangenManager->setHeightMapToNormalMapDepthScale(
			getValueFrom( m_normalMapDepth1TextCtrl, 1.0f ), 1u );
		m_yangenManager->process();
	}
}

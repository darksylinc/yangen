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
		new ConvertScaled( m_normalMapDepth0Slider, m_normalMapDepth0TextCtrl, -20.0f, 20.0f ),
		m_yangenManager->getHeightMapToNormalMapStrength( 0u ) );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapDepth1Slider, m_normalMapDepth1TextCtrl, -20.0f, 20.0f ),
		m_yangenManager->getHeightMapToNormalMapStrength( 1u ) );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapDepth2Slider, m_normalMapDepth2TextCtrl, -20.0f, 20.0f ),
		m_yangenManager->getHeightMapToNormalMapStrength( 2u ) );

	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapRadius1Slider, m_normalMapRadius1TextCtrl, 2.0f, 64.0f, true ),
		m_yangenManager->getHeightMapToNormalMapRadius( 1u ) );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapRadius2Slider, m_normalMapRadius2TextCtrl, 2.0f, 64.0f, true ),
		m_yangenManager->getHeightMapToNormalMapRadius( 2u ) );

	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapSteepness0Slider, m_normalMapSteepness0TextCtrl, -10.0f, 10.0f ),
		m_yangenManager->getNormalMapSteepness( 0u ) );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapSteepness1Slider, m_normalMapSteepness1TextCtrl, -10.0f, 10.0f ),
		m_yangenManager->getNormalMapSteepness( 1u ) );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_normalMapSteepness2Slider, m_normalMapSteepness2TextCtrl, -10.0f, 10.0f ),
		m_yangenManager->getNormalMapSteepness( 2u ) );

	mapSliderAndTextCtrl( new ConvertScaled( m_roughnessBlurOffsetSlider, m_roughnessBlurOffsetTextCtrl,
											 0.0f, 32.0f, true ),
						  m_yangenManager->getRoughnessBlurOffset() );
	mapSliderAndTextCtrl( new ConvertScaled( m_roughnessBlurAmplitudeSlider,
											 m_roughnessBlurAmplitudeTextCtrl, 0.0f, 32.0f, true ),
						  m_yangenManager->getRoughnessBlurAmplitude() );

	mapSliderAndTextCtrl(
		new ConvertScaled( m_roughnessMidpointSlider, m_roughnessMidpointTextCtrl, -0.5f, 1.5f ),
		m_yangenManager->getRoughnessMidpoint() );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_roughnessScaleSlider, m_roughnessScaleTextCtrl, -4.0f, 4.0f ),
		m_yangenManager->getRoughnessScale() );
	mapSliderAndTextCtrl(
		new ConvertScaled( m_roughnessExponentSlider, m_roughnessExponentTextCtrl, 0.0f, 2.0f ),
		m_yangenManager->getRoughnessExponent() );

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

	const size_t c_numStrengthTextCtrls = 3u;
	wxTextCtrl *nmStrengthTextCtrls[c_numStrengthTextCtrls] = { m_normalMapDepth0TextCtrl,
																m_normalMapDepth1TextCtrl,
																m_normalMapDepth2TextCtrl };

	const size_t c_numNmRadiusTextCtrls = 2u;
	wxTextCtrl *nmRadiusTextCtrls[c_numNmRadiusTextCtrls] = { m_normalMapRadius1TextCtrl,
															  m_normalMapRadius2TextCtrl };

	const size_t c_numSteepnessTextCtrls = 3u;
	wxTextCtrl *steepnessTextCtrls[c_numSteepnessTextCtrls] = { m_normalMapSteepness0TextCtrl,
																m_normalMapSteepness1TextCtrl,
																m_normalMapSteepness2TextCtrl };

	for( uint8_t i = 0u; i < c_numStrengthTextCtrls; ++i )
	{
		if( textCtrl == nmStrengthTextCtrls[i] )
		{
			m_yangenManager->setHeightMapToNormalMapStrength( getValueFrom( textCtrl, 1.0f ), i );
			m_yangenManager->process();
			return;
		}
	}

	for( uint8_t i = 0u; i < c_numNmRadiusTextCtrls; ++i )
	{
		if( textCtrl == nmRadiusTextCtrls[i] )
		{
			uint8_t radius = static_cast<uint8_t>( getValueFrom( textCtrl, 2.0f ) );
			if( radius & 0x01u )
				radius = radius + 1u;
			m_yangenManager->setHeightMapToNormalMapRadius( radius, i + 1u );
			m_yangenManager->process();
			return;
		}
	}

	for( uint8_t i = 0u; i < c_numSteepnessTextCtrls; ++i )
	{
		if( textCtrl == steepnessTextCtrls[i] )
		{
			m_yangenManager->setNormalMapSteepness( getValueFrom( textCtrl, 0.0f ), i );
			m_yangenManager->process();
			return;
		}
	}

	if( textCtrl == m_roughnessBlurOffsetTextCtrl )
	{
		uint8_t offset = static_cast<uint8_t>( getValueFrom( textCtrl, 2.0f ) );
		if( offset & 0x01u )
			offset = offset + 1u;
		m_yangenManager->setRoughnessBlurOffset( offset );
		m_yangenManager->process();
		return;
	}
	if( textCtrl == m_roughnessBlurAmplitudeTextCtrl )
	{
		uint8_t amplitude = static_cast<uint8_t>( getValueFrom( textCtrl, 2.0f ) );
		if( amplitude & 0x01u )
			amplitude = amplitude + 1u;
		m_yangenManager->setRoughnessBlurAmplitude( amplitude );
		m_yangenManager->process();
		return;
	}
	if( textCtrl == m_roughnessMidpointTextCtrl )
	{
		m_yangenManager->setRoughnessMidpoint( getValueFrom( textCtrl, 0.0f ) );
		m_yangenManager->process();
		return;
	}
	if( textCtrl == m_roughnessScaleTextCtrl )
	{
		m_yangenManager->setRoughnessScale( getValueFrom( textCtrl, 1.0f ) );
		m_yangenManager->process();
		return;
	}
	if( textCtrl == m_roughnessExponentTextCtrl )
	{
		m_yangenManager->setRoughnessExponent( getValueFrom( textCtrl, 1.0f ) );
		m_yangenManager->process();
		return;
	}
}

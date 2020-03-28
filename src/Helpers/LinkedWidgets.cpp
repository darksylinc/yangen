//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Helpers/LinkedWidgets.h"

#include <wx/slider.h>
#include <wx/string.h>
#include <wx/textctrl.h>

#include <algorithm>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ConvertUnit::ConvertUnit( wxSlider *slider, wxTextCtrl *textCtrl, bool isInteger ) :
	ConversionBase( slider, textCtrl, isInteger )
{
}

void ConvertUnit::toSlider()
{
	wxString value = m_textCtrl->GetValue();
	double fValue = wxAtof( value );
	if( m_isInteger )
		fValue = round( fValue );
	m_slider->SetValue( static_cast<int>( fValue * 100.0 + 0.5 ) );
}
void ConvertUnit::toTextCtrl()
{
	double value = (double)m_slider->GetValue();
	value /= 100.0;
	if( m_isInteger )
	{
		value = round( value );
		if( value == -0 )
			value = 0;
		m_textCtrl->SetValue( wxString::Format( wxT("%.0lf"), value ) );
	}
	else
	{
		m_textCtrl->SetValue( wxString::Format( wxT("%.05lf"), value ) );
	}
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ConvertScaled::ConvertScaled( wxSlider *slider, wxTextCtrl *textCtrl, float minValue, float maxValue,
							  bool isInteger ) :
	ConversionBase( slider, textCtrl, isInteger ),
	m_minValue( minValue ),
	m_maxValue( maxValue )
{
}

void ConvertScaled::toSlider()
{
	wxString value = m_textCtrl->GetValue();
	double fValue = wxAtof( value );
	fValue = std::max<float>( fValue, m_minValue );
	if( m_isInteger )
		fValue = round( fValue );
	m_slider->SetValue(
		static_cast<int>( ( fValue - m_minValue ) / ( m_maxValue - m_minValue ) * 100.0 + 0.5 ) );
}
void ConvertScaled::toTextCtrl()
{
	double value = (double)m_slider->GetValue() / 100.0;
	value = value * ( m_maxValue - m_minValue ) + m_minValue;
	if( m_isInteger )
	{
		value = round( value );
		if( value == -0 )
			value = 0;
		m_textCtrl->SetValue( wxString::Format( wxT("%.0lf"), value ) );
	}
	else
	{
		m_textCtrl->SetValue( wxString::Format( wxT("%.05lf"), value ) );
	}
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ConvertPair::ConvertPair( ConversionBase *min, ConversionBase *max ) : m_min( min ), m_max( max )
{
}
//-----------------------------------------------------------------------------
ConvertPair::~ConvertPair()
{
	delete m_min;
	delete m_max;
	m_min = 0;
	m_max = 0;
}
//-----------------------------------------------------------------------------
void ConvertPair::ensureRange( ConversionBase *elementModified, ConversionBase *elementToModify )
{
	double fMin = wxAtof( m_min->m_textCtrl->GetValue() );
	double fMax = wxAtof( m_max->m_textCtrl->GetValue() );

	if( fMin > fMax )
	{
		elementToModify->m_textCtrl->SetValue( elementModified->m_textCtrl->GetValue() );
		elementToModify->toSlider();
	}
}
//-----------------------------------------------------------------------------
void ConvertPair::sliderChanged( wxSlider *slider )
{
	if( slider == m_min->m_slider )
	{
		m_min->toTextCtrl();
		ensureRange( m_min, m_max );
	}
	else if( slider == m_max->m_slider )
	{
		m_max->toTextCtrl();
		ensureRange( m_max, m_min );
	}
}
//-----------------------------------------------------------------------------
void ConvertPair::textCtrlChanged( wxTextCtrl *textCtrl )
{
	if( textCtrl == m_min->m_textCtrl )
	{
		m_min->toSlider();
		ensureRange( m_min, m_max );
	}
	else if( textCtrl == m_max->m_textCtrl )
	{
		m_max->toSlider();
		ensureRange( m_max, m_min );
	}
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void setValue( wxTextCtrl *textCtrl, float value )
{
	textCtrl->SetValue( wxString::Format( wxT( "%.05lf" ), value ) );
}

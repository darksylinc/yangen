//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

class wxSlider;
class wxTextCtrl;

struct ConversionBase
{
	wxSlider	*m_slider;
	wxTextCtrl	*m_textCtrl;

	ConversionBase( wxSlider *slider, wxTextCtrl *textCtrl ) :
		m_slider( slider ), m_textCtrl( textCtrl ) {}
	virtual ~ConversionBase() {}

	virtual void toSlider() = 0;
	virtual void toTextCtrl() = 0;
};

struct ConvertUnit : public ConversionBase
{
	ConvertUnit( wxSlider *slider, wxTextCtrl *textCtrl );
	virtual void toSlider();
	virtual void toTextCtrl();
};
struct ConvertScaled : public ConversionBase
{
	float m_minValue;
	float m_maxValue;

	ConvertScaled( wxSlider *slider, wxTextCtrl *textCtrl, float minValue, float maxValue );
	virtual void toSlider();
	virtual void toTextCtrl();
};

struct ConvertPair
{
	ConversionBase *m_min;
	ConversionBase *m_max;

	ConvertPair( ConversionBase *min, ConversionBase *max );
	~ConvertPair();

	void ensureRange( ConversionBase *elementModified, ConversionBase *elementToModify );

	void sliderChanged( wxSlider *slider );
	void textCtrlChanged( wxTextCtrl *textCtrl );
};

void setValue( wxTextCtrl *textCtrl, float value );

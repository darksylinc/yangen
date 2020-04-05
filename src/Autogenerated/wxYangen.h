///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul  5 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_LOAD_DIFFUSE 1000
#define wxID_LOAD_HEIGHTMAP 1001
#define wxID_LOAD_DIFFUSE_AND_HEIGHTMAP 1002
#define wxID_SAVEFILE 1003
#define wxID_RELOAD_SHADERS 1004
#define wxID_EROSION_SLIDER 1005
#define wxID_EROSION_CTRL 1006

///////////////////////////////////////////////////////////////////////////////
/// Class YangenWindow
///////////////////////////////////////////////////////////////////////////////
class YangenWindow : public wxFrame
{
	private:

	protected:
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar;
		wxMenu* m_fileMenu;

		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }


	public:

		YangenWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Yangen Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1600,900 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~YangenWindow();

};

///////////////////////////////////////////////////////////////////////////////
/// Class TexturePanel
///////////////////////////////////////////////////////////////////////////////
class TexturePanel : public wxPanel
{
	private:

	protected:
		wxScrolledWindow* m_scrolledWindow2;
		wxBoxSizer* m_bSizer2;
		wxButton* m_button7;
		wxStaticText* m_staticText7;
		wxSlider* m_normalMapDepth0Slider;
		wxTextCtrl* m_normalMapDepth0TextCtrl;
		wxStaticText* m_staticText73;
		wxSlider* m_normalMapSteepness0Slider;
		wxTextCtrl* m_normalMapSteepness0TextCtrl;
		wxStaticText* m_staticText71;
		wxSlider* m_normalMapDepth1Slider;
		wxTextCtrl* m_normalMapDepth1TextCtrl;
		wxStaticText* m_staticText72;
		wxSlider* m_normalMapRadius1Slider;
		wxTextCtrl* m_normalMapRadius1TextCtrl;
		wxStaticText* m_staticText731;
		wxSlider* m_normalMapSteepness1Slider;
		wxTextCtrl* m_normalMapSteepness1TextCtrl;
		wxStaticText* m_staticText711;
		wxSlider* m_normalMapDepth2Slider;
		wxTextCtrl* m_normalMapDepth2TextCtrl;
		wxStaticText* m_staticText721;
		wxSlider* m_normalMapRadius2Slider;
		wxTextCtrl* m_normalMapRadius2TextCtrl;
		wxStaticText* m_staticText7311;
		wxSlider* m_normalMapSteepness2Slider;
		wxTextCtrl* m_normalMapSteepness2TextCtrl;
		wxStaticText* m_staticText7111;
		wxSlider* m_roughnessBlurOffsetSlider;
		wxTextCtrl* m_roughnessBlurOffsetTextCtrl;
		wxStaticText* m_staticText7211;
		wxSlider* m_roughnessBlurAmplitudeSlider;
		wxTextCtrl* m_roughnessBlurAmplitudeTextCtrl;
		wxStaticText* m_staticText73111;
		wxSlider* m_roughnessMidpointSlider;
		wxTextCtrl* m_roughnessMidpointTextCtrl;
		wxStaticText* m_staticText731111;
		wxSlider* m_roughnessScaleSlider;
		wxTextCtrl* m_roughnessScaleTextCtrl;
		wxStaticText* m_staticText7311111;
		wxSlider* m_roughnessExponentSlider;
		wxTextCtrl* m_roughnessExponentTextCtrl;

		// Virtual event handlers, overide them in your derived class
		virtual void OnScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }


	public:

		TexturePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 444,771 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~TexturePanel();

};

///////////////////////////////////////////////////////////////////////////////
/// Class VisualizationPanel
///////////////////////////////////////////////////////////////////////////////
class VisualizationPanel : public wxPanel
{
	private:

	protected:
		wxChoice* m_visualizationChoice;

		// Virtual event handlers, overide them in your derived class
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }


	public:

		VisualizationPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~VisualizationPanel();

};


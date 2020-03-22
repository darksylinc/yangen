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
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_LOADFILE 1000
#define wxID_SAVEFILE 1001
#define wxID_RELOAD_SHADERS 1002
#define wxID_EROSION_SLIDER 1003
#define wxID_EROSION_CTRL 1004

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
		wxChoice* m_choice1;
		wxStaticText* m_staticText7;
		wxSlider* m_normalMapDepthSlider;
		wxTextCtrl* m_normalMapDepthTextCtrl;

		// Virtual event handlers, overide them in your derived class
		virtual void OnScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }


	public:

		TexturePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 444,771 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~TexturePanel();

};


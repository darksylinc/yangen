///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul  5 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../../src/Autogenerated/wxYangen.h"

///////////////////////////////////////////////////////////////////////////

YangenWindow::YangenWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1024,768 ), wxDefaultSize );

	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_menubar = new wxMenuBar( 0 );
	m_fileMenu = new wxMenu();
	wxMenuItem* m_loadMenuItem;
	m_loadMenuItem = new wxMenuItem( m_fileMenu, wxID_LOADFILE, wxString( wxT("&Load") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_loadMenuItem );

	wxMenuItem* m_saveMenuItem;
	m_saveMenuItem = new wxMenuItem( m_fileMenu, wxID_SAVEFILE, wxString( wxT("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_saveMenuItem );

	m_fileMenu->AppendSeparator();

	wxMenuItem* m_reloadShadersMenuItem;
	m_reloadShadersMenuItem = new wxMenuItem( m_fileMenu, wxID_RELOAD_SHADERS, wxString( wxT("&Reload Shaders") ) + wxT('\t') + wxT("F5"), wxEmptyString, wxITEM_NORMAL );
	m_fileMenu->Append( m_reloadShadersMenuItem );

	m_menubar->Append( m_fileMenu, wxT("&File") );

	this->SetMenuBar( m_menubar );


	this->Centre( wxBOTH );

	// Connect Events
	m_fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YangenWindow::OnMenuSelection ), this, m_loadMenuItem->GetId());
	m_fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YangenWindow::OnMenuSelection ), this, m_saveMenuItem->GetId());
	m_fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YangenWindow::OnMenuSelection ), this, m_reloadShadersMenuItem->GetId());
}

YangenWindow::~YangenWindow()
{
	// Disconnect Events

}

TexturePanel::TexturePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindow2 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow2->SetScrollRate( 5, 5 );
	m_bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_button7 = new wxButton( m_scrolledWindow2, wxID_ANY, wxT("Open File"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button7, 0, wxALL, 5 );

	wxString m_choice1Choices[] = { wxT("Final Render"), wxT("Normal Map") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( m_scrolledWindow2, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer7->Add( m_choice1, 0, wxALL, 5 );


	m_bSizer2->Add( bSizer7, 0, 0, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );


	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Small Details"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer15->Add( m_staticText7, 0, wxALL, 5 );


	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer15, 1, wxEXPAND, 5 );

	m_normalMapDepth0Slider = new wxSlider( m_scrolledWindow2, wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer3->Add( m_normalMapDepth0Slider, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );

	m_normalMapDepth0TextCtrl = new wxTextCtrl( m_scrolledWindow2, wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_normalMapDepth0TextCtrl, 0, wxALL, 5 );

	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer( wxVERTICAL );


	bSizer151->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText71 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Medium Details"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer151->Add( m_staticText71, 0, wxALL, 5 );


	bSizer151->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer151, 1, wxEXPAND, 5 );

	m_normalMapDepth1Slider = new wxSlider( m_scrolledWindow2, wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer3->Add( m_normalMapDepth1Slider, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_normalMapDepth1TextCtrl = new wxTextCtrl( m_scrolledWindow2, wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_normalMapDepth1TextCtrl, 0, wxALL, 5 );

	wxBoxSizer* bSizer1511;
	bSizer1511 = new wxBoxSizer( wxVERTICAL );


	bSizer1511->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText711 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Large Details"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText711->Wrap( -1 );
	bSizer1511->Add( m_staticText711, 0, wxALL, 5 );


	bSizer1511->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer1511, 1, wxEXPAND, 5 );

	m_normalMapDepth2Slider = new wxSlider( m_scrolledWindow2, wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer3->Add( m_normalMapDepth2Slider, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_normalMapDepth2TextCtrl = new wxTextCtrl( m_scrolledWindow2, wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_normalMapDepth2TextCtrl, 0, wxALL, 5 );

	wxBoxSizer* bSizer152;
	bSizer152 = new wxBoxSizer( wxVERTICAL );


	bSizer152->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText72 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Medium Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	bSizer152->Add( m_staticText72, 0, wxALL, 5 );


	bSizer152->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer152, 1, wxEXPAND, 5 );

	m_normalMapRadius1Slider = new wxSlider( m_scrolledWindow2, wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer3->Add( m_normalMapRadius1Slider, 1, wxALL|wxEXPAND, 5 );

	m_normalMapRadius1TextCtrl = new wxTextCtrl( m_scrolledWindow2, wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_normalMapRadius1TextCtrl, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer1521;
	bSizer1521 = new wxBoxSizer( wxVERTICAL );


	bSizer1521->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText721 = new wxStaticText( m_scrolledWindow2, wxID_ANY, wxT("Large Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText721->Wrap( -1 );
	bSizer1521->Add( m_staticText721, 0, wxALL, 5 );


	bSizer1521->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer3->Add( bSizer1521, 1, wxEXPAND, 5 );

	m_normalMapRadius2Slider = new wxSlider( m_scrolledWindow2, wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer3->Add( m_normalMapRadius2Slider, 1, wxALL|wxEXPAND, 5 );

	m_normalMapRadius2TextCtrl = new wxTextCtrl( m_scrolledWindow2, wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_normalMapRadius2TextCtrl, 1, wxALL|wxEXPAND, 5 );


	m_bSizer2->Add( fgSizer3, 0, wxEXPAND, 5 );


	m_scrolledWindow2->SetSizer( m_bSizer2 );
	m_scrolledWindow2->Layout();
	m_bSizer2->Fit( m_scrolledWindow2 );
	bSizer2->Add( m_scrolledWindow2, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	// Connect Events
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
}

TexturePanel::~TexturePanel()
{
	// Disconnect Events
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth0TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth1TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapDepth2TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius1TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapRadius2TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );

}

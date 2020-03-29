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


	m_bSizer2->Add( bSizer7, 0, 0, 5 );

	wxStaticBoxSizer* Small;
	Small = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow2, wxID_ANY, wxT("Small Details") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer5->AddGrowableCol( 1 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );


	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( Small->GetStaticBox(), wxID_ANY, wxT("Strength"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer15->Add( m_staticText7, 0, wxALL, 5 );


	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer5->Add( bSizer15, 1, wxEXPAND, 5 );

	m_normalMapDepth0Slider = new wxSlider( Small->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer5->Add( m_normalMapDepth0Slider, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );

	m_normalMapDepth0TextCtrl = new wxTextCtrl( Small->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_normalMapDepth0TextCtrl, 0, wxALL, 5 );

	wxBoxSizer* bSizer153;
	bSizer153 = new wxBoxSizer( wxVERTICAL );


	bSizer153->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText73 = new wxStaticText( Small->GetStaticBox(), wxID_ANY, wxT("Steepness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	bSizer153->Add( m_staticText73, 0, wxALL, 5 );


	bSizer153->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer5->Add( bSizer153, 1, wxEXPAND, 5 );

	m_normalMapSteepness0Slider = new wxSlider( Small->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer5->Add( m_normalMapSteepness0Slider, 1, wxALL|wxEXPAND, 5 );

	m_normalMapSteepness0TextCtrl = new wxTextCtrl( Small->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_normalMapSteepness0TextCtrl, 1, wxALL|wxEXPAND, 5 );


	Small->Add( fgSizer5, 1, wxEXPAND, 5 );


	m_bSizer2->Add( Small, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* Medium;
	Medium = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow2, wxID_ANY, wxT("Medium Details") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer6->AddGrowableCol( 1 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer( wxVERTICAL );


	bSizer151->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText71 = new wxStaticText( Medium->GetStaticBox(), wxID_ANY, wxT("Strength"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer151->Add( m_staticText71, 0, wxALL, 5 );


	bSizer151->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer6->Add( bSizer151, 1, wxEXPAND, 5 );

	m_normalMapDepth1Slider = new wxSlider( Medium->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer6->Add( m_normalMapDepth1Slider, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_normalMapDepth1TextCtrl = new wxTextCtrl( Medium->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_normalMapDepth1TextCtrl, 0, wxALL, 5 );

	wxBoxSizer* bSizer152;
	bSizer152 = new wxBoxSizer( wxVERTICAL );


	bSizer152->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText72 = new wxStaticText( Medium->GetStaticBox(), wxID_ANY, wxT("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	bSizer152->Add( m_staticText72, 0, wxALL, 5 );


	bSizer152->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer6->Add( bSizer152, 1, wxEXPAND, 5 );

	m_normalMapRadius1Slider = new wxSlider( Medium->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer6->Add( m_normalMapRadius1Slider, 1, wxALL|wxEXPAND, 5 );

	m_normalMapRadius1TextCtrl = new wxTextCtrl( Medium->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_normalMapRadius1TextCtrl, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer1531;
	bSizer1531 = new wxBoxSizer( wxVERTICAL );


	bSizer1531->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText731 = new wxStaticText( Medium->GetStaticBox(), wxID_ANY, wxT("Steepness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText731->Wrap( -1 );
	bSizer1531->Add( m_staticText731, 0, wxALL, 5 );


	bSizer1531->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer6->Add( bSizer1531, 1, wxEXPAND, 5 );

	m_normalMapSteepness1Slider = new wxSlider( Medium->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer6->Add( m_normalMapSteepness1Slider, 1, wxALL|wxEXPAND, 5 );

	m_normalMapSteepness1TextCtrl = new wxTextCtrl( Medium->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_normalMapSteepness1TextCtrl, 1, wxALL|wxEXPAND, 5 );


	Medium->Add( fgSizer6, 1, wxEXPAND, 5 );


	m_bSizer2->Add( Medium, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* Large;
	Large = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow2, wxID_ANY, wxT("Large Details") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer7->AddGrowableCol( 1 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer1511;
	bSizer1511 = new wxBoxSizer( wxVERTICAL );


	bSizer1511->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText711 = new wxStaticText( Large->GetStaticBox(), wxID_ANY, wxT("Strength"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText711->Wrap( -1 );
	bSizer1511->Add( m_staticText711, 0, wxALL, 5 );


	bSizer1511->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer7->Add( bSizer1511, 1, wxEXPAND, 5 );

	m_normalMapDepth2Slider = new wxSlider( Large->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer7->Add( m_normalMapDepth2Slider, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_normalMapDepth2TextCtrl = new wxTextCtrl( Large->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_normalMapDepth2TextCtrl, 0, wxALL, 5 );

	wxBoxSizer* bSizer1521;
	bSizer1521 = new wxBoxSizer( wxVERTICAL );


	bSizer1521->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText721 = new wxStaticText( Large->GetStaticBox(), wxID_ANY, wxT("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText721->Wrap( -1 );
	bSizer1521->Add( m_staticText721, 0, wxALL, 5 );


	bSizer1521->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer7->Add( bSizer1521, 1, wxEXPAND, 5 );

	m_normalMapRadius2Slider = new wxSlider( Large->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer7->Add( m_normalMapRadius2Slider, 1, wxALL|wxEXPAND, 5 );

	m_normalMapRadius2TextCtrl = new wxTextCtrl( Large->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_normalMapRadius2TextCtrl, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer15311;
	bSizer15311 = new wxBoxSizer( wxVERTICAL );


	bSizer15311->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText7311 = new wxStaticText( Large->GetStaticBox(), wxID_ANY, wxT("Steepness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7311->Wrap( -1 );
	bSizer15311->Add( m_staticText7311, 0, wxALL, 5 );


	bSizer15311->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer7->Add( bSizer15311, 1, wxEXPAND, 5 );

	m_normalMapSteepness2Slider = new wxSlider( Large->GetStaticBox(), wxID_EROSION_SLIDER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer7->Add( m_normalMapSteepness2Slider, 1, wxALL|wxEXPAND, 5 );

	m_normalMapSteepness2TextCtrl = new wxTextCtrl( Large->GetStaticBox(), wxID_EROSION_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_normalMapSteepness2TextCtrl, 1, wxALL|wxEXPAND, 5 );


	Large->Add( fgSizer7, 1, wxEXPAND, 5 );


	m_bSizer2->Add( Large, 1, wxEXPAND, 5 );


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
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
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
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
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
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2TextCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
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
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness0TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
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
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness1TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );
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
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2Slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TexturePanel::OnScroll ), NULL, this );
	m_normalMapSteepness2TextCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TexturePanel::OnText ), NULL, this );

}

VisualizationPanel::VisualizationPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	wxString m_visualizationChoiceChoices[] = { wxT("Final Render"), wxT("Normal Map") };
	int m_visualizationChoiceNChoices = sizeof( m_visualizationChoiceChoices ) / sizeof( wxString );
	m_visualizationChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_visualizationChoiceNChoices, m_visualizationChoiceChoices, 0 );
	m_visualizationChoice->SetSelection( 0 );
	bSizer12->Add( m_visualizationChoice, 0, wxALL, 5 );


	this->SetSizer( bSizer12 );
	this->Layout();

	// Connect Events
	m_visualizationChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VisualizationPanel::OnChoice ), NULL, this );
}

VisualizationPanel::~VisualizationPanel()
{
	// Disconnect Events
	m_visualizationChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VisualizationPanel::OnChoice ), NULL, this );

}

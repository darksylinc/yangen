//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Core/YangenWindowImpl.h"

#include "Constants.h"
#include "Core/ResourceListener.h"
#include "Core/TexturePanelImpl.h"
#include "Core/VisualizationPanelImpl.h"

#include "Core/CameraController.h"
#include "Core/wxOgreRenderWindow.h"

#include "Yangen/MaterialSwitcher.h"
#include "Yangen/YangenManager.h"

#include "OgreException.h"
#include "OgreFrameStats.h"
#include "OgreImage2.h"
#include "OgreMesh2.h"
#include "OgreMeshManager2.h"
#include "OgreResourceGroupManager.h"
#include "OgreRoot.h"
#include "OgreWindow.h"

#include "OgreItem.h"

#include "Compositor/OgreCompositorManager2.h"
#include "Compositor/OgreCompositorWorkspace.h"

#include "OgreHlmsCompute.h"
#include "OgreHlmsPbs.h"
#include "OgreHlmsUnlit.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#	include "OSX/macUtils.h"
#endif

#include <wx/aboutdlg.h>
#include <wx/aui/aui.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <wx/wx.h>

#ifdef __WXMSW__
#	include <ShlObj.h>
#endif

YangenWindowImpl::YangenWindowImpl( wxWindow *parent, const CmdSettings &cmdSettings ) :
	YangenWindow( parent ),
	m_wxAuiManager( 0 ),
	m_ogreLog( 0 ),
	m_root( 0 ),
	m_sceneManager( 0 ),
	m_camera( 0 ),
	m_workspace( 0 ),
	m_resourceListener( 0 ),
	m_wxOgreRenderWindow( 0 ),
	m_mainNotebook( 0 ),
	m_texturePanelImpl( 0 ),
	m_visualizationPanelImpl( 0 ),
	m_wasLeftPressed( false ),
	m_wasRightPressed( false ),
	m_mouseX( 0 ),
	m_mouseY( 0 ),
	m_cameraController( 0 ),
	m_yangenManager( 0 ),
	m_materialSwitcher( 0 ),
	m_previewItem( 0 ),
	m_previewSceneNode( 0 )
{
#ifndef __WXMSW__
	// Set config directory to user home directory
	m_configDirectory = std::string( wxGetHomeDir().mb_str() ) + "/.config/yangen/";
	{
		wxString configDir( m_configDirectory.c_str(), wxConvUTF8 );
		if( !wxDirExists( configDir ) )
		{
			if( !wxMkdir( configDir ) )
			{
				wxMessageBox( wxT( "Warning, no R/W access to " ) + configDir +
								  wxT( "\nYangen may not function properly or crash" ),
							  wxT( "ACCESS ERROR" ), wxOK | wxICON_ERROR | wxCENTRE );
				m_configDirectory = "";
			}
		}
		else
		{
			// Folder already exists, but we don't own it
			if( !wxIsReadable( configDir ) || !wxIsWritable( configDir ) )
			{
				wxMessageBox( wxT( "Warning, no R/W access to " ) + configDir +
								  wxT( "\nYangen may not function properly or crash" ),
							  wxT( "ACCESS ERROR" ), wxOK | wxICON_ERROR | wxCENTRE );
			}
		}
	}
#else
	// TODO: Use wxStandardPaths::GetUserConfigDir()
	// Windows: use User/AppData
	TCHAR path[MAX_PATH];
	if( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path ) != S_OK ) )
	{
		// Need to convert to OEM codepage so that fstream can
		// use it properly on international systems.
#	if defined( _UNICODE ) || defined( UNICODE )
		int size_needed =
			WideCharToMultiByte( CP_OEMCP, 0, path, (int)wcslen( path ), NULL, 0, NULL, NULL );
		m_configDirectory = std::string( size_needed, 0 );
		WideCharToMultiByte( CP_OEMCP, 0, path, (int)wcslen( path ), &m_configDirectory[0], size_needed,
							 NULL, NULL );
#	else
		TCHAR oemPath[MAX_PATH];
		CharToOem( path, oemPath );
		m_configDirectory = std::string( oemPath );
#	endif
		m_configDirectory += "\\Yangen\\";

		// Attempt to create directory where config files go
		if( !CreateDirectoryA( m_configDirectory.c_str(), NULL ) &&
			GetLastError() != ERROR_ALREADY_EXISTS )
		{
			// Couldn't create directory (no write access?),
			// fall back to current working dir
			m_configDirectory = "";
		}
	}
#endif

#ifdef __WXMSW__
	SetIcon( wxIcon( wxT( "OgreIcon" ) ) );  // This probably only works in Windows
#else
//	SetIcon( wxIcon(wxT("Resources/OgreIcon.ico")) ); //This probably only works in Windows
#endif

	// Prepare status bar to show FPS, batch count, etc
	int widths[] = { -1, 80, 80, 80, 80 };
	m_statusBar1->SetFieldsCount( sizeof( widths ) / sizeof( int ), widths );

	// Create the Advanced UI system to handle dockable windows
	m_wxAuiManager = new wxAuiManager();
	m_wxAuiManager->SetManagedWindow( this );

	// Don't create the log window by default
	createLogWindow( false );

	// Initialize Ogre and the control that renders it
	initOgre( cmdSettings.setupRenderSystems );

	m_mainNotebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
										wxAUI_NB_BOTTOM | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE |
											wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_EXTERNAL_MOVE );
	m_texturePanelImpl = new TexturePanelImpl( this, m_yangenManager );
	m_visualizationPanelImpl = new VisualizationPanelImpl( this );

	m_mainNotebook->AddPage( m_texturePanelImpl, wxT( "Normal Map" ) );
	m_mainNotebook->AddPage( m_visualizationPanelImpl, wxT( "Visualization" ) );

	m_wxAuiManager->AddPane( m_mainNotebook, wxAuiPaneInfo()
												 .Name( wxT( "TabsPane" ) )
												 .Caption( wxT( "Tabs" ) )
												 .MinSize( 400, -1 )
												 .Left()
												 .Layer( 1 )
												 .CloseButton( false )
												 .PaneBorder( false ) );

	// Commit changes made to the AUI manager
	m_wxAuiManager->Update();

#if TODO
	// Initialize internal, permanent resources
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup( c_InterMeshPermGroup, true );

	// Create this one here now to avoid an exception when we try to delete it
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup( c_InternMeshGroup );
#endif

	loadSettings();
}

YangenWindowImpl::~YangenWindowImpl()
{
	// saveSettings();
	unloadPreview();

	delete m_materialSwitcher;
	m_materialSwitcher = 0;

	delete m_yangenManager;
	m_yangenManager = 0;

	delete m_cameraController;
	m_cameraController = 0;

	if( m_wxOgreRenderWindow )
	{
		m_wxOgreRenderWindow->Destroy();
		m_wxOgreRenderWindow = 0;
	}

	Ogre::ResourceGroupManager &resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();
	resourceGroupManager.setLoadingListener( 0 );
	delete m_resourceListener;
	m_resourceListener = 0;

	Ogre::LogManager::getSingleton().getDefaultLog()->removeListener( this );

	if( m_root )
	{
		delete m_root;
		m_root = 0;
	}

	if( m_wxAuiManager )
	{
		m_wxAuiManager->UnInit();
		delete m_wxAuiManager;
		m_wxAuiManager = 0;
	}
}

//-----------------------------------------------------------------------------
// loadSettings()
// Description:
//	Loads user settings from a file (if exists)
// Notes:
//	1) Assumes the values are already defaulted (in case settings file wasn't
//	   found)
//-----------------------------------------------------------------------------
void YangenWindowImpl::loadSettings()
{
	{
		// Load wxAUI layout
		std::ifstream myFile( ( m_configDirectory + c_layoutSettingsFile ).c_str(),
							  std::ios_base::in | std::ios_base::ate | std::ios_base::binary );
		if( myFile.is_open() )
		{
			std::string layoutString;
			layoutString.resize( myFile.tellg() );
			myFile.seekg( 0 );
			myFile.read( &layoutString[0], layoutString.size() );
			m_wxAuiManager->LoadPerspective( wxString( layoutString.c_str(), wxConvUTF8 ), true );
			myFile.close();
		}
	}
}

//-----------------------------------------------------------------------------
// initOgre()
// Description:
//	Initializes Ogre and the control that renders it
//-----------------------------------------------------------------------------
void YangenWindowImpl::initOgre( bool bForceSetup )
{
#ifndef __WXMSW__
	// Set config directory
	// const std::string c_pluginsCfg = "/opt/yangen/";
	const std::string c_pluginsCfg = "";
#else
	const std::string c_pluginsCfg = "";
#endif

	const char *pluginsFile = "Plugins.cfg";

	m_root = new Ogre::Root( c_pluginsCfg + pluginsFile, m_configDirectory + "ogre.cfg",
							 m_configDirectory + "Ogre.log" );
	if( bForceSetup || !m_root->restoreConfig() )
		m_root->showConfigDialog();

	m_root->initialise( false );

	wxOgreRenderWindow::SetOgreRoot( m_root );
	// wxWindow *dummy = new wxWindow( this, wxID_ANY ); //put it in a dummy window to behave nice w/ AUI
	m_wxOgreRenderWindow = new wxOgreRenderWindow( this, wxID_ANY );
	m_wxOgreRenderWindow->Show();
	m_wxOgreRenderWindow->setRenderWindowListener( this );

	m_wxOgreRenderWindow->SetFocus();

	Ogre::LogManager::getSingleton().getDefaultLog()->addListener( this );
	createSystems();

	// Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	// m_wxAuiManager->AddPane( m_wxOgreRenderWindow, wxLEFT|wxTOP, wxT("OGRE Render Window"));
	m_wxAuiManager->AddPane( m_wxOgreRenderWindow, wxAuiPaneInfo()
													   .Name( wxT( "RenderWindow" ) )
													   .Caption( wxT( "OGRE Render Window" ) )
													   .CenterPane()
													   .PaneBorder( false )
													   .MinSize( 256, 256 )
													   .CloseButton( false ) );
}

//-----------------------------------------------------------------------------
// createSystems()
// Description:
//	Creates Ogre secondary stuff like cameras and scene managers
//-----------------------------------------------------------------------------
void YangenWindowImpl::createSystems()
{
	m_resourceListener = new ResourceListener();
	Ogre::ResourceGroupManager &resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();
	resourceGroupManager.setLoadingListener( m_resourceListener );

	m_sceneManager = m_root->createSceneManager( Ogre::ST_GENERIC, 1u );
	m_camera = m_sceneManager->createCamera( "Main Camera" );
	m_camera->setPosition( Ogre::Vector3( 0.0f, 0.0f, 1.25f ) );
	m_camera->setOrientation( Ogre::Quaternion::IDENTITY );
	m_camera->setNearClipDistance( 0.02f );
	m_camera->setFarClipDistance( 10.0f );
	m_camera->setAutoAspectRatio( true );

	m_cameraController = new TF::CameraController( false );

	m_root->addFrameListener( this );

	m_root->clearEventTimes();

	m_sceneManager->setAmbientLight( Ogre::ColourValue( 0.2f, 0.2f, 0.2f ),
									 Ogre::ColourValue( 0.1f, 0.1f, 0.1f ), Ogre::Vector3::UNIT_Y );

	Ogre::Light *sunLight = m_sceneManager->createLight();
	Ogre::SceneNode *lightNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject( sunLight );
	sunLight->setPowerScale( Ogre::Math::PI );
	sunLight->setType( Ogre::Light::LT_DIRECTIONAL );
	sunLight->setDirection( Ogre::Vector3( -1, -1.2, -0.1 ).normalisedCopy() );

	loadResources();

	Ogre::CompositorManager2 *compositorManager = m_root->getCompositorManager2();

	const Ogre::String workspaceName( "Yangen Workspace" );
	if( !compositorManager->hasWorkspaceDefinition( workspaceName ) )
	{
		compositorManager->createBasicWorkspaceDef( workspaceName, Ogre::ColourValue( 0.2f, 0.4f, 0.6f ),
													Ogre::IdString() );
	}

	m_workspace = compositorManager->addWorkspace( m_sceneManager,
												   m_wxOgreRenderWindow->GetRenderWindow()->getTexture(),
												   m_camera, workspaceName, true );

	createPlaneMesh();
	loadPlanePreview();

	m_yangenManager =
		new Ogre::YangenManager( "GeneratedTexture", m_root->getHlmsManager(), compositorManager,
								 m_root->getRenderSystem()->getTextureGpuManager(), m_sceneManager );
	m_materialSwitcher = new Ogre::MaterialSwitcher( m_root->getHlmsManager(), m_yangenManager );
	try
	{
		m_yangenManager->loadFromDiffusemap( "Sample04.png",
											 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		m_materialSwitcher->prepareMaterials();
		m_yangenManager->process();

		m_previewItem->setDatablock(
			m_materialSwitcher->getDatablock( Ogre::YangenVisualizationModes::FinalRender ) );

		Ogre::TextureGpu *heightmapTex = m_yangenManager->getHeightMap();
		heightmapTex->waitForMetadata();
		const float aspectRatio = static_cast<float>( heightmapTex->getWidth() ) /
								  static_cast<float>( heightmapTex->getHeight() );
		m_previewSceneNode->setScale( aspectRatio, 1.0f, 1.0f );
	}
	catch( Ogre::Exception &e )
	{
		Ogre::LogManager::getSingleton().logMessage( e.getFullDescription() );
		throw;
	}
}

//-----------------------------------------------------------------------------
void YangenWindowImpl::addResourceLocation( const Ogre::String &archName, const Ogre::String &typeName,
											const Ogre::String &secName )
{
#if( OGRE_PLATFORM == OGRE_PLATFORM_APPLE ) || ( OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS )
	// OS X does not set the working directory relative to the app,
	// In order to make things portable on OS X we need to provide
	// the loading with it's own bundle path location
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		Ogre::String( Ogre::macBundlePath() + "/" + archName ), typeName, secName );
#else
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName );
#endif
}

//-----------------------------------------------------------------------------
// loadResources()
// Description:
//	Adds and initializes Ogre resource locations
//-----------------------------------------------------------------------------
void YangenWindowImpl::loadResources()
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load( "../Data/Resources.cfg" );

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while( seci.hasMoreElements() )
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

		if( secName != "Hlms" )
		{
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for( i = settings->begin(); i != settings->end(); ++i )
			{
				typeName = i->first;
				archName = i->second;
				addResourceLocation( archName, typeName, secName );
			}
		}
	}

	registerHlms();

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups( true );
}

//-----------------------------------------------------------------------------
// registerHlms()
// Description:
//	Registers Hlms
//-----------------------------------------------------------------------------
void YangenWindowImpl::registerHlms()
{
	Ogre::ConfigFile cf;
	cf.load( "../Data/Resources.cfg" );

	Ogre::String resourcePath = "";

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	Ogre::String rootHlmsFolder =
		Ogre::macBundlePath() + '/' + cf.getSetting( "DoNotUseAsResource", "Hlms", "" );
#else
	Ogre::String rootHlmsFolder = resourcePath + cf.getSetting( "DoNotUseAsResource", "Hlms", "" );
#endif

	if( rootHlmsFolder.empty() )
		rootHlmsFolder = "./";
	else if( *( rootHlmsFolder.end() - 1 ) != '/' )
		rootHlmsFolder += "/";

	// At this point rootHlmsFolder should be a valid path to the Hlms data folder

	Ogre::HlmsUnlit *hlmsUnlit = 0;
	Ogre::HlmsPbs *hlmsPbs = 0;

	// For retrieval of the paths to the different folders needed
	Ogre::String mainFolderPath;
	Ogre::StringVector libraryFoldersPaths;
	Ogre::StringVector::const_iterator libraryFolderPathIt;
	Ogre::StringVector::const_iterator libraryFolderPathEn;

	Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();
	Ogre::HlmsManager *hlmsManager = m_root->getHlmsManager();

	{
		// Create & Register HlmsUnlit
		// Get the path to all the subdirectories used by HlmsUnlit
		Ogre::HlmsUnlit::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
		Ogre::Archive *archiveUnlit =
			archiveManager.load( rootHlmsFolder + mainFolderPath, "FileSystem", true );
		Ogre::ArchiveVec archiveUnlitLibraryFolders;
		libraryFolderPathIt = libraryFoldersPaths.begin();
		libraryFolderPathEn = libraryFoldersPaths.end();
		while( libraryFolderPathIt != libraryFolderPathEn )
		{
			Ogre::Archive *archiveLibrary =
				archiveManager.load( rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
			archiveUnlitLibraryFolders.push_back( archiveLibrary );
			++libraryFolderPathIt;
		}

		// Create and register the unlit Hlms
		hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit( archiveUnlit, &archiveUnlitLibraryFolders );
		hlmsManager->registerHlms( hlmsUnlit );
	}

	{
		// Create & Register HlmsPbs
		// Do the same for HlmsPbs:
		Ogre::HlmsPbs::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
		Ogre::Archive *archivePbs =
			archiveManager.load( rootHlmsFolder + mainFolderPath, "FileSystem", true );

		// Get the library archive(s)
		Ogre::ArchiveVec archivePbsLibraryFolders;
		libraryFolderPathIt = libraryFoldersPaths.begin();
		libraryFolderPathEn = libraryFoldersPaths.end();
		while( libraryFolderPathIt != libraryFolderPathEn )
		{
			Ogre::Archive *archiveLibrary =
				archiveManager.load( rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
			archivePbsLibraryFolders.push_back( archiveLibrary );
			++libraryFolderPathIt;
		}

		// Create and register
		hlmsPbs = OGRE_NEW Ogre::HlmsPbs( archivePbs, &archivePbsLibraryFolders );
		hlmsManager->registerHlms( hlmsPbs );
	}

	Ogre::RenderSystem *renderSystem = m_root->getRenderSystem();
	if( renderSystem->getName() == "Direct3D11 Rendering Subsystem" )
	{
		// Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
		// and below to avoid saturating AMD's discard limit (8MB) or
		// saturate the PCIE bus in some low end machines.
		bool supportsNoOverwriteOnTextureBuffers;
		renderSystem->getCustomAttribute( "MapNoOverwriteOnDynamicBufferSRV",
										  &supportsNoOverwriteOnTextureBuffers );

		if( !supportsNoOverwriteOnTextureBuffers )
		{
			hlmsPbs->setTextureBufferDefaultSize( 512 * 1024 );
			hlmsUnlit->setTextureBufferDefaultSize( 512 * 1024 );
		}
	}
}

//-----------------------------------------------------------------------------
// createLogWindow()
// Description:
//	Creates a text window which displays Ogre log messages.
//	If the window was hidden, it is displayed again
// Notes:
//	1) This only creates the wx control. The log listener is registered
//	   in initOgre()
// Input:
//	1) True to show it, false to hide the window
//-----------------------------------------------------------------------------
void YangenWindowImpl::createLogWindow( bool bShow )
{
	// Create the log window
	if( !m_ogreLog )
	{
		m_ogreLog =
			new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 500, 150 ),
							wxNO_BORDER | wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_RICH | wxTE_RICH2 );
		m_ogreLog->SetEditable( false );

		m_wxAuiManager->AddPane( m_ogreLog, wxAuiPaneInfo()
												.Name( wxT( "LogWindow" ) )
												.Caption( wxT( "Ogre Log" ) )
												.Bottom()
												.Layer( 0 )
												.Position( 2 )
												.LeftDockable( false )
												.RightDockable( false )
												.CloseButton()
												.DestroyOnClose( false ) );

		wxAuiPaneInfo &logPane = m_wxAuiManager->GetPane( wxT( "LogWindow" ) );
		logPane.Hide();

		m_wxAuiManager->Update();
	}

	if( m_ogreLog && bShow )
	{
		// Already created, let's see if it needs to be shown again
		if( !m_ogreLog->IsShown() )
		{
			// m_ogreLog->Show( true );
			wxAuiPaneInfo &logPane = m_wxAuiManager->GetPane( wxT( "LogWindow" ) );
			logPane.Show();
			m_wxAuiManager->Update();
		}
	}
}
//-----------------------------------------------------------------------------
/**
@brief YangenWindowImpl::setVisualizationMode
	Applies the datablock to the currently loaded items
	based on requested visualization mode
@param visualizationMode
	See YangenVisualizationModes::YangenVisualizationModes
*/
void YangenWindowImpl::setVisualizationMode( uint8_t visualizationMode )
{
	assert( visualizationMode < Ogre::YangenVisualizationModes::NumYangenVisualizationModes );

	if( m_previewItem )
	{
		m_previewItem->setDatablock( m_materialSwitcher->getDatablock(
			static_cast<Ogre::YangenVisualizationModes::YangenVisualizationModes>(
				visualizationMode ) ) );
	}
}

//-----------------------------------------------------------------------------
// frameStarted()
// Description:
//
// Input:
//	1)
//-----------------------------------------------------------------------------
bool YangenWindowImpl::frameStarted( const Ogre::FrameEvent &evt )
{
	m_cameraController->update( evt.timeSinceLastFrame, m_camera );

#ifdef DEBUG
	m_yangenManager->process();
#endif

	// update statusbar info
	const Ogre::FrameStats *frameStats = m_root->getFrameStats();

	m_statusBar1->SetStatusText( wxString::Format( wxT( "%0.2f FPS" ), frameStats->getAvgFps() ), 1 );

	return true;
}

//-----------------------------------------------------------------------------
// OnMouseEvents()
// Description:
//	Called when our render window receives mouse events
// Input:
//	1)
//-----------------------------------------------------------------------------
void YangenWindowImpl::OnMouseEvents( wxMouseEvent &evt )
{
	const int oldX = m_mouseX;
	const int oldY = m_mouseY;

	if( evt.LeftIsDown() )
	{
		evt.GetPosition( &m_mouseX, &m_mouseY );

		if( m_wasLeftPressed )
		{
			int x = m_mouseX - oldX;
			int y = m_mouseY - oldY;
			m_cameraController->mouseMoved( x * 2, y * 2, m_wxOgreRenderWindow->GetRenderWindow() );
		}
	}
	else if( evt.RightIsDown() )
	{
	}
	/*else if( evt.RightIsDown() )
	{
		evt.GetPosition( &m_mouseX, &m_mouseY );

		if( m_wasRightPressed )
		{
			if( !evt.ShiftDown() )
				zoomInCamera( (m_mouseY - oldY) * 0.05f );
			else
				slideCamera( m_mouseX - oldX, m_mouseY - oldY );
		}
	}

	//TODO: Wheel event seems being missed...
	if( evt.GetWheelRotation() )
		zoomInCamera( -evt.GetWheelRotation()/(float)evt.GetWheelDelta() );*/

	m_wasLeftPressed = evt.LeftIsDown();
	m_wasRightPressed = evt.RightIsDown();

	m_wxOgreRenderWindow->Update();

	evt.Skip();
}

void YangenWindowImpl::OnKeyDown( wxKeyEvent &evt )
{
	int keyCode = evt.GetKeyCode();
	const float fScaler = 1.0f;
	m_cameraController->mCameraSpeedBoost = 20.0f * fScaler;
	if( !evt.ControlDown() && !evt.CmdDown() )
		m_cameraController->keyPressed( keyCode, evt.ShiftDown() );
	evt.Skip();
}

void YangenWindowImpl::OnKeyUp( wxKeyEvent &evt )
{
	const int keyCode = evt.GetKeyCode();
	m_cameraController->keyReleased( keyCode, evt.ShiftDown() );
	evt.Skip();
}

void YangenWindowImpl::messageLogged( const Ogre::String &message, Ogre::LogMessageLevel lml,
									  bool maskDebug, const Ogre::String &logName,
									  bool &skipThisMessage )
{
	static const wxColour c_darkYellow( 130, 127, 0 );
	static const wxColour c_darkGreen( 0, 127, 0 );

	// now if we got an error, show the log!
	wxString msg = wxString( ( message + "\n" ).c_str(), wxConvUTF8 );

	// black is the default
	m_ogreLog->SetDefaultStyle( wxTextAttr( *wxBLACK ) );

	// check for the levels
	if( lml == Ogre::LML_CRITICAL )
		m_ogreLog->SetDefaultStyle( wxTextAttr( *wxRED ) );
	else if( lml == Ogre::LML_TRIVIAL )
		m_ogreLog->SetDefaultStyle( wxTextAttr( c_darkGreen ) );

	// then look for keywords
	if( message.find( "you should upgrade it as soon as possible using the OgreMeshUpgrade tool" ) !=
		Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle( wxTextAttr( c_darkYellow ) );
	}
	else if( message.find( "WARNING:" ) != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle( wxTextAttr( c_darkYellow ) );
	}
	else if( message.find( "Can't assign material " ) != Ogre::String::npos )
	{
		createLogWindow( true );
		m_ogreLog->SetDefaultStyle( wxTextAttr( *wxRED ) );
	}
	else if( message.find( "Compiler error: " ) != Ogre::String::npos )
	{
		createLogWindow( true );
		m_ogreLog->SetDefaultStyle( wxTextAttr( *wxRED ) );
	}
	else if( message.find( "Error loading texture " ) != Ogre::String::npos )
	{
		createLogWindow( true );
		m_ogreLog->SetDefaultStyle( wxTextAttr( *wxRED ) );
	}
	else if( message.find( "Mesh: Loading " ) != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle( wxTextAttr( c_darkGreen ) );
	}
	else if( message.find( "Loading 2D Texture" ) != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle( wxTextAttr( c_darkGreen ) );
	}
	else if( message.find( "Texture: " ) != Ogre::String::npos )
	{
		m_ogreLog->SetDefaultStyle( wxTextAttr( c_darkGreen ) );
	}

	if( message.find( "D3D9 Device " ) != Ogre::String::npos )
	{
		// discard this ...
		return;
	}
	else if( message.find( "D3D9 device: " ) != Ogre::String::npos )
	{
		// discard this ...
		return;
	}
	else if( message.find( "!!! Direct3D Device successfully restored." ) != Ogre::String::npos )
	{
		// discard this ...
		return;
	}

	m_ogreLog->AppendText( msg );

	/* Disabled: Ogre Log window should not steal focus, it makes editing editboxes
	very hard.
	if( m_ogreLog->IsShown() )
	{
		//HACK: Seems only way to smoothly move update it (may be wxWidgets bug?)
		m_ogreLog->SetFocus();
	}*/
}
//-----------------------------------------------------------------------------
void YangenWindowImpl::createPlaneMesh()
{
	Ogre::v1::MeshPtr planeMeshV1 = Ogre::v1::MeshManager::getSingleton().createPlane(
		"Plane v1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane( Ogre::Vector3::UNIT_Z, 0.0f ), 1.0f, 1.0f, 1, 1, true, 1, 1.0f, 1.0f,
		Ogre::Vector3::UNIT_Y, Ogre::v1::HardwareBuffer::HBU_STATIC,
		Ogre::v1::HardwareBuffer::HBU_STATIC );

	Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton().createManual(
		"Plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

	planeMeshV1->buildTangentVectors( Ogre::VES_TANGENT, 0, 0, true, true, true );

	planeMesh->importV1( planeMeshV1.get(), true, true, true );

	Ogre::v1::MeshManager::getSingleton().remove( planeMeshV1 );
}
//-----------------------------------------------------------------------------
void YangenWindowImpl::loadPlanePreview()
{
	m_previewItem = m_sceneManager->createItem(
		"Plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::SCENE_STATIC );
	// m_previewItem->setDatablock( "Marble" );

	m_previewSceneNode = m_sceneManager->getRootSceneNode( Ogre::SCENE_STATIC )
							 ->createChildSceneNode( Ogre::SCENE_STATIC );
	m_previewSceneNode->attachObject( m_previewItem );
}
//-----------------------------------------------------------------------------
void YangenWindowImpl::unloadPreview()
{
	if( m_previewSceneNode )
	{
		m_previewSceneNode->getParentSceneNode()->removeAndDestroyChild( m_previewSceneNode );
		m_previewSceneNode = 0;
	}

	if( m_previewItem )
	{
		m_sceneManager->destroyItem( m_previewItem );
		m_previewItem = 0;
	}
}
//-----------------------------------------------------------------------------
void YangenWindowImpl::loadTexture( const Ogre::String &diffuseFullpath,
									const Ogre::String &heightmapFullpath )
{
	try
	{
		if( !diffuseFullpath.empty() && !heightmapFullpath.empty() )
		{
			m_yangenManager->loadFromDiffuseAndHeightmap( diffuseFullpath, "Listener Group",
														  heightmapFullpath, "Listener Group" );
		}
		else
		{
			if( diffuseFullpath.empty() )
				m_yangenManager->loadFromHeightmap( heightmapFullpath, "Listener Group" );
			else
				m_yangenManager->loadFromDiffusemap( diffuseFullpath, "Listener Group" );
		}
		m_materialSwitcher->prepareMaterials();
		m_yangenManager->process();

		Ogre::TextureGpu *heightmapTex = m_yangenManager->getHeightMap();
		heightmapTex->waitForMetadata();
		const float aspectRatio = static_cast<float>( heightmapTex->getWidth() ) /
								  static_cast<float>( heightmapTex->getHeight() );
		m_previewSceneNode->setScale( aspectRatio, 1.0f, 1.0f );
		m_previewSceneNode->_getFullTransformUpdated();
	}
	catch( Ogre::Exception &e )
	{
		Ogre::LogManager::getSingleton().logMessage( e.getFullDescription() );
		throw;
	}
}
//-----------------------------------------------------------------------------
void YangenWindowImpl::saveTexture( const Ogre::String &fullpath, Ogre::TextureGpu *texToSave )
{
	try
	{
		Ogre::Image2 image;
		image.convertFromTexture( texToSave, 0u, 0u );
		/*image._setAutoDelete( false );
		image.loadDynamicImage();*/
		image.save( fullpath, 0u, 1u );
	}
	catch( Ogre::Exception &e )
	{
		Ogre::LogManager::getSingleton().logMessage( e.getFullDescription() );
		throw;
	}
}
//-----------------------------------------------------------------------------
/**
@brief YangenWindowImpl::stripFilenameFromPath
	Takes a Windows or UNIX fullpath (e.g. /home/user/filaname.png or C:\Folder\filename.png)
	and strips the filename from the path (e.g. /home/user and C:\Folder)
@param inFullpath
@param outFolder
*/
void YangenWindowImpl::stripFilenameFromPath( const wxString &inFullpath, wxString &outFolder )
{
	const size_t pos = outFolder.find_last_of( "/\\" );
	outFolder = inFullpath.substr( 0, pos );
}
//-----------------------------------------------------------------------------
/**
@brief YangenWindowImpl::splitFilenameAndFolderFromPath
	Takes a Windows or UNIX fullpath (e.g. /home/user/filaname.png or C:\Folder\filename.png)
	and splits the filename & folder from the path
	(e.g. outFolder = /home/user or C:\Folder and outFilename = filename.png)
@param inFullpath
@param outFolder
@param outFilename
*/
void YangenWindowImpl::splitFilenameAndFolderFromPath( const wxString &inFullpath, wxString &outFolder,
													   wxString &outFilename )
{
	assert( &inFullpath != &outFolder );
	assert( &inFullpath != &outFilename );
	const size_t pos = inFullpath.find_last_of( "/\\" );
	outFolder = inFullpath.substr( 0, pos );
	outFilename = inFullpath.substr( pos + 1u );
}
//-----------------------------------------------------------------------------
// loadTextureDialog()
// Description:
//	Shows a dialog so the user can choose a file to load
//-----------------------------------------------------------------------------
void YangenWindowImpl::loadTextureDialog( bool bHeightmap )
{
	wxFileDialog openFileDialog( this, _( "Open Texture file" ), m_lastOpenDir, wxT( "" ),
								 wxT( "*.png;*.jpg;*.bmp" ), wxFD_OPEN | wxFD_FILE_MUST_EXIST,
								 wxDefaultPosition );

	if( openFileDialog.ShowModal() == wxID_OK )
	{
		stripFilenameFromPath( openFileDialog.GetPath(), m_lastOpenDir );

		Ogre::String diffuseFullpath = Ogre::String( openFileDialog.GetPath().mb_str() );
		Ogre::String heightmapFullpath;
		if( bHeightmap )
			diffuseFullpath.swap( heightmapFullpath );
		loadTexture( diffuseFullpath, heightmapFullpath );
	}
}
//-----------------------------------------------------------------------------
void YangenWindowImpl::loadTextureDialogBoth()
{
	wxFileDialog openFileDialog( this, wxT( "Open DIFFUSE file" ), m_lastOpenDir, wxT( "" ),
								 wxT( "*.png;*.jpg;*.bmp" ), wxFD_OPEN | wxFD_FILE_MUST_EXIST,
								 wxDefaultPosition );

	if( openFileDialog.ShowModal() == wxID_OK )
	{
		Ogre::String diffuseFullpath = Ogre::String( openFileDialog.GetPath().mb_str() );
		openFileDialog.SetMessage( wxT( "Open HEIGHTMAP file" ) );
		if( openFileDialog.ShowModal() == wxID_OK )
		{
			stripFilenameFromPath( openFileDialog.GetPath(), m_lastOpenDir );
			Ogre::String heightmapFullpath = Ogre::String( openFileDialog.GetPath().mb_str() );
			loadTexture( diffuseFullpath, heightmapFullpath );
		}
	}
}
//-----------------------------------------------------------------------------
// saveTextureDialog()
// Description:
//	Shows a dialog so the user can choose a file to load
//-----------------------------------------------------------------------------
void YangenWindowImpl::saveTextureDialog( bool bSavingNormalmap )
{
	wxFileDialog saveFileDialog( this, _( "Save Texture file" ), m_lastOpenDir, wxT( "" ),
								 wxT( "*.png" ), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition );

	if( saveFileDialog.ShowModal() == wxID_OK )
	{
		stripFilenameFromPath( saveFileDialog.GetPath(), m_lastOpenDir );

		const Ogre::String fullpath = Ogre::String( saveFileDialog.GetPath().mb_str() );

		Ogre::TextureGpu *texToSave =
			bSavingNormalmap ? m_yangenManager->getNormalMap() : m_yangenManager->getRoughnessMap();
		saveTexture( fullpath, texToSave );
	}
}
//-----------------------------------------------------------------------------
// saveTextureDialog()
// Description:
//	Shows a dialog so the user can choose a file to load
//-----------------------------------------------------------------------------
void YangenWindowImpl::saveAllTexturesDialog()
{
	wxFileDialog saveFileDialog( this, _( "Save Texture file" ), m_lastOpenDir, wxT( "" ),
								 wxT( "*.png" ), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition );

	if( saveFileDialog.ShowModal() == wxID_OK )
	{
		wxString fullPath = saveFileDialog.GetPath();

		wxString filename, extension;
		splitFilenameAndFolderFromPath( saveFileDialog.GetPath(), m_lastOpenDir, filename );

		{
			size_t pos = filename.find_last_of( '.' );
			if( pos == wxString::npos )
			{
				filename += ".png";
				pos = filename.find_last_of( '.' );
			}
			extension = filename.substr( pos );
			filename = filename.substr( 0u, pos );
		}

		wxString finalPath = m_lastOpenDir + "/" + filename;

		const size_t beforeExtension = finalPath.size();

		finalPath.resize( beforeExtension );
		finalPath += "_nm" + extension;
		saveTexture( std::string( finalPath.mb_str() ), m_yangenManager->getNormalMap() );

		finalPath.resize( beforeExtension );
		finalPath += "_roughness" + extension;
		saveTexture( std::string( finalPath.mb_str() ), m_yangenManager->getRoughnessMap() );
	}
}
//-----------------------------------------------------------------------------
// OnMenuSelection()
// Description:
//	Reacts to menu events (open, save, etc)
//-----------------------------------------------------------------------------
void YangenWindowImpl::OnMenuSelection( wxCommandEvent &event )
{
	switch( event.GetId() )
	{
	case wxID_LOAD_DIFFUSE:
		loadTextureDialog( false );
		break;
	case wxID_LOAD_HEIGHTMAP:
		loadTextureDialog( true );
		break;
	case wxID_LOAD_DIFFUSE_AND_HEIGHTMAP:
		loadTextureDialogBoth();
		break;
	case wxID_SAVEFILE:
		saveAllTexturesDialog();
		break;
	case wxID_SAVE_NORMAL_MAP:
		saveTextureDialog( true );
		break;
	case wxID_SAVE_ROUGHNESS_MAP:
		saveTextureDialog( false );
		break;
	case wxID_COPY_PRESET:
		m_texturePanelImpl->dumpPresetToClipboard();
		break;
	case wxID_RELOAD_SHADERS:
	{
		Ogre::HlmsManager *hlmsManager = m_root->getHlmsManager();

		// Ogre::Hlms *hlms = hlmsManager->getHlms( Ogre::HLMS_USER3 );
		Ogre::GpuProgramManager::getSingleton().clearMicrocodeCache();
		// hlms->reloadFrom( hlms->getDataFolder() );
		hlmsManager->getComputeHlms()->reloadFrom( 0, 0 );
	}
	break;
	default:
		event.Skip();
		break;
	}

	/*7.0f, 0.8f,
	20.0f, 13u, 4.6f,
	0, 1u, 0.0,

	2u, 15u,
	0.42f, -0.32, 0.24*/

	struct YangenPreset
	{
		float nmStrength[3];
		float nmSteepness[3];
		uint8_t nmRadius[2];

		uint8_t roughnessBlurOffset;
		uint8_t roughnessAmplitude;
		float roughnessMidpoint;
		float roughnessScale;
		float roughnessExponent;
	};

	/*YangenPreset preset;

	preset.nmStrength[%i] = %f;
	preset.nmSteepness[%i] = %f;*/
}

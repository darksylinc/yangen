//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Core/CameraController.h"

#include "OgreCamera.h"
#include "OgreWindow.h"

#include "wx/defs.h"

namespace TF
{
	CameraController::CameraController( bool useSceneNode ) :
		mUseSceneNode( useSceneNode ),
		mSpeedMofifier( false ),
		mCameraYaw( 0 ),
		mCameraPitch( 0 ),
		mCameraBaseSpeed( 2 ),
		mCameraSpeedBoost( 20 )
	{
		memset( mWASD, 0, sizeof( mWASD ) );
		memset( mSlideUpDown, 0, sizeof( mSlideUpDown ) );
	}
	//-----------------------------------------------------------------------------------
	void CameraController::update( float timeSinceLast, Ogre::Camera *camera )
	{
		if( mCameraYaw || mCameraPitch )
		{
			if( mUseSceneNode )
			{
				Ogre::Node *cameraNode = camera->getParentNode();

				// Update now as yaw needs the derived orientation.
				cameraNode->_getFullTransformUpdated();
				cameraNode->yaw( Ogre::Radian( mCameraYaw ), Ogre::Node::TS_WORLD );
				cameraNode->pitch( Ogre::Radian( mCameraPitch ) );
			}
			else
			{
				camera->yaw( Ogre::Radian( mCameraYaw ) );
				camera->pitch( Ogre::Radian( mCameraPitch ) );
			}

			mCameraYaw = 0.0f;
			mCameraPitch = 0.0f;
		}

		int camMovementZ = mWASD[2] - mWASD[0];
		int camMovementX = mWASD[3] - mWASD[1];
		int slideUpDown = mSlideUpDown[0] - mSlideUpDown[1];

		if( camMovementZ || camMovementX || slideUpDown )
		{
			Ogre::Vector3 camMovementDir( camMovementX, slideUpDown, camMovementZ );
			camMovementDir.normalise();
			camMovementDir *=
				timeSinceLast * mCameraBaseSpeed * ( 1 + mSpeedMofifier * mCameraSpeedBoost );

			if( mUseSceneNode )
			{
				Ogre::Node *cameraNode = camera->getParentNode();
				cameraNode->translate( camMovementDir, Ogre::Node::TS_LOCAL );
			}
			else
			{
				camera->moveRelative( camMovementDir );
			}
		}
	}
	//-----------------------------------------------------------------------------------
	bool CameraController::keyPressed( int keyCode, bool shiftEnabled )
	{
		mSpeedMofifier = shiftEnabled;

		if( keyCode == 'W' )
			mWASD[0] = true;
		else if( keyCode == 'A' )
			mWASD[1] = true;
		else if( keyCode == 'S' )
			mWASD[2] = true;
		else if( keyCode == 'D' )
			mWASD[3] = true;
		else if( keyCode == 'Q' || keyCode == WXK_PAGEUP )
			mSlideUpDown[0] = true;
		else if( keyCode == 'E' || keyCode == WXK_PAGEDOWN )
			mSlideUpDown[1] = true;
		else
			return false;

		return true;
	}
	//-----------------------------------------------------------------------------------
	bool CameraController::keyReleased( int keyCode, bool shiftEnabled )
	{
		if( keyCode == 'W' )
			mWASD[0] = false;
		else if( keyCode == 'A' )
			mWASD[1] = false;
		else if( keyCode == 'S' )
			mWASD[2] = false;
		else if( keyCode == 'D' )
			mWASD[3] = false;
		else if( keyCode == 'Q' || keyCode == WXK_PAGEUP )
			mSlideUpDown[0] = false;
		else if( keyCode == 'E' || keyCode == WXK_PAGEDOWN )
			mSlideUpDown[1] = false;
		else
		{
			if( shiftEnabled )
				mSpeedMofifier = false;
			return false;
		}

		return true;
	}
	//-----------------------------------------------------------------------------------
	void CameraController::mouseMoved( int x, int y, Ogre::Window *renderWindow )
	{
		float width = static_cast<float>( renderWindow->getWidth() );
		float height = static_cast<float>( renderWindow->getHeight() );

		mCameraYaw += -x / width;
		mCameraPitch += -y / height;
	}
}  // namespace TF

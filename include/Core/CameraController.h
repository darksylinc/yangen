//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

#include "OgrePrerequisites.h"

namespace TF
{
    class CameraController
    {
        bool                mUseSceneNode;

        bool                mSpeedMofifier;
        bool                mWASD[4];
        bool                mSlideUpDown[2];
        float               mCameraYaw;
        float               mCameraPitch;
        public: float       mCameraBaseSpeed;
        public: float       mCameraSpeedBoost;

    public:
		CameraController( bool useSceneNode=false );

		void update( float timeSinceLast, Ogre::Camera *camera );

        /// Returns true if we've handled the event
		bool keyPressed( int keyCode, bool shiftEnabled );
        /// Returns true if we've handled the event
		bool keyReleased( int keyCode , bool shiftEnabled );

		void mouseMoved( int x, int y, Ogre::Window *renderWindow );
    };
}

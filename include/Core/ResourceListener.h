//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#pragma once

#include "OgreResourceGroupManager.h"

class ResourceListener : public Ogre::ResourceLoadingListener
{
public:
	virtual Ogre::DataStreamPtr resourceLoading( const Ogre::String &name, const Ogre::String &group,
												 Ogre::Resource *resource );
	virtual void resourceStreamOpened( const Ogre::String &name, const Ogre::String &group,
									   Ogre::Resource *resource, Ogre::DataStreamPtr &dataStream );
	virtual bool resourceCollision( Ogre::Resource *resource, Ogre::ResourceManager *resourceManager );

	virtual bool                grouplessResourceExists( const Ogre::String &name );
	virtual Ogre::DataStreamPtr grouplessResourceLoading( const Ogre::String &name );
	virtual Ogre::DataStreamPtr grouplessResourceOpened( const Ogre::String & name,
														 Ogre::Archive *      archive,
														 Ogre::DataStreamPtr &dataStream );
};

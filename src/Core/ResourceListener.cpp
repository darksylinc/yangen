//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Core/ResourceListener.h"

#include <fstream>

Ogre::DataStreamPtr ResourceListener::resourceLoading( const Ogre::String &name,
													   const Ogre::String &group,
													   Ogre::Resource *resource )
{
	Ogre::DataStreamPtr retVal;
	if( group == "Listener Group" )
	{
		std::ifstream *ifs = OGRE_NEW_T( std::ifstream, Ogre::MEMCATEGORY_GENERAL )(
			name.c_str(), std::ios::binary | std::ios::in );
		if( ifs->is_open() )
		{
			const Ogre::String::size_type extPos = name.find_last_of( '.' );
			if( extPos != Ogre::String::npos )
			{
				const Ogre::String texExt = name.substr( extPos + 1 );
				retVal = Ogre::DataStreamPtr( OGRE_NEW Ogre::FileStreamDataStream( name, ifs ) );
			}
		}
		else
		{
			OGRE_DELETE_T( ifs, basic_ifstream, Ogre::MEMCATEGORY_GENERAL );
		}
	}
	return retVal;
}
//-----------------------------------------------------------------------------
void ResourceListener::resourceStreamOpened( const Ogre::String &name, const Ogre::String &group,
											 Ogre::Resource *resource, Ogre::DataStreamPtr &dataStream )
{
}
//-----------------------------------------------------------------------------
bool ResourceListener::resourceCollision( Ogre::Resource *resource,
										  Ogre::ResourceManager *resourceManager )
{
	return false;
}
//-----------------------------------------------------------------------------
bool ResourceListener::grouplessResourceExists( const Ogre::String &name )
{
	std::ifstream infile( name.c_str() );
	return infile.good();
}
//-----------------------------------------------------------------------------
Ogre::DataStreamPtr ResourceListener::grouplessResourceLoading( const Ogre::String &name )
{
	Ogre::DataStreamPtr retVal;

	std::ifstream *ifs = OGRE_NEW_T( std::ifstream, Ogre::MEMCATEGORY_GENERAL )(
		name.c_str(), std::ios::binary | std::ios::in );
	if( ifs->is_open() )
	{
		const Ogre::String::size_type extPos = name.find_last_of( '.' );
		if( extPos != Ogre::String::npos )
		{
			const Ogre::String texExt = name.substr( extPos + 1 );
			retVal = Ogre::DataStreamPtr( OGRE_NEW Ogre::FileStreamDataStream( name, ifs ) );
		}
	}
	else
	{
		OGRE_DELETE_T( ifs, basic_ifstream, Ogre::MEMCATEGORY_GENERAL );
	}

	return retVal;
}
//-----------------------------------------------------------------------------
Ogre::DataStreamPtr ResourceListener::grouplessResourceOpened( const Ogre::String &name,
															   Ogre::Archive *archive,
															   Ogre::DataStreamPtr &dataStream )
{
	return dataStream;
}

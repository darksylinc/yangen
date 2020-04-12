
#pragma once

#include "OgreLwString.h"

template <size_t T>
class EmptyLwString : public Ogre::LwString
{
protected:
	char buffer[T];

public:
	EmptyLwString() : Ogre::LwString( Ogre::LwString::FromEmptyPointer( buffer, T ) ) {}
};

#-------------------------------------------------------------------
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

project( Yangen )

cmake_minimum_required(VERSION 3.5)

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG" )

if( NOT MSVC )
	set( CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG}			-DDEBUG=1 -D_DEBUG=1 -g"	)
	set( CMAKE_CXX_FLAGS_MINSIZEREL		"${CMAKE_CXX_FLAGS_MINSIZEREL}		-Os -DNDEBUG"				)
	set( CMAKE_CXX_FLAGS_RELEASE		"${CMAKE_CXX_FLAGS_RELEASE}			-O2 -DNDEBUG"				)
	set( CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO}	-O2 -g"						)

	set( CMAKE_C_FLAGS_DEBUG			"${CMAKE_C_FLAGS_DEBUG}				-DDEBUG=1 -D_DEBUG=1 -g"	)
	set( CMAKE_C_FLAGS_MINSIZEREL		"${CMAKE_C_FLAGS_MINSIZEREL}		-Os -DNDEBUG"				)
	set( CMAKE_C_FLAGS_RELEASE			"${CMAKE_C_FLAGS_RELEASE}			-O2 -DNDEBUG"				)
	set( CMAKE_C_FLAGS_RELWITHDEBINFO	"${CMAKE_C_FLAGS_RELWITHDEBINFO}	-O2 -g"						)

	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-overloaded-virtual" )
endif()

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
	set( CMAKE_CXX_FLAGS_DEBUG		"${CMAKE_CXX_FLAGS_DEBUG}			-fno-limit-debug-info"		)
endif()

set( CMAKE_SUPPRESS_REGENERATION TRUE )

if( NOT APPLE )
	if( CMAKE_BUILD_TYPE STREQUAL "" )
		set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "One of: Debug Release RelWithDebInfo MinSizeRel." FORCE )
	endif()
else()
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.ogre3d.\${PRODUCT_NAME:rfc1034identifier}")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
endif()

# Determine if we are compiling for a 32bit or 64bit system
if( NOT APPLE )
	include(CheckTypeSize)
	CHECK_TYPE_SIZE("void*" ARCH_PTR_SIZE BUILTIN_TYPES_ONLY)
	if (ARCH_PTR_SIZE EQUAL 8)
	  set(PLATFORM_X64 TRUE)
	else ()
	  set(PLATFORM_X64 FALSE)
	endif ()
endif()

include( CMake/Dependencies/OGRE.cmake )
include( CMake/Dependencies/wxWidgets.cmake )

setupOgre( OGRE_SOURCE, OGRE_BINARIES, OGRE_LIBRARIES FALSE )
setupWxWidgets( wxWidgets_LIBRARIES GTK2_LIBRARIES OPENGL_LIBRARIES )

# Setup our application
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}" )
if( MSVC )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	if( NOT PLATFORM_X64 )
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
	endif()
	add_definitions( -DUNICODE -D_UNICODE )
endif()

if( APPLE )
	macro( add_recursive dir retVal )
		file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c ${dir}/*.mm ${dir}/*.m )
	endmacro()
else()
	macro( add_recursive dir retVal )
		file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c )
	endmacro()
endif()

include_directories( "./include" )

add_recursive( ./src SOURCES )
add_recursive( ./include HEADERS )

include_directories( ${OGRE_SOURCE}/Dependencies/include )

#if( WIN32 )
#	# Add embedded icon
#	set( SOURCES ${SOURCES} ./scripts/Resources/Resource.rc )
#endif()
#if( APPLE )
#	file( GLOB_RECURSE RESOURCES ./src/*.storyboard )
#	set( RESOURCES ${RESOURCES} ./Data/Resources.cfg ./bin/Data )
#endif()

add_executable( ${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCES} ${HEADERS} )
target_link_libraries( ${PROJECT_NAME} ${OGRE_LIBRARIES} ${wxWidgets_LIBRARIES} )

if( UNIX )
	target_link_libraries( Yangen ${GTK2_LIBRARIES} ${OPENGL_LIBRARIES} )
endif()


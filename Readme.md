# Yet Another Normalmap GENerator (Yangen)

# Compiling

You'll need:

 * At least MSVC 2008 (Express Editions work)
 * OgreNext 4.0 built via [Quick Start scripts](https://bintray.com/darksylinc/ogre-next/download_file?file_path=build_ogre_scripts-master.7z). Otherwise check out [Setting Up Ogre](https://ogrecave.github.io/ogre-next/api/latest/_setting_up_ogre.html) on how to compile it.
 * wxWidgets 3.0 SDK with AUI support
 * CMake 4.0 or so.
 * x64 is highly recommended.

## Windows only:
1. Download wxWidgets 3.0 from <https://www.wxwidgets.org/downloads/>
1. Extract it to C:\Projects\wxWidgets-3.0
1. Go to C:\Projects\wxWidgets-3.0\build\msw and open wx_vc8.sln (VS2008) / wx_vc10.sln (VS2012) / etc. If your Visual Studio version is newer, converting the latest project may just work
1. Compile the non-DLL library builds. It may give you a few errors, just hit compile again. The dependency chain is not well setup so you have to hit build a few times (i.e. it tried to link Project B before finishing compiling Project A)

Create a folder called "Dependencies"
Under it you will create two symbolic links to where Ogre and wxWidgets source code repositories lay:

    mkdir Dependencies
    cd Dependencies
    mklink /D Ogre C:\Projects\Ogre
    mklink /D wxWidgets_3_0 C:\Projects\wxWidgets-3.0


## Ubuntu only:
Install wxWidgets:

    sudo apt-get install libwxgtk3.0-dev

Create a folder called "Dependencies"
Under it you will create two symbolic links to where Ogre source code repositories lay:

    mkdir Dependencies
    cd Dependencies
    ln -s /home/username/Projects/Ogre Ogre

> [!NOTE]  
> Due to an incopatibility between FreeImage/libpng bundled by OgreNext 4.0 and the one bundled by Ubuntu
> you must set environment variable to `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libpng.so` before launching Yangen.

## All platforms:
Set CMake GUI binaries to point to Project/Yangen/build

Click Generate.


# Showcase

Yangen supports Normal Map preview. With 3 different slider sections for small, medium and large details:

![]("Docs/NormalMapExample.mp4")


Yangen also supports generating roughness maps:

![]("Docs/RoughnessExample.mp4")


And of course Yangen supports the final result preview:

![]("Docs/CompletePreviewExample.mp4")


# Uninstall

## Windows

All config files are in %APPDATA%\Yangen
Remove that folder and you're done

## Linux

All config files are in ~/.config/yangen
Remove that folder and you're done

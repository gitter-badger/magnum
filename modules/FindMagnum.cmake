# - Find Magnum
#
# Basic usage:
#  find_package(Magnum [REQUIRED])
# This command tries to find base Magnum library and then defines:
#  MAGNUM_FOUND                 - Whether the base library was found
#  MAGNUM_LIBRARIES             - Magnum library and dependent libraries
#  MAGNUM_INCLUDE_DIRS          - Root include dir and include dirs of
#   dependencies
#  MAGNUM_PLUGINS_DEBUG_DIR     - Base directory with dynamic plugins for
#   debug builds, defaults to magnum-d/ subdirectory of dir where Magnum
#   library was found
#  MAGNUM_PLUGINS_RELEASE_DIR   - Base directory with dynamic plugins for
#   release builds, defaults to magnum/ subdirectory of dir where Magnum
#   library was found
#  MAGNUM_PLUGINS_DIR           - Base directory with dynamic plugins, defaults
#   to MAGNUM_PLUGINS_RELEASE_DIR in release builds and multi-configuration
#   builds or to MAGNUM_PLUGINS_DEBUG_DIR in debug builds. You can modify all
#   three variable (e.g. set them to . when deploying on Windows with plugins
#   stored relatively to the executable), the following MAGNUM_PLUGINS_*_DIR
#   variables depend on it.
#  MAGNUM_PLUGINS_FONT[|_DEBUG|_RELEASE]_DIR - Directory with dynamic font
#   plugins
#  MAGNUM_PLUGINS_FONTCONVERTER[|_DEBUG|_RELEASE]_DIR - Directory with dynamic
#   font converter plugins
#  MAGNUM_PLUGINS_IMAGECONVERTER[|_DEBUG|_RELEASE]_DIR - Directory with dynamic
#   image converter plugins
#  MAGNUM_PLUGINS_IMPORTER[|_DEBUG|_RELEASE]_DIR  - Directory with dynamic
#   importer plugins
#  MAGNUM_PLUGINS_AUDIOIMPORTER[|_DEBUG|_RELEASE]_DIR - Directory with dynamic
#   audio importer plugins
# This command will try to find only the base library, not the optional
# components. The base library depends on Corrade and OpenGL libraries (or
# OpenGL ES libraries). Additional dependencies are specified by the
# components. The optional components are:
#  Audio            - Audio library
#  DebugTools       - DebugTools library
#  MeshTools        - MeshTools library
#  Primitives       - Primitives library
#  SceneGraph       - SceneGraph library
#  Shaders          - Shaders library
#  Shapes           - Shapes library
#  Text             - Text library
#  TextureTools     - TextureTools library
#  MagnumFont       - Magnum bitmap font plugin
#  MagnumFontConverter - Magnum bitmap font converter plugin
#  ObjImporter      - OBJ importer plugin
#  TgaImageConverter - TGA image converter plugin
#  TgaImporter      - TGA importer plugin
#  WavAudioImporter - WAV audio importer plugin
#  GlutApplication  - GLUT application
#  GlxApplication   - GLX application
#  NaClApplication  - NaCl application
#  Sdl2Application  - SDL2 application
#  XEglApplication  - X/EGL application
#  WindowlessCglApplication - Windowless CGL application
#  WindowlessGlxApplication - Windowless GLX application
#  WindowlessNaClApplication - Windowless NaCl application
#  WindowlessWglApplication - Windowless WGL application
#  WindowlessWindowsEglApplication - Windowless Windows/EGL application
#  CglContext       - CGL context
#  EglContext       - EGL context
#  GlxContext       - GLX context
#  WglContext       - WGL context
# Example usage with specifying additional components is:
#  find_package(Magnum [REQUIRED|COMPONENTS]
#               MeshTools Primitives GlutApplication)
# For each component is then defined:
#  MAGNUM_*_FOUND   - Whether the component was found
#  MAGNUM_*_LIBRARIES - Component library and dependent libraries
#  MAGNUM_*_INCLUDE_DIRS - Include dirs of module dependencies
# If exactly one *Application or exactly one Windowless*Application
# component is requested and found, its libraries and include dirs are
# available in convenience aliases MAGNUM_APPLICATION_LIBRARIES /
# MAGNUM_WINDOWLESSAPPLICATION_LIBRARIES and MAGNUM_APPLICATION_INCLUDE_DIRS
# / MAGNUM_WINDOWLESSAPPLICATION_INCLUDE_DIRS to simplify porting. Similarly,
# if exactly one *Context component is requested and found, its libraries and
# include dirs are available in convenience aliases MAGNUM_CONTEXT_LIBRARIES
# and MAGNUM_CONTEXT_INCLUDE_DIRS.
#
# The package is found if either debug or release version of each requested
# library (or plugin) is found. If both debug and release libraries (or
# plugins) are found, proper version is chosen based on actual build
# configuration of the project (i.e. Debug build is linked to debug libraries,
# Release build to release libraries). Note that this autodetection might fail
# for the MAGNUM_PLUGINS_DIR variable, especially on multi-configuration build
# systems. You can make use of CORRADE_IS_DEBUG_BUILD preprocessor variable
# along with MAGNUM_PLUGINS_*_DEBUG_DIR / MAGNUM_PLUGINS_*_RELEASE_DIR
# variables to decide in preprocessing step.
#
# Features of found Magnum library are exposed in these variables:
#  MAGNUM_BUILD_DEPRECATED      - Defined if compiled with deprecated APIs
#   included
#  MAGNUM_BUILD_STATIC          - Defined if compiled as static libraries
#  MAGNUM_TARGET_GLES           - Defined if compiled for OpenGL ES
#  MAGNUM_TARGET_GLES2          - Defined if compiled for OpenGL ES 2.0
#  MAGNUM_TARGET_GLES3          - Defined if compiled for OpenGL ES 3.0
#  MAGNUM_TARGET_DESKTOP_GLES   - Defined if compiled with OpenGL ES
#   emulation on desktop OpenGL
#  MAGNUM_TARGET_WEBGL          - Defined if compiled for WebGL
#
# Additionally these variables are defined for internal usage:
#  MAGNUM_INCLUDE_DIR           - Root include dir (w/o dependencies)
#  MAGNUM_LIBRARY               - Magnum library (w/o dependencies)
#  MAGNUM_LIBRARY_DEBUG         - Debug version of Magnum library, if found
#  MAGNUM_LIBRARY_RELEASE       - Release version of Magnum library, if found
#  MAGNUM_*_LIBRARY             - Component libraries (w/o dependencies)
#  MAGNUM_*_LIBRARY_DEBUG       - Debug version of given library, if found
#  MAGNUM_*_LIBRARY_RELEASE     - Release version of given library, if found
#  MAGNUM_BINARY_INSTALL_DIR    - Binary installation directory
#  MAGNUM_LIBRARY_INSTALL_DIR   - Library installation directory
#  MAGNUM_DATA_INSTALL_DIR      - Data installation directory
#  MAGNUM_PLUGINS_[DEBUG|RELEASE]_INSTALL_DIR - Plugin installation directory
#  MAGNUM_PLUGINS_FONT_[DEBUG|RELEASE]_INSTALL_DIR - Font plugin installation
#   directory
#  MAGNUM_PLUGINS_FONTCONVERTER_[DEBUG|RELEASE]_INSTALL_DIR - Font converter
#   plugin installation directory
#  MAGNUM_PLUGINS_IMAGECONVERTER_[DEBUG|RELEASE]_INSTALL_DIR - Image converter
#   plugin installation directory
#  MAGNUM_PLUGINS_IMPORTER_[DEBUG|RELEASE]_INSTALL_DIR  - Importer plugin
#   installation directory
#  MAGNUM_PLUGINS_AUDIOIMPORTER_[DEBUG|RELEASE]_INSTALL_DIR - Audio importer
#   plugin installation directory
#  MAGNUM_INCLUDE_INSTALL_DIR   - Header installation directory
#  MAGNUM_PLUGINS_INCLUDE_INSTALL_DIR - Plugin header installation directory
#

#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013, 2014, 2015
#             Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

# Dependencies
find_package(Corrade REQUIRED)

# Base Magnum library
find_library(MAGNUM_LIBRARY_DEBUG Magnum-d)
find_library(MAGNUM_LIBRARY_RELEASE Magnum)

# Set the MAGNUM_LIBRARY variable based on what was found, use that information
# to guess also build type of dynamic plugins
if(MAGNUM_LIBRARY_DEBUG AND MAGNUM_LIBRARY_RELEASE)
    set(MAGNUM_LIBRARY
        debug ${MAGNUM_LIBRARY_DEBUG}
        optimized ${MAGNUM_LIBRARY_RELEASE})
    get_filename_component(_MAGNUM_LIBRARY_PATH ${MAGNUM_LIBRARY_DEBUG} PATH)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(_MAGNUM_PLUGINS_DIR_SUFFIX "-d")
    endif()
elseif(MAGNUM_LIBRARY_DEBUG)
    set(MAGNUM_LIBRARY ${MAGNUM_LIBRARY_DEBUG})
    get_filename_component(_MAGNUM_LIBRARY_PATH ${MAGNUM_LIBRARY_DEBUG} PATH)
    set(_MAGNUM_PLUGINS_DIR_SUFFIX "-d")
elseif(MAGNUM_LIBRARY_RELEASE)
    set(MAGNUM_LIBRARY ${MAGNUM_LIBRARY_RELEASE})
    get_filename_component(_MAGNUM_LIBRARY_PATH ${MAGNUM_LIBRARY_RELEASE} PATH)
endif()

# Root include dir
find_path(MAGNUM_INCLUDE_DIR
    NAMES Magnum/Magnum.h)

# We need to open configure.h file from MAGNUM_INCLUDE_DIR before we check for
# the components. Bail out with proper error message if it wasn't found. The
# complete check with all components is further below.
if(NOT MAGNUM_INCLUDE_DIR)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Magnum
        REQUIRED_VARS MAGNUM_LIBRARY MAGNUM_INCLUDE_DIR)
endif()

# Configuration
file(READ ${MAGNUM_INCLUDE_DIR}/Magnum/configure.h _magnumConfigure)
set(_magnumFlags
    BUILD_DEPRECATED
    BUILD_STATIC
    TARGET_GLES
    TARGET_GLES2
    TARGET_GLES3
    TARGET_DESKTOP_GLES
    TARGET_WEBGL)
foreach(_magnumFlag ${_magnumFlags})
    string(FIND "${_magnumConfigure}" "#define MAGNUM_${_magnumFlag}" _magnum_${_magnumFlag})
    if(NOT _magnum_${_magnumFlag} EQUAL -1)
        set(MAGNUM_${_magnumFlag} 1)
    endif()
endforeach()

# Dependent libraries and includes
set(MAGNUM_INCLUDE_DIRS ${MAGNUM_INCLUDE_DIR}
    ${MAGNUM_INCLUDE_DIR}/MagnumExternal/OpenGL
    ${CORRADE_INCLUDE_DIR})
set(MAGNUM_LIBRARIES ${MAGNUM_LIBRARY}
    ${CORRADE_UTILITY_LIBRARIES}
    ${CORRADE_PLUGINMANAGER_LIBRARIES})
if(NOT MAGNUM_TARGET_GLES OR MAGNUM_TARGET_DESKTOP_GLES)
    find_package(OpenGL REQUIRED)
    set(MAGNUM_LIBRARIES ${MAGNUM_LIBRARIES} ${OPENGL_gl_LIBRARY})
elseif(MAGNUM_TARGET_GLES2)
    find_package(OpenGLES2 REQUIRED)
    set(MAGNUM_LIBRARIES ${MAGNUM_LIBRARIES} ${OPENGLES2_LIBRARY})
elseif(MAGNUM_TARGET_GLES3)
    find_package(OpenGLES3 REQUIRED)
    set(MAGNUM_LIBRARIES ${MAGNUM_LIBRARIES} ${OPENGLES3_LIBRARY})
endif()

# Emscripten needs special flag to use WebGL 2
if(CORRADE_TARGET_EMSCRIPTEN AND NOT MAGNUM_TARGET_GLES2 AND NOT CMAKE_EXE_LINKER_FLAGS MATCHES "USE_WEBGL2")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGL2=1")
endif()

# Ensure that all inter-component dependencies are specified as well
set(_MAGNUM_ADDITIONAL_COMPONENTS )
foreach(component ${Magnum_FIND_COMPONENTS})
    string(TOUPPER ${component} _COMPONENT)

    # The dependencies need to be sorted by their dependency order as well
    if(component STREQUAL Shapes)
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES SceneGraph)
    elseif(component STREQUAL Text)
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES TextureTools)
    elseif(component STREQUAL DebugTools)
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES MeshTools Primitives SceneGraph Shaders Shapes)
    elseif(component STREQUAL MagnumFont)
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES TgaImporter) # and below
    elseif(component STREQUAL MagnumFontConverter)
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES TgaImageConverter) # and below
    elseif(component STREQUAL ObjImporter)
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES MeshTools)
    endif()

    if(component MATCHES ".+AudioImporter")
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES ${_MAGNUM_${_COMPONENT}_DEPENDENCIES} Audio)
    elseif(component MATCHES ".+(Font|FontConverter)")
        set(_MAGNUM_${_COMPONENT}_DEPENDENCIES ${_MAGNUM_${_COMPONENT}_DEPENDENCIES} Text TextureTools)
    endif()

    list(APPEND _MAGNUM_ADDITIONAL_COMPONENTS ${_MAGNUM_${_COMPONENT}_DEPENDENCIES})
endforeach()

# Join the lists, remove duplicate components
if(_MAGNUM_ADDITIONAL_COMPONENTS)
    list(INSERT Magnum_FIND_COMPONENTS 0 ${_MAGNUM_ADDITIONAL_COMPONENTS})
endif()
if(Magnum_FIND_COMPONENTS)
    list(REMOVE_DUPLICATES Magnum_FIND_COMPONENTS)
endif()

# Find all components
foreach(component ${Magnum_FIND_COMPONENTS})
    string(TOUPPER ${component} _COMPONENT)

    # AudioImporter plugin specific name suffixes
    if(${component} MATCHES ".+AudioImporter$")
        set(_MAGNUM_${_COMPONENT}_IS_PLUGIN 1)
        set(_MAGNUM_${_COMPONENT}_PATH_SUFFIX audioimporters)

        # Audio importer class is Audio::*Importer, thus we need to convert
        # *AudioImporter.h to *Importer.h
        string(REPLACE "AudioImporter" "Importer" _MAGNUM_${_COMPONENT}_HEADER_NAME "${component}")
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES ${_MAGNUM_${_COMPONENT}_HEADER_NAME}.h)

    # Importer plugin specific name suffixes
    elseif(${component} MATCHES ".+Importer$")
        set(_MAGNUM_${_COMPONENT}_IS_PLUGIN 1)
        set(_MAGNUM_${_COMPONENT}_PATH_SUFFIX importers)

    # Font plugin specific name suffixes
    elseif(${component} MATCHES ".+Font$")
        set(_MAGNUM_${_COMPONENT}_IS_PLUGIN 1)
        set(_MAGNUM_${_COMPONENT}_PATH_SUFFIX fonts)

    # ImageConverter plugin specific name suffixes
    elseif(${component} MATCHES ".+ImageConverter$")
        set(_MAGNUM_${_COMPONENT}_IS_PLUGIN 1)
        set(_MAGNUM_${_COMPONENT}_PATH_SUFFIX imageconverters)

    # FontConverter plugin specific name suffixes
    elseif(${component} MATCHES ".+FontConverter$")
        set(_MAGNUM_${_COMPONENT}_IS_PLUGIN 1)
        set(_MAGNUM_${_COMPONENT}_PATH_SUFFIX fontconverters)
    endif()

    # Set plugin defaults, find the plugin
    if(_MAGNUM_${_COMPONENT}_IS_PLUGIN)
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_SUFFIX MagnumPlugins/${component})

        # Don't override the one for *AudioImporter plugins
        if(NOT _MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES)
            set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES ${component}.h)
        endif()

        # Dynamic plugins don't have any prefix (e.g. `lib` on Linux), search
        # with empty prefix and then reset that back so we don't accidentaly
        # break something else
        set(_tmp_prefixes ${CMAKE_FIND_LIBRARY_PREFIXES})
        set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")

        find_library(MAGNUM_${_COMPONENT}_LIBRARY ${component}
            PATH_SUFFIXES magnum/${_MAGNUM_${_COMPONENT}_PATH_SUFFIX})

        # Try to find both debug and release version. Dynamic and static debug
        # libraries are on different places.
        find_library(MAGNUM_${_COMPONENT}_LIBRARY_DEBUG ${component}
            PATH_SUFFIXES magnum-d/${_MAGNUM_${_COMPONENT}_PATH_SUFFIX})
        find_library(MAGNUM_${_COMPONENT}_LIBRARY_DEBUG ${component}-d
            PATH_SUFFIXES magnum/${_MAGNUM_${_COMPONENT}_PATH_SUFFIX})
        find_library(MAGNUM_${_COMPONENT}_LIBRARY_RELEASE ${component}
            PATH_SUFFIXES magnum/${_MAGNUM_${_COMPONENT}_PATH_SUFFIX})

        # Set the _LIBRARY variable based on what was found
        if(MAGNUM_${_COMPONENT}_LIBRARY_DEBUG AND MAGNUM_${_COMPONENT}_LIBRARY_RELEASE)
            set(MAGNUM_${_COMPONENT}_LIBRARY
                debug ${MAGNUM_${_COMPONENT}_LIBRARY_DEBUG}
                optimized ${MAGNUM_${_COMPONENT}_LIBRARY_RELEASE})
        elseif(MAGNUM_${_COMPONENT}_LIBRARY_DEBUG)
            set(MAGNUM_${_COMPONENT}_LIBRARY ${MAGNUM_${_COMPONENT}_LIBRARY_DEBUG})
        elseif(MAGNUM_${_COMPONENT}_LIBRARY_RELEASE)
            set(MAGNUM_${_COMPONENT}_LIBRARY ${MAGNUM_${_COMPONENT}_LIBRARY_RELEASE})
        endif()

        set(CMAKE_FIND_LIBRARY_PREFIXES ${_tmp_prefixes})

    # Set library defaults, find the library
    else()
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_SUFFIX Magnum/${component})
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES ${component}.h)

        # Try to find both debug and release version
        find_library(MAGNUM_${_COMPONENT}_LIBRARY_DEBUG Magnum${component}-d)
        find_library(MAGNUM_${_COMPONENT}_LIBRARY_RELEASE Magnum${component})

        # Set the _LIBRARY variable based on what was found
        if(MAGNUM_${_COMPONENT}_LIBRARY_DEBUG AND MAGNUM_${_COMPONENT}_LIBRARY_RELEASE)
            set(MAGNUM_${_COMPONENT}_LIBRARY
                debug ${MAGNUM_${_COMPONENT}_LIBRARY_DEBUG}
                optimized ${MAGNUM_${_COMPONENT}_LIBRARY_RELEASE})
        elseif(MAGNUM_${_COMPONENT}_LIBRARY_DEBUG)
            set(MAGNUM_${_COMPONENT}_LIBRARY ${MAGNUM_${_COMPONENT}_LIBRARY_DEBUG})
        elseif(MAGNUM_${_COMPONENT}_LIBRARY_RELEASE)
            set(MAGNUM_${_COMPONENT}_LIBRARY ${MAGNUM_${_COMPONENT}_LIBRARY_RELEASE})
        endif()
    endif()

    # Applications
    if(${component} MATCHES ".+Application")
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_SUFFIX Magnum/Platform)

        # Android application dependencies
        if(${component} STREQUAL AndroidApplication)
            find_package(EGL)
            if(EGL_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES android ${EGL_LIBRARY})
                set(_MAGNUM_${_COMPONENT}_INCLUDE_DIRS ${ANDROID_NATIVE_APP_GLUE_INCLUDE_DIR})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()

        # GLUT application dependencies
        elseif(${component} STREQUAL GlutApplication)
            find_package(GLUT)
            if(GLUT_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES ${GLUT_glut_LIBRARY})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()

        # SDL2 application dependencies
        elseif(${component} STREQUAL Sdl2Application)
            find_package(SDL2)
            if(SDL2_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES ${SDL2_LIBRARY})
                set(_MAGNUM_${_COMPONENT}_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()

            # Find also EGL library, if on ES (and not on WebGL)
            if(MAGNUM_TARGET_GLES AND NOT MAGNUM_TARGET_DESKTOP_GLES AND NOT MAGNUM_TARGET_WEBGL)
                find_package(EGL)
                if(EGL_FOUND)
                    list(APPEND _MAGNUM_${_COMPONENT}_LIBRARIES ${EGL_LIBRARY})
                else()
                    unset(MAGNUM_${_COMPONENT}_LIBRARY)
                endif()
            endif()

        # (Windowless) NaCl application dependencies
        elseif(${component} STREQUAL NaClApplication OR ${component} STREQUAL WindowlessNaClApplication)
            set(_MAGNUM_${_COMPONENT}_LIBRARIES ppapi_cpp ppapi)

        # (Windowless) GLX application dependencies
        elseif(${component} STREQUAL GlxApplication OR ${component} STREQUAL WindowlessGlxApplication)
            find_package(X11)
            if(X11_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES ${X11_LIBRARIES})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()

        # Windowless CGL application has no additional dependencies
        # Windowless WGL application has no additional dependencies

        # Windowless Windows/EGL application dependencies
        elseif(${component} STREQUAL WindowlessWindowsEglApplication)
            find_package(EGL)
            if(EGL_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES ${EGL_LIBRARY})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()

        # X/EGL application dependencies
        elseif(${component} STREQUAL XEglApplication)
            find_package(EGL)
            find_package(X11)
            if(EGL_FOUND AND X11_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES ${EGL_LIBRARY} ${X11_LIBRARIES})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()
        endif()

    # Context libraries
    elseif(${component} MATCHES ".+Context")
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_SUFFIX Magnum/Platform)
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES Context.h)

        # GLX context dependencies
        if(${component} STREQUAL GlxContext)
            find_package(X11)
            if(X11_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES ${X11_LIBRARIES})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()
        endif()

        # EGL context dependencies
        if(${component} STREQUAL EglContext)
            find_package(EGL)
            if(EGL_FOUND)
                set(_MAGNUM_${_COMPONENT}_LIBRARIES ${EGL_LIBRARIES})
            else()
                unset(MAGNUM_${_COMPONENT}_LIBRARY)
            endif()
        endif()

        # No additional dependencies for CGL context
        # No additional dependencies for WGL context

    # Audio library
    elseif(${component} STREQUAL Audio)
        find_package(OpenAL)
        if(OPENAL_FOUND)
            set(_MAGNUM_${_COMPONENT}_LIBRARIES ${OPENAL_LIBRARY})
            set(_MAGNUM_${_COMPONENT}_INCLUDE_DIRS ${OPENAL_INCLUDE_DIR})
        else()
            unset(MAGNUM_${_COMPONENT}_LIBRARY)
        endif()

    # No special setup for DebugTools library

    # Mesh tools library
    elseif(${component} STREQUAL MeshTools)
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES CompressIndices.h)

    # Primitives library
    elseif(${component} STREQUAL Primitives)
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES Cube.h)

    # No special setup for SceneGraph library
    # No special setup for Shaders library
    # No special setup for Shapes library
    # No special setup for Text library

    # TextureTools library
    elseif(${component} STREQUAL TextureTools)
        set(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES Atlas.h)
    endif()

    # No special setup for plugins

    # Try to find the includes
    if(_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES)
        find_path(_MAGNUM_${_COMPONENT}_INCLUDE_DIR
            NAMES ${_MAGNUM_${_COMPONENT}_INCLUDE_PATH_NAMES}
            PATHS ${MAGNUM_INCLUDE_DIR}/${_MAGNUM_${_COMPONENT}_INCLUDE_PATH_SUFFIX})
    endif()

    # Add inter-project dependencies, mark the component as not found if
    # any dependency is not found
    set(_MAGNUM_${_COMPONENT}_DEPENDENCY_LIBRARIES )
    set(_MAGNUM_${_COMPONENT}_DEPENDENCY_INCLUDE_DIRS )
    foreach(dependency ${_MAGNUM_${_COMPONENT}_DEPENDENCIES})
        string(TOUPPER ${dependency} _DEPENDENCY)
        if(MAGNUM_${_DEPENDENCY}_LIBRARY)
            list(APPEND _MAGNUM_${_COMPONENT}_DEPENDENCY_LIBRARIES ${MAGNUM_${_DEPENDENCY}_LIBRARY} ${_MAGNUM_${_DEPENDENCY}_LIBRARIES})
            list(APPEND _MAGNUM_${_COMPONENT}_DEPENDENCY_INCLUDE_DIRS ${_MAGNUM_${_DEPENDENCY}_INCLUDE_DIRS})
        else()
            unset(MAGNUM_${_COMPONENT}_LIBRARY)
        endif()
    endforeach()

    # Decide if the library was found
    if(MAGNUM_${_COMPONENT}_LIBRARY AND _MAGNUM_${_COMPONENT}_INCLUDE_DIR)
        set(MAGNUM_${_COMPONENT}_LIBRARIES
            ${MAGNUM_${_COMPONENT}_LIBRARY}
            ${_MAGNUM_${_COMPONENT}_LIBRARIES}
            ${_MAGNUM_${_COMPONENT}_DEPENDENCY_LIBRARIES}
            ${MAGNUM_LIBRARIES})
        set(MAGNUM_${_COMPONENT}_INCLUDE_DIRS
            ${_MAGNUM_${_COMPONENT}_INCLUDE_DIRS}
            ${_MAGNUM_${_COMPONENT}_DEPENDENCY_INCLUDE_DIRS})

        set(Magnum_${component}_FOUND TRUE)

        # Don't expose variables w/o dependencies to end users
        mark_as_advanced(FORCE
            MAGNUM_${_COMPONENT}_LIBRARY_DEBUG
            MAGNUM_${_COMPONENT}_LIBRARY_RELEASE
            MAGNUM_${_COMPONENT}_LIBRARY
            _MAGNUM_${_COMPONENT}_INCLUDE_DIR)

        # Global aliases for Windowless*Application and *Application
        # components. If already set, unset them to avoid ambiguity.
        if(${component} MATCHES "Windowless.+Application")
            if(NOT DEFINED MAGNUM_WINDOWLESSAPPLICATION_LIBRARIES AND NOT DEFINED MAGNUM_WINDOWLESSAPPLICATION_INCLUDE_DIRS)
                set(MAGNUM_WINDOWLESSAPPLICATION_LIBRARIES ${MAGNUM_${_COMPONENT}_LIBRARIES})
                set(MAGNUM_WINDOWLESSAPPLICATION_INCLUDE_DIRS ${MAGNUM_${_COMPONENT}_INCLUDE_DIRS})
            else()
                unset(MAGNUM_WINDOWLESSAPPLICATION_LIBRARIES)
                unset(MAGNUM_WINDOWLESSAPPLICATION_INCLUDE_DIRS)
            endif()
        elseif(${component} MATCHES ".+Application")
            if(NOT DEFINED MAGNUM_APPLICATION_LIBRARIES AND NOT DEFINED MAGNUM_APPLICATION_INCLUDE_DIRS)
                set(MAGNUM_APPLICATION_LIBRARIES ${MAGNUM_${_COMPONENT}_LIBRARIES})
                set(MAGNUM_APPLICATION_INCLUDE_DIRS ${MAGNUM_${_COMPONENT}_INCLUDE_DIRS})
            else()
                unset(MAGNUM_APPLICATION_LIBRARIES)
                unset(MAGNUM_APPLICATION_INCLUDE_DIRS)
            endif()
        endif()

        # Global aliases for *Context components. If already set, unset them to
        # avoid ambiguity.
        if(${component} MATCHES ".+Context")
            if(NOT DEFINED MAGNUM_CONTEXT_LIBRARIES AND NOT DEFINED MAGNUM_CONTEXT_INCLUDE_DIRS)
                set(MAGNUM_CONTEXT_LIBRARIES ${MAGNUM_${_COMPONENT}_LIBRARIES})
                set(MAGNUM_CONTEXT_INCLUDE_DIRS ${MAGNUM_${_COMPONENT}_INCLUDE_DIRS})
            else()
                unset(MAGNUM_CONTEXT_LIBRARIES)
                unset(MAGNUM_CONTEXT_INCLUDE_DIRS)
            endif()
        endif()
    else()
        set(Magnum_${component}_FOUND FALSE)
    endif()
endforeach()

# Complete the check with also all components
find_package_handle_standard_args(Magnum
    REQUIRED_VARS MAGNUM_LIBRARY MAGNUM_INCLUDE_DIR
    HANDLE_COMPONENTS)

# Installation dirs
include(${CORRADE_LIB_SUFFIX_MODULE})
set(MAGNUM_BINARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set(MAGNUM_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
set(MAGNUM_DATA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/magnum)
set(MAGNUM_PLUGINS_DEBUG_INSTALL_DIR ${MAGNUM_LIBRARY_INSTALL_DIR}/magnum-d)
set(MAGNUM_PLUGINS_RELEASE_INSTALL_DIR ${MAGNUM_LIBRARY_INSTALL_DIR}/magnum)
set(MAGNUM_PLUGINS_FONT_DEBUG_INSTALL_DIR ${MAGNUM_PLUGINS_DEBUG_INSTALL_DIR}/fonts)
set(MAGNUM_PLUGINS_FONT_RELEASE_INSTALL_DIR ${MAGNUM_PLUGINS_RELEASE_INSTALL_DIR}/fonts)
set(MAGNUM_PLUGINS_FONTCONVERTER_DEBUG_INSTALL_DIR ${MAGNUM_PLUGINS_DEBUG_INSTALL_DIR}/fontconverters)
set(MAGNUM_PLUGINS_FONTCONVERTER_RELEASE_INSTALL_DIR ${MAGNUM_PLUGINS_RELEASE_INSTALL_DIR}/fontconverters)
set(MAGNUM_PLUGINS_IMAGECONVERTER_DEBUG_INSTALL_DIR ${MAGNUM_PLUGINS_DEBUG_INSTALL_DIR}/imageconverters)
set(MAGNUM_PLUGINS_IMAGECONVERTER_RELEASE_INSTALL_DIR ${MAGNUM_PLUGINS_RELEASE_INSTALL_DIR}/imageconverters)
set(MAGNUM_PLUGINS_IMPORTER_DEBUG_INSTALL_DIR ${MAGNUM_PLUGINS_DEBUG_INSTALL_DIR}/importers)
set(MAGNUM_PLUGINS_IMPORTER_RELEASE_INSTALL_DIR ${MAGNUM_PLUGINS_RELEASE_INSTALL_DIR}/importers)
set(MAGNUM_PLUGINS_AUDIOIMPORTER_DEBUG_INSTALL_DIR ${MAGNUM_PLUGINS_DEBUG_INSTALL_DIR}/audioimporters)
set(MAGNUM_PLUGINS_AUDIOIMPORTER_RELEASE_INSTALL_DIR ${MAGNUM_PLUGINS_RELEASE_INSTALL_DIR}/audioimporters)
set(MAGNUM_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/Magnum)
set(MAGNUM_PLUGINS_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/MagnumPlugins)
mark_as_advanced(FORCE
    MAGNUM_LIBRARY_DEBUG
    MAGNUM_LIBRARY_RELEASE
    MAGNUM_LIBRARY
    MAGNUM_INCLUDE_DIR
    MAGNUM_BINARY_INSTALL_DIR
    MAGNUM_LIBRARY_INSTALL_DIR
    MAGNUM_PLUGINS_DEBUG_INSTALL_DIR
    MAGNUM_PLUGINS_RELEASE_INSTALL_DIR
    MAGNUM_PLUGINS_FONT_DEBUG_INSTALL_DIR
    MAGNUM_PLUGINS_FONT_RELEASE_INSTALL_DIR
    MAGNUM_PLUGINS_FONTCONVERTER_DEBUG_INSTALL_DIR
    MAGNUM_PLUGINS_FONTCONVERTER_RELEASE_INSTALL_DIR
    MAGNUM_PLUGINS_IMAGECONVERTER_DEBUG_INSTALL_DIR
    MAGNUM_PLUGINS_IMAGECONVERTER_RELEASE_INSTALL_DIR
    MAGNUM_PLUGINS_IMPORTER_DEBUG_INSTALL_DIR
    MAGNUM_PLUGINS_IMPORTER_RELEASE_INSTALL_DIR
    MAGNUM_PLUGINS_AUDIOIMPORTER_DEBUG_INSTALL_DIR
    MAGNUM_PLUGINS_AUDIOIMPORTER_RELEASE_INSTALL_DIR
    MAGNUM_CMAKE_MODULE_INSTALL_DIR
    MAGNUM_INCLUDE_INSTALL_DIR
    MAGNUM_PLUGINS_INCLUDE_INSTALL_DIR)

# Get base plugin directory from main library location
set(MAGNUM_PLUGINS_DEBUG_DIR ${_MAGNUM_LIBRARY_PATH}/magnum-d
    CACHE PATH "Base directory where to look for Magnum plugins for debug builds")
set(MAGNUM_PLUGINS_RELEASE_DIR ${_MAGNUM_LIBRARY_PATH}/magnum
    CACHE PATH "Base directory where to look for Magnum plugins for release builds")
set(MAGNUM_PLUGINS_DIR ${_MAGNUM_LIBRARY_PATH}/magnum${_MAGNUM_PLUGINS_DIR_SUFFIX}
    CACHE PATH "Base directory where to look for Magnum plugins")

# Plugin directories
set(MAGNUM_PLUGINS_FONT_DIR ${MAGNUM_PLUGINS_DIR}/fonts)
set(MAGNUM_PLUGINS_FONT_DEBUG_DIR ${MAGNUM_PLUGINS_DEBUG_DIR}/fonts)
set(MAGNUM_PLUGINS_FONT_RELEASE_DIR ${MAGNUM_PLUGINS_RELEASE_DIR}/fonts)
set(MAGNUM_PLUGINS_FONTCONVERTER_DIR ${MAGNUM_PLUGINS_DIR}/fontconverters)
set(MAGNUM_PLUGINS_FONTCONVERTER_DEBUG_DIR ${MAGNUM_PLUGINS_DEBUG_DIR}/fontconverters)
set(MAGNUM_PLUGINS_FONTCONVERTER_RELEASE_DIR ${MAGNUM_PLUGINS_RELEASE_DIR}/fontconverters)
set(MAGNUM_PLUGINS_IMAGECONVERTER_DIR ${MAGNUM_PLUGINS_DIR}/imageconverters)
set(MAGNUM_PLUGINS_IMAGECONVERTER_DEBUG_DIR ${MAGNUM_PLUGINS_DEBUG_DIR}/imageconverters)
set(MAGNUM_PLUGINS_IMAGECONVERTER_RELEASE_DIR ${MAGNUM_PLUGINS_RELEASE_DIR}/imageconverters)
set(MAGNUM_PLUGINS_IMPORTER_DIR ${MAGNUM_PLUGINS_DIR}/importers)
set(MAGNUM_PLUGINS_IMPORTER_DEBUG_DIR ${MAGNUM_PLUGINS_DEBUG_DIR}/importers)
set(MAGNUM_PLUGINS_IMPORTER_RELEASE_DIR ${MAGNUM_PLUGINS_RELEASE_DIR}/importers)
set(MAGNUM_PLUGINS_AUDIOIMPORTER_DIR ${MAGNUM_PLUGINS_DIR}/audioimporters)
set(MAGNUM_PLUGINS_AUDIOIMPORTER_DEBUG_DIR ${MAGNUM_PLUGINS_DEBUG_DIR}/audioimporters)
set(MAGNUM_PLUGINS_AUDIOIMPORTER_RELEASE_DIR ${MAGNUM_PLUGINS_RELEASE_DIR}/audioimporters)

/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "WindowlessGlxApplication.h"

#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/Debug.h>

#include "Magnum/Platform/Context.h"

#define None 0L // redef Xlib nonsense

namespace Magnum { namespace Platform {

#ifndef DOXYGEN_GENERATING_OUTPUT
WindowlessGlxApplication::WindowlessGlxApplication(const Arguments& arguments): WindowlessGlxApplication{arguments, Configuration{}}  {}
#endif

WindowlessGlxApplication::WindowlessGlxApplication(const Arguments& arguments, const Configuration& configuration): WindowlessGlxApplication{arguments, nullptr} {
    createContext(configuration);
}

WindowlessGlxApplication::WindowlessGlxApplication(const Arguments&, std::nullptr_t) {}

void WindowlessGlxApplication::createContext() { createContext({}); }

void WindowlessGlxApplication::createContext(const Configuration& configuration) {
    if(!tryCreateContext(configuration)) std::exit(1);
}

bool WindowlessGlxApplication::tryCreateContext(const Configuration&) {
    CORRADE_ASSERT(!_context, "Platform::WindowlessGlxApplication::tryCreateContext(): context already created", false);

    _display = XOpenDisplay(nullptr);

    /* Check version */
    int major, minor;
    glXQueryVersion(_display, &major, &minor);
    if(major == 1 && minor < 4) {
        Error() << "Platform::WindowlessGlxApplication::tryCreateContext(): GLX version 1.4 or greater is required";
        return false;
    }

    /* Choose config */
    int configCount = 0;
    static const int fbAttributes[] = { None };
    GLXFBConfig* configs = glXChooseFBConfig(_display, DefaultScreen(_display), fbAttributes, &configCount);
    if(!configCount) {
        Error() << "Platform::WindowlessGlxApplication::tryCreateContext(): no supported framebuffer configuration found";
        return false;
    }

    GLint contextAttributes[] = {
        #ifdef MAGNUM_TARGET_GLES
        #ifdef MAGNUM_TARGET_GLES3
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        #elif defined(MAGNUM_TARGET_GLES2)
        GLX_CONTEXT_MAJOR_VERSION_ARB, 2,
        #else
        #error unsupported OpenGL ES version
        #endif
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_ES2_PROFILE_BIT_EXT,
        #else
        /* Similarly to what's done in Sdl2Application, try to request core
           context first */
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 1,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        #endif
        0
    };

    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    _glContext = glXCreateContextAttribsARB(_display, configs[0], nullptr, True, contextAttributes);

    #ifndef MAGNUM_TARGET_GLES
    /* Fall back to (forward compatible) GL 2.1, if core context creation fails
       and if version is not user-specified */
    if(!_glContext) {
        Warning() << "Platform::WindowlessGlxApplication::tryCreateContext(): cannot create core context, falling back to compatibility context";

        _glContext = glXCreateContextAttribsARB(_display, configs[0], nullptr, True, contextAttributes);
    }
    #endif

    if(!_glContext) {
        Error() << "Platform::WindowlessGlxApplication::tryCreateContext(): cannot create context";
        return false;
    }

    /* Create pbuffer */
    int pbufferAttributes[] = {
        GLX_PBUFFER_WIDTH,  32,
        GLX_PBUFFER_HEIGHT, 32,
        None
    };
    _pbuffer = glXCreatePbuffer(_display, configs[0], pbufferAttributes);

    XFree(configs);

    /* Set OpenGL context as current */
    if(!glXMakeContextCurrent(_display, _pbuffer, _pbuffer, _glContext)) {
        Error() << "Platform::WindowlessGlxApplication::tryCreateContext(): cannot make context current";
        return false;
    }

    _context.reset(new Platform::Context);
    return true;
}

WindowlessGlxApplication::~WindowlessGlxApplication() {
    _context.reset();

    glXMakeCurrent(_display, None, nullptr);
    glXDestroyContext(_display, _glContext);
}

}}

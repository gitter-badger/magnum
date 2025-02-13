#ifndef Magnum_Implementation_FramebufferState_h
#define Magnum_Implementation_FramebufferState_h
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

#include <string>
#include <vector>

#include "Magnum/Framebuffer.h"

#ifdef _MSC_VER
/* Otherwise the member function pointers will have different size based on
   whether the header was included or not. CAUSES SERIOUS MEMORY CORRUPTION AND
   IS NOT CAUGHT BY ANY WARNING WHATSOEVER! AARGH! */
#include "Magnum/Renderbuffer.h"
#endif

namespace Magnum { namespace Implementation {

struct FramebufferState {
    #ifndef CORRADE_MSVC2015_COMPATIBILITY
    /* MSVC complains that in-class initialization is not yet implemented */
    constexpr static const Range2Di DisengagedViewport{{}, {-1, -1}};
    #else
    static const Range2Di DisengagedViewport;
    #endif

    explicit FramebufferState(Context& context, std::vector<std::string>& extensions);

    void reset();

    #if !(defined(MAGNUM_TARGET_WEBGL) && defined(MAGNUM_TARGET_GLES2))
    void(*blitImplementation)(AbstractFramebuffer&, AbstractFramebuffer&, const Range2Di&, const Range2Di&, FramebufferBlitMask, FramebufferBlitFilter);
    #endif
    GLenum(AbstractFramebuffer::*checkStatusImplementation)(FramebufferTarget);
    void(AbstractFramebuffer::*drawBuffersImplementation)(GLsizei, const GLenum*);
    #ifndef MAGNUM_TARGET_GLES
    void(AbstractFramebuffer::*drawBufferImplementation)(GLenum);
    #endif
    #if !(defined(MAGNUM_TARGET_WEBGL) && defined(MAGNUM_TARGET_GLES2))
    void(AbstractFramebuffer::*readBufferImplementation)(GLenum);
    void(AbstractFramebuffer::*invalidateImplementation)(GLsizei, const GLenum*);
    #endif
    #ifndef MAGNUM_TARGET_GLES2
    void(AbstractFramebuffer::*invalidateSubImplementation)(GLsizei, const GLenum*, const Range2Di&);
    #endif
    #if defined(MAGNUM_TARGET_GLES2) && !defined(MAGNUM_TARGET_WEBGL)
    void(AbstractFramebuffer::*bindImplementation)(FramebufferTarget);
    FramebufferTarget(AbstractFramebuffer::*bindInternalImplementation)();
    #endif

    void(Framebuffer::*createImplementation)();
    void(Framebuffer::*renderbufferImplementation)(Framebuffer::BufferAttachment, GLuint);
    #ifndef MAGNUM_TARGET_GLES
    void(Framebuffer::*texture1DImplementation)(Framebuffer::BufferAttachment, GLuint, GLint);
    #endif
    void(Framebuffer::*texture2DImplementation)(Framebuffer::BufferAttachment, GLenum, GLuint, GLint);
    #if !(defined(MAGNUM_TARGET_WEBGL) && defined(MAGNUM_TARGET_GLES2))
    void(Framebuffer::*textureLayerImplementation)(Framebuffer::BufferAttachment, GLuint, GLint, GLint);
    #endif

    void(Renderbuffer::*createRenderbufferImplementation)();
    void(Renderbuffer::*renderbufferStorageImplementation)(RenderbufferFormat, const Vector2i&);
    #if !(defined(MAGNUM_TARGET_WEBGL) && defined(MAGNUM_TARGET_GLES2))
    void(Renderbuffer::*renderbufferStorageMultisampleImplementation)(GLsizei, RenderbufferFormat, const Vector2i&);
    #endif

    void(*readImplementation)(const Range2Di&, PixelFormat, PixelType, std::size_t, GLvoid*);

    GLuint readBinding, drawBinding, renderbufferBinding;
    GLint maxDrawBuffers, maxColorAttachments, maxRenderbufferSize;
    #if !(defined(MAGNUM_TARGET_WEBGL) && defined(MAGNUM_TARGET_GLES2))
    GLint maxSamples;
    #endif
    #ifndef MAGNUM_TARGET_GLES
    GLint maxDualSourceDrawBuffers;
    #endif
    Range2Di viewport;
    Vector2i maxViewportSize;
};

}}

#endif

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

#include <Corrade/Utility/Arguments.h>
#include <Corrade/Utility/Debug.h>
#ifdef CORRADE_TARGET_NACL
#include <Corrade/Utility/NaClStreamBuffer.h>
#endif

#include "Magnum/AbstractShaderProgram.h"
#include "Magnum/Buffer.h"
#if !defined(MAGNUM_TARGET_GLES2) && !defined(MAGNUM_TARGET_WEBGL)
#include "Magnum/BufferTexture.h"
#endif
#include "Magnum/Context.h"
#if !defined(MAGNUM_TARGET_GLES2) && !defined(MAGNUM_TARGET_WEBGL)
#include "Magnum/CubeMapTextureArray.h"
#endif
#include "Magnum/DebugOutput.h"
#include "Magnum/Extensions.h"
#include "Magnum/Framebuffer.h"
#include "Magnum/Mesh.h"
#ifndef MAGNUM_TARGET_GLES2
#include "Magnum/MultisampleTexture.h"
#endif
#ifndef MAGNUM_TARGET_GLES
#include "Magnum/RectangleTexture.h"
#endif
#include "Magnum/Renderbuffer.h"
#include "Magnum/Shader.h"
#include "Magnum/Texture.h"
#ifndef MAGNUM_TARGET_GLES2
#include "Magnum/TextureArray.h"
#include "Magnum/TransformFeedback.h"
#endif

#ifdef CORRADE_TARGET_NACL
#include "Magnum/Platform/WindowlessNaClApplication.h"
#elif defined(CORRADE_TARGET_APPLE)
#include "Magnum/Platform/WindowlessCglApplication.h"
#elif defined(CORRADE_TARGET_UNIX)
#include "Magnum/Platform/WindowlessGlxApplication.h"
#elif defined(CORRADE_TARGET_WINDOWS)
#if !defined(MAGNUM_TARGET_GLES) || defined(MAGNUM_TARGET_DESKTOP_GLES)
#include "Magnum/Platform/WindowlessWglApplication.h"
#else
#include "Magnum/Platform/WindowlessWindowsEglApplication.h"
#endif
#else
#error no windowless application available on this platform
#endif

namespace Magnum {

/** @page magnum-info Magnum Info
@brief Displays information about Magnum engine and OpenGL capabilities

@section magnum-info-usage Usage

    magnum-info [-h|--help] [--all-extensions] [--limits]

Arguments:

-   ` -h`, `--help` -- display this help message and exit
-   `--all-extensions` -- show extensions also for fully supported versions
-   `--limits` -- display also limits and implementation-defined values

@section magnum-info-example Example output

      +---------------------------------------------------------+
      | Information about Magnum engine and OpenGL capabilities |
      +---------------------------------------------------------+

    Used application: Platform::WindowlessGlxApplication
    Compilation flags:
        CORRADE_BUILD_DEPRECATED
        MAGNUM_BUILD_DEPRECATED

    Vendor: NVIDIA Corporation
    Renderer: GeForce GT 740M/PCIe/SSE2
    OpenGL version: OpenGL 4.4 (4.4.0 NVIDIA 337.25)
    Context flags:
    Supported GLSL versions:
        440 core
        430 core
        420 core
        410 core
        400 core
        330 core
        310 es
        300 es
        100

    Vendor extension support:
        GL_AMD_vertex_shader_layer                                        -
        GL_AMD_shader_trinary_minmax                                      -
        GL_ARB_robustness                                             SUPPORTED
        GL_ATI_texture_mirror_once                                    SUPPORTED
        GL_EXT_texture_filter_anisotropic                             SUPPORTED
        GL_EXT_texture_mirror_clamp                                   SUPPORTED
        GL_EXT_direct_state_access                                    SUPPORTED
        GL_EXT_texture_sRGB_decode                                    SUPPORTED
        GL_EXT_shader_integer_mix                                     SUPPORTED
        GL_EXT_debug_label                                                -
        GL_EXT_debug_marker                                               -
        GL_GREMEDY_string_marker                                          -

*/

class MagnumInfo: public Platform::WindowlessApplication {
    public:
        explicit MagnumInfo(const Arguments& arguments);

        int exec() override { return 0; }
};

MagnumInfo::MagnumInfo(const Arguments& arguments): Platform::WindowlessApplication(arguments, nullptr) {
    Utility::Arguments args;
    args.addBooleanOption("all-extensions")
        .setHelp("all-extensions", "show extensions also for fully supported versions")
        .addBooleanOption("limits")
        .setHelp("limits", "display also limits and implementation-defined values")
        .setHelp("Displays information about Magnum engine and OpenGL capabilities.");

    /**
     * @todo Make this work in NaCl, somehow the arguments aren't passed to
     *      constructor but to Init() or whatnot
     */
    #ifndef CORRADE_TARGET_NACL
    args.parse(arguments.argc, arguments.argv);
    #endif

    /* Pass debug output as messages to JavaScript */
    #ifdef CORRADE_TARGET_NACL
    Utility::NaClMessageStreamBuffer buffer(this);
    std::ostream out(&buffer);
    Debug::setOutput(&out);
    #endif

    Debug() << "";
    Debug() << "  +---------------------------------------------------------+";
    Debug() << "  | Information about Magnum engine and OpenGL capabilities |";
    Debug() << "  +---------------------------------------------------------+";
    Debug() << "";

    #ifdef CORRADE_TARGET_NACL
    Debug() << "Used application: Platform::WindowlessNaClApplication";
    #elif defined(CORRADE_TARGET_APPLE)
    Debug() << "Used application: Platform::WindowlessCglApplication";
    #elif defined(CORRADE_TARGET_UNIX)
    Debug() << "Used application: Platform::WindowlessGlxApplication";
    #elif defined(CORRADE_TARGET_WINDOWS)
    Debug() << "Used application: Platform::WindowlessWglApplication";
    #else
    #error no windowless application available on this platform
    #endif
    Debug() << "Compilation flags:";
    #ifdef CORRADE_GCC47_COMPATIBILITY
    Debug() << "    CORRADE_GCC47_COMPATIBILITY";
    #endif
    #ifdef CORRADE_BUILD_DEPRECATED
    Debug() << "    CORRADE_BUILD_DEPRECATED";
    #endif
    #ifdef CORRADE_BUILD_STATIC
    Debug() << "    CORRADE_BUILD_STATIC";
    #endif
    #ifdef CORRADE_TARGET_UNIX
    Debug() << "    CORRADE_TARGET_UNIX";
    #endif
    #ifdef CORRADE_TARGET_APPLE
    Debug() << "    CORRADE_TARGET_APPLE";
    #endif
    #ifdef CORRADE_TARGET_WINDOWS
    Debug() << "    CORRADE_TARGET_WINDOWS";
    #endif
    #ifdef CORRADE_TARGET_NACL
    Debug() << "    CORRADE_TARGET_NACL";
    #endif
    #ifdef CORRADE_TARGET_NACL_NEWLIB
    Debug() << "    CORRADE_TARGET_NACL_NEWLIB";
    #endif
    #ifdef CORRADE_TARGET_NACL_GLIBC
    Debug() << "    CORRADE_TARGET_NACL_GLIBC";
    #endif
    #ifdef CORRADE_TARGET_EMSCRIPTEN
    Debug() << "    CORRADE_TARGET_EMSCRIPTEN";
    #endif
    #ifdef CORRADE_TARGET_ANDROID
    Debug() << "    CORRADE_TARGET_ANDROID";
    #endif
    #ifdef MAGNUM_BUILD_DEPRECATED
    Debug() << "    MAGNUM_BUILD_DEPRECATED";
    #endif
    #ifdef MAGNUM_BUILD_STATIC
    Debug() << "    MAGNUM_BUILD_STATIC";
    #endif
    #ifdef MAGNUM_TARGET_GLES
    Debug() << "    MAGNUM_TARGET_GLES";
    #endif
    #ifdef MAGNUM_TARGET_GLES2
    Debug() << "    MAGNUM_TARGET_GLES2";
    #endif
    #ifdef MAGNUM_TARGET_DESKTOP_GLES
    Debug() << "    MAGNUM_TARGET_DESKTOP_GLES";
    #endif
    #ifdef MAGNUM_TARGET_WEBGL
    Debug() << "    MAGNUM_TARGET_WEBGL";
    #endif
    Debug() << "";

    /* Create context here, so the context creation info is displayed at proper
       place */
    createContext();
    Context* c = Context::current();

    Debug() << "Context flags:";
    #ifndef MAGNUM_TARGET_GLES
    for(const auto flag: {Context::Flag::Debug, Context::Flag::RobustAccess})
    #else
    for(const auto flag: {Context::Flag::Debug})
    #endif
        if(c->flags() & flag) Debug() << "   " << flag;

    Debug() << "Supported GLSL versions:";
    const std::vector<std::string> shadingLanguageVersions = c->shadingLanguageVersionStrings();
    for(const auto& version: shadingLanguageVersions)
        Debug() << "   " << version;

    Debug() << "";

    /* Get first future (not supported) version */
    std::vector<Version> versions{
        #ifndef MAGNUM_TARGET_GLES
        Version::GL300,
        Version::GL310,
        Version::GL320,
        Version::GL330,
        Version::GL400,
        Version::GL410,
        Version::GL420,
        Version::GL430,
        Version::GL440,
        Version::GL450,
        #else
        Version::GLES300,
        #ifndef MAGNUM_TARGET_WEBGL
        Version::GLES310,
        #endif
        #endif
        Version::None
    };
    std::size_t future = 0;

    if(!args.isSet("all-extensions"))
        while(versions[future] != Version::None && c->isVersionSupported(versions[future]))
            ++future;

    /* Display supported OpenGL extensions from unsupported versions */
    for(std::size_t i = future; i != versions.size(); ++i) {
        if(versions[i] != Version::None)
            Debug() << versions[i] << "extension support:";
        else Debug() << "Vendor extension support:";

        for(const auto& extension: Extension::extensions(versions[i])) {
            std::string extensionName = extension.string();
            Debug d;
            d << "   " << extensionName << std::string(60-extensionName.size(), ' ');
            if(c->isExtensionSupported(extension))
                d << "SUPPORTED";
            else if(c->isExtensionDisabled(extension))
                d << " removed";
            else if(c->isVersionSupported(extension.requiredVersion()))
                d << "    -";
            else
                d << "   n/a";
        }

        Debug() << "";
    }

    if(!args.isSet("limits")) return;

    /* Limits and implementation-defined values */
    #define _h(val) Debug() << "\n " << Extensions::GL::val::string() + std::string(":");
    #define _l(val) Debug() << "   " << #val << (sizeof(#val) > 64 ? "\n" + std::string(68, ' ') : std::string(64 - sizeof(#val), ' ')) << val;
    #define _lvec(val) Debug() << "   " << #val << (sizeof(#val) > 48 ? "\n" + std::string(52, ' ') : std::string(48 - sizeof(#val), ' ')) << val;

    Debug() << "Limits and implementation-defined values:";
    _lvec(AbstractFramebuffer::maxViewportSize())
    _l(AbstractFramebuffer::maxDrawBuffers())
    _l(Framebuffer::maxColorAttachments())
    #ifndef MAGNUM_TARGET_GLES2
    _l(Mesh::maxElementIndex())
    _l(Mesh::maxElementsIndices())
    _l(Mesh::maxElementsVertices())
    #endif
    _l(Renderbuffer::maxSize())
    _l(Renderbuffer::maxSamples())
    _l(Shader::maxVertexOutputComponents())
    _l(Shader::maxFragmentInputComponents())
    _l(Shader::maxTextureImageUnits(Shader::Type::Vertex))
    #ifndef MAGNUM_TARGET_GLES2
    _l(Shader::maxTextureImageUnits(Shader::Type::TessellationControl))
    _l(Shader::maxTextureImageUnits(Shader::Type::TessellationEvaluation))
    _l(Shader::maxTextureImageUnits(Shader::Type::Geometry))
    _l(Shader::maxTextureImageUnits(Shader::Type::Compute))
    #endif
    _l(Shader::maxTextureImageUnits(Shader::Type::Fragment))
    _l(Shader::maxCombinedTextureImageUnits())
    _l(Shader::maxUniformComponents(Shader::Type::Vertex))
    #ifndef MAGNUM_TARGET_GLES2
    _l(Shader::maxUniformComponents(Shader::Type::TessellationControl))
    _l(Shader::maxUniformComponents(Shader::Type::TessellationEvaluation))
    _l(Shader::maxUniformComponents(Shader::Type::Geometry))
    _l(Shader::maxUniformComponents(Shader::Type::Compute))
    #endif
    _l(Shader::maxUniformComponents(Shader::Type::Fragment))
    _l(AbstractShaderProgram::maxVertexAttributes())
    #ifndef MAGNUM_TARGET_GLES2
    _l(AbstractTexture::maxLodBias())
    #endif
    #ifndef MAGNUM_TARGET_GLES
    _lvec(Texture1D::maxSize())
    #endif
    _lvec(Texture2D::maxSize())
    #ifndef MAGNUM_TARGET_GLES2
    _lvec(Texture3D::maxSize()) /* Checked ES2 version below */
    #endif
    _lvec(CubeMapTexture::maxSize())

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::blend_func_extended>()) {
        _h(ARB::blend_func_extended)

        _l(AbstractFramebuffer::maxDualSourceDrawBuffers())
    }
    #endif

    #ifndef MAGNUM_TARGET_GLES2
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::compute_shader>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::compute_shader)
        #endif

        _l(AbstractShaderProgram::maxComputeSharedMemorySize())
        _l(AbstractShaderProgram::maxComputeWorkGroupInvocations())
    }

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::explicit_uniform_location>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::explicit_uniform_location)
        #endif

        _l(AbstractShaderProgram::maxUniformLocations())
    }
    #endif

    #ifndef MAGNUM_TARGET_GLES2
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::shader_atomic_counters>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::shader_atomic_counters)
        #endif

        _l(Buffer::maxAtomicCounterBindings())
        _l(Shader::maxAtomicCounterBuffers(Shader::Type::Vertex))
        _l(Shader::maxAtomicCounterBuffers(Shader::Type::TessellationControl))
        _l(Shader::maxAtomicCounterBuffers(Shader::Type::TessellationEvaluation))
        _l(Shader::maxAtomicCounterBuffers(Shader::Type::Geometry))
        _l(Shader::maxAtomicCounterBuffers(Shader::Type::Compute))
        _l(Shader::maxAtomicCounterBuffers(Shader::Type::Fragment))
        _l(Shader::maxCombinedAtomicCounterBuffers())
        _l(Shader::maxAtomicCounters(Shader::Type::Vertex))
        _l(Shader::maxAtomicCounters(Shader::Type::TessellationControl))
        _l(Shader::maxAtomicCounters(Shader::Type::TessellationEvaluation))
        _l(Shader::maxAtomicCounters(Shader::Type::Geometry))
        _l(Shader::maxAtomicCounters(Shader::Type::Compute))
        _l(Shader::maxAtomicCounters(Shader::Type::Fragment))
        _l(Shader::maxCombinedAtomicCounters())
        _l(AbstractShaderProgram::maxAtomicCounterBufferSize())
    }

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::shader_image_load_store>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::shader_image_load_store)
        #endif

        _l(Shader::maxImageUniforms(Shader::Type::Vertex))
        _l(Shader::maxImageUniforms(Shader::Type::TessellationControl))
        _l(Shader::maxImageUniforms(Shader::Type::TessellationEvaluation))
        _l(Shader::maxImageUniforms(Shader::Type::Geometry))
        _l(Shader::maxImageUniforms(Shader::Type::Compute))
        _l(Shader::maxImageUniforms(Shader::Type::Fragment))
        _l(Shader::maxCombinedImageUniforms())
        _l(AbstractShaderProgram::maxCombinedShaderOutputResources())
        _l(AbstractShaderProgram::maxImageUnits())
        #ifndef MAGNUM_TARGET_GLES
        _l(AbstractShaderProgram::maxImageSamples())
        #endif
    }

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::shader_storage_buffer_object>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::shader_storage_buffer_object)
        #endif

        _l(Shader::maxShaderStorageBlocks(Shader::Type::Vertex))
        _l(Shader::maxShaderStorageBlocks(Shader::Type::TessellationControl))
        _l(Shader::maxShaderStorageBlocks(Shader::Type::TessellationEvaluation))
        _l(Shader::maxShaderStorageBlocks(Shader::Type::Geometry))
        _l(Shader::maxShaderStorageBlocks(Shader::Type::Compute))
        _l(Shader::maxShaderStorageBlocks(Shader::Type::Fragment))
        _l(Shader::maxCombinedShaderStorageBlocks())
        /* AbstractShaderProgram::maxCombinedShaderOutputResources() already in shader_image_load_store */
        _l(AbstractShaderProgram::maxShaderStorageBlockSize())
    }
    #endif

    #ifndef MAGNUM_TARGET_GLES2
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::texture_multisample>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::texture_multisample)
        #endif

        _l(AbstractTexture::maxColorSamples())
        _l(AbstractTexture::maxDepthSamples())
        _l(AbstractTexture::maxIntegerSamples())
        _lvec(MultisampleTexture2D::maxSize())
        _lvec(MultisampleTexture2DArray::maxSize())
    }
    #endif

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::texture_rectangle>()) {
        _h(ARB::texture_rectangle)

        _lvec(RectangleTexture::maxSize())
    }
    #endif

    #ifndef MAGNUM_TARGET_GLES2
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::uniform_buffer_object>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::uniform_buffer_object)
        #endif

        _l(Buffer::uniformOffsetAlignment())
        _l(Shader::maxUniformBlocks(Shader::Type::Vertex))
        _l(Shader::maxUniformBlocks(Shader::Type::TessellationControl))
        _l(Shader::maxUniformBlocks(Shader::Type::TessellationEvaluation))
        _l(Shader::maxUniformBlocks(Shader::Type::Geometry))
        _l(Shader::maxUniformBlocks(Shader::Type::Compute))
        _l(Shader::maxUniformBlocks(Shader::Type::Fragment))
        _l(Shader::maxCombinedUniformBlocks())
        _l(Shader::maxCombinedUniformComponents(Shader::Type::Vertex))
        _l(Shader::maxCombinedUniformComponents(Shader::Type::TessellationControl))
        _l(Shader::maxCombinedUniformComponents(Shader::Type::TessellationEvaluation))
        _l(Shader::maxCombinedUniformComponents(Shader::Type::Geometry))
        _l(Shader::maxCombinedUniformComponents(Shader::Type::Compute))
        _l(Shader::maxCombinedUniformComponents(Shader::Type::Fragment))
        _l(AbstractShaderProgram::maxUniformBlockSize())
    }

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::EXT::gpu_shader4>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(EXT::gpu_shader4)
        #endif

        _l(Buffer::maxUniformBindings())
        _l(AbstractShaderProgram::minTexelOffset())
        _l(AbstractShaderProgram::maxTexelOffset())
    }

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::EXT::texture_array>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(EXT::texture_array)
        #endif

        #ifndef MAGNUM_TARGET_GLES
        _lvec(Texture1DArray::maxSize())
        #endif
        _lvec(Texture2DArray::maxSize())
    }
    #endif

    #ifndef MAGNUM_TARGET_GLES2
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::EXT::transform_feedback>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(EXT::transform_feedback)
        #endif

        _l(TransformFeedback::maxInterleavedComponents())
        _l(TransformFeedback::maxSeparateAttributes())
        _l(TransformFeedback::maxSeparateComponents())
    }
    #endif

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::transform_feedback3>()) {
        _h(ARB::transform_feedback3)

        _l(TransformFeedback::maxBuffers())
    }
    #endif

    #if !defined(MAGNUM_TARGET_GLES2) && !defined(MAGNUM_TARGET_WEBGL)
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::geometry_shader4>())
    #else
    if(c->isExtensionSupported<Extensions::GL::EXT::geometry_shader>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::geometry_shader4)
        #else
        _h(EXT::geometry_shader)
        #endif

        _l(Shader::maxGeometryInputComponents())
        _l(Shader::maxGeometryOutputComponents())
        _l(Shader::maxGeometryTotalOutputComponents())
    }
    #endif

    #if !defined(MAGNUM_TARGET_GLES2) && !defined(MAGNUM_TARGET_WEBGL)
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::tessellation_shader>())
    #else
    if(c->isExtensionSupported<Extensions::GL::EXT::tessellation_shader>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::tessellation_shader)
        #else
        _h(EXT::tessellation_shader)
        #endif

        _l(Buffer::shaderStorageOffsetAlignment())
        _l(Buffer::maxShaderStorageBindings())
        _l(Shader::maxTessellationControlInputComponents())
        _l(Shader::maxTessellationControlOutputComponents())
        _l(Shader::maxTessellationControlTotalOutputComponents())
        _l(Shader::maxTessellationEvaluationInputComponents())
        _l(Shader::maxTessellationEvaluationOutputComponents())
    }
    #endif

    #if !defined(MAGNUM_TARGET_GLES2) && !defined(MAGNUM_TARGET_WEBGL)
    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::texture_buffer_object>())
    #else
    if(c->isExtensionSupported<Extensions::GL::EXT::texture_buffer>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::texture_buffer_object)
        #else
        _h(EXT::texture_buffer)
        #endif

        _l(BufferTexture::maxSize())
    }

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::texture_buffer_range>())
    #else
    if(c->isExtensionSupported<Extensions::GL::EXT::texture_buffer>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::texture_buffer_range)
        #else
        /* Header added above */
        #endif

        _l(BufferTexture::offsetAlignment())
    }

    #ifndef MAGNUM_TARGET_GLES
    if(c->isExtensionSupported<Extensions::GL::ARB::texture_cube_map_array>())
    #else
    if(c->isExtensionSupported<Extensions::GL::EXT::texture_cube_map_array>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        _h(ARB::texture_cube_map_array)
        #else
        _h(EXT::texture_cube_map_array)
        #endif

        _lvec(CubeMapTextureArray::maxSize())
    }
    #endif

    if(c->isExtensionSupported<Extensions::GL::EXT::texture_filter_anisotropic>()) {
        _h(EXT::texture_filter_anisotropic)

        _l(Sampler::maxMaxAnisotropy())
    }

    if(c->isExtensionSupported<Extensions::GL::KHR::debug>()) {
        _h(KHR::debug)

        _l(AbstractObject::maxLabelLength())
        _l(DebugOutput::maxLoggedMessages())
        _l(DebugOutput::maxMessageLength())
        _l(DebugGroup::maxStackDepth())
    }

    #ifdef MAGNUM_TARGET_GLES2
    if(c->isExtensionSupported<Extensions::GL::OES::texture_3D>()) {
        _h(OES::texture_3D)

        _lvec(Texture3D::maxSize())
    }
    #endif

    #undef _l
    #undef _h
}

}

MAGNUM_WINDOWLESSAPPLICATION_MAIN(Magnum::MagnumInfo)

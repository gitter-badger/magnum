#ifndef Magnum_TransformFeedback_h
#define Magnum_TransformFeedback_h
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

#include <Corrade/Containers/ArrayView.h>

#include "Magnum/AbstractObject.h"
#include "Magnum/Tags.h"

#ifndef MAGNUM_TARGET_GLES2
/** @file
 * @brief Class @ref Magnum::TransformFeedback
 */
#endif

#ifndef MAGNUM_TARGET_GLES2
namespace Magnum {

namespace Implementation { struct TransformFeedbackState; }

/**
@brief Transform feedback

## Performance optimizations

The engine tracks currently bound transform feedback to avoid unnecessary calls
to @fn_gl{BindTransformFeedback}. Transform feedback limits and
implementation-defined values (such as @ref maxSeparateComponents()) are
cached, so repeated queries don't result in repeated @fn_gl{Get} calls. See
also @ref Context::resetState() and @ref Context::State::TransformFeedback.

If @extension{ARB,direct_state_access} (part of OpenGL 4.5) is available,
functions @ref attachBuffer() and @ref attachBuffers() use DSA to avoid
unnecessary calls to @fn_gl{BindTransformFeedback}. See their respective
documentation for more information.

@see @ref PrimitiveQuery
@requires_gl40 Extension @extension{ARB,transform_feedback2}
@requires_gles30 Transform feedback is not available in OpenGL ES 2.0.
@requires_webgl20 Transform feedback is not available in WebGL 1.0.
@todo @extension{AMD,transform_feedback3_lines_triangles}?
*/
class MAGNUM_EXPORT TransformFeedback: public AbstractObject {
    friend Implementation::TransformFeedbackState;

    public:
        /**
         * @brief Transform feedback primitive mode
         *
         * @see @ref begin()
         */
        enum class PrimitiveMode: GLenum {
            /**
             * Points. If no geometry shader is present, allowed only in
             * combination with @ref MeshPrimitive::Points mesh primitive type.
             * If geometry shader is present, allowed only in combination with
             * `points` output primitive type.
             */
            Points = GL_POINTS,

            /**
             * Lines. If no geometry shader is present, allowed only in
             * combination with @ref MeshPrimitive::LineStrip,
             * @ref MeshPrimitive::LineLoop, @ref MeshPrimitive::Lines,
             * @ref MeshPrimitive::LineStripAdjacency and
             * @ref MeshPrimitive::LinesAdjacency mesh primitive type. If
             * geometry shader is present, allowed only in combination with
             * `line_strip` output primitive type.
             */
            Lines = GL_LINES,

            /**
             * Triangles. If no geometry shader is present, allowed only in
             * combination with @ref MeshPrimitive::TriangleStrip,
             * @ref MeshPrimitive::TriangleFan, @ref MeshPrimitive::Triangles,
             * @ref MeshPrimitive::TriangleStripAdjacency and
             * @ref MeshPrimitive::TrianglesAdjacency mesh primitive type. If
             * geometry shader is present, allowed only in commbination with
             * `triangle_strip` output primitive type.
             */
            Triangles = GL_TRIANGLES
        };

        /**
         * @brief Max supported interleaved component count
         *
         * The result is cached, repeated queries don't result in repeated
         * OpenGL calls. If extension @extension{EXT,transform_feedback}
         * (part of OpenGL 3.0) is not available, returns `0`.
         * @see @fn_gl{Get} with @def_gl{MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS}
         */
        static Int maxInterleavedComponents();

        /**
         * @brief Max supported separate attribute count
         *
         * The result is cached, repeated queries don't result in repeated
         * OpenGL calls. If extension @extension{EXT,transform_feedback}
         * (part of OpenGL 3.0) is not available, returns `0`.
         * @see @fn_gl{Get} with @def_gl{MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS}
         */
        static Int maxSeparateAttributes();

        /**
         * @brief Max supported separate component count
         *
         * The result is cached, repeated queries don't result in repeated
         * OpenGL calls. If extension @extension{EXT,transform_feedback}
         * (part of OpenGL 3.0) is not available, returns `0`.
         * @see @fn_gl{Get} with @def_gl{MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS}
         */
        static Int maxSeparateComponents();

        #ifndef MAGNUM_TARGET_GLES
        /**
         * @brief Max supported buffer count
         *
         * The result is cached, repeated queries don't result in repeated
         * OpenGL calls. If extension @extension{ARB,transform_feedback3}
         * (part of OpenGL 4.0) is not available, returns the same value as
         * @ref maxSeparateAttributes().
         * @see @fn_gl{Get} with @def_gl{MAX_TRANSFORM_FEEDBACK_BUFFERS}
         * @requires_gl Use @ref maxSeparateAttributes() in OpenGL ES and
         *      WebGL.
         */
        static Int maxBuffers();
        #endif

        /**
         * @brief Wrap existing OpenGL transform feedback object
         * @param id            OpenGL transform feedback ID
         * @param flags         Object creation flags
         *
         * The @p id is expected to be of an existing OpenGL transform feedback
         * object. Unlike renderbuffer created using constructor, the OpenGL
         * object is by default not deleted on destruction, use @p flags for
         * different behavior.
         * @see @ref release()
         */
        static TransformFeedback wrap(GLuint id, ObjectFlags flags = {}) {
            return TransformFeedback{id, flags};
        }

        /**
         * @brief Constructor
         *
         * Creates new OpenGL transform feedback object. If
         * @extension{ARB,direct_state_access} (part of OpenGL 4.5) is not
         * available, the transform feedback object is created on first use.
         * @see @ref TransformFeedback(NoCreateT), @ref wrap(),
         *      @fn_gl{CreateTransformFeedbacks}, eventually
         *      @fn_gl{GenTransformFeedbacks}
         */
        explicit TransformFeedback();

        /**
         * @brief Construct without creating the underlying OpenGL object
         *
         * The constructed instance is equivalent to moved-from state. Useful
         * in cases where you will overwrite the instance later anyway. Move
         * another object over it to make it useful.
         * @see @ref TransformFeedback(), @ref wrap()
         */
        explicit TransformFeedback(NoCreateT) noexcept: _id{0}, _flags{ObjectFlag::DeleteOnDestruction} {}

        /** @brief Copying is not allowed */
        TransformFeedback(const TransformFeedback&) = delete;

        /** @brief Move constructor */
        TransformFeedback(TransformFeedback&& other) noexcept;

        /**
         * @brief Destructor
         *
         * Deletes associated OpenGL transform feedback object.
         * @see @ref wrap(), @ref release(), @fn_gl{DeleteTransformFeedbacks}
         */
        ~TransformFeedback();

        /** @brief Copying is not allowed */
        TransformFeedback& operator=(const TransformFeedback&) = delete;

        /** @brief Move assignment */
        TransformFeedback& operator=(TransformFeedback&& other) noexcept;

        /** @brief OpenGL transform feedback ID */
        GLuint id() const { return _id; }

        /**
         * @brief Release OpenGL object
         *
         * Releases ownership of OpenGL transform feedback object and returns
         * its ID so it is not deleted on destruction. The internal state is
         * then equivalent to moved-from state.
         * @see @ref wrap()
         */
        GLuint release();

        #ifndef MAGNUM_TARGET_WEBGL
        /**
         * @brief Transform feedback label
         *
         * The result is *not* cached, repeated queries will result in repeated
         * OpenGL calls. If OpenGL 4.3 is not supported and neither
         * @extension{KHR,debug} (covered also by @es_extension{ANDROID,extension_pack_es31a})
         * nor @extension2{EXT,debug_label} desktop or ES extension is
         * available, this function returns empty string.
         * @see @fn_gl{GetObjectLabel} or @fn_gl_extension2{GetObjectLabel,EXT,debug_label}
         *      with @def_gl{TRANSFORM_FEEDBACK}
         * @requires_gles Debug output is not available in WebGL.
         */
        std::string label();

        /**
         * @brief Set transform feedback label
         * @return Reference to self (for method chaining)
         *
         * Default is empty string. If OpenGL 4.3 is not supported and neither
         * @extension{KHR,debug} (covered also by @es_extension{ANDROID,extension_pack_es31a})
         * nor @extension2{EXT,debug_label} desktop or ES extension is
         * available, this function does nothing.
         * @see @ref maxLabelLength(), @fn_gl{ObjectLabel} or
         *      @fn_gl_extension2{LabelObject,EXT,debug_label} with
         *      @def_gl{TRANSFORM_FEEDBACK}
         * @requires_gles Debug output is not available in WebGL.
         */
        TransformFeedback& setLabel(const std::string& label) {
            return setLabelInternal({label.data(), label.size()});
        }
        #endif

        /** @overload */
        template<std::size_t size> TransformFeedback& setLabel(const char(&label)[size]) {
            return setLabelInternal(label);
        }

        /**
         * @brief Attach range of buffer
         * @return Reference to self (for method chaining)
         *
         * The @p offset parameter must be aligned to 4 bytes. If
         * @extension{ARB,direct_state_access} (part of OpenGL 4.5) is not
         * available, the transform feedback object is bound (if not already)
         * and the operation is then done equivalently to
         * @ref Buffer::bind(Buffer::Target, UnsignedInt, GLintptr, GLsizeiptr).
         * @note This function is meant to be used only internally from
         *      @ref AbstractShaderProgram subclasses. See its documentation
         *      for more information.
         * @see @ref attachBuffers(), @ref maxBuffers()/@ref maxSeparateAttributes(),
         *      @fn_gl{TransformFeedbackBufferRange}, eventually
         *      @fn_gl{BindTransformFeedback} and @fn_gl{BindBuffersRange} or
         *      @fn_gl{BindBufferRange}
         */
        TransformFeedback& attachBuffer(UnsignedInt index, Buffer& buffer, GLintptr offset, GLsizeiptr size);

        /**
         * @brief Attach buffer
         * @return Reference to self (for method chaining)
         *
         * If @extension{ARB,direct_state_access} (part of OpenGL 4.5) is not
         * available, the transform feedback object is bound (if not already)
         * and the operation is then done equivalently to
         * @ref Buffer::bind(Buffer::Target, UnsignedInt).
         * @note This function is meant to be used only internally from
         *      @ref AbstractShaderProgram subclasses. See its documentation
         *      for more information.
         * @see @ref attachBuffers(), @ref maxBuffers()/@ref maxSeparateAttributes(),
         *      @fn_gl{TransformFeedbackBufferRange}, eventually
         *      @fn_gl{BindTransformFeedback} and @fn_gl{BindBuffersRange} or
         *      @fn_gl{BindBufferRange}
         */
        TransformFeedback& attachBuffer(UnsignedInt index, Buffer& buffer);

        /**
         * @brief Attach ranges of buffers
         * @return Reference to self (for method chaining)
         *
         * Attches first buffer in the list to @p firstIndex, second to
         * `firstIndex + 1` etc. Second parameter is offset, third is size. If
         * any buffer is `nullptr`, given attachment point is detached. The
         * range of indices must respect @ref maxBuffers() (@ref maxSeparateComponents()
         * in OpenGL ES or if @extension{ARB,transform_feedback3} (part of
         * OpenGL 4.0) is not available). The offsets must be aligned to 4
         * bytes. All the buffers must have allocated data store. If
         * @extension{ARB,direct_state_access} (part of OpenGL 4.5) is not
         * available, the transform feedback object is bound (if not already)
         * and the operation is then done equivalently to
         * @ref Buffer::bind(Buffer::Target, UnsignedInt, std::initializer_list<std::tuple<Buffer*, GLintptr, GLsizeiptr>>).
         * @note This function is meant to be used only internally from
         *      @ref AbstractShaderProgram subclasses. See its documentation
         *      for more information.
         * @see @ref attachBuffer(), @fn_gl{TransformFeedbackBufferRange},
         *      eventually @fn_gl{BindTransformFeedback} and
         *      @fn_gl{BindBuffersRange} or @fn_gl{BindBufferRange}
         */
        TransformFeedback& attachBuffers(UnsignedInt firstIndex, std::initializer_list<std::tuple<Buffer*, GLintptr, GLsizeiptr>> buffers);

        /**
         * @brief Attach buffers
         * @return Reference to self (for method chaining)
         *
         * Attches first buffer in the list to @p firstIndex, second to
         * `firstIndex + 1` etc. If any buffer is `nullptr`, given index is
         * detached. The range of indices must respect @ref maxBuffers()
         * (@ref maxSeparateComponents() in OpenGL ES or if
         * @extension{ARB,transform_feedback3} (part of OpenGL 4.0) is not
         * available). All the buffers must have allocated data store. If
         * @extension{ARB,direct_state_access} (part of OpenGL 4.5) is not
         * available, the transform feedback object is bound (if not already)
         * and the operation then is done equivalently to
         * @ref Buffer::bind(Buffer::Target, UnsignedInt, std::initializer_list<Buffer*>).
         * @note This function is meant to be used only internally from
         *      @ref AbstractShaderProgram subclasses. See its documentation
         *      for more information.
         * @see @ref attachBuffer(), @fn_gl{TransformFeedbackBufferBase},
         *      eventually @fn_gl{BindTransformFeedback} and
         *      @fn_gl{BindBuffersBase} or @fn_gl{BindBufferBase}
         */
        TransformFeedback& attachBuffers(UnsignedInt firstIndex, std::initializer_list<Buffer*> buffers);

        /**
         * @brief Begin transform feedback
         * @param shader        Shader from which to capture data
         * @param mode          Primitive mode
         *
         * When transform feedback is active, only shader given in @p shader
         * and meshes with primitive type (or geometry shaders with output
         * primitive type) compatible with @p mode can be used. Only one
         * transform feedback object can be active at a time.
         * @see @ref pause(), @ref end(), @fn_gl{BindTransformFeedback} and
         *      @fn_gl{BeginTransformFeedback}
         */
        void begin(AbstractShaderProgram& shader, PrimitiveMode mode);

        /**
         * @brief Pause transform feedback
         *
         * Pausing transform feedback makes it inactive, allowing to use
         * different shader, or starting another transform feedback.
         * @see @ref resume(), @ref end(), @fn_gl{BindTransformFeedback} and
         *      @fn_gl{PauseTransformFeedback}
         */
        void pause();

        /**
         * @brief Resume transform feedback
         *
         * Resumes transform feedback so the next captured data are appended to
         * already captured ones. The restrictions specified for @ref begin()
         * still apply after resuming. Only one transform feedback object can
         * be active at a time.
         * @see @ref pause(), @ref end(), @fn_gl{BindTransformFeedback} and
         *      @fn_gl{ResumeTransformFeedback}
         */
        void resume();

        /**
         * @brief End transform feedback
         *
         * Ends transform feedback so the captured data can be used.
         * @see @ref begin(), @fn_gl{BindTransformFeedback} and
         *      @fn_gl{EndTransformFeedback}
         */
        void end();

    private:
        explicit TransformFeedback(GLuint id, ObjectFlags flags) noexcept: _id{id}, _flags{flags} {}

        void bindInternal();

        void MAGNUM_LOCAL createIfNotAlready();

        void MAGNUM_LOCAL createImplementationDefault();
        #ifndef MAGNUM_TARGET_GLES
        void MAGNUM_LOCAL createImplementationDSA();
        #endif

        void MAGNUM_LOCAL attachImplementationFallback(GLuint index, Buffer& buffer, GLintptr offset, GLsizeiptr size);
        void MAGNUM_LOCAL attachImplementationFallback(GLuint index, Buffer& buffer);
        #ifndef MAGNUM_TARGET_GLES
        void MAGNUM_LOCAL attachImplementationDSA(GLuint index, Buffer& buffer, GLintptr offset, GLsizeiptr size);
        void MAGNUM_LOCAL attachImplementationDSA(GLuint index, Buffer& buffer);
        #endif

        void MAGNUM_LOCAL attachImplementationFallback(GLuint firstIndex, std::initializer_list<std::tuple<Buffer*, GLintptr, GLsizeiptr>> buffers);
        void MAGNUM_LOCAL attachImplementationFallback(GLuint firstIndex, std::initializer_list<Buffer*> buffers);
        #ifndef MAGNUM_TARGET_GLES
        void MAGNUM_LOCAL attachImplementationDSA(GLuint firstIndex, std::initializer_list<std::tuple<Buffer*, GLintptr, GLsizeiptr>> buffers);
        void MAGNUM_LOCAL attachImplementationDSA(GLuint firstIndex, std::initializer_list<Buffer*> buffers);
        #endif

        #ifndef MAGNUM_TARGET_WEBGL
        TransformFeedback& setLabelInternal(Containers::ArrayView<const char> label);
        #endif

        GLuint _id;
        ObjectFlags _flags;
};

inline TransformFeedback::TransformFeedback(TransformFeedback&& other) noexcept: _id{other._id}, _flags{other._flags} {
    other._id = 0;
}

inline TransformFeedback& TransformFeedback::operator=(TransformFeedback&& other) noexcept {
    using std::swap;
    swap(_id, other._id);
    swap(_flags, other._flags);
    return *this;
}

inline GLuint TransformFeedback::release() {
    const GLuint id = _id;
    _id = 0;
    return id;
}

}
#else
#error this header is not available in OpenGL ES 2.0 build
#endif

#endif

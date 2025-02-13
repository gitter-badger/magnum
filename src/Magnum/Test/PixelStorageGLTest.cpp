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

#include <Corrade/TestSuite/Compare/Container.h>

#include "Magnum/Image.h"
#include "Magnum/PixelFormat.h"
#include "Magnum/Texture.h"
#ifndef MAGNUM_TARGET_GLES2
#include "Magnum/TextureArray.h"
#endif
#include "Magnum/TextureFormat.h"
#include "Magnum/Test/AbstractOpenGLTester.h"

#ifdef MAGNUM_TARGET_GLES
#include "Magnum/Framebuffer.h"
#endif

namespace Magnum { namespace Test {

struct PixelStorageGLTest: AbstractOpenGLTester {
    explicit PixelStorageGLTest();

    void unpack2D();
    void pack2D();
    #ifndef MAGNUM_TARGET_GLES2
    void unpack3D();
    #endif
    #ifndef MAGNUM_TARGET_GLES
    void pack3D();
    #endif

    #ifndef MAGNUM_TARGET_GLES
    void unpackCompressed2D();
    void packCompressed2D();
    void unpackCompressed3D();
    void packCompressed3D();
    #endif
};

PixelStorageGLTest::PixelStorageGLTest() {
    addTests({&PixelStorageGLTest::unpack2D,
              &PixelStorageGLTest::pack2D,
              #ifndef MAGNUM_TARGET_GLES2
              &PixelStorageGLTest::unpack3D,
              #endif
              #ifndef MAGNUM_TARGET_GLES
              &PixelStorageGLTest::pack3D,
              #endif

              #ifndef MAGNUM_TARGET_GLES
              &PixelStorageGLTest::unpackCompressed2D,
              &PixelStorageGLTest::packCompressed2D,
              &PixelStorageGLTest::unpackCompressed3D,
              &PixelStorageGLTest::packCompressed3D
              #endif
              });
}

namespace {
    constexpr const char Data2D[] = {
        /* Skip */
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',

        /* Data */                                      /* Row length */        /* Alignment */
        '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x00', '\x00', '\x00', '\x00',
        '\x06', '\x07', '\x08', '\x09', '\x0a', '\x0b', '\x00', '\x00', '\x00', '\x00',
        '\x0c', '\x0d', '\x0e', '\x0f', '\x10', '\x11', '\x00', '\x00', '\x00', '\x00'
    };

    constexpr const char ActualData[] = {
        '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x00', '\x00',
        '\x06', '\x07', '\x08', '\x09', '\x0a', '\x0b', '\x00', '\x00',
        '\x0c', '\x0d', '\x0e', '\x0f', '\x10', '\x11', '\x00', '\x00'
    };
}

void PixelStorageGLTest::unpack2D() {
    #ifdef MAGNUM_TARGET_GLES2
    if(!Context::current()->isExtensionSupported<Extensions::GL::EXT::unpack_subimage>())
        CORRADE_SKIP(Extensions::GL::EXT::unpack_subimage::string() + std::string(" is not supported."));
    #endif

    PixelStorage storage;
    storage.setAlignment(2)
        .setRowLength(3)
        .setSkip({2, 3, 0});

    ImageView2D image{storage, PixelFormat::RGB, PixelType::UnsignedByte, {2, 3}, Data2D};

    Texture2D texture;
    texture.setStorage(1, TextureFormat::RGB8, {2, 3})
        .setSubImage(0, {}, image);

    MAGNUM_VERIFY_NO_ERROR();

    Image2D actual{PixelFormat::RGB, PixelType::UnsignedByte};

    #ifndef MAGNUM_TARGET_GLES
    texture.image(0, actual);
    #else
    Framebuffer fb{{{}, {2, 3}}};
    fb.attachTexture(Framebuffer::ColorAttachment{0}, texture, 0);
    fb.read(fb.viewport(), actual);
    #endif

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS(actual.data(), Containers::ArrayView<const char>{ActualData},
        TestSuite::Compare::Container);
}

void PixelStorageGLTest::pack2D() {
    #ifdef MAGNUM_TARGET_GLES2
    if(!Context::current()->isExtensionSupported<Extensions::GL::NV::pack_subimage>())
        CORRADE_SKIP(Extensions::GL::NV::pack_subimage::string() + std::string(" is not supported."));
    #endif

    ImageView2D actual{PixelFormat::RGB, PixelType::UnsignedByte, {2, 3}, ActualData};

    Texture2D texture;
    texture.setStorage(1, TextureFormat::RGB8, {2, 3})
        .setSubImage(0, {}, actual);

    MAGNUM_VERIFY_NO_ERROR();

    /* Pre-allocate and zero out the data array so we can conveniently compare */
    Image2D image{PixelStorage{}
        .setAlignment(2)
        .setRowLength(3)
        .setSkip({2, 3, 0}),
        PixelFormat::RGB, PixelType::UnsignedByte, {}, Containers::Array<char>{Containers::ValueInit, sizeof(Data2D)}};

    #ifndef MAGNUM_TARGET_GLES
    texture.image(0, image);
    #else
    Framebuffer fb{{{}, {2, 3}}};
    fb.attachTexture(Framebuffer::ColorAttachment{0}, texture, 0);
    fb.read(fb.viewport(), image);
    #endif

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS(image.data(), Containers::ArrayView<const char>{Data2D},
        TestSuite::Compare::Container);
}

#ifndef MAGNUM_TARGET_GLES2
namespace {
    constexpr const char Data3D[] = {
        /* Skip */
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',

        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',

        /* Data */                                      /* Row length */        /* Alignment */
        '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x00', '\x00', '\x00', '\x00',
        '\x06', '\x07', '\x08', '\x09', '\x0a', '\x0b', '\x00', '\x00', '\x00', '\x00',
        '\x0c', '\x0d', '\x0e', '\x0f', '\x10', '\x11', '\x00', '\x00', '\x00', '\x00',

        /* Filling to image height not needed */
    };
}

void PixelStorageGLTest::unpack3D() {
    PixelStorage storage;
    storage.setAlignment(2)
        .setRowLength(3)
        .setImageHeight(5)
        .setSkip({2, 3, 1});

    ImageView3D image{storage, PixelFormat::RGB, PixelType::UnsignedByte, {2, 3, 1}, Data3D};

    Texture3D texture;
    texture.setStorage(1, TextureFormat::RGB8, {2, 3, 1})
        .setSubImage(0, {}, image);

    MAGNUM_VERIFY_NO_ERROR();

    /* Testing mainly image height here, which is not available as pack
       parameter in ES */
    #ifndef MAGNUM_TARGET_GLES
    Image3D actual = texture.image(0, {PixelFormat::RGB, PixelType::UnsignedByte});

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS(actual.data(), Containers::ArrayView<const char>{ActualData},
        TestSuite::Compare::Container);
    #endif
}
#endif

/* Testing mainly image height here, which is not available as pack parameter
   in ES */
#ifndef MAGNUM_TARGET_GLES
void PixelStorageGLTest::pack3D() {
    ImageView3D actual{PixelFormat::RGB, PixelType::UnsignedByte, {2, 3, 1}, ActualData};

    Texture3D texture;
    texture.setStorage(1, TextureFormat::RGB8, {2, 3, 1})
        .setSubImage(0, {}, actual);

    MAGNUM_VERIFY_NO_ERROR();

    Image3D image{PixelStorage{}
        .setAlignment(2)
        .setRowLength(3)
        .setImageHeight(5)
        .setSkip({2, 3, 1}),
        PixelFormat::RGB, PixelType::UnsignedByte, {}, Containers::Array<char>{Containers::ValueInit, sizeof(Data3D)}};
    texture.image(0, image);

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS(image.data(), Containers::ArrayView<const char>{Data3D},
        TestSuite::Compare::Container);
}
#endif

#ifndef MAGNUM_TARGET_GLES
namespace {
    constexpr const UnsignedByte CompressedData2D[] = {
        /* Skip */
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

          0,  17,  17,  34,  34,  51,  51,  67,
        232,  57,   0,   0, 213, 255, 170,   2
    };

    /* Just 4x4 0x00 - 0x3f compressed using RGBA DXT3 by the driver */
    constexpr const UnsignedByte ActualCompressedData[] = {
          0,  17,  17,  34,  34,  51,  51,  67,
        232,  57,   0,   0, 213, 255, 170,   2,
    };
}

void PixelStorageGLTest::unpackCompressed2D() {
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::compressed_texture_pixel_storage>())
        CORRADE_SKIP(Extensions::GL::ARB::compressed_texture_pixel_storage::string() + std::string(" is not supported."));

    CompressedPixelStorage storage;
    storage.setCompressedBlockSize({4, 4, 1})
        .setCompressedBlockDataSize(16)
        .setRowLength(12)
        .setSkip({4, 4, 0});

    CompressedImageView2D image{storage, CompressedPixelFormat::RGBAS3tcDxt3, {4, 4}, CompressedData2D};

    Texture2D texture;
    texture.setStorage(1, TextureFormat::CompressedRGBAS3tcDxt3, {4, 4})
        .setCompressedSubImage(0, {}, image);

    MAGNUM_VERIFY_NO_ERROR();

    CompressedImage2D actual = texture.compressedImage(0, {});

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS((Containers::ArrayView<const UnsignedByte>{actual.data<UnsignedByte>(), actual.data().size()}),
        Containers::ArrayView<const UnsignedByte>{ActualCompressedData},
        TestSuite::Compare::Container);
}

void PixelStorageGLTest::packCompressed2D() {
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::compressed_texture_pixel_storage>())
        CORRADE_SKIP(Extensions::GL::ARB::compressed_texture_pixel_storage::string() + std::string(" is not supported."));

    CompressedImageView2D actual{CompressedPixelFormat::RGBAS3tcDxt3, {4, 4}, ActualCompressedData};

    Texture2D texture;
    texture.setStorage(1, TextureFormat::CompressedRGBAS3tcDxt3, {4, 4})
        .setCompressedSubImage(0, {}, actual);

    MAGNUM_VERIFY_NO_ERROR();

    /* Pre-allocate and zero out the data array so we can conveniently compare */
    CompressedImage2D image{CompressedPixelStorage{}
        .setCompressedBlockSize({4, 4, 1})
        .setCompressedBlockDataSize(16)
        .setRowLength(12)
        .setSkip({4, 4, 0}),
        CompressedPixelFormat::RGBAS3tcDxt3, {}, Containers::Array<char>{Containers::ValueInit, sizeof(CompressedData2D)}};
    texture.compressedImage(0, image);

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS((Containers::ArrayView<const UnsignedByte>{image.data<UnsignedByte>(), image.data().size()}),
        Containers::ArrayView<const UnsignedByte>{CompressedData2D},
        TestSuite::Compare::Container);
}

namespace {
    constexpr const UnsignedByte CompressedData3D[] = {
        /* Skip image */
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

        /* Skip rows and pixels */
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

          0,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,

          0,  17,  17,  34,  34,  51,  51,  67,
        232,  57,   0,   0, 213, 255, 170,   2
    };
}

void PixelStorageGLTest::unpackCompressed3D() {
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::compressed_texture_pixel_storage>())
        CORRADE_SKIP(Extensions::GL::ARB::compressed_texture_pixel_storage::string() + std::string(" is not supported."));

    CompressedPixelStorage storage;
    storage.setCompressedBlockSize({4, 4, 1})
        .setCompressedBlockDataSize(16)
        .setRowLength(8)
        .setImageHeight(8)
        .setSkip({4, 4, 4});

    CompressedImageView3D image{storage, CompressedPixelFormat::RGBAS3tcDxt3, {4, 4, 1}, CompressedData3D};

    Texture2DArray texture;
    texture.setStorage(1, TextureFormat::CompressedRGBAS3tcDxt3, {4, 4, 1})
        .setCompressedSubImage(0, {}, image);

    MAGNUM_VERIFY_NO_ERROR();

    CompressedImage3D actual = texture.compressedImage(0, {});

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS((Containers::ArrayView<const UnsignedByte>{actual.data<UnsignedByte>(), actual.data().size()}),
        Containers::ArrayView<const UnsignedByte>{ActualCompressedData},
        TestSuite::Compare::Container);
}

void PixelStorageGLTest::packCompressed3D() {
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::compressed_texture_pixel_storage>())
        CORRADE_SKIP(Extensions::GL::ARB::compressed_texture_pixel_storage::string() + std::string(" is not supported."));

    CompressedImageView3D actual{CompressedPixelFormat::RGBAS3tcDxt3, {4, 4, 1}, ActualCompressedData};

    Texture2DArray texture;
    texture.setStorage(1, TextureFormat::CompressedRGBAS3tcDxt3, {4, 4, 1})
        .setCompressedSubImage(0, {}, actual);

    MAGNUM_VERIFY_NO_ERROR();

    /* Pre-allocate and zero out the data array so we can conveniently compare */
    CompressedImage3D image{CompressedPixelStorage{}
        .setCompressedBlockSize({4, 4, 1})
        .setCompressedBlockDataSize(16)
        .setRowLength(8)
        .setImageHeight(8)
        .setSkip({4, 4, 4}),
        CompressedPixelFormat::RGBAS3tcDxt3, {}, Containers::Array<char>{Containers::ValueInit, sizeof(CompressedData3D)}};
    texture.compressedImage(0, image);

    MAGNUM_VERIFY_NO_ERROR();

    CORRADE_COMPARE_AS((Containers::ArrayView<const UnsignedByte>{image.data<UnsignedByte>(), image.data().size()}),
        Containers::ArrayView<const UnsignedByte>{CompressedData3D},
        TestSuite::Compare::Container);
}
#endif

}}

MAGNUM_GL_TEST_MAIN(Magnum::Test::PixelStorageGLTest)

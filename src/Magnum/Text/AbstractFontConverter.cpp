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

#include "AbstractFontConverter.h"

#include <algorithm>
#include <Corrade/Containers/Array.h>
#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/Unicode.h>

#include "Magnum/Text/GlyphCache.h"

namespace Magnum { namespace Text {

namespace {

std::u32string uniqueUnicode(const std::string& characters)
{
    /* Convert UTF-8 to UTF-32 */
    std::u32string result = Utility::Unicode::utf32(characters);

    /* Remove duplicate glyphs */
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

}

AbstractFontConverter::AbstractFontConverter() = default;

AbstractFontConverter::AbstractFontConverter(PluginManager::AbstractManager& manager, std::string plugin): PluginManager::AbstractPlugin(manager, std::move(plugin)) {}

std::vector<std::pair<std::string, Containers::Array<char>>> AbstractFontConverter::exportFontToData(AbstractFont& font, GlyphCache& cache, const std::string& filename, const std::string& characters) const {
    CORRADE_ASSERT(features() >= (Feature::ExportFont|Feature::ConvertData),
        "Text::AbstractFontConverter::exportFontToData(): feature not supported", {});

    return doExportFontToData(font, cache, filename, uniqueUnicode(characters));
}

std::vector<std::pair<std::string, Containers::Array<char>>> AbstractFontConverter::doExportFontToData(AbstractFont& font, GlyphCache& cache, const std::string& filename, const std::u32string& characters) const {
    CORRADE_ASSERT(!(features() & Feature::MultiFile),
        "Text::AbstractFontConverter::exportFontToData(): feature advertised but not implemented", {});

    std::vector<std::pair<std::string, Containers::Array<char>>> out;
    out.emplace_back(filename, std::move(doExportFontToSingleData(font, cache, characters)));
    return out;
}

Containers::Array<char> AbstractFontConverter::exportFontToSingleData(AbstractFont& font, GlyphCache& cache, const std::string& characters) const {
    CORRADE_ASSERT(features() >= (Feature::ExportFont|Feature::ConvertData),
        "Text::AbstractFontConverter::exportFontToSingleData(): feature not supported", nullptr);
    CORRADE_ASSERT(!(features() & Feature::MultiFile),
        "Text::AbstractFontConverter::exportFontToSingleData(): the format is not single-file", nullptr);

    return doExportFontToSingleData(font, cache, uniqueUnicode(characters));
}

Containers::Array<char> AbstractFontConverter::doExportFontToSingleData(AbstractFont&, GlyphCache&, const std::u32string&) const {
    CORRADE_ASSERT(false,
        "Text::AbstractFontConverter::exportFontToSingleData(): feature advertised but not implemented", nullptr);
    return nullptr;
}

bool AbstractFontConverter::exportFontToFile(AbstractFont& font, GlyphCache& cache, const std::string& filename, const std::string& characters) const {
    CORRADE_ASSERT(features() & Feature::ExportFont,
        "Text::AbstractFontConverter::exportFontToFile(): feature not supported", false);

    return doExportFontToFile(font, cache, filename, uniqueUnicode(characters));
}

bool AbstractFontConverter::doExportFontToFile(AbstractFont& font, GlyphCache& cache, const std::string& filename, const std::u32string& characters) const {
    CORRADE_ASSERT(features() & Feature::ConvertData,
        "Text::AbstractFontConverter::exportFontToFile(): not implemented", false);

    /* Export all data */
    const auto data = doExportFontToData(font, cache, filename, characters);
    for(const auto& d: data) if(!Utility::Directory::write(d.first, d.second)) {
        Error() << "Text::AbstractFontConverter::exportFontToFile(): cannot write to file" << d.first;
        return false;
    }

    return true;
}

std::vector<std::pair<std::string, Containers::Array<char>>> AbstractFontConverter::exportGlyphCacheToData(GlyphCache& cache, const std::string& filename) const {
    CORRADE_ASSERT(features() >= (Feature::ExportGlyphCache|Feature::ConvertData),
        "Text::AbstractFontConverter::exportGlyphCacheToData(): feature not supported", {});

    return doExportGlyphCacheToData(cache, filename);
}

std::vector<std::pair<std::string, Containers::Array<char>>> AbstractFontConverter::doExportGlyphCacheToData(GlyphCache& cache, const std::string& filename) const {
    CORRADE_ASSERT(!(features() & Feature::MultiFile),
        "Text::AbstractFontConverter::exportGlyphCacheToData(): feature advertised but not implemented", {});

    std::vector<std::pair<std::string, Containers::Array<char>>> out;
    out.emplace_back(filename, std::move(doExportGlyphCacheToSingleData(cache)));
    return out;
}

Containers::Array<char> AbstractFontConverter::exportGlyphCacheToSingleData(GlyphCache& cache) const {
    CORRADE_ASSERT(features() >= (Feature::ExportGlyphCache|Feature::ConvertData),
        "Text::AbstractFontConverter::exportGlyphCacheToSingleData(): feature not supported", nullptr);
    CORRADE_ASSERT(!(features() & Feature::MultiFile),
        "Text::AbstractFontConverter::exportGlyphCacheToSingleData(): the format is not single-file", nullptr);

    return doExportGlyphCacheToSingleData(cache);
}

Containers::Array<char> AbstractFontConverter::doExportGlyphCacheToSingleData(GlyphCache&) const {
    CORRADE_ASSERT(false,
        "Text::AbstractFontConverter::exportGlyphCacheToSingleData(): feature advertised but not implemented", nullptr);
    return nullptr;
}

bool AbstractFontConverter::exportGlyphCacheToFile(GlyphCache& cache, const std::string& filename) const {
    CORRADE_ASSERT(features() & Feature::ExportGlyphCache,
        "Text::AbstractFontConverter::exportGlyphCacheToFile(): feature not supported", false);

    return doExportGlyphCacheToFile(cache, filename);
}

bool AbstractFontConverter::doExportGlyphCacheToFile(GlyphCache& cache, const std::string& filename) const {
    CORRADE_ASSERT(features() & Feature::ConvertData,
        "Text::AbstractFontConverter::exportGlyphCacheToFile(): not implemented", false);

    /* Export all data */
    const auto data = doExportGlyphCacheToData(cache, filename);
    for(const auto& d: data) if(!Utility::Directory::write(d.first, d.second)) {
        Error() << "Text::AbstractFontConverter::exportGlyphCacheToFile(): cannot write to file" << d.first;
        return false;
    }

    return true;
}

std::unique_ptr<GlyphCache> AbstractFontConverter::importGlyphCacheFromData(const std::vector<std::pair<std::string, Containers::ArrayView<const char>>>& data) const {
    CORRADE_ASSERT(features() >= (Feature::ImportGlyphCache|Feature::ConvertData),
        "Text::AbstractFontConverter::importGlyphCacheFromData(): feature not supported", nullptr);
    CORRADE_ASSERT(!data.empty(),
        "Text::AbstractFontConverter::importGlyphCacheFromData(): no data passed", nullptr);

    return doImportGlyphCacheFromData(data);
}

std::unique_ptr<GlyphCache> AbstractFontConverter::doImportGlyphCacheFromData(const std::vector<std::pair<std::string, Containers::ArrayView<const char>>>& data) const {
    CORRADE_ASSERT(!(features() & Feature::MultiFile),
        "Text::AbstractFontConverter::importGlyphCacheFromData(): feature advertised but not implemented", nullptr);
    CORRADE_ASSERT(data.size() == 1,
        "Text::AbstractFontConverter::importGlyphCacheFromData(): expected just one file for single-file format", nullptr);

    return doImportGlyphCacheFromSingleData(data[0].second);
}

std::unique_ptr<GlyphCache> AbstractFontConverter::importGlyphCacheFromSingleData(Containers::ArrayView<const char> data) const {
    CORRADE_ASSERT(features() >= (Feature::ImportGlyphCache|Feature::ConvertData),
        "Text::AbstractFontConverter::importGlyphCacheFromSingleData(): feature not supported", nullptr);
    CORRADE_ASSERT(!(features() & Feature::MultiFile),
        "Text::AbstractFontConverter::importGlyphCacheFromSingleData(): the format is not single-file", nullptr);

    return doImportGlyphCacheFromSingleData(data);
}

std::unique_ptr<GlyphCache> AbstractFontConverter::doImportGlyphCacheFromSingleData(Containers::ArrayView<const char>) const {
    CORRADE_ASSERT(false,
        "Text::AbstractFontConverter::importGlyphCacheFromSingleData(): feature advertised but not implemented", nullptr);
    return nullptr;
}

std::unique_ptr<GlyphCache> AbstractFontConverter::importGlyphCacheFromFile(const std::string& filename) const {
    CORRADE_ASSERT(features() & Feature::ImportGlyphCache,
        "Text::AbstractFontConverter::importGlyphCacheFromFile(): feature not supported", nullptr);

    return doImportGlyphCacheFromFile(filename);
}

std::unique_ptr<GlyphCache> AbstractFontConverter::doImportGlyphCacheFromFile(const std::string& filename) const {
    CORRADE_ASSERT(features() & Feature::ConvertData && !(features() & Feature::MultiFile),
        "Text::AbstractFontConverter::importGlyphCacheFromFile(): not implemented", nullptr);

    /* Open file */
    if(!Utility::Directory::fileExists(filename)) {
        Error() << "Trade::AbstractFontConverter::importGlyphCacheFromFile(): cannot open file" << filename;
        return nullptr;
    }

    return doImportGlyphCacheFromSingleData(Utility::Directory::read(filename));
}

}}

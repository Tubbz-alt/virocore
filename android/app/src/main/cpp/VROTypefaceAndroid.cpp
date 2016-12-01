//
//  VROTypefaceAndroid.cpp
//  ViroRenderer
//
//  Created by Raj Advani on 11/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#include "VROTypefaceAndroid.h"
#include "VROLog.h"
#include "VROGlyphOpenGL.h"

static const std::string kSystemFont = "Roboto-Regular";

VROTypefaceAndroid::VROTypefaceAndroid(std::string name, int size) :
        VROTypeface(name, size) {

}

VROTypefaceAndroid::~VROTypefaceAndroid() {

}

FT_Face VROTypefaceAndroid::loadFace(std::string name, int size, FT_Library ft) {
    FT_Face face;
    if (FT_New_Face(_ft, getFontPath(name).c_str(), 0, &face)) {
        pinfo("Failed to load font %s, loading system font", name.c_str());

        if (FT_New_Face(_ft, getFontPath(kSystemFont).c_str(), 0, &face)) {
            pabort("Failed to load system font %s", kSystemFont.c_str());
        }
    }

    FT_Set_Pixel_Sizes(face, 0, size);
    return face;
}

std::unique_ptr<VROGlyph> VROTypefaceAndroid::loadGlyph(FT_ULong charCode, bool forRendering) {
    std::unique_ptr<VROGlyph> glyph = std::unique_ptr<VROGlyph>(new VROGlyphOpenGL());
    glyph->load(_face, charCode, forRendering);

    return glyph;
}

std::string VROTypefaceAndroid::getFontPath(std::string fontName) {
    std::string prefix = "/system/fonts/";
    std::string suffix = ".ttf";

    return prefix + fontName + suffix;
}
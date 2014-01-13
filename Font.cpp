/*
 * Copyright 2013 Cepheid
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*/
#include <utility>
#include <boost/filesystem.hpp>
#include "Font.hpp"
#include <ft2build.h>
#include <iostream>
#include "Utility.hpp"
#include "Common.hpp"
#include FT_FREETYPE_H

using namespace boost::filesystem;

namespace io {
  Font::Font(FT_Library library, const std::string& filename) {
    this->library = library;
    activeGlyphSet = nullptr;
    face = nullptr;
    mesh = new Mesh();
    pixelSize = 0;

    loadFont(filename);
    setPixelSize(Font::DEFAULT_PIXEL_SIZE);
  }
  
  Font::~Font() {
    if (face) {
      FT_Done_Face(face);
    }

    for (std::pair<uint32_t, GlyphSet*> glyphSet : glyphSets) {
      delete glyphSet.second->tex;
      delete glyphSet.second;
    }
  }

  void Font::drawText(const std::string& text, const Colour& colour) {
    activeGlyphSet->tex->makeActive();
    mesh->begin(GL_TRIANGLES);
    float x = 0;
    uint8_t previous = 0;
    float texH = activeGlyphSet->tex->getHeight();
    float texW = activeGlyphSet->tex->getWidth();

    for (uint8_t c : text) {
      FontGlyph g = activeGlyphSet->glyphs[c];
      
      float y = pixelSize - g.bl;
      
      if (previous != 0) {
        FT_Vector delta;
        FT_Get_Kerning(face, previous, c, FT_KERNING_DEFAULT, &delta);
        x += delta.x >> 6;
      }

      mesh->addVertex(Vector3(x, y, 0), Vector2(g.x, g.y), Colour(255, 255, 255));
      mesh->addVertex(Vector3(x, y + (g.h * texH), 0), Vector2(g.x, g.y + g.h), Colour(255, 255, 255));
      mesh->addVertex(Vector3(x + (g.w * texW), y + (g.h * texH), 0), Vector2(g.x + g.w, g.y + g.h), Colour(255, 255, 255));

      mesh->addVertex(Vector3(x + (g.w * texW), y + (g.h * texH), 0), Vector2(g.x + g.w, g.y + g.h), Colour(255, 255, 255));
      mesh->addVertex(Vector3(x + (g.w * texW), y, 0), Vector2(g.x + g.w, g.y), Colour(255, 255, 255));
      mesh->addVertex(Vector3(x, y, 0), Vector2(g.x, g.y), Colour(255, 255, 255));

      x += g.a;
      previous = c;
    }
    mesh->end();

    glEnable(GL_BLEND);
    glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_COLOR);
    float r, g, b, a;
    r = (float)colour.getR() / 255.0f;
    g = (float)colour.getG() / 255.0f;
    b = (float)colour.getB() / 255.0f;
    a = (float)colour.getA() / 255.0f;

    glBlendColor(r, g, b, a);
    mesh->draw();
    glDisable(GL_BLEND);
  }

  BoundingBox Font::getTextBoundingBox(const std::string& text) {
    float w = 0.0f;
    float h = 0.0f;

    uint8_t previous = 0;
    h = pixelSize;
    for (uint8_t c : text) {
      FontGlyph g = activeGlyphSet->glyphs[c];

      float y = pixelSize - g.bl;

      if (previous != 0) {
        FT_Vector delta;
        FT_Get_Kerning(face, previous, c, FT_KERNING_DEFAULT, &delta);
        w += delta.x >> 6;
      }

      w += g.w;
      w += g.a;
      previous = c;
    }
    return BoundingBox(0, 0, w, h);
  }

  bool Font::loadFont(const std::string& filename) {
    path p(filename);
    if (exists(p) && is_regular_file(p)) {
      FT_New_Face(library, filename.c_str(), 0, &face);
    }

    return true;
  }

  void Font::setPixelSize(const uint32_t pixelSize) {
    FT_Set_Char_Size(face, 0, pixelSize << 6, 96, 96);

    if (glyphSets.count(pixelSize) > 0) {
      activeGlyphSet = glyphSets.at(pixelSize);
    }

    //  First, figure out what the dimensions are of the largest glyph in
    //  pixels.
    uint32_t maxGlyphWidth = 0;
    uint32_t maxGlyphHeight = 0;

    for (uint32_t i = 0; i < Font::MAX_GLYPHS; i++) {
      FT_Load_Char(face, i, FT_LOAD_RENDER);
      if (face->glyph->bitmap.width > maxGlyphWidth) {
        maxGlyphWidth = face->glyph->bitmap.width;
      }

      if (face->glyph->bitmap.rows > maxGlyphHeight) {
        maxGlyphHeight = face->glyph->bitmap.rows;
      }
    }

    //  Calculate image dimensions based on the above.
    uint32_t imageWidth = minPowerOfTwo(16 * maxGlyphWidth);
    uint32_t imageHeight = minPowerOfTwo(16 * maxGlyphHeight);
    uint32_t xOffset = imageWidth / 16;
    uint32_t yOffset = imageHeight / 16;

    Image m;
    m.setSize(imageWidth, imageHeight);
    GlyphSet* glyphSet = new GlyphSet;

    for (uint32_t i = 0; i < Font::MAX_GLYPHS; i++) {
      FT_Load_Char(face, i, FT_LOAD_RENDER);

      float texX = (float)((i % 16) * xOffset) / imageWidth;
      float texY = (float)((i / 16) * yOffset) / imageHeight;

      //  Store all the attributes of the glyph that we're interested in,
      //  namely, texture position, dimensions, advance and baseline
      //  adjustment.
      glyphSet->glyphs[i].x = texX;
      glyphSet->glyphs[i].y = texY;
      glyphSet->glyphs[i].w = (float)face->glyph->bitmap.width / imageWidth;
      glyphSet->glyphs[i].h = (float)face->glyph->bitmap.rows / imageHeight;
      glyphSet->glyphs[i].a = face->glyph->metrics.horiAdvance >> 6;
      glyphSet->glyphs[i].bl = face->glyph->metrics.horiBearingY >> 6;

      //  Where to draw in the texture image.
      uint32_t baseX = (i % 16) * xOffset;
      uint32_t baseY = (i / 16) * yOffset;

      for (uint32_t y = 0; y < face->glyph->bitmap.rows; y++) {
        for (uint32_t x = 0; x < face->glyph->bitmap.width; x++) {
          uint8_t pix = face->glyph->bitmap.buffer[(y * face->glyph->bitmap.width) + x];
          m.setPixel(baseX + x, baseY + y, pix, pix, pix, 255);
        }
      }
    }
    glyphSet->tex = new Texture(&m);
    this->glyphSets[pixelSize] = glyphSet;
    activeGlyphSet = glyphSet;
    this->pixelSize = pixelSize;
  }
}

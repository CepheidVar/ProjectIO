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
#ifndef FontHPP
#define FontHPP

#include "FontGlyph.hpp"
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Texture.hpp"
#include "Mesh.hpp"
#include "BoundingBox.hpp"
#include "Resource.hpp"
#include "Colour.hpp"

namespace io {
  /**
   * To make proper use of this class in OpenGL, GL_BLEND needs to be enabled,
   * and the blending functions need to be set to GL_CONSTANT_COLOR for the
   * source factor, and GL_ONE_MINUS_SRC_COLOR for the destination factor.
   * To change the rendering colour of the text, use glBlendColor().
   */
  class Font : public Resource {
    public:
      Font();
      virtual ~Font();
      
      static Font* fontFromFile(const std::string& filename);
      
      void drawText(const std::string& text,
                    const Colour& colour = Colour(255, 255, 255, 255));
      
      uint32_t getPixelSize() const {
        return pixelSize;
      }

      void setPixelSize(const uint32_t pixelSize);
      BoundingBox getTextBoundingBox(const std::string& text);

      virtual Font* toFont() {
        return this;
      }
    private:
      const static uint32_t DEFAULT_PIXEL_SIZE = 12;
      const static uint32_t MAX_GLYPHS = 256;
      
      Font(const Font&) = delete;
      Font& operator=(const Font&) = delete;

      struct GlyphSet {
          Texture* tex = nullptr;
          FontGlyph glyphs[Font::MAX_GLYPHS];
      };

      std::map<uint32_t, GlyphSet*> glyphSets;

      GlyphSet* activeGlyphSet = nullptr;;
      FT_Face face = nullptr;
      Mesh* mesh = new Mesh();
      uint32_t pixelSize = 0;
  };
}

#endif // FontHPP

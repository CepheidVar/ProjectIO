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
#ifndef LabelHPP
#define LabelHPP

#include "Font.hpp"
#include "Component.hpp"
#include "Graphics.hpp"
#include <string>

namespace io {
  enum class HAlign { 
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2,
  };
  
  enum class VAlign {
    TOP = 0,
    MIDDLE = 1,
    BOTTOM = 2,
  };
  
  class Label : public Component {
  public:
    Label();
    
    virtual ~Label() {
    }
    
    Font* getFont() const {
      return font;
    }

    HAlign getHorizontalAlignment() const {
      return horizontalAlignment;
    }
    
    std::string getText() const {
      return text;
    }
    
    VAlign getVerticalAlignment() const {
      return verticalAlignment;
    }
    
    void setFont(Font* font) {
      this->font = font;
    }

    void setHorizontalAlignment(const HAlign horizontalAlignment) {
      this->horizontalAlignment = horizontalAlignment;
    }
    
    void setText(const std::string& text) {
      this->text = text;
    }
    
    void setVerticalAlignment(const VAlign verticalAlignment) {
      this->verticalAlignment = verticalAlignment;
    }
  protected:
    virtual void render(Graphics* g);
  private:
    Font* font;
    std::string text;
    
    HAlign horizontalAlignment;
    VAlign verticalAlignment;
  };
}

#endif // LabelHPP

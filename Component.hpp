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
#ifndef ComponentHPP
#define ComponentHPP

#include "InputEvent.hpp"
#include "Graphics.hpp"
#include "Utility.hpp"

namespace io {
  class Component {
  public:
    Component() {}
    virtual ~Component() {}
    
    void draw(Graphics* g);
    
    float getHeight() const {
      return size.height;
    }
    
    Point getPosition() const {
      return position;
    }
    
    Dimension getSize() const {
      return size;
    }
    
    float getWidth() const {
      return size.width;
    }
    
    float getX() const {
      return position.x;
    }
    
    float getY() const {
      return position.y;
    }
    
    virtual bool handleInputEvent(const InputEvent& event) {
      return false;
    }

    void setPosition(const Point& position) {
      this->position = position;
    }
    
    void setPosition(const float x, const float y) {
      setPosition(Point(x, y));
    }
    
    void setSize(const Dimension& size) {
      this->size = size;
    }
    
    void setSize(const float width, const float height) {
      setSize(Dimension(width, height));
    }
  protected:
    virtual void render(Graphics* g) = 0;
  private:
    Point position;
    Dimension size;
  };
}

#endif // ComponentHPP

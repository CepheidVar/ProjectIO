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
#ifndef ColourHPP
#define ColourHPP

#include <cstdint>
#include <cmath>
#include <iostream>

namespace io {
  class Colour {
  public:
    Colour() { }
    
    /*
     * Rather than a sized type, plain int is used here to ensure that all calls
     * using plain integers are guaranteed to work regardless of what the
     * underlying int type is(int32_t or int64_t, for example.)
     */

    explicit Colour(const int r, const int g, const int b)
      : Colour() {
      setColour(r, g, b, 255);
    }
    
    explicit Colour(const int r, const int g, const int b, const int a)
      : Colour() {
      setColour(r, g, b, a);
    }
    
    explicit Colour(const float r, const float g, const float b)
      : Colour() {
      setColour(std::round(255.0f * r),
                std::round(255.0f * g),
                std::round(255.0f * b),
                255);
    }
    
    explicit Colour(const float r, const float g, const float b,
                    const float a) 
      : Colour() {
      setColour(std::round(255.0f * r),
                std::round(255.0f * g),
                std::round(255.0f * b),
                std::round(255.0f * a));
    }
    
    explicit Colour(const double r, const double g, const double b)
      : Colour((float)r, (float)g, float(b)) {
    }
    
    explicit Colour(const double r, const double g, const double b,
                    const double a)
      : Colour((float)r, (float)g, (float)b, (float)a) {
    }
    
    Colour(const Colour& rhs) {
      setColour(rhs.getR(), rhs.getG(), rhs.getB(), rhs.getA());
    }
    
    bool operator==(const Colour& rhs) {
      return (getR() == rhs.getR() && getG() == rhs.getG() &&
              getB() == rhs.getB() && getA() == rhs.getA());
    }

    Colour& operator=(const Colour& rhs) {
      if (&rhs != this) {
        setColour(rhs.getR(), rhs.getG(), rhs.getB(), rhs.getA());
      }

      return *this;
    }
    
    uint8_t getA() const {
      return this->a;
    }

    uint8_t getB() const {
      return this->b;
    }

    uint8_t getG() const {
      return this->g;
    }

    uint8_t getR() const {
      return this->r;
    }
  private:
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    void setColour(const int r, const int g, const int b, const int a);
    uint8_t clamp(const int value);
  };
}

#endif

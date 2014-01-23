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
#include "Colour.hpp"

namespace io {
  /*  Simply clamps the value between 0 and 255.
   *  I use int to allow for saturating arithmetic.  It will still break
   *  with larger numbers, admittedly, but it's better than nothing at all.
   */
  uint8_t Colour::clamp(const int value) {
    if (value < 0) {
      return (uint8_t)0;
    }
    
    if (value > 255) {
      return (uint8_t)255;
    }
    
    return (uint8_t)value;
  }

  void Colour::setColour(const int r, const int g, const int b, const int a) {
    this->r = this->clamp(r);
    this->g = this->clamp(g);
    this->b = this->clamp(b);
    this->a = this->clamp(a);
  }
}

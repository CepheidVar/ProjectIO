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
#ifndef UtilityHPP
#define UtilityHPP

#include <cmath>
#include <cstdint>

namespace io {
#ifndef M_PI
#define M_PI 3.14159265359
#endif

#ifndef M_TAU
#define M_TAU M_PI * 2
#endif

  struct Dimension {
    Dimension() {
      width = 0.0f;
      height = 0.0f;
    }
    
    Dimension(const float width, const float height) {
      this->width = width;
      this->height = height;
    }
    
    float width;
    float height;
  };
  
  struct Point {
    Point() {
      x = 0.0f;
      y = 0.0;
    }
    
    Point(const float x, const float y) {
      this->x = x;
      this->y = y;
    }
    
    float x;
    float y;
  };

  uint8_t hexToByte(const char upper, const char lower);
  float toRadians(float degrees);
  float toDegrees(float radians);
  uint32_t minPowerOfTwo(const uint32_t in);
}

#endif // UtilityHPP

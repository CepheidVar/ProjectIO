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
#ifndef ClassHPP
#define ClassHPP

#include <string>
#include <stdexcept>
#include "Resource.hpp"

namespace io {
  struct StatBlock {
    int16_t hp;
    int16_t tp;
    int16_t str;
    int16_t vit;
    int16_t tec;
    int16_t agi;
    int16_t luk;
  };

  /**
   * This isn't what you think it is.  This represents a character class,
   * detailing the stats at each level of the class.  It also stores the name
   * and identifying short name of the class for equipment restrictions.
   *
   * The short name is usually one or two characters, often the first letter
   * of the full name, or the first few letters.
   */
  class Class : public Resource {
  public:
    const static int16_t MAX_LEVEL = 70;

    Class(const std::string& filename);
    virtual ~Class() {

    }

    /**
     * Returns the stat block of the class for the given level.  One indexed.
     * @param level The class level to retrieve the stat block for.
     * @return The stat block requested.
     */
    StatBlock getStatsForLevel(const int16_t level) {
      if (level == 0 || level > Class::MAX_LEVEL) {
        throw std::range_error("Class:getStatsForLevel():  Level out of range.");
      }

      return stats[level - 1];
    }
    
    std::string getName() const {
      return name;
    }

    std::string getShortName() const {
      return shortName;
    }

    virtual Class* toClass() {
      return this;
    }
  private:
    std::string name;
    std::string shortName;

    //  The stats are stored in the order they're read in.
    StatBlock stats[Class::MAX_LEVEL];
  };
}

#endif

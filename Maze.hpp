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
#ifndef MazeHPP
#define MazeHPP

#include <list>
#include <string>
#include "Map.hpp"

namespace io {
  /**
   * Represents an entrance point into the maze.  This can be on the first
   * floor, or on a floor with a geomagnetic pole.
   */
  class Entrance {
  public:
    Entrance() { }
    
    Entrance(const uint32_t floor, const int32_t x, const int32_t y,
             Facing facing) {
      this->facing = facing;
      this->floor = floor;
      this->x = x;
      this->y = y;
    }
    
    Entrance(const Entrance& e) {
      this->facing = e.getFacing();
      this->floor = e.getFloor();
      this->x = e.getX();
      this->y = e.getY();
    }
    
    Entrance& operator=(const Entrance& e) {
      if (this != &e) {
        this->facing = e.getFacing();
        this->floor = e.getFloor();
        this->x = e.getX();
        this->y = e.getY();
      }
      
      return *this;
    }
    
    Facing getFacing() const {
      return facing;
    }
    
    uint32_t getFloor() const {
      return floor;
    }
    
    int32_t getX() const {
      return x;
    }
    
    int32_t getY() const {
      return y;
    }
  private:
    Facing facing = Facing::NORTH;
    uint32_t floor = 0;
    int32_t x = 0;
    int32_t y = 0;
  };
  
  /**
   * Represents a multi-floor maze.  Not much to say about it.
   */
  class Maze {
  public:
    Maze() { }

    static Maze* mazeFromXML(const std::string& filename);

    ~Maze() {
      for (Map* m : floors) {
        delete m;
      }
      floors.clear();
    }

    //  Floors are 1-indexed.  Floor 0 indicates the town.
    Map* getFloor(const uint32_t which) {
      if (which == 0 || which > floors.size()) {
        return nullptr;
      }

      return floors.at(which - 1);
    }

    uint32_t getFloorCount() const {
      return floors.size();
    }
    
    Entrance getMazeEntrance() const {
      return mazeEntrance;
    }
  private:
    const static uint16_t MAX_FLOORS = 256;
    
    Maze(const Maze&) = delete;
    Maze& operator=(const Maze&) = delete;
    
    std::vector<Map*> floors;
    Entrance mazeEntrance;
    
    void scanForEntrances();
  };
}

#endif

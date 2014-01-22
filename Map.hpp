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
#ifndef MapHPP
#define MapHPP

#include "Graphics.hpp"
#include "Activatables.hpp"
#include <string>
#include <cstdint>

namespace io {
  /**
   * A MapCell represents a single cell in a map.  The resource IDs represent
   * how the cell looks from the outside, not from the inside.  Thus, when the
   * player is standing to the north, looking south, they will be seeing the
   * north facing resource of the cell.
   */
  class MapCell {
  public:
    MapCell() { }
    
    ~MapCell() {
      if (activatable) {
        delete activatable;
        activatable = nullptr;
      }
    }

    Activatable* getActivatable() const {
      return activatable;
    }

    bool canEntityEnter() const {
      if (getActivatable()) {
        return !(isSolid() || getActivatable()->isSolid());
      }

      return !isSolid();
    }

    uint8_t getCeiling() const {
      return ceiling;
    }

    uint8_t getFloor() const {
      return floor;
    }

    uint8_t getEastWall() const {
      return eastWall;
    }

    uint8_t getNorthWall() const {
      return northWall;
    }

    uint8_t getSouthWall() const {
      return southWall;
    }

    uint8_t getWestWall() const {
      return westWall;
    }

    bool isSolid() const {
      return solid;
    }

    /**
     * Sets the object associated with the cell.  The cell takes ownership of
     * the object and is responsible for deleting it.
     * @param activatable The object to assign to the cell.
     */
    void setActivatable(Activatable* activatable) {
      if (this->activatable) {
        delete this->activatable;
        this->activatable = nullptr;
      }
      
      this->activatable = activatable;
    }

    void setCeiling(const uint8_t cellCeiling) {
      this->ceiling = cellCeiling;
    }

    void setFloor(const uint8_t cellFloor) {
      this->floor = cellFloor;
    }

    void setEastWall(const uint8_t eastWall) {
      this->eastWall = eastWall;
    }

    void setNorthWall(const uint8_t northWall) {
      this->northWall = northWall;
    }

    void setSouthWall(const uint8_t southWall) {
      this->southWall = southWall;
    }

    void setWestWall(const uint8_t westWall) {
      this->westWall = westWall;
    }

    void setSolid(bool solid) {
      this->solid = solid;
    }
  private:
    MapCell(const MapCell&) = delete;
    MapCell& operator=(const MapCell&) = delete;
    
    Activatable* activatable = nullptr;
    uint8_t ceiling = 0;
    uint8_t floor = 0;
    uint8_t eastWall = 0;
    uint8_t northWall = 0;
    uint8_t southWall = 0;
    uint8_t westWall = 0;
    bool solid = true;
  };

  /**
   * Represents a map.  Currently hard-coded to 35 * 30 cells in size.
   */
  class Map {
  public:
    const static int32_t MAX_MAP_WIDTH = 128;
    const static int32_t MAX_MAP_HEIGHT = 128;
    
    Map(const int32_t width, const int32_t height) {
      if (width <= 0|| height <= 0 || width > MAX_MAP_WIDTH ||
          height > MAX_MAP_HEIGHT) {
        throw std::range_error("Map::Map():  Map dimensions are out of range.");
      }
      
      cells = new MapCell[width * height];
      
      this->width = width;
      this->height = height;
    }

    ~Map() {
      delete [] cells;
      cells = nullptr;
    }

    static Map* mapFromXML(const std::string& filename);
    static Map* mapFromImage(const std::string& filename);
    
    void draw(Graphics* g, const int32_t x, const int32_t y);
    
    Activatable* getActivatable(const int32_t x, const int32_t y) {
      if (getCell(x, y)) {
        return getCell(x, y)->getActivatable();
      }
      
      return nullptr;
    }
    
    int32_t getWidth() const {
      return width;
    }
    
    int32_t getHeight() const {
      return height;
    }

    bool isSolid(const int32_t x, const int32_t y) {
      if (getCell(x, y)) {
        return getCell(x, y)->isSolid();
      }
      
      return true;
    }
    
    bool canEntityEnter(const int32_t x, const int32_t y) {
      if (getCell(x, y)) {
        return getCell(x, y)->canEntityEnter();
      }
      
      return false;
    }
  private:
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;
    
    MapCell* cells;
    int32_t width;
    int32_t height;
    
    MapCell* getCell(const int32_t x, const int32_t y) {
      if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) {
        return nullptr;
      }
      
      return (cells + (y * getWidth()) + x);
    }
  };
}
#endif // mapHPP

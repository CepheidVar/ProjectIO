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
    MapCell() {
      setActivatable(nullptr);
      setCellCeiling(0);
      setCellFloor(0);
      setEastWall(0);
      setNorthWall(0);
      setSouthWall(0);
      setWestWall(0);
      setSolid(true);
    }

    Activatable* getActivatable() const {
      return activatable;
    }

    bool canEntityEnterCell() const {
      if (getActivatable()) {
        return !(isSolid() || getActivatable()->isSolid());
      }

      return !isSolid();
    }

    uint8_t getCellCeiling() const {
      return cellCeiling;
    }

    uint8_t getCellFloor() const {
      return cellFloor;
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

    void setActivatable(Activatable* activatable) {
      this->activatable = activatable;
    }

    void setCellCeiling(const uint8_t cellCeiling) {
      this->cellCeiling = cellCeiling;
    }

    void setCellFloor(const uint8_t cellFloor) {
      this->cellFloor = cellFloor;
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
    Activatable* activatable;
    uint8_t cellCeiling;
    uint8_t cellFloor;
    uint8_t eastWall;
    uint8_t northWall;
    uint8_t southWall;
    uint8_t westWall;
    bool solid;
  };

  /**
   * Represents a map.  Currently hard-coded to 35 * 30 cells in size with a
   * border of 1 cell on each side.
   */
  class Map {
  public:
    const static int32_t MAP_WIDTH = 35;
    const static int32_t MAP_HEIGHT = 30;

    Map() {
    }

    ~Map() {
      for (int32_t y = 0; y < Map::MAP_HEIGHT; y++) {
        for (int32_t x = 0; x < Map::MAP_WIDTH; x++) {
          Activatable* cellActivatable = getCell(x, y).getActivatable();
          if (cellActivatable) {
            delete cellActivatable;
          }
        }
      }
    }

    static Map* mapFromXML(const std::string& filename);
    static Map* mapFromImage(const std::string& filename);
    
    void draw(Graphics* g, const int32_t x, const int32_t y);

    MapCell& getCell(int32_t x, int32_t y) {
      clampCoordinates(x, y);

      return cells[y][x];
    }

    bool canEntityEnterCell(const int32_t x, const int32_t y) {
      return getCell(x, y).canEntityEnterCell();
    }
  private:
    //  There is a border around the map.
    MapCell cells[MAP_HEIGHT + 2][MAP_WIDTH + 2];

    void clampCoordinates(int32_t& x, int32_t& y) {
      if (x < 0) {
        x = 0;
      }

      if (x >= Map::MAP_WIDTH + 2) {
        x = (Map::MAP_WIDTH + 2) - 1;
      }

      if (y < 0) {
        y = 0;
      }

      if (y >= Map::MAP_HEIGHT + 2) {
        y = (Map::MAP_HEIGHT + 2) - 1;
      }
    }

    bool isCellSolid(const int32_t x, const int32_t y) {
      return getCell(x, y).isSolid();
    }
  };
}
#endif // mapHPP

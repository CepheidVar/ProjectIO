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
#include "Maze.hpp"
#include "tinyxml2.h"
#include "Log.hpp"
#include <exception>
#include <stdexcept>
#include <iostream>

using namespace tinyxml2;

namespace io {
  Maze* Maze::mazeFromXML(const std::string& filename) {
    Maze* newMaze = nullptr;

    try {
      XMLDocument doc;
      if (doc.LoadFile(filename.c_str()) == XML_SUCCESS) {
        XMLElement* root = doc.RootElement();

        if (!root || std::string(root->Name()).compare("maze") != 0) {
          throw std::runtime_error("Maze::mazeFromXML():  Not a maze file.");
        }

        XMLElement* floorElement = root->FirstChildElement("floor");
        if (!floorElement) {
          throw std::runtime_error("Maze::mazeFromXML():  Empty maze.");
        }

        newMaze = new Maze();

        while (floorElement && newMaze->getFloorCount() < Maze::MAX_FLOORS) {
          if (!floorElement->GetText()) {
            throw std::runtime_error("Maze::mazeFromXML():  Empty floor element.");
          }

          Map* floor = Map::mapFromXML(floorElement->GetText());
          if (!floor) {
            throw std::runtime_error("Maze::mazeFromXML():  Could not load a floor.");
          }

          newMaze->floors.push_back(floor);

          floorElement = floorElement->NextSiblingElement("floor");
        }
        
        if (floorElement) {
          //  There was more than MAX_FLOORS floors.  Best report it.
          writeToLog(MessageLevel::WARNING, "Maze::mazeFromXML():  More than 256 floors specified for the maze.  Stop this madness!");
        }
        
        newMaze->scanForEntrances();
      }
    }
    catch (std::exception& e) {
      if (newMaze) {
        delete newMaze;
        newMaze = nullptr;
      }

      writeToLog(MessageLevel::ERROR, "%s\n", e.what());
    }

    return newMaze;
  }
  
  void Maze::scanForEntrances() {
    for (uint32_t i = 1; i <= getFloorCount(); i++) {
      Map* curMap = getFloor(i);
      
      if (i == 1) {
        //  In this case, we want up stairs.
        bool foundEntrance = false;
        bool multipleWarned = false;
        for (int32_t y = 0; y < Map::MAP_HEIGHT; y++) {
          for (int32_t x = 0; x < Map::MAP_WIDTH; x++) {
            Activatable* act = curMap->getActivatable(x, y);
            if (act) {
              Stairs* stairs = act->asStairs();
              if (stairs && stairs->getDirection() == StairDirection::UP) {
                Facing dstFacing = stairs->getDestinationFacing();
                
                //  Adjust the position so that when the player enters, they're
                //  facing the right direction, and in the right position.
                int32_t dstX = x;
                int32_t dstY = y;
                switch(dstFacing) {
                case Facing::NORTH:
                  dstY--;
                  break;
                case Facing::EAST:
                  dstX++;
                  break;
                case Facing::SOUTH:
                  dstY++;
                  break;
                case Facing::WEST:
                  dstX--;
                  break;
                }

                if (foundEntrance && !multipleWarned) {
                  writeToLog(MessageLevel::WARNING, "Maze::scanForEntrances():  Multiple up stairs found on floor of maze.");
                  multipleWarned = true;
                }
                else {
                  if (!curMap->canEntityEnter(dstX, dstY)) {
                    writeToLog(MessageLevel::WARNING, "Maze::scanForEntrances():  Entrance as specified would place player in solid wall.");
                  }
                  else {
                    std::cout << i << "/" << dstX << "/" << dstY << "/" << (int)dstFacing << std::endl;
                    mazeEntrance = Entrance(i, dstX, dstY, dstFacing);
                    foundEntrance = true;
                  }
                }
              }
            }
          }
        }
      }
      else {
        //  Otherwise, we're looking for geomagnetic poles.  This is currently
        //  not implemented.  Mostly because geopoles don't exist.
      }
    }
  }
}

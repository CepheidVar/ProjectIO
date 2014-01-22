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
#include "Map.hpp"
#include "ResourceManager.hpp"
#include "tinyxml2.h"
#include "Log.hpp"
#include <exception>
#include <stdexcept>
#include <iostream>

using namespace tinyxml2;

namespace io {
  const int32_t MAX_DISTANCE = 64;

  Map* Map::mapFromXML(const std::string& filename) {
    writeToLog(MessageLevel::INFO, "Loading map from file \"%s\"\n", filename.c_str());

    Map* newMap = nullptr;
    XMLDocument doc;
    try {
      if (doc.LoadFile(filename.c_str()) == XML_SUCCESS) {
        XMLElement* root = doc.RootElement();

        if (std::string(root->Name()).compare("floor") != 0) {
          throw std::runtime_error("Map::mapFromXML():  Not a floor file.");
        }

        XMLElement* titleElement = root->FirstChildElement("title");
        XMLElement* imageElement = root->FirstChildElement("image");
        XMLElement* objectsElement = root->FirstChildElement("objects");

        if (!titleElement || !imageElement) {
          throw std::runtime_error("Map::mapFromXML():  Missing title or image.");
        }

        const char* title = titleElement->GetText();
        const char* image = imageElement->GetText();

        if (!title || !image) {
          throw std::runtime_error("Map::mapFromXML():  Empty title or image.");
        }

        newMap = Map::mapFromImage(image);
        if (!newMap) {
          throw std::runtime_error("Map::mapFromXML():  Unable to load map image.");
        }

        //  Start parsing any objects that exist.
        if (objectsElement) {
          XMLNode* curNode = objectsElement->FirstChild();
          while (curNode) {
            XMLElement* element = curNode->ToElement();
            if (element) {
              //  All objects have an X/Y element guaranteed.
              XMLElement* xElement = element->FirstChildElement("x");
              XMLElement* yElement = element->FirstChildElement("y");

              if (!xElement || !yElement) {
                throw std::runtime_error("Map::mapFromXM()L:  Missing X or Y element in object.");
              }

              int32_t x = 0;
              if (xElement->QueryIntText(&x) != XML_SUCCESS) {
                throw std::runtime_error("Map::mapFromXML():  Could not parse X value.");
              }

              int32_t y = 0;
              if (yElement->QueryIntText(&y) != XML_SUCCESS) {
                throw std::runtime_error("Map::mapFromXML():  Could not parse Y value.");
              }

              if (x < 0 || x >= Map::MAP_WIDTH || y < 0 || y >= Map::MAP_HEIGHT) {
                throw std::out_of_range("Map::mapFromXML():  X or Y outside of map.");
              }

              std::string name = element->Name();
              if (name.compare("door") == 0) {
                XMLElement* orientation = element->FirstChildElement("orientation");
                if (!orientation || !orientation->GetText()) {
                  throw std::runtime_error("Map::mapFromXM():  Empty or missing door orientation.");
                }

                std::string orientationString = orientation->GetText();
                Orientation orientationEnum;
                if (orientationString.compare("horizontal") == 0) {
                  orientationEnum = Orientation::HORIZONTAL;
                }
                else if (orientationString.compare("vertical") == 0) {
                  orientationEnum = Orientation::VERTICAL;
                }
                else {
                  throw std::runtime_error("Map::mapFromXML():  Invalid door orientation specified.");
                }

                Door* d = new Door();
                d->setOrientation(orientationEnum);

                newMap->getCell(x, y)->setActivatable(d);
              }
              else if (name.compare("secretDoor") == 0) {
                XMLElement* direction = element->FirstChildElement("direction");
                if (!direction || !direction->GetText()) {
                  throw std::runtime_error("Map::mapFromXML():  Empty or missing secret door direction.");
                }

                std::string directionString = direction->GetText();
                SecretDoorDirection directionEnum;
                if (directionString.compare("north") == 0) {
                  directionEnum = SecretDoorDirection::NORTH;
                }
                else if (directionString.compare("east") == 0) {
                  directionEnum = SecretDoorDirection::EAST;
                }
                else if (directionString.compare("south") == 0) {
                  directionEnum = SecretDoorDirection::SOUTH;
                }
                else if (directionString.compare("west") == 0) {
                  directionEnum = SecretDoorDirection::WEST;
                }
                else if (directionString.compare("north-south") == 0) {
                  directionEnum = SecretDoorDirection::NORTH_SOUTH;
                }
                else if (directionString.compare("east-west") == 0) {
                  directionEnum = SecretDoorDirection::EAST_WEST;
                }
                else {
                  throw std::runtime_error("Map::mapFromXML():  Invalid secret door direction.");
                }

                SecretDoor* sd = new SecretDoor();
                sd->setDirection(directionEnum);

                newMap->getCell(x, y)->setActivatable(sd);
              }
              else if (name.compare("stairs") == 0) {
                XMLElement* dir = element->FirstChildElement("direction");
                XMLElement* dstX = element->FirstChildElement("destinationX");
                XMLElement* dstY = element->FirstChildElement("destinationY");
                XMLElement* dstFacing = element->FirstChildElement("destinationFacing");
                
                if (!dir || !dir->GetText()) {
                  throw std::runtime_error("Map::mapFromXML():  Empty or missing stairs direction.");
                }
                
                if (!dstFacing || !dstFacing->GetText()) {
                  throw std::runtime_error("Map::mapFromXML():  Empty or missing stairs destination facing.");
                }
                
                int32_t dstIntX = 0;
                if (!dstX || dstX->QueryIntText(&dstIntX) != XML_SUCCESS) {
                  throw std::runtime_error("Map::mapFromXML():  Could not parse stairs destination X.");
                }
                
                int32_t dstIntY = 0;
                if (!dstY || dstY->QueryIntText(&dstIntY) != XML_SUCCESS) {
                  throw std::runtime_error("Map::mapFromXML():  Could not parse stairs destination Y.");
                }
                
                StairDirection dirStairs = StairDirection::UP;
                std::string dirString = dir->GetText();
                if (dirString.compare("up") == 0) {
                  dirStairs = StairDirection::UP;
                }
                else if (dirString.compare("down") == 0) {
                  dirStairs = StairDirection::DOWN;
                }
                else {
                  throw std::runtime_error("Map::mapFromXML():  Unknown stairs direction specified.");
                }
                
                Facing stairsFacing = Facing::NORTH;
                std::string dstFacingString = dstFacing->GetText();
                if (dstFacingString.compare("north") == 0) {
                  stairsFacing = Facing::NORTH;
                }
                else if (dstFacingString.compare("east") == 0) {
                  stairsFacing = Facing::EAST;
                }
                else if (dstFacingString.compare("south") == 0) {
                  stairsFacing = Facing::SOUTH;
                }
                else if (dstFacingString.compare("west") == 0) {
                  stairsFacing = Facing::WEST;
                }
                else {
                  throw std::runtime_error("Map::mapFromXML():  Unknown stairs destination facing specified.");
                }
                
                Stairs* s = new Stairs();
                s->setDestinationX(dstIntX);
                s->setDestinationY(dstIntY);
                s->setDirection(dirStairs);
                s->setDestinationFacing(stairsFacing);
                
                newMap->getCell(x, y)->setActivatable(s);
              }
              else {
                throw std::runtime_error("Map::mapFromXML():  Unknown object type.");
              }
            }

            curNode = curNode->NextSibling();
          }
        }
        else {
          writeToLog(MessageLevel::WARNING, "Map::mapFromXML():  Missing objects element.  Possibly a mistake?");
        }
      }
      else {
        writeToLog(MessageLevel::ERROR, "Map::mapFromXML():  Error parsing floor file.");
      }
    }
    catch (std::exception& e) {
      if (newMap) {
        delete newMap;
        newMap = nullptr;
      }

      writeToLog(MessageLevel::ERROR, "%s\n", e.what());
    }

    return newMap;
  }

  Map* Map::mapFromImage(const std::string& filename) {
    Colour unfilled(200, 200, 250, 255);

    Map* newMap = nullptr;
    
    /**
     *  Here's how it works.  RGB(200, 200, 250) is treated as empty.
     *  Anything else, for the time being, is treated as filled.
     *  I recommend using RGB(150, 150, 200) as the filled colour.
     */
    Image* m = ResourceManager::getInstance()->getResource(filename)->toImage();
    if (m) {
      if (m->getWidth() == Map::MAP_WIDTH && m->getHeight() == Map::MAP_HEIGHT) {
        newMap = new Map();
        for (uint32_t y = 0; y < m->getHeight(); y++) {
          for (uint32_t x = 0; x < m->getWidth(); x++) {
            if (m->getPixel(x, y) == unfilled) {
              newMap->getCell(x, y)->setSolid(false);
            }
            else {
              newMap->getCell(x, y)->setSolid(true);
            }
          }
        }
      }
      else {
        throw std::runtime_error("Map::mapFromImage():  Map image dimensions are incorrect.");
      }
      
      delete m;
    }
    
    return newMap;
  }
  
  void Map::draw(Graphics* g, const int32_t cx, const int32_t cy) {
    for (int32_t y = -MAX_DISTANCE; y < MAX_DISTANCE; y++) {
      for (int32_t x = -MAX_DISTANCE; x < MAX_DISTANCE; x++) {
        if (!isSolid(cx + x, cy + y)) {
          g->drawFloorTile(x, y, getCell(cx + x, cy + y)->getFloor());
          g->drawCeilingTile(x, y, getCell(cx + x, cy + y)->getCeiling());

          /*
           * Remember, the get*Wall() methods return the wall you'd see if you
           * were facing the cell from that direction.  If we're to the east
           * of the current cell, and facing it(We're facing west), we want
           * the EAST wall of the cell.
           */

          uint8_t wallID;
          if (isSolid(cx + x, cy + y - 1)) {
            wallID = getCell(cx + x, cy + y - 1)->getSouthWall();
            g->drawWallTile(x, y, Facing::NORTH, wallID);
          }

          if (isSolid(cx + x + 1, cy + y)) {
            wallID = getCell(cx + x + 1, cy + y)->getWestWall();
            g->drawWallTile(x, y, Facing::EAST, wallID);
          }

          if (isSolid(cx + x, cy + y + 1)) {
            wallID = getCell(cx + x, cy + y + 1)->getNorthWall();
            g->drawWallTile(x, y, Facing::SOUTH, wallID);
          }

          if (isSolid(cx + x - 1, cy + y)) {
            wallID = getCell(cx + x - 1, cy + y)->getEastWall();
            g->drawWallTile(x, y, Facing::WEST, wallID);
          }
          
          Activatable* act = getActivatable(cx + x, cy + y);
          if (act) {
            act->draw(g);
          }
        }
      }
    }
  }
}

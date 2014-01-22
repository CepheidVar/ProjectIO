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
#ifndef ActivatablesHPP
#define ActivatablesHPP

#include "Graphics.hpp"
#include "Common.hpp"
#include "Entity.hpp"

namespace io {
  class Door;
  class SecretDoor;
  class Stairs;
  
  class Activatable {
  public:
    Activatable() { }

    virtual ~Activatable() { }

    virtual Door* asDoor() {
      return nullptr;
    }
    
    virtual SecretDoor* asSecretDoor() {
      return nullptr;
    }
    
    virtual Stairs* asStairs() {
      return nullptr;
    }
    
    virtual void draw(Graphics* g) { }

    virtual bool isSolid() const {
      return true;
    }
  private:
    Activatable(const Activatable&) = delete;
    Activatable& operator=(const Activatable&) = delete;
  };

  enum class Orientation : int8_t {
    HORIZONTAL,
    VERTICAL
  };

  class Door : public Activatable {
  public:
    Door() { }

    virtual ~Door() {

    }
    
    virtual Door* asDoor() {
      return this;
    }

    Orientation getOrientation() const {
      return orientation;
    }

    void setOrientation(const Orientation orientation) {
      this->orientation = orientation;
    }
  private:
    Door(const Door&) = delete;
    Door& operator=(const Door&) = delete;
    
    Orientation orientation = Orientation::HORIZONTAL;
  };

  enum class SecretDoorDirection : int8_t {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NORTH_SOUTH,
    EAST_WEST
  };

  class SecretDoor : public Activatable {
  public:
    SecretDoor() { }

    virtual ~SecretDoor() { }
    
    virtual SecretDoor* asSecretDoor() {
      return this;
    }

    SecretDoorDirection getDirection() const {
      return direction;
    }

    void setDirection(const SecretDoorDirection direction) {
      this->direction = direction;
    }
  private:
    SecretDoor(const SecretDoor&) = delete;
    SecretDoor& operator=(const SecretDoor&) = delete;
    
    SecretDoorDirection direction = SecretDoorDirection::NORTH;
  };

  enum class StairDirection {
    UP,
    DOWN
  };

  class Stairs : public Activatable {
  public:
    Stairs() { }

    virtual ~Stairs() { }
    
    virtual Stairs* asStairs() {
      return this;
    }

    StairDirection getDirection() const {
      return direction;
    }

    Facing getDestinationFacing() const {
      return destinationFacing;
    }

    int32_t getDestinationX() const {
      return destinationX;
    }

    int32_t getDestinationY() const {
      return destinationY;
    }

    void setDestinationFacing(const Facing destinationFacing) {
      this->destinationFacing = destinationFacing;
    }

    void setDestinationX(const int32_t destinationX) {
      this->destinationX = destinationX;
    }

    void setDestinationY(const int32_t destinationY) {
      this->destinationY = destinationY;
    }

    void setDirection(const StairDirection direction) {
      this->direction = direction;
    }

  private:
    Stairs(const Stairs&) = delete;
    Stairs& operator=(const Stairs&) = delete;
    
    Facing destinationFacing = Facing::NORTH;
    int32_t destinationX = 0;
    int32_t destinationY = 0;
    StairDirection direction = StairDirection::UP;
  };
}

#endif

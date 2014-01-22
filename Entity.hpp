#ifndef EntityHPP
#define EntityHPP

#include "Common.hpp"

namespace io {
  class Entity {
  public:
    Entity() {
      facing = Facing::NORTH;
      x = 0;
      y = 0;
    }

    virtual ~Entity() {

    }

    Facing getFacing() const {
      return facing;
    }

    int32_t getX() const {
      return x;
    }

    int32_t getY() const {
      return y;
    }

    void setFacing(const Facing facing) {
      this->facing = facing;
    }

    void setX(const int32_t x) {
      this->x = x;
    }

    void setY(const int32_t y) {
      this->y = y;
    }
  private:
    Facing facing;
    int32_t x;
    int32_t y;
  };
}

#endif

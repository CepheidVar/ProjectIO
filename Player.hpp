#ifndef PlayerHPP
#define PlayerHPP

#include "Entity.hpp"
#include "Guild.hpp"
#include "Party.hpp"

namespace io {
  class Player : public Entity {
  public:
    Player() {
      guild = new Guild();
      party = new Party();
    }

    ~Player() {
      delete party;
      party = nullptr;

      delete guild;
      guild = nullptr;
    }

    Guild* getGuild() const {
      return guild;
    }

    Party* getParty() const {
      return party;
    }
  private:
    Guild* guild;
    Party* party;
  };
}

#endif

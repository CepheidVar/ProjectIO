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
#ifndef TownStateHPP
#define TownStateHPP

#include "Guild.hpp"
#include "Party.hpp"
#include "Game.hpp"
#include "State.hpp"
#include "TownMainScreen.hpp"
#include "ScreenManager.hpp"
#include "GuildHallController.hpp"

namespace io {
  class TownState : public State, MenuActionListener {
  public:
    TownState(Game* game) {
      this->screenManager = game->getScreenManager();
      this->game = game;
      this->guild = game->getPlayer()->getGuild();
      this->party = game->getPlayer()->getParty();

      mainScreen = new TownMainScreen();
      guildHallController = new GuildHallController(screenManager, guild, party);

      mainScreen->getMainMenu()->addMenuActionListener(this);
    }

    virtual ~TownState() { }

    virtual void tick() { }
    virtual void draw(Graphics* g) { }
    virtual void handleInputEvent(const InputEvent& event) { }
    virtual void onActivate() {
      screenManager->pushScreen(mainScreen);
    }

    virtual void onMenuItemSelected(Menu* notifier, MenuItem* item) {
      if (item == mainScreen->getLabyrinthMenuItem()) {
        game->setGameState(GameState::MAZE);
      }
      else {
        if (item == mainScreen->getGuildHallMenuItem()) {
          guildHallController->switchTo();
        }
      }
    }
  private:
    Game* game;
    Guild* guild;
    Party* party;
    ScreenManager* screenManager;
    TownMainScreen* mainScreen;
    GuildHallController* guildHallController;
  };
}

#endif

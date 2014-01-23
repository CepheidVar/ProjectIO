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
#ifndef GameHPP
#define GameHPP

#include "Player.hpp"
#include "Graphics.hpp"
#include "State.hpp"
#include "InputEvent.hpp"
#include "ScreenManager.hpp"
#include <cstdint>

namespace io {
  enum class GameState {
    TOWN,
    MAZE,
    BATTLE,
  };
  
  class Game {
  public:
    Game();
    ~Game();
    
    void draw(Graphics* g) {
      state->draw(g);
      screenManager->draw(g);
    }
    
    GameState getGameState() const {
      return gameState;
    }
    
    Player* getPlayer() const {
      return player;
    }
    
    ScreenManager* getScreenManager() const {
      return screenManager;
    }
    
    void handleInputEvent(const InputEvent& event) {
      if (!screenManager->handleInputEvent(event)) {
        state->handleInputEvent(event);
      }
    }
    
    void tick() {
      state->tick();
    }

    void setGameState(const GameState state);
  private:
    GameState gameState;
    State* state;
    State* mazeState;
    Player* player;
    ScreenManager* screenManager;
    State* townState;
  };
}

#endif // GameHPP

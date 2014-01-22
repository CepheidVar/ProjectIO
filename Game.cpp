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
#include "Game.hpp"
#include "MazeState.hpp"
#include "TownState.hpp"

namespace io {
  Game::Game() {
    screenManager = new ScreenManager();
    player = new Player();
    currentState = nullptr;
    curGameState = GameState::MAZE;
    mazeState = new MazeState(this, player);
    townState = new TownState(this, screenManager, player->getGuild(),
                              player->getParty());

    currentState = townState;
    townState->onActivate();
  }
  
  Game::~Game() {
    delete mazeState;
    mazeState = nullptr;

    delete townState;
    townState = nullptr;

    delete player;
    player = nullptr;

    delete screenManager;
    screenManager = nullptr;
  }
  
  void Game::draw(Graphics* g) {
    currentState->draw(g);
    screenManager->draw(g);
  }

  void Game::tick() {
    currentState->tick();
  }
}

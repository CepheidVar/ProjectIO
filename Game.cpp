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
    state = nullptr;
    mazeState = new MazeState(this);
    townState = new TownState(this);

    setGameState(GameState::TOWN);
  }
  
  Game::~Game() {
    state = nullptr;
    
    delete townState;
    townState = nullptr;
    
    delete mazeState;
    mazeState = nullptr;

    delete player;
    player = nullptr;

    delete screenManager;
    screenManager = nullptr;
  }
  
  void Game::setGameState(const GameState gameState) {
    switch(gameState) {
      case GameState::TOWN:
        state = townState;
        break;
      case GameState::MAZE:
        state = mazeState;
        break;
      case GameState::BATTLE:
        return;
    }
    screenManager->clear();

    this->gameState = gameState;
    state->onActivate();
  }
}

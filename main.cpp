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
#include "SDL.h"
#include "Common.hpp"
#include "Graphics.hpp"
#include "Utility.hpp"
#include <iostream>
#include "Texture.hpp"
#include "Log.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Resource.hpp"
#include "InputTranslator.hpp"

using namespace io;

int main(int, char**) {
  initLog();
  
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    writeToLog(MessageLevel::ERROR, "Could not initialize SDL.\n");
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);

  SDL_Window* win = SDL_CreateWindow("ProjectIO", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     640, 480, SDL_WINDOW_OPENGL);

  SDL_GLContext context = SDL_GL_CreateContext(win);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  Graphics* graphics = new Graphics();
  Game* game = new Game();
  
  Resource* checker = ResourceManager::getInstance()->getResource("data/checker.png");
  Texture* basicTex = new Texture(checker->toImage());
  basicTex->makeActive();

  Resource* font = ResourceManager::getInstance()->getResource("data/DejaVuSansMono.ttf");
  InputTranslator* t = InputTranslator::getInstance();

  ResourceManager::getInstance()->getResource("data/knight.class");

  uint32_t curTime;
  uint32_t prvTime;
  uint32_t dltTime;

  SDL_Event event;
  bool isRunning = true;
  SDL_StartTextInput();

  while (isRunning) {
    while (SDL_PollEvent(&event) > 0) {
      switch(event.type) {
      case SDL_QUIT:
        isRunning = false;
        break;
      case SDL_KEYUP:
      case SDL_KEYDOWN: {
          InputEvent inputEvent = t->keyToEvent(event);
          game->handleInputEvent(inputEvent);
        }
        break;
      case SDL_TEXTINPUT: {
        SDL_StopTextInput();
          InputEvent inputEvent = InputEvent(InputEventType::NOT_BOUND,
                                             InputEventState::DOWN,
                                             event.text.text[0]);
          game->handleInputEvent(inputEvent);

          SDL_StartTextInput();
        }
        break;
      default:
        break;
      }
    }
    
    prvTime = curTime;
    curTime = SDL_GetTicks();
    dltTime += (curTime - prvTime) * 60;
    
    while (dltTime >= 1000) {
      game->tick();
      dltTime -= 1000;
    }

    graphics->setMatrixMode(MatrixMode::MODEL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    basicTex->makeActive();
    game->draw(graphics);
    
    /*
    graphics->setMatrixMode(MatrixMode::VIEW);
    graphics->pushMatrix();
    graphics->loadIdentity();
    
    graphics->setMatrixMode(MatrixMode::PROJECTION);
    graphics->pushMatrix();
    graphics->loadIdentity();
    graphics->ortho(0, 640, 480, 0, 0, 1);
    
    graphics->setMatrixMode(MatrixMode::MODEL);
    graphics->loadIdentity();
    graphics->translate(25, 25, 0);
    glEnable(GL_BLEND);
    //f->drawText(graphics, "Hello, my honey, hello, my darling~");
    glDisable(GL_BLEND);
    
    graphics->setMatrixMode(MatrixMode::PROJECTION);
    graphics->popMatrix();
    graphics->setMatrixMode(MatrixMode::VIEW);
    graphics->popMatrix();
    */
    SDL_GL_SwapWindow(win);
  }

  delete game;

  return 0;
}

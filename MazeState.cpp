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
#include "MazeState.hpp"
#include "Map.hpp"
#include "Common.hpp"
#include "Utility.hpp"
#include "Log.hpp"
#include <stdexcept>

namespace io {
  MazeState::MazeState(Game* game) {
    currentFloor = 1;

    maze = Maze::mazeFromXML("data/maze.xml");
    if (!maze || maze->getFloorCount() == 0) {
      throw std::runtime_error("Maze::Maze():  Could not load maze.");
    }

    currentMap = maze->getFloor(1);

    this->player = game->getPlayer();
    inputDisabled = false;
    inputDisabledTicks = 0;
    animationState = MazeAnimationState::TURNING_LEFT;
    cameraX = 0;
    cameraY = 0;
    cameraAngle = 0;

    this->game = game;
  }

  MazeState::~MazeState() {
    delete maze;
    maze = nullptr;
  }

  void MazeState::tick() {
    if (inputDisabled) {
      inputDisabledTicks--;

      //if (inputDisabledTicks == 0) {
      inputDisabled = false;
      //}
    }
  }

  void MazeState::draw(Graphics* g) {
    g->setMatrixMode(MatrixMode::VIEW);
    g->loadIdentity();
    g->translate(cameraX, -6.0f, cameraY);
    g->rotate(cameraAngle, 0.0f, 1.0f, 0.0f);
    switch (player->getFacing()) {
    case Facing::NORTH:
      g->rotate(toRadians(0), 0.0f, 1.0f, 0.0f);
      break;
    case Facing::EAST:
      g->rotate(toRadians(90), 0.0f, 1.0f, 0.0f);
      break;
    case Facing::SOUTH:
      g->rotate(toRadians(180), 0.0f, 1.0f, 0.0f);
      break;
    case Facing::WEST:
      g->rotate(toRadians(270), 0.0f, 1.0f, 0.0f);
      break;
    default:
      break;
    }

    g->setMatrixMode(MatrixMode::MODEL);
    g->loadIdentity();

    currentMap->draw(g, player->getX(), player->getY());
  }

  void MazeState::handleInputEvent(const InputEvent& event) {
    if (event.getState() == InputEventState::DOWN) {
      switch (event.getType()) {
      case InputEventType::ACCEPT:
        activateActivatable();
        break;
      case InputEventType::UP:
        movePlayerForward();
        break;
      case InputEventType::DOWN:
        movePlayerBackward();
        break;
      case InputEventType::LEFT:
        turnPlayerLeft();
        break;
      case InputEventType::RIGHT:
        turnPlayerRight();
        break;
      default:
        break;
      }
    }
  }

  void MazeState::onActivate() {
    Entrance e = maze->getMazeEntrance();
    
    player->setX(e.getX());
    player->setY(e.getY());
    player->setFacing(e.getFacing());

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraAngle = 0;
  }

  void MazeState::activateActivatable() {
    int32_t lookX = 0;
    int32_t lookY = 0;
    switch (player->getFacing()) {
    case Facing::NORTH:
      lookX = player->getX();
      lookY = player->getY() - 1;
      break;
    case Facing::EAST:
      lookX = player->getX() + 1;
      lookY = player->getY();
      break;
    case Facing::SOUTH:
      lookX = player->getX();
      lookY = player->getY() + 1;
      break;
    case Facing::WEST:
      lookX = player->getX() - 1;
      lookY = player->getY();
      break;
    }
    
    Activatable* act = currentMap->getActivatable(lookX, lookY);

    if (act) {
      if (act->asDoor()) {
        activateDoor(act->asDoor());
      }
      
      if (act->asSecretDoor()) {
        activateSecretDoor(act->asSecretDoor());
      }
      
      if (act->asStairs()) {
        activateStairs(act->asStairs());
      }
    }
  }

  void MazeState::activateDoor(Door* door) {
    int32_t newX = player->getX();
    int32_t newY = player->getY();
    
    if (door->getOrientation() == Orientation::VERTICAL) {
      switch(player->getFacing()) {
      case Facing::NORTH:
        newY -= 2;
        break;
      case Facing::SOUTH:
        newY += 2;
        break;
      default:
        break;
      }
    }
    else {
      switch(player->getFacing()) {
      case Facing::EAST:
        newX += 2;
        break;
      case Facing::WEST:
        newX -= 2;
        break;
      default:
        break;
      }
    }
    
    if (currentMap->canEntityEnter(newX, newY)) {
      player->setX(newX);
      player->setY(newY);
    }
    else {
      writeToLog(MessageLevel::WARNING, "MazeState::activateDoor():  Attempted to move player into invalid location.");
    }
  }
  
  void MazeState::activateSecretDoor(SecretDoor* secretDoor) {
    int32_t newX = player->getX();
    int32_t newY = player->getY();
    
    switch(secretDoor->getDirection()) {
    case SecretDoorDirection::NORTH:
      if (player->getFacing() == Facing::NORTH) {
        newY -= 2;
      }
      break;
    case SecretDoorDirection::EAST:
      if (player->getFacing() == Facing::EAST) {
        newX += 2;
      }
      break;
    case SecretDoorDirection::SOUTH:
      if (player->getFacing() == Facing::SOUTH) {
        newY += 2;
      }
      break;
    case SecretDoorDirection::WEST:
      if (player->getFacing() == Facing::WEST) {
        newX -= 2;
      }
      break;
    case SecretDoorDirection::NORTH_SOUTH:
      switch (player->getFacing()) {
      case Facing::NORTH:
        newY -= 2;
        break;
      case Facing::SOUTH:
        newY += 2;
        break;
      default:
        break;
      }
      break;
    case SecretDoorDirection::EAST_WEST:
      switch(player->getFacing()) {
      case Facing::EAST:
        newX += 2;
        break;
      case Facing::WEST:
        newX -= 2;
        break;
      default:
        break;
      }
      break;
    }

    if (currentMap->canEntityEnter(newX, newY)) {
      player->setX(newX);
      player->setY(newY);
    }
    else {
      writeToLog(MessageLevel::WARNING, "MazeState::activateSecretDoor():  Attempted to move player into invalid location.");
    }
  }
  
  void MazeState::activateStairs(Stairs* stairs) {
    int32_t newX = stairs->getDestinationX();
    int32_t newY = stairs->getDestinationY();
    int32_t newFloor = currentFloor;

    //  Check make sure the player is facing the right direction.
    bool isFacing = false;
    switch(stairs->getDestinationFacing()) {
    case Facing::NORTH:
      isFacing = (player->getFacing() == Facing::SOUTH);
      break;
    case Facing::EAST:
      isFacing = (player->getFacing() == Facing::WEST);
      break;
    case Facing::SOUTH:
      isFacing = (player->getFacing() == Facing::NORTH);
      break;
    case Facing::WEST:
      isFacing = (player->getFacing() == Facing::EAST);
      break;
    }
    
    if (!isFacing) {
      return;
    }
    
    switch(stairs->getDirection()) {
    case StairDirection::UP:
      newFloor--;
      break;
    case StairDirection::DOWN:
      newFloor++;
      break;
    }
    
    if (newFloor > 0) {
      Map* newMap = maze->getFloor(newFloor);
      if (newMap->canEntityEnter(newX, newY)) {
        currentMap = newMap;
        currentFloor = newFloor;
        player->setX(newX);
        player->setY(newY);
        player->setFacing(stairs->getDestinationFacing());
      }
      else {

      }
    }
    else {
      game->setGameState(GameState::TOWN);
    }
  }
    
  void MazeState::movePlayerForward() {
    if (inputDisabled) {
      return;
    }

    int32_t newX = player->getX();
    int32_t newY = player->getY();

    switch (player->getFacing()) {
    case Facing::NORTH:
      newY--;
      break;
    case Facing::EAST:
      newX++;
      break;
    case Facing::SOUTH:
      newY++;
      break;
    case Facing::WEST:
      newX--;
      break;
    }

    if (currentMap->canEntityEnter(newX, newY)) {
      player->setX(newX);
      player->setY(newY);
    }

    //animationState = MazeAnimationState::MOVING_FORWARD;
    //inputDisabledTicks = 30;
    //inputDisabled = true;
  }

  void MazeState::movePlayerBackward() {
    if (inputDisabled) {
      return;
    }

    int32_t newX = player->getX();
    int32_t newY = player->getY();

    switch (player->getFacing()) {
    case Facing::NORTH:
      newY++;
      break;
    case Facing::EAST:
      newX--;
      break;
    case Facing::SOUTH:
      newY--;
      break;
    case Facing::WEST:
      newX++;
      break;
    }

    if (currentMap->canEntityEnter(newX, newY)) {
      player->setX(newX);
      player->setY(newY);
    }

    //animationState = MazeAnimationState::MOVING_BACKWARD;
    //inputDisabledTicks = 30;
    //inputDisabled = true;
  }

  void MazeState::turnPlayerLeft() {
    if (inputDisabled) {
      return;
    }

    switch (player->getFacing()) {
    case Facing::NORTH:
      player->setFacing(Facing::WEST);
      break;
    case Facing::EAST:
      player->setFacing(Facing::NORTH);
      break;
    case Facing::SOUTH:
      player->setFacing(Facing::EAST);
      break;
    case Facing::WEST:
      player->setFacing(Facing::SOUTH);
      break;
    }

    //animationState = MazeAnimationState::TURNING_LEFT;
    //inputDisabledTicks = 30;
    //inputDisabled = true;
  }

  void MazeState::turnPlayerRight() {
    if (inputDisabled) {
      return;
    }

    switch (player->getFacing()) {
    case Facing::NORTH:
      player->setFacing(Facing::EAST);
      break;
    case Facing::EAST:
      player->setFacing(Facing::SOUTH);
      break;
    case Facing::SOUTH:
      player->setFacing(Facing::WEST);
      break;
    case Facing::WEST:
      player->setFacing(Facing::NORTH);
      break;
    }

    //animationState = MazeAnimationState::TURNING_RIGHT;
    //inputDisabledTicks = 30;
    //inputDisabled = true;
  }
}

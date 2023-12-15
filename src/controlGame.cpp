#include "controlGame.hpp"

GameStatus ControlGame::gameStatus;
bool ControlGame::running;
Vector2f ControlGame::pacPosition;
bool ControlGame::pacIsDead = false;
int ControlGame::dotsEat = 0;
mutex ControlGame::scoreMutex;
int ControlGame::score;
Vector2f ControlGame::blinkyPosition;
GhostStatus ControlGame::ghostStatus;
int ControlGame::demoPos[5][2] = { {6,13}, {6,8},{6,9},{6,10},{6,11} };
int ControlGame::ghostBonus = 100;
bool ControlGame::isBlueGhost = false;
int ControlGame::attackInterval = 20;
int ControlGame::scaterInterval = 7;
int ControlGame::blueInterval = 6;
bool ControlGame::gameRun = false;
int ControlGame::level = 0;
#include "Entity.hpp"
#include "controlGame.hpp"


bool Entity::collidWall(int x, int y){
    string str = getMazeStr(x, y);
    return str == '#' || str == '-';
}

string Entity::getMazeStr(int x, int y){
    return maze.mazeGrid[x][y];
}

void Entity::setGridPosition(int x, int y){
    entityPos = Vector2f((x * SPR_SIZE) + (SPR_SIZE/2), (y * SPR_SIZE) + (SPR_SIZE/2));
}

bool Entity::xyModul(){
    int modulX = (int)entitySpr.getPosition().x % SPR_SIZE;
    int modulY = (int)entitySpr.getPosition().y % SPR_SIZE;
    return modulX == 15 && modulY == 15;
}

int Entity::getPosGridX(){
    return (float)entitySpr.getPosition().x/SPR_SIZE;
}

int Entity::getPosGridY(){
    return (float)entitySpr.getPosition().y/SPR_SIZE;
}
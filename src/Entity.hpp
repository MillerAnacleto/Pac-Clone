#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Maze.hpp"

class Entity{
    public:
        int delayLoop = 20;
        int sprOfsX = 0;
        int sprOfsY = 0;
        float charAnim = 0;
        float speed = 3;
        float moveX = 0;
        float moveY = 0;
        int startX;
        int startY;
        bool stopMove = false;
        Maze maze;
        Sprite entitySpr;
        Vector2f entityPos;


        bool collidWall(int x, int y);
        string getMazeStr(int x, int y);
        void setGridPosition(int x, int y);
        int getPosGridX();
        int getPosGridY();
        bool xyModul();
        
};


#endif
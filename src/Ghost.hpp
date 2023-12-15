#ifndef GHOST_HPP
#define GHOST_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "controlGame.hpp"
#include <thread>

#include <iostream>
#include "Pac.hpp"
#include "TargetCalc.hpp"
#include "GameText.hpp"

using namespace std;
using namespace sf;

template <class C> class Ghost : private Entity{
    public:
        int Id;
        GhostStatus thisGhostStatus;
        GhostDirection currGhostDir;
	    Vector2f edgePos;
        Vector2f* targetPos;
        bool isInHome;

        Ghost(Texture* Texture, int id, C* _starter){
            Id = id;
            name = nameArr[id];
            starter = _starter;
            entitySpr.setTexture(*Texture);
            cout << "Ghost class OK Name: " << name << endl;

            startX = Maze::ghostStartPosArr[id - 1][0];
            startY = Maze::ghostStartPosArr[id - 1][1];
            edgePos = Vector2f(Maze::ghostEdgePosArr[Id - 1][0] * SPR_SIZE, Maze::ghostEdgePosArr[Id - 1][1] * SPR_SIZE);
            targetPos = &edgePos;

            sprOfsX = 0; 
            sprOfsY = SPR_SIZE * (Id);
            entitySpr.setOrigin(SPR_SIZE / 2, SPR_SIZE / 2);
            entitySpr.setTextureRect(IntRect(sprOfsX, sprOfsY, SPR_SIZE, SPR_SIZE));
            ghostNameTxt = gameText.startGhostNameTxt(name);
            reset();
            loopThread = new thread(&Ghost::loop, this);
        }

        ~Ghost(){
            if(loopThread->joinable())
                loopThread->join();
            
            delete loopThread;
            delete ghostNameTxt;
            cout << "deleted " << name << endl;

        }

        void reset(){
            charAnim = 0;
            moveX = moveY = 0;
            speed = 3;
            if(ControlGame::gameStatus == Demo){
                setGridPosition(ControlGame::demoPos[Id][0], ControlGame::demoPos[Id][1]);
                ghostNameTxt->setPosition(
                    Vector2f(ControlGame::demoPos[Id][0] * (SPR_SIZE + 10),
                    ControlGame::demoPos[Id][1] * SPR_SIZE));
                direction(GhostDirection::DOWN);
            }
            else{
                    goHome();
            }
            thisGhostStatus = Scater;
            stop();
        }

        void run(){
            stopMove = false;
        }

        void stop(){
            stopMove = true;
        }

        Sprite getSprite(){
            return entitySpr;
        }

        Text getNameTxt(){
            return *ghostNameTxt;
        }

        void changeGhostState(){			
            switch(ControlGame::ghostStatus){
                case Attack:		
                    thisGhostStatus = Attack;
                    break;
                case Scater:
                    thisGhostStatus = Scater;
                    break;
                case Blue:
                    thisGhostStatus = Blue;
                    break;
                case Death:
                    thisGhostStatus = Death;
                    break;
            }

            if(Id == 1 && thisGhostStatus == Scater)// for Blinky
                if(Maze::dotsCount - ControlGame::dotsEat < 30)
                    thisGhostStatus = Attack;

            if(thisGhostStatus == Attack)	  	
                targetPos = &ControlGame::pacPosition;

            else if(thisGhostStatus == Scater || thisGhostStatus==Blue)
                targetPos = &edgePos;
            
            if(thisGhostStatus != Blue){
                sprOfsY = SPR_SIZE * Id;
            }

            if(isInHome && ControlGame::gameRun)
                run();
        }
        
        void invertCurrDirection(){
            GhostDirection nextDir;
            switch(currGhostDir){
                case DOWN:
                    nextDir = UP;
                    break;
                    
                case UP:
                    nextDir = DOWN;
                    break;
                    
                case LEFT:
                    nextDir = RIGHT;
                    break;
                    
                case RIGHT:
                    nextDir = LEFT;
                    break;

                default:
                    nextDir = currGhostDir;
                    break;
            }
            
            direction(nextDir);
        }

    private:
        string name;
        C* starter;
	    TargetCalc targetCalc;
        string nameArr[5] = {"", "Blinky", "Pinky", "Inky", "Clyde"};
        thread* loopThread;
        GameText gameText;
        Text* ghostNameTxt;
        int sprOfsXX;

        void loop(){
            while(ControlGame::running){
                sleep(milliseconds(delayLoop));
                if(!stopMove){
                    if(xyModul())
                        checkCross();
                    entityPos.x += moveX;
                    entityPos.y += moveY;
                }
                charAnim += .1;
                if(charAnim > 2)
                    charAnim = 0;
                animation();
            }
        }

        void goHome(){
            setGridPosition(startX, startY);
            entitySpr.setPosition(entityPos);
            isInHome = true;
            if(Id < 3)
                direction(GhostDirection::RIGHT);
            else
                direction(GhostDirection::LEFT);
            
            sprOfsY = SPR_SIZE * Id;
            targetPos = &edgePos;
            thisGhostStatus = Scater;
            stop();
        }

        void checkCross(){
            if(getPosGridX() >= 22)
                setGridPosition(1, 11);

            if(getPosGridX() <= 0)
                setGridPosition(20, 11);

            string str = getMazeStr(getPosGridX(), getPosGridY());
            if(str == "=" || thisGhostStatus == Blue)
                delayLoop = 25;
            else
                delayLoop = 20;
            if(str == "-")
                isInHome = false;
            else if(str == "#"){
                cout <<"Ghost error stoped. Name = " << name << endl;
                stop();
            }

            const char* ch = &str[0];
            GhostDirection dir = getDirection(ch, currGhostDir);
            if(dir == NONE){
                if(Id == 1)
                    ControlGame::blinkyPosition = entityPos;
                dir = TargetCalc::getToTargetDir(Id, entityPos, currGhostDir, thisGhostStatus, &targetPos, edgePos, Pac<C>::pacDirection, ch);
                if(dir == NONE)
                    dir = currGhostDir;
            }
            direction(dir);
        }

        void direction(GhostDirection direction){

            switch (direction){
                case UP:
                    moveY = -speed;
                    moveX = 0;
                    break;
                
                case RIGHT:
                    moveY = 0;
                    moveX = speed;
                    break;
                
                case DOWN:
                    moveY = speed;
                    moveX = 0;
                    break;
                
                case LEFT:
                    moveY = 0;
                    moveX = -speed;
                    break;
                
                case NONE:
                    break;
            }

            currGhostDir = direction;
        }

        void animation(){
            if(thisGhostStatus != Blue){
                switch (currGhostDir){
                    case UP:
                        sprOfsXX = SPR_SIZE * 4;
                        break;
                        
                    case RIGHT:
                        sprOfsXX = 0;
                        break;
                        
                    case DOWN:
                        sprOfsXX = SPR_SIZE * 2;
                        break;
                        
                    case LEFT:
                        sprOfsXX = SPR_SIZE * 6;
                        break;
                        
                    case NONE:
                        break;
                }
            }
            else{
                sprOfsY = 5 * SPR_SIZE;
                sprOfsXX = 0;
            }

            sprOfsX = (SPR_SIZE * (int)charAnim) + sprOfsXX;
            entitySpr.setTextureRect(IntRect(sprOfsX, sprOfsY, SPR_SIZE, SPR_SIZE));
            entitySpr.setPosition(entityPos);

            if(ControlGame::gameStatus == Play && !ControlGame::pacIsDead){
                if(collidToPac()){
                    if(ControlGame::isBlueGhost){
                        // FALTA PLAY SOUND
                        if(ControlGame::ghostBonus > 1600)
                            ControlGame::ghostBonus = 100;
                        ControlGame::ghostBonus *= 2;
                        lock_guard<mutex> lock(ControlGame::scoreMutex);
                        ControlGame::score += ControlGame::ghostBonus;
                        starter->gameText->scoreTxt.setString("SCORE: "+to_string(ControlGame::score));
                    ControlGame::scoreMutex.unlock();
                        starter->gameText->showBonus(ControlGame::ghostBonus, entityPos, false);
                        goHome();
                    }
                    else{
                        starter->hitPac();
                    }
                    cout << "Collided to pac" << endl;
                }
            }
        }

        GhostDirection getDirection(const char* crossCh, GhostDirection currDir){
            switch(*crossCh){
                case '*':
                    return UP;
                    break;

                case 'Q':
                    return (currDir == LEFT)? DOWN: RIGHT;
                    break;

                case 'W':
                    return (currDir == RIGHT)? DOWN: LEFT;
                    break;

                case 'E':
                    return (currDir == LEFT)? UP: RIGHT;
                    break;

                case 'R':
                    return (currDir == RIGHT)? UP: LEFT;
                    break;
            }
            
            return NONE;
        }

        bool collidToPac(){
            int dx = abs(entitySpr.getPosition().x - ControlGame::pacPosition.x);
            int dy = abs(entitySpr.getPosition().y - ControlGame::pacPosition.y);
            return dx < SPR_SIZE/2 && dy < SPR_SIZE/2;		
        }
};


#endif
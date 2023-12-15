#ifndef PAC_HPP
#define PAC_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include <thread>
#include "controlGame.hpp"
#include <iostream>

using namespace std;
using namespace sf;

enum PacDirection{
    Up, Right, Down, Left
};

template <class C> class Pac : private Entity{
    public:
        int pacLife;
        static PacDirection pacDirection, pacPrevDirection;

        Pac(Texture* texture, C* _starter){
            starter = _starter;
            pacLife = 3;
            startX = 11;
            startY = 18;
            cout << "Pac class OK" << endl;
            entitySpr.setTexture(*texture);
            lifeSpr.setTexture(*texture);
            
            sprOfsX = 0;
            sprOfsY = 0;
            entitySpr.setOrigin(SPR_SIZE/2, SPR_SIZE/2);
            entitySpr.setTextureRect(IntRect(sprOfsX, sprOfsY, SPR_SIZE, SPR_SIZE));
            lifeSpr.setTextureRect(IntRect(30, 0, SPR_SIZE, SPR_SIZE));
            lifeSpr.scale(.8, .8);
            reset();
            loopThread = new thread(&Pac::loop, this);
        }

        ~Pac(){
            if(loopThread->joinable())
                loopThread->join();
            
            delete loopThread;
            cout << "deleted P" << endl;
        }

        Sprite getSprite(){
            return entitySpr;
        }

        Sprite getLifeSpr(){
            return lifeSpr;
        }

        void rotation(Keyboard::Key key){
            if(checkKeyPressed(key)){
                move(key);
            }

        }

        void reset(){
            ControlGame::pacIsDead = false;
            charAnim = 0;
            moveX = moveY = 0;
            pacRot = 0;
            forwX = 1;
            forwY = 0;
            Pac::pacDirection = pacPrevDirection = Right;
            if(ControlGame::gameStatus == Demo){
                setGridPosition(ControlGame::demoPos[0][0], ControlGame::demoPos[0][1]);
                pacDirection = pacPrevDirection = Left;
            }
            else{
                setGridPosition(startX, startY);
            }
            entitySpr.setPosition(entityPos);
            stop();
        }

        void run(){
            stopMove = false;
        }

        void stop(){
            stopMove = true;
        }
    
    private:
        thread* loopThread;
        C* starter;
        Sprite pacSpr;
        Sprite lifeSpr;
        int forwX;
        int forwY;
        int pacRot;

        bool checkKeyPressed(Keyboard::Key key){
            return key == Keyboard::Up     ||
                    key == Keyboard::Right ||
                    key == Keyboard::Down  ||
                    key == Keyboard::Left;
        }

        void move(Keyboard::Key key){

            if(ControlGame::gameStatus == Demo)
                return;
            
            switch (key){
                case Keyboard::Up:
                    if(forwY == 1)
                        return;
                    Pac::pacDirection = Up;
                    break;
                    
                case Keyboard::Right:
                    if(forwX == -1)
                        return;
                    Pac::pacDirection = Right;
                    break;
                    
                case Keyboard::Down:
                    if(forwY == -1)
                        return;
                    Pac::pacDirection = Down;
                    break;
                    
                case Keyboard::Left:
                    if(forwX == 1)
                        return;
                    Pac::pacDirection = Left;
                    break;
                
                default:
                    break;
            }

        }

        void loop(){
            while(ControlGame::running){
                sleep(milliseconds(delayLoop));
                charAnim += .4;
                if(charAnim > 3)
                    charAnim = 0;
                animation();
                // FALTA DIVIDIR ANIMATION DE MOVE
            }
        }

        void animation(){

            if(getPosGridX() >= 22)
                setGridPosition(1, 11);

            if(getPosGridX() <= 0)
                setGridPosition(20, 11);

            if(xyModul()){

                direction(Pac::pacDirection);
                if(collidWall(getPosGridX() + forwX, getPosGridY() + forwY)){
                    if(Pac::pacDirection == pacPrevDirection){
                        moveX = moveY = 0;
                    }
                    else{
                        direction(pacPrevDirection);
                        if(collidWall(getPosGridX() + forwX, getPosGridY() + forwY)){
                            moveX = moveY = 0;
                        }
                        Pac::pacDirection = pacPrevDirection;
                    }
                }
                
                pacPrevDirection = Pac::pacDirection;
            }

            if(xyModul() && collidWall(getPosGridX() + forwX, getPosGridY() + forwY)){
                moveX = moveY = 0;
            }
            else if(!stopMove){
                if(pacRot != entitySpr.getRotation()){
                    entitySpr.setRotation(pacRot);
                }
                
                entityPos.x += moveX;
                entityPos.y += moveY;
                ControlGame::pacPosition = entityPos;
                entitySpr.setPosition(entityPos);
                if(xyModul()){
                    eatDot();
                }
            }

            // FALTA COMER FRUTA

            sprOfsX = SPR_SIZE * (int)charAnim;
            entitySpr.setTextureRect(IntRect(sprOfsX, sprOfsY, SPR_SIZE, SPR_SIZE));
        }

        void direction(PacDirection  direct){
            forwX = forwY = 0;
            switch(direct){
                case Up:
                    moveX = 0;
                    moveY = -speed;
                    forwY = -1;
                    pacRot = -90;
                    break;
                    
                case Right:
                    moveX = speed;
                    moveY = 0;
                    forwX = 1;
                    pacRot = 0;
                    break;
                    
                case Down:
                    moveX = 0;
                    moveY = speed;
                    forwY = 1;
                    pacRot = 90;
                    break;
                    
                case Left:
                    moveX = -speed;
                    moveY = 0;
                    forwX = -1;
                    pacRot = 180;
                    break;
            }
                
        }

        void eatDot(){

            int pacX = getPosGridX();
            int pacY = getPosGridY();

            int addScore = 0;

            for(int i = 0; i < maze.dotsCount; i++){
                CircleShape dot = maze.dotsArr[i];
                if(dot.getFillColor() == Color::Black)
                    continue;
                Vector2f dotPos = dot.getPosition();
                int dotX = (int)dotPos.x/SPR_SIZE;
                int dotY = (int)dotPos.y/SPR_SIZE;
                if(dotX == pacX && dotY == pacY && !ControlGame::pacIsDead){
                    
                    if(dot.getRadius() == 10){  //BOLONA
                        addScore += 40;
                        if(ControlGame::blueInterval > 0){
                            ControlGame::isBlueGhost = true;
                            starter->setBlueGhost();
                        }
                    }

                    dot.setFillColor(Color::Black);
                    maze.dotsArr[i] = dot;
                    // FALTA PLAY SOUND
                    ControlGame::dotsEat++;
                    addScore += 10;
                    lock_guard<mutex> lock(ControlGame::scoreMutex);
                    ControlGame::score += addScore;
                    starter->gameText->scoreTxt.setString("SCORE: "+to_string(ControlGame::score));
                    ControlGame::scoreMutex.unlock();
                    break;
                }
            }
        }
};

template <class C> PacDirection Pac<C>::pacDirection;
template <class C> PacDirection Pac<C>::pacPrevDirection;

#endif
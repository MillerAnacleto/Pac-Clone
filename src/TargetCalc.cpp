#include "TargetCalc.hpp"
#include <iostream>

GhostDirection TargetCalc::getToTargetDir(int ghId, Vector2f ghostPos, GhostDirection ghostDir, GhostStatus ghostStatus, Vector2f** targetPos, Vector2f edgePos, PacDirection pacDir, const char* crossCh){
    int targetOfsX = 0;
    int targetOfsY = 0;

    getTargetOfs(&targetOfsX, &targetOfsY, ghId, **targetPos, pacDir);

    int targetX = (**targetPos).x + targetOfsX;
    int targetY = (**targetPos).y + targetOfsY;

    if(noCondition(crossCh))
        return NONE;

    int dX = abs(ghostPos.x - (ControlGame::pacPosition.x + targetOfsX));
    int dY = abs(ghostPos.y - (ControlGame::pacPosition.y + targetOfsY));

    string str = string(1, *crossCh);
    if(ghId == 4){
        if(ControlGame::ghostStatus == Attack){
            if(dX/SPR_SIZE < 8 && dY/SPR_SIZE < 8){
                (*targetPos) = &edgePos;
            }
            else{
                (*targetPos) = &ControlGame::pacPosition;
            }
        }
    }
    
    double distUP, distDOWN, distLEFT, distRIGHT;
    if(ghostStatus != Blue){
        distUP = abs(ghostPos.x - targetX) + abs(ghostPos.y - targetY - SPR_SIZE);
        distDOWN = abs(ghostPos.x - targetX) + abs(ghostPos.y - targetY + SPR_SIZE);
        distLEFT = abs(ghostPos.x - targetX - SPR_SIZE) + abs(ghostPos.y - targetY);
        distRIGHT = abs(ghostPos.x - targetX + SPR_SIZE) + abs(ghostPos.y - targetY);
    }
    else{
        srand(time(0));

        distUP = rand();
        distDOWN = rand();
        distLEFT = rand();
        distRIGHT = rand();
    }

    if(str == "A"){
        // cout << "A: " << distUP << " " << distLEFT << " " << distRIGHT << " " << ghostDir << endl ;
        switch (ghostDir){
            case DOWN:
                if(distLEFT < distRIGHT)
                    return LEFT;
                else
                    return RIGHT;
                break;
            
            case LEFT:
                if(distUP < distLEFT)
                    return UP;
                else
                    return LEFT;
                break;

            case RIGHT:
                if(distRIGHT < distUP)
                    return RIGHT;
                else
                    return UP;
                break;
            
            default:
                return NONE;
        }
    }
    else if(str == "S"){
        // cout << "S: " << distDOWN << " " << distLEFT << " " << distRIGHT << " " << ghostDir << endl ;
        switch (ghostDir){
            case UP:
                if(distLEFT < distRIGHT)
                    return LEFT;
                else
                    return RIGHT;
                break;
            
            case LEFT:
                if(distDOWN < distLEFT)
                    return DOWN;
                else
                    return LEFT;
                break;

            case RIGHT:
                if(distRIGHT < distDOWN)
                    return RIGHT;
                else
                    return DOWN;
                break;
            
            default:
                return NONE;
        }
    }
    else if(str == "D"){
        // cout << "D: " << distUP << " " << distDOWN << " " << distRIGHT << " " << ghostDir << endl ;
        switch (ghostDir){
            case UP:
                if(distUP < distRIGHT)
                    return UP;
                else
                    return RIGHT;
                break;
            
            case LEFT:
                if(distDOWN < distUP)
                    return DOWN;
                else
                    return UP;
                break;

            case DOWN:
                if(distRIGHT < distDOWN)
                    return RIGHT;
                else
                    return DOWN;
                break;
            
            default:
                return NONE;
        }
    }
    else if(str == "F"){
        // cout << "F: " << distUP << " " << distDOWN << " " << distLEFT << " " << ghostDir << endl ;
        switch (ghostDir){
            case UP:
                if(distUP < distLEFT)
                    return UP;
                else
                    return LEFT;
                break;
            
            case RIGHT:
                if(distDOWN < distUP)
                    return DOWN;
                else
                    return UP;
                break;

            case DOWN:
                if(distLEFT < distDOWN)
                    return LEFT;
                else
                    return DOWN;
                break;
            
            default:
                return NONE;
        }
    }
    else if(str == "X"){
        // cout << "X: " << distUP << " " << distDOWN << " " << distRIGHT << " " << distLEFT << " " << ghostDir << endl ;
        switch (ghostDir){
            case UP:
                if(distUP < distLEFT)
                    if(distUP < distRIGHT)
                        return UP;
                    else
                        return RIGHT;
                else
                    if(distLEFT < distRIGHT)
                        return LEFT;
                    else
                        return RIGHT;
                break;
            
            case RIGHT:
                if(distUP < distDOWN)
                    if(distUP < distRIGHT)
                        return UP;
                    else
                        return RIGHT;
                else
                    if(distDOWN < distRIGHT)
                        return DOWN;
                    else
                        return RIGHT;
                break;

            case DOWN:
                if(distLEFT < distDOWN)
                    if(distLEFT < distRIGHT)
                        return LEFT;
                    else
                        return RIGHT;
                else
                    if(distDOWN < distRIGHT)
                        return DOWN;
                    else
                        return RIGHT;
                break;
                
            case LEFT:
                if(distLEFT < distDOWN)
                    if(distLEFT < distUP)
                        return LEFT;
                    else
                        return UP;
                else
                    if(distDOWN < distUP)
                        return DOWN;
                    else
                        return UP;
                break;
            
            default:
                return NONE;
        }
    }
    else if(str == "G"){
        switch (ghostDir){
            case UP:
                if(distLEFT < distRIGHT)
                    return LEFT;
                else
                    return RIGHT;
                break;
            
            case LEFT:
                return LEFT;

            case RIGHT:
                return RIGHT;

            default:
                return NONE;
        }
    }
    

    return NONE;
}

void TargetCalc::getTargetOfs(int* targetOfsX, int* targetOfsY, int ghId, Vector2f targetPos, PacDirection pacDir){

    if(ghId == 2 || ghId == 3){
        switch (pacDir){
            case Up:
                *targetOfsY = (ghId == 3)?-2:-4;
                break;
            
            case Right:
                *targetOfsX = (ghId == 3)?2:4;
                break;
                
            case Down:
                *targetOfsY = (ghId == 3)?2:4;
                break;
                
            case Left:
                *targetOfsX = (ghId == 3)?-2:-4;
                break;

            default:
                break;
        }
    }

    *targetOfsX *= SPR_SIZE;
    *targetOfsY *= SPR_SIZE;

    
    if (ghId == 3 && ControlGame::ghostStatus == Attack){ // Inky
        *targetOfsX += (targetPos.x - ControlGame::blinkyPosition.x);
        *targetOfsY += (targetPos.y - ControlGame::blinkyPosition.y);
    }
}

bool TargetCalc::noCondition(const char* ch){
    return *ch == 'B' || *ch == 'P' || *ch == 'I' || *ch == 'C' || 
           *ch == '0' || *ch == '=' || *ch == '1' || *ch == '@' ||
           *ch == '#' || *ch == ' ' || *ch == '-' || *ch == 'c';
}
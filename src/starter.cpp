#include "starter.hpp"
#include <iostream>
#include "controlGame.hpp"
#include <thread>



Starter::Starter(RenderWindow *window, Texture* sprTexture) {
    cout << "Starter class OK - Start game" << endl;
    
    // FALTA INSTANCIAR SONS
    intro = false;
    ControlGame::running = true;
    ControlGame::gameStatus = Demo;
    threadRunning = false;
    toNextLevel = false;
    _window = window;
    ghStatusThread = NULL;
    backText.loadFromFile("texture/map1.png");
    backFlashText.loadFromFile("texture/mapB.png");
    backSpr.setTexture(backText);
    // FALTA INSTANCIAR FRUTA
    pac = new Pac<Starter>(sprTexture, this);
    Blinky = new Ghost<Starter>(sprTexture, 1, this);
    Pinky = new Ghost<Starter>(sprTexture, 2, this);
    Inky = new Ghost<Starter>(sprTexture, 3, this);
    Clyde = new Ghost<Starter>(sprTexture, 4, this);
    gameText = new GameText();

    while(window->isOpen()) {
        // Processamento de eventos e renderização aqui
        if(ControlGame::dotsEat == Maze::dotsCount)
            nextLevel();
        while(window->pollEvent(event)){
            if (event.type == Event::Closed){
                window->close();
            }
            else if(event.type == Event::KeyPressed)
            {
                if(ControlGame::gameStatus == Demo && event.key.code == Keyboard::Enter){
                    startGame();
                }
                pac->rotation(event.key.code);
            }
        }
        loop(window);
    }
}

Starter::~Starter(){
    ControlGame::running = false;
    threadRunning = false;
    
    delete pac;
    delete Blinky;
    delete Pinky;
    delete Inky;
    delete Clyde;
    delete gameText;
    
    if(ghStatusThread != NULL && ghStatusThread->joinable())
        ghStatusThread->join();
        
    delete ghStatusThread;

    cout << "deleted S" << endl;
}


void Starter::loop(RenderWindow* window){
    window->clear();
    // FALTA Win life condition
    // FALTA HABILITAR O gameStatus condition
    if(ControlGame::gameStatus == Play){
        window->draw(backSpr);
        maze.drawDots(window);
        drawLife(window);
        window->draw(gameText->scoreTxt);
        window->draw(gameText->levelTxt);
        if(ControlGame::ghostStatus == Blue){
            string dif = to_string(curTime - time(0));
            if(stoi(dif) == 0)
                dif = "";
            gameText->countTxt.setString(dif);
            window->draw(gameText->countTxt);
        }
        if(gameText->bonusTxt.getString() != "")
            window->draw(gameText->bonusTxt);
        // FALTA DESENHAR A FRUTA SE VISÍVEL
    }
    else{
        window->draw(gameText->titleTxt);
        window->draw(gameText->enterTxt);
        window->draw(Blinky->getNameTxt());
        window->draw(Pinky->getNameTxt());
        window->draw(Inky->getNameTxt());
        window->draw(Clyde->getNameTxt());
    }

    // //drawn chars
    window->draw(pac->getSprite());
    window->draw(Blinky->getSprite());
    window->draw(Pinky->getSprite());
    window->draw(Inky->getSprite());
    window->draw(Clyde->getSprite());

    if(ControlGame::gameStatus == Play)
        window->draw(gameText->gameOverTxt);

    window->display();
}

void Starter::drawLife(RenderWindow* window){
    Sprite spr = pac->getLifeSpr();

    for(int i = 0; i < pac->pacLife; i++){
        spr.setPosition(Vector2f(SPR_SIZE * i, SPR_SIZE * 24));
        window->draw(spr);
    }
}

// chamada do ghost
void Starter::hitPac(){
    ControlGame::pacIsDead = true;
    pac->pacLife--;
    stopAll();
    if(pac->pacLife <= 0){
        cout << "ACABOU!!" << endl;
        gameOver();
        return;
    }
}

// chamada do pac
void Starter::setBlueGhost(){
    if(ControlGame::pacIsDead)
        return;

    cout << "Blue Status" << endl;

    threadRunning = false;
    if(ghStatusThread != NULL && ghStatusThread->joinable())
        ghStatusThread->join();
    delete ghStatusThread;
    ghStatusThread = NULL;

    sleep(milliseconds(20));
    ControlGame::ghostStatus = Blue;
    creatGhostThr();
    // FALTA SOM
}

void Starter::startGame(){
    maze.redrawDots();
    gameText->gameOverTxt.setString("");
    pac->pacLife = 3;
    ControlGame::gameStatus = Play;
    resetPacGhost();
    intro = true;
    ControlGame::gameRun = true;
    ControlGame::level = 1;
    lock_guard<mutex> lock(ControlGame::scoreMutex);
    ControlGame::score = 0;
    ControlGame::scoreMutex.unlock();
    ControlGame::dotsEat = 0;
    ControlGame::blueInterval = 6;
    // FALTA FRUTA
    // FALTA SONS
    startLevel();
}

void Starter::startLevel(){
    if(intro){
        intro = false;
        // FALTA PLAY MUSICA
    }
    ControlGame::ghostStatus = Scater;
    // FALTA PLAY SOUND
    ControlGame::gameRun = true;
    resetPacGhost();
    pac->run();
    creatGhostThr();
    // FALTA FRUTA
    gameText->levelTxt.setString("LEVEL: " + to_string(ControlGame::level));
}

void Starter::creatGhostThr(){
    while(threadRunning){}
    threadRunning = true;
    ghStatusThread = new thread(&Starter::changeGhostState, this);
}

void Starter::resetPacGhost(){
    pac->reset();
    Blinky->reset();
    Pinky->reset();
    Inky->reset();
    Clyde->reset();
}

void Starter::changeGhostState(){
    cout << "Start thread for GhostStatus" << endl;

    int delay = ControlGame::scaterInterval;

    if(ControlGame::ghostStatus == Blue){
        delay = ControlGame::blueInterval;
        ControlGame::isBlueGhost = true;
        changeAllGhostDir();
    }

    changeAllGhostState();

    while(threadRunning){

        curTime = time(0);
        curTime += delay;
        
        while(threadRunning)
            if(curTime <= time(0))
                break;

        sleep(milliseconds(10));
        if(ControlGame::ghostStatus == Blue){
            ControlGame::isBlueGhost = false;
            ControlGame::ghostBonus = 100;
            // FALTA PLAY SOUNDS
        }
        if(ControlGame::ghostStatus == Attack){
            ControlGame::ghostStatus = Scater;
            delay = ControlGame::scaterInterval;
            cout << "Scatter" << endl;
        }
        else{
            ControlGame::ghostStatus = Attack;
            delay = ControlGame::attackInterval;
            cout << "Attack" << endl;
        }

        changeAllGhostState();

        if(Maze::dotsCount - ControlGame::dotsEat < 30){
            // FALTA PLAY SOUND
        }
    }
    
    cout << "Ended Thread GhostStatus" << endl;
    cout << "************************" << endl;
}

void Starter::changeAllGhostState(){
    Blinky->changeGhostState();
    Pinky->changeGhostState();
    Inky->changeGhostState();
    Clyde->changeGhostState();
}

void Starter::changeAllGhostDir(){
    Blinky->invertCurrDirection();
    Pinky->invertCurrDirection();
    Inky->invertCurrDirection();
    Clyde->invertCurrDirection();
}

void Starter::stopAll(){
    pac->stop();
    Blinky->stop();
    Pinky->stop();
    Inky->stop();
    Clyde->stop();	
    ControlGame::gameRun = false;
    gameText->stopThread();
    // FALTA FRUTA
    // FALTA SOM
    threadRunning = false;
    if(ghStatusThread != NULL && ghStatusThread->joinable())
        ghStatusThread->join();
    delete ghStatusThread;
    ghStatusThread = NULL;
    resetLevel();
    wait(2);
    if(pac->pacLife >= 0)
        startLevel();	
}

void Starter::resetLevel(){		
    ControlGame::ghostBonus = 100;
    if(pac->pacLife <= 0){
        gameText->gameOverTxt.setString("GAME OVER"); 
    }
}

void Starter::gameOver(){
    ControlGame::gameStatus = Demo;
    lock_guard<mutex> lock(ControlGame::scoreMutex);
    ControlGame::score = 0;
    ControlGame::scoreMutex.unlock();
    ControlGame::level = 0;
    // FALTA LIFE WIN
    gameText->scoreTxt.setString("SCORE: 0");
    pac->stop();
    Blinky->stop();
    Pinky->stop();
    Inky->stop();
    Clyde->stop();
    threadRunning = false;
    if(ghStatusThread != NULL && ghStatusThread->joinable())
        ghStatusThread->join();
    delete ghStatusThread;
    ghStatusThread = NULL;
    resetPacGhost();
    resetLevel();
}

void Starter::nextLevel(){
    // FALTA SOM
    toNextLevel = true;
    stopAll();
    ControlGame::dotsEat = 0;
    maze.redrawDots();
    ControlGame::level++;
    gameText->levelTxt.setString("LEVEL: " + to_string(ControlGame::level));
    // FALTA FRUTA
    if(ControlGame::level >= 2 && ControlGame::level <= 5)
        ControlGame::blueInterval--;
    else if(ControlGame::level >= 10)
        ControlGame::blueInterval = 0;
}

void Starter::wait(int delaySec){
    int curTime = time(0);
    int counter = 0;
    curTime += delaySec;
    while(curTime >= time(0)){
        counter++;
        if(toNextLevel){
            if(counter % 30 == 0)
                backSpr.setTexture(backFlashText);
            else if(counter % 30 == 15)
                backSpr.setTexture(backText);
            
            loop(_window);
        }

        sleep(milliseconds(10));
    }
    toNextLevel = false;
    backSpr.setTexture(backText);
}
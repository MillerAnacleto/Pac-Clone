#include "GameText.hpp"
#include <iostream>
#include <chrono>

GameText::GameText(){
    cout << "TextGame class OK" << endl;
    fnt.loadFromFile("./font/emulogic.ttf");
    fntTitle.loadFromFile("./font/pacfont.ttf");
    bonusThr = NULL;
    setText();
}

GameText::~GameText(){
}

Text* GameText::startGhostNameTxt(string name){
    Text* txt = new Text();
    txt->setFont(fnt);
    txt->setString(name);
    txt->setFillColor(Color::White);
    txt->setCharacterSize(25);
    return txt;
}

void GameText::showBonus(int bonus, Vector2f pos, bool fruit){
    cout << "Bonus ***** " << bonus << endl;
    if(fruit)
        bonusTxt.setFillColor(Color::Green);
    else
        bonusTxt.setFillColor(Color::Blue);
    bonusTxt.setString(to_string(bonus));
    bonusTxt.setPosition(pos);
    curTime = time(0);
    cout << curTime << endl;
    curTime += delay;
    cout << curTime << endl;
    sleep(milliseconds(100));
    if(bonusThr == NULL || !bonusThr->joinable()){
        cout << "TextThread created" << endl;
        bonusThr = new thread(&GameText::show, this);
    }
}

void GameText::show(){
    while(curTime >= time(0)){
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    bonusTxt.setString("");
}

void GameText::stopThread(){
    if(bonusThr != NULL){
        if(bonusThr->joinable())
            bonusThr->join();
        cout << "TextThread deleted" << endl;
        delete bonusThr;
    }
    bonusThr = NULL;
}

void GameText::setText(){
    titleTxt.setFont(fntTitle);
    titleTxt.setString("PAC-MAN");
    titleTxt.setCharacterSize(70);
    titleTxt.setPosition(Vector2f(150, 100));

    enterTxt.setFont(fnt);
    enterTxt.setString("PRESS ENTER");
    enterTxt.setCharacterSize(35);
    enterTxt.setPosition(Vector2f(150, 560));
    
    scoreTxt.setFont(fnt);
    scoreTxt.setString("SCORE: 0");
    scoreTxt.setCharacterSize(22);
    scoreTxt.setPosition(getGridPosXY(1, 0));
    
    levelTxt.setFont(fnt);
    levelTxt.setString("LEVEL: 0");
    levelTxt.setCharacterSize(18);
    levelTxt.setPosition(getGridPosXY(17, 24));

    countTxt.setFont(fnt);
    countTxt.setFillColor(Color::Red);
    countTxt.setString("");
    countTxt.setCharacterSize(35);
    countTxt.setOutlineThickness(2);
    countTxt.setPosition(getGridPosXY(11, 14));
    
    bonusTxt.setFont(fnt);
    bonusTxt.setFillColor(Color::Red);
    bonusTxt.setString("");
    bonusTxt.setCharacterSize(14);
    bonusTxt.setOutlineThickness(2);
    
    gameOverTxt.setFont(fnt);
    gameOverTxt.setString("");
    gameOverTxt.setFillColor(Color::Red);
    gameOverTxt.setOutlineColor(Color::White);
    gameOverTxt.setCharacterSize(35);
    gameOverTxt.setOutlineThickness(2);
    gameOverTxt.setPosition(getGridPosXY(6, 14));
}

Vector2f GameText::getGridPosXY(float x, float y){
    return Vector2f(x * SPR_SIZE, y * SPR_SIZE);
}
#ifndef GAMETEXT_HPP
#define GAMETEXT_HPP

#include <SFML/Graphics.hpp>
#include <thread>
#include "controlGame.hpp"

using namespace std;
using namespace sf;

class GameText{
    public:
        Font fnt;
        Font fntTitle;
		Text titleTxt;
        Text enterTxt;
		Text scoreTxt;
		Text levelTxt;
		Text countTxt;
		Text gameOverTxt;
		Text bonusTxt;
		int curTime;
		int delay = 2;// seconds
		thread* bonusThr;

        GameText();
        ~GameText();
        Text* startGhostNameTxt(string name);
        void showBonus(int bonus, Vector2f pos, bool fruit);
        void stopThread();

    private:
        void setText();
        Vector2f getGridPosXY(float x,float y);
        void show();
};

#endif
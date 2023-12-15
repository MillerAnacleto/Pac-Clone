#ifndef STARTER_HPP
#define STARTER_HPP

#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include "Maze.hpp"
#include "Pac.hpp"
#include "Ghost.hpp"
#include "GameText.hpp"

#define TEXTURE_W 691
#define TEXTURE_H 749

using namespace std;
using namespace sf;

class Starter {
    public:
        Texture backText;
        Texture backFlashText;
        Sprite backSpr;
        Event event;
        Maze maze;
        Pac<Starter>* pac;
        Ghost<Starter> *Blinky, *Pinky, *Inky, *Clyde;
        GameText* gameText;
	    thread* ghStatusThread;

        Starter(RenderWindow *win, Texture* sprTexture);
        ~Starter();
        void hitPac();
        void setBlueGhost();

    private:
        bool intro;
        int curTime;
        bool threadRunning;
        bool toNextLevel;
        RenderWindow* _window;
        void loop(RenderWindow *win);
        void drawLife(RenderWindow* win);
        void startGame();
        void resetPacGhost();
        void startLevel();
        void changeGhostState();
        void changeAllGhostState();
        void changeAllGhostDir();
        void creatGhostThr();
        void stopAll();
        void resetLevel();
        void gameOver();
        void nextLevel();
        void wait(int delaySec);
};

#endif
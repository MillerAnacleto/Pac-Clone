#ifndef CONTROLGAME_HPP
#define CONTROLGAME_HPP

#include <SFML/Graphics.hpp>
#include <mutex>

using namespace std;
using namespace sf;

#define SPR_SIZE 30

enum GhostStatus{
    Attack,
    Scater,
    Blue,
    Death
};

enum GhostDirection
{
	UP, RIGHT, DOWN, LEFT, NONE
};

enum GameStatus{
    Demo, Play
};

class ControlGame{
    public:
        static bool running;
        static bool gameRun;
        static GameStatus gameStatus;
        static Vector2f pacPosition;
        static bool pacIsDead;
        static int dotsEat;
        static mutex scoreMutex;
        static int score;
        static Vector2f blinkyPosition;
        static GhostStatus ghostStatus;
        static int demoPos[5][2];
        static int ghostBonus;
	    static bool isBlueGhost;
        static int attackInterval;
        static int scaterInterval;
        static int blueInterval;
        static int level;

        ControlGame();
        ~ControlGame();
    private:
};

#endif

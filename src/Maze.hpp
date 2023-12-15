#ifndef MAZE_HPP
#define MAZE_HPP

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Maze{
    public:

        static CircleShape dotsArr[222];
        static int dotsCount;
        
        static int ghostStartPosArr[4][2];
        static int ghostEdgePosArr[4][2];

        string mazeGrid[23][25];
        string mazePattern[25]  = { 
        "#######################" , // 0   // #= Bricks
        "#Q   S    W#Q    S   W#" , // 1   // X= Ghosts direction condition
        "#@### #### # #### ###@#" , // 2   // c= Pacman char
        "# ### #### # #### ### #" , // 3   // Q=RD
        "#D   X S  A A  S X   F#" , // 4   // W=LD
        "# ### # ####### # ### #" , // 5   // E=RU
        "#E   F# ####### #D   R#" , // 6   // R=LU
        "##### #E  W#Q  R# #####" , // 7   //-------
        "##### #### # #### #####" , // 8   // A=LRU 
        "##### #Q  AGA  W# #####" , // 9   // S=LRD
        "##### # ###-### # #####" , // 12  // D=UDR
        "0====X F#BP*IC#D X====0" , // 11  // F=UDL
        "##### # ####### # #####" , // 12  // G=LR   
        "##### #D0001000F# #####" , // 13  // 1=Fruit location
        "##### # ####### # #####" , // 14  // L=Low speed in tunel for ghosts
        "##### # ####### # #####" , // 15  // BPIC=Ghosts char
        "#Q   X A  W#Q  A X   W#" , // 16
        "# ### #### # #### ### #" , // 17
        "#E W#D S  AcA  S F#Q R#" , // 18
        "### # # ####### # # ###" , // 19
        "#Q A R#E  W#Q  R#E A W#" , // 20
        "# ######## # ######## #" , // 24
        "#@######## # ########@#" , // 22
        "#E        A A        R#" , // 23
        "#######################" };// 24
        // 01234567890123456789012

        Maze();
        ~Maze();
        void drawDots(RenderWindow* window);
        void redrawDots();

    private:
        static int mazeW, mazeH;

        bool isDot(string str);
        
        CircleShape getDots(int col, int row, string str);
};

#endif
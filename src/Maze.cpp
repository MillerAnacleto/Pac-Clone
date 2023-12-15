#include "Maze.hpp"
#include <iostream>

int Maze::mazeW = 23;
int Maze::mazeH = 25;
CircleShape Maze::dotsArr[222];
int Maze::dotsCount;

int Maze::ghostStartPosArr[4][2] = {{9, 11}, {10, 11}, {12, 11}, {13, 11}};
int Maze::ghostEdgePosArr[4][2] = {{-2, -5}, {22, -2}, {3, 19}, {18, 19}};

Maze::Maze(){
    cout << "Maze class OK" << endl;
    dotsCount = 0;
    
    for(int column = 0; column < Maze::mazeW; column++){
        for(int row = 0; row < Maze::mazeH; row++){
            string str = mazePattern[row].substr(column, 1);
            mazeGrid[column][row] = str;
            
            if(isDot(str)){
                dotsArr[dotsCount] = getDots(column, row, str); 
                dotsCount++;
            }
        }
    }
}

Maze::~Maze(){
    
}

void Maze::drawDots(RenderWindow* window){
    for(int i = 0; i < dotsCount; i++){
        window->draw(dotsArr[i]);
    }
}

bool Maze::isDot(string str){
    return str != "#" && str != "0" && str != "=" &&
            str != "B" && str != "P" &&
            str != "I" && str != "C" &&
            str != "G" && str != "*" &&
            str != "1" && str != "-" && str != "c";
}

CircleShape Maze::getDots(int col, int row, string str){
    float radius = 3;
    int offset = 12;
    Color color = Color::Yellow;

    if(str == "@"){
        radius = 10;
        offset = 6;
    }          

    CircleShape dot;
    dot.setRadius(radius);
    dot.setFillColor(color);
    dot.setPosition(col * 30 + offset, row * 30 + offset);      
    return dot;
}

void Maze::redrawDots(){
    for(int i = 0; i < dotsCount; i++){
        CircleShape dot = dotsArr[i];
        dot.setFillColor(Color::Yellow);
        dotsArr[i] = dot;
    }
}
#include <SFML/Graphics.hpp>
#include <iostream>
#include "./src/starter.hpp"

using namespace std;
using namespace sf;

int winW = 23 * 30;
int winH = 25 * 30;

Texture sprTexture;
Starter *starter;

int main() {
    // Crie uma janela
    while(!sprTexture.loadFromFile("./texture/PMSprites.png")){}
    RenderWindow window(VideoMode(winW, winH), "PAC sem MAN");
    // window.setActive(false);
    starter = new Starter(&window , &sprTexture);
    delete starter;
    return 0;
}
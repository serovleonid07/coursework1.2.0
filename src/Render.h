#pragma once
#include <SFML/Graphics.hpp>
#include "GameLogic.h"
#include "Menu.h"

class Render {
private:
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;
    const int margin = 10;
    bool victorySaved;

    sf::Texture backgroundTexture;          // фон для меню и игры
    sf::Texture historyBackgroundTexture;   // фон для истории

    void drawBoard(const GameLogic& game);
    void showHistory();
public:
    Render();
    void run(GameLogic& game);
};
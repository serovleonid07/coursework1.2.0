#include "Render.h"
#include "History.h"
#include <string>
#include <iostream>

Render::Render() : currentState(GameState::MENU), victorySaved(false) {
    window.create(sf::VideoMode({1920, 1080}), "Game application 'Tag'");
    window.setFramerateLimit(60);
    
    if (!font.openFromFile("arial.ttf")) {
        font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
    
    if (!backgroundTexture.loadFromFile("background.png")) {
        std::cerr << "ERROR: no load image background.png" << std::endl;
        // можно задать запасной цвет фона (или просто игнорировать)
    } else {
        std::cout << "Image load, size: " 
                << backgroundTexture.getSize().x << "x" 
                << backgroundTexture.getSize().y << std::endl;
    }
    // Загружаем фон для истории
    if (!historyBackgroundTexture.loadFromFile("backgroundh.png")) {
        std::cerr << "Warning: backgroundh.png not loaded" << std::endl;
    }
}

void Render::drawBoard(const GameLogic& game) {
    window.clear(sf::Color(45, 45, 48));

    sf::Vector2u windowSize = window.getSize();
    float boardSizePx = std::min(windowSize.x, windowSize.y - 100); // место под текст
    float cellSize = boardSizePx / game.getSize();
    float offsetX = (windowSize.x - boardSizePx) / 2.0f;
    float offsetY = (windowSize.y - boardSizePx - 100) / 2.0f;

    for (int i = 0; i < game.getSize() * game.getSize(); ++i) {
        int val = game.getTile(i);
        if (val == 0) continue;

        int row = i / game.getSize();
        int col = i % game.getSize();
        float posX = offsetX + col * cellSize;
        float posY = offsetY + row * cellSize;

        sf::RectangleShape tile(sf::Vector2f(cellSize - margin, cellSize - margin));
        tile.setPosition({posX + margin/2.0f, posY + margin/2.0f});
        tile.setFillColor(sf::Color(0, 122, 204));

        int fontSize = static_cast<int>(cellSize * 0.35f);
        if (fontSize < 16) fontSize = 16;
        sf::Text text(font, std::to_string(val), fontSize);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                        textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({posX + cellSize / 2.0f,
                          posY + cellSize / 2.0f});
        window.draw(tile);
        window.draw(text);
    }

    // Информационная панель
    sf::Text info(font, "Moves: " + std::to_string(game.getMoves()) + "   [ESC] - Level Select", 24);
    info.setPosition({20.0f, static_cast<float>(windowSize.y) - 50.0f});
    window.draw(info);

    if (game.isSolved() && game.getMoves() > 0) {
        sf::Text win(font, "YOU WIN!", 70);
        win.setFillColor(sf::Color::Green);
        sf::FloatRect winRect = win.getLocalBounds();
        win.setPosition({(windowSize.x - winRect.size.x) / 2.0f,
                         (windowSize.y - winRect.size.y) / 2.0f});
        window.draw(win);

        if (!victorySaved) {
            History::addRecord(game.getSize(), game.getMoves());
            victorySaved = true;
        }
    } else {
        victorySaved = false;
    }
}

void Render::showHistory() {
    auto records = History::loadRecords();
    sf::Vector2u windowSize = window.getSize();

    window.clear();

    // Рисуем фон для истории, если текстура загружена
    if (historyBackgroundTexture.getSize().x != 0) {
        sf::Sprite bgSprite(historyBackgroundTexture);
        bgSprite.setScale(sf::Vector2f(
            static_cast<float>(windowSize.x) / historyBackgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / historyBackgroundTexture.getSize().y
        ));
        window.draw(bgSprite);
    }

    // Заголовок
    sf::Text title(font, "GAME HISTORY (latest first 15)", 30);
    title.setFillColor(sf::Color::Red);
    sf::FloatRect titleRect = title.getLocalBounds();
    title.setPosition({(windowSize.x - titleRect.size.x) / 2.0f, 30.0f});
    window.draw(title);

    // Список записей
    int y = 100;
    int lineHeight = 35;
    for (size_t i = 0; i < records.size() && i < 20; ++i) {
        sf::Text line(font, History::formatRecord(records[i]), 18);
        line.setPosition({50.0f, static_cast<float>(y)});
        line.setFillColor(sf::Color::White);
        window.draw(line);
        y += lineHeight;
    }

    // Подсказка
    sf::Text hint(font, "Press any key or ESC to return", 20);
    hint.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect hintRect = hint.getLocalBounds();
    hint.setPosition({(windowSize.x - hintRect.size.x) / 2.0f,
                      static_cast<float>(windowSize.y) - 80.0f});
    window.draw(hint);
    window.display();

    // Ожидание нажатия
    while (true) {
        auto optEvent = window.waitEvent();
        if (optEvent) {
            auto& ev = *optEvent;
            if (ev.is<sf::Event::Closed>()) {
                window.close();
                break;
            }
            if (ev.is<sf::Event::KeyPressed>() || ev.is<sf::Event::MouseButtonPressed>()) {
                break;
            }
        }
    }
}

void Render::run(GameLogic& game) {
    sf::Vector2u winSize = window.getSize();
    Menu menu(static_cast<float>(winSize.x), static_cast<float>(winSize.y), font);

    while (window.isOpen()) {
        while (auto optEvent = window.pollEvent()) {
            auto& event = *optEvent;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // MENU
            if (currentState == GameState::MENU) {
                if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scan::Up) menu.moveUp(currentState);
                    if (key->scancode == sf::Keyboard::Scan::Down) menu.moveDown(currentState);
                    if (key->scancode == sf::Keyboard::Scan::Enter) {
                        int choice = menu.getPressedItem();
                        if (choice == 0) { currentState = GameState::DIFFICULTY_SELECT; menu.resetSelection(); }
                        else if (choice == 1) { currentState = GameState::HISTORY; }
                        else if (choice == 2) { currentState = GameState::ABOUT; }
                        else if (choice == 3) { window.close(); }
                    }
                }
                if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        int click = menu.getClickedItem(mousePos, currentState);
                        if (click == 0) { currentState = GameState::DIFFICULTY_SELECT; menu.resetSelection(); }
                        else if (click == 1) { currentState = GameState::HISTORY; }
                        else if (click == 2) { currentState = GameState::ABOUT; }
                        else if (click == 3) { window.close(); }
                    }
                }
            }
            // DIFFICULTY SELECT
            else if (currentState == GameState::DIFFICULTY_SELECT) {
                if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scan::Up) menu.moveUp(currentState);
                    if (key->scancode == sf::Keyboard::Scan::Down) menu.moveDown(currentState);
                    if (key->scancode == sf::Keyboard::Scan::Enter) {
                        int choice = menu.getPressedItem();
                        if (choice == 0) { game.init(3); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (choice == 1) { game.init(4); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (choice == 2) { game.init(5); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (choice == 3) { currentState = GameState::MENU; menu.resetSelection(); }
                    }
                }
                if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        int click = menu.getClickedItem(mousePos, currentState);
                        if (click == 0) { game.init(3); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (click == 1) { game.init(4); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (click == 2) { game.init(5); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (click == 3) { currentState = GameState::MENU; menu.resetSelection(); }
                    }
                }
            }
            // PLAYING
            else if (currentState == GameState::PLAYING) {
                if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2u winSize = window.getSize();
                        float boardSizePx = std::min(winSize.x, winSize.y - 100);
                        float cellSize = boardSizePx / game.getSize();
                        float offsetX = (winSize.x - boardSizePx) / 2.0f;
                        float offsetY = (winSize.y - boardSizePx - 100) / 2.0f;
                        int col = static_cast<int>((mouse->position.x - offsetX) / cellSize);
                        int row = static_cast<int>((mouse->position.y - offsetY) / cellSize);
                        if (col >= 0 && col < game.getSize() && row >= 0 && row < game.getSize())
                            game.move(row * game.getSize() + col);
                    }
                }
                if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scan::Escape) {
                        currentState = GameState::DIFFICULTY_SELECT;
                        menu.resetSelection();
                        victorySaved = false;
                    }
                }
            }
            // HISTORY
            else if (currentState == GameState::HISTORY) {
                if (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>()) {
                    currentState = GameState::MENU;
                }
            }
            // ABOUT
            else if (currentState == GameState::ABOUT) {
                if (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>()) {
                    currentState = GameState::MENU;
                }
            }
        }

        // Отрисовка
        window.clear(sf::Color(45, 45, 48));

        // Рисуем фон, если текстура загружена
        if (backgroundTexture.getSize().x != 0) {
            sf::Sprite bgSprite(backgroundTexture);
            sf::Vector2u winSize = window.getSize();
            bgSprite.setScale(sf::Vector2f(
                static_cast<float>(winSize.x) / backgroundTexture.getSize().x,
                static_cast<float>(winSize.y) / backgroundTexture.getSize().y
            ));
            window.draw(bgSprite);
        }

        if (currentState == GameState::MENU || currentState == GameState::DIFFICULTY_SELECT) {
            menu.draw(window, currentState);
        } else if (currentState == GameState::PLAYING) {
            drawBoard(game);
        } else if (currentState == GameState::HISTORY) {
            showHistory();
            currentState = GameState::MENU;
            continue;
        } else if (currentState == GameState::ABOUT) {
            sf::Text about(font, "Coursework: Game application 'Tag'\nCompleted: Serov Leonid Aleksandrovich\nGroup: 25-IVT-2-1\nScientific supervisor: Senior lecturer at the Department of 'VST'\n Matinov Dmitry Sergeevich\n\nCity: Nigniy Novgorod\nYear of writing: 2026\n\nPress any key...", 28);
            about.setFillColor(sf::Color::Blue);
            sf::FloatRect aboutRect = about.getLocalBounds();
            about.setPosition({(winSize.x - aboutRect.size.x) / 2.0f,
                            (winSize.y - aboutRect.size.y) / 2.0f});
            window.draw(about);
        }
        window.display();
    }
}
#include "Render.h"
#include "History.h"
#include <string>
#include <iostream>

Render::Render() : currentState(GameState::MENU), victorySaved(false) {
    window.create(sf::VideoMode({450, 600}), "15 Puzzle - Coursework");
    window.setFramerateLimit(60);
    font.openFromFile("C:/Windows/Fonts/arial.ttf");
}

void Render::drawBoard(const GameLogic& game) {
    window.clear(sf::Color(45, 45, 48));

    float currentTileSize = 450.0f / game.getSize();

    for (int i = 0; i < game.getSize() * game.getSize(); ++i) {
        int val = game.getTile(i);
        if (val == 0) continue;

        sf::RectangleShape tile({currentTileSize - margin, currentTileSize - margin});
        float posX = static_cast<float>(i % game.getSize()) * currentTileSize + margin / 2.0f;
        float posY = static_cast<float>(i / game.getSize()) * currentTileSize + margin / 2.0f;
        tile.setPosition({posX, posY});
        tile.setFillColor(sf::Color(0, 122, 204));

        int fontSize = (game.getSize() >= 5) ? 24 : 36;
        sf::Text text(font, std::to_string(val), fontSize);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                        textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({posX + (currentTileSize - margin) / 2.0f,
                          posY + (currentTileSize - margin) / 2.0f});

        window.draw(tile);
        window.draw(text);
    }

    sf::Text info(font, "Moves: " + std::to_string(game.getMoves()) + "   [ESC] - Level Select", 20);
    info.setPosition({20.0f, 500.0f});
    window.draw(info);

    if (game.isSolved() && game.getMoves() > 0) {
        sf::Text win(font, "YOU WIN!", 50);
        win.setFillColor(sf::Color::Green);
        win.setPosition({110.0f, 220.0f});
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
    sf::Text title(font, "GAME HISTORY (latest first)", 26);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({60, 30});

    window.clear(sf::Color(45, 45, 48));
    window.draw(title);

    int y = 100;
    int lineHeight = 30;
    for (size_t i = 0; i < records.size() && i < 15; ++i) {
        sf::Text line(font, History::formatRecord(records[i]), 16);
        line.setPosition({20, static_cast<float>(y)});
        line.setFillColor(sf::Color::White);
        window.draw(line);
        y += lineHeight;
    }
    sf::Text hint(font, "Press any key or ESC to return", 16);
    hint.setFillColor(sf::Color(180,180,180));
    hint.setPosition({70, 550});
    window.draw(hint);
    window.display();

    // Ждём события (клавиша или мышь) для выхода
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
    Menu menu(450.0f, 550.0f, font);

    while (window.isOpen()) {
        // Обработка событий
        while (auto optEvent = window.pollEvent()) {
            auto& event = *optEvent;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // Состояние MENU
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
            // Состояние DIFFICULTY_SELECT
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
            // Состояние PLAYING
            else if (currentState == GameState::PLAYING) {
                if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        float curTileSize = 450.0f / game.getSize();
                        int x = static_cast<int>(mouse->position.x / curTileSize);
                        int y = static_cast<int>(mouse->position.y / curTileSize);
                        if (x < game.getSize() && y < game.getSize())
                            game.move(y * game.getSize() + x);
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
            // Состояние HISTORY
            else if (currentState == GameState::HISTORY) {
                if (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>()) {
                    currentState = GameState::MENU;
                }
            }
            // Состояние ABOUT
            else if (currentState == GameState::ABOUT) {
                if (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>()) {
                    currentState = GameState::MENU;
                }
            }
        }

        // Отрисовка
        window.clear(sf::Color(45, 45, 48));
        if (currentState == GameState::MENU || currentState == GameState::DIFFICULTY_SELECT) {
            menu.draw(window, currentState);
        } else if (currentState == GameState::PLAYING) {
            drawBoard(game);
        } else if (currentState == GameState::HISTORY) {
            showHistory();
            currentState = GameState::MENU;
            continue;
        } else if (currentState == GameState::ABOUT) {
            sf::Text about(font, "Coursework: 15 Puzzle\nAuthor: Serov L.A.\nGroup 25-IVT-2-1\n\nPress any key...", 24);
            about.setPosition({50, 150});
            window.draw(about);
        }
        window.display();
    }
}
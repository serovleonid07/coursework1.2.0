#include "Render.h"
#include "History.h"
#include <string>
#include <iostream>

Render::Render() : currentState(GameState::MENU), victorySaved(false) {
    window.create(sf::VideoMode(450, 600), "Пятнашки - Курсовая работа");
    window.setFramerateLimit(60);
    if (!font.loadFromFile("arial.ttf")) {
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }
}

void Render::drawBoard(const GameLogic& game) {
    window.clear(sf::Color(45, 45, 48));

    float currentTileSize = 450.0f / game.getSize();

    for (int i = 0; i < game.getSize() * game.getSize(); ++i) {
        int val = game.getTile(i);
        if (val == 0) continue;

        sf::RectangleShape tile(sf::Vector2f(currentTileSize - margin, currentTileSize - margin));
        float posX = static_cast<float>(i % game.getSize()) * currentTileSize + margin / 2.0f;
        float posY = static_cast<float>(i / game.getSize()) * currentTileSize + margin / 2.0f;
        tile.setPosition(posX, posY);
        tile.setFillColor(sf::Color(0, 122, 204));

        int fontSize = (game.getSize() >= 5) ? 24 : 36;
        sf::Text text(std::to_string(val), font, fontSize);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(posX + (currentTileSize - margin) / 2.0f, posY + (currentTileSize - margin) / 2.0f);
        window.draw(tile);
        window.draw(text);
    }

    sf::Text info("Ходы: " + std::to_string(game.getMoves()) + "   [ESC] - выбор уровня", font, 20);
    info.setPosition(20.0f, 500.0f);
    window.draw(info);

    if (game.isSolved() && game.getMoves() > 0) {
        sf::Text win("ВЫ ПОБЕДИЛИ!", font, 50);
        win.setFillColor(sf::Color::Green);
        win.setPosition(110.0f, 220.0f);
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
    sf::Text title("ИСТОРИЯ ИГР (последние сверху)", font, 26);
    title.setPosition(60, 30);
    title.setFillColor(sf::Color::Yellow);

    window.clear(sf::Color(45, 45, 48));
    window.draw(title);

    int y = 100;
    int lineHeight = 30;
    for (size_t i = 0; i < records.size() && i < 15; ++i) {
        sf::Text line(History::formatRecord(records[i]), font, 16);
        line.setPosition(20, y);
        line.setFillColor(sf::Color::White);
        window.draw(line);
        y += lineHeight;
    }
    sf::Text hint("Нажмите любую клавишу или ESC для возврата", font, 16);
    hint.setPosition(70, 550);
    hint.setFillColor(sf::Color(180,180,180));
    window.draw(hint);
    window.display();

    sf::Event ev;
    while (true) {
        window.waitEvent(ev);
        if (ev.type == sf::Event::KeyPressed || ev.type == sf::Event::MouseButtonPressed) {
            break;
        }
        if (ev.type == sf::Event::Closed) {
            window.close();
            break;
        }
    }
}

void Render::run(GameLogic& game) {
    Menu menu(450.0f, 550.0f, font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (currentState == GameState::MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) menu.moveUp(currentState);
                    if (event.key.code == sf::Keyboard::Down) menu.moveDown(currentState);
                    if (event.key.code == sf::Keyboard::Enter) {
                        int choice = menu.getPressedItem();
                        if (choice == 0) { currentState = GameState::DIFFICULTY_SELECT; menu.resetSelection(); }
                        else if (choice == 1) { currentState = GameState::HISTORY; }
                        else if (choice == 2) { currentState = GameState::ABOUT; }
                        else if (choice == 3) { window.close(); }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int click = menu.getClickedItem(mousePos, currentState);
                    if (click == 0) { currentState = GameState::DIFFICULTY_SELECT; menu.resetSelection(); }
                    else if (click == 1) { currentState = GameState::HISTORY; }
                    else if (click == 2) { currentState = GameState::ABOUT; }
                    else if (click == 3) { window.close(); }
                }
            }
            else if (currentState == GameState::DIFFICULTY_SELECT) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) menu.moveUp(currentState);
                    if (event.key.code == sf::Keyboard::Down) menu.moveDown(currentState);
                    if (event.key.code == sf::Keyboard::Enter) {
                        int choice = menu.getPressedItem();
                        if (choice == 0) { game.init(3); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (choice == 1) { game.init(4); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (choice == 2) { game.init(5); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                        else if (choice == 3) { currentState = GameState::MENU; menu.resetSelection(); }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int click = menu.getClickedItem(mousePos, currentState);
                    if (click == 0) { game.init(3); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                    else if (click == 1) { game.init(4); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                    else if (click == 2) { game.init(5); game.shuffle(); currentState = GameState::PLAYING; victorySaved = false; }
                    else if (click == 3) { currentState = GameState::MENU; menu.resetSelection(); }
                }
            }
            else if (currentState == GameState::PLAYING) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    float curTileSize = 450.0f / game.getSize();
                    int x = static_cast<int>(event.mouseButton.x / curTileSize);
                    int y = static_cast<int>(event.mouseButton.y / curTileSize);
                    if (x < game.getSize() && y < game.getSize()) game.move(y * game.getSize() + x);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::DIFFICULTY_SELECT;
                    menu.resetSelection();
                    victorySaved = false;
                }
            }
            else if (currentState == GameState::HISTORY) {
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                    currentState = GameState::MENU;
                }
            }
            else if (currentState == GameState::ABOUT) {
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                    currentState = GameState::MENU;
                }
            }
        }

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
            sf::Text about("Курсовая работа: Пятнашки\nАвтор: Серов Л.А.\nГруппа 25-ИВТ-2-1\n\nНажмите любую клавишу...", font, 24);
            about.setPosition(50, 150);
            window.draw(about);
        }
        window.display();
    }
}
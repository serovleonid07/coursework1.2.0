#include "Menu.h"

Menu::Menu(float width, float height, sf::Font& sharedFont) : font(sharedFont) {
    selectedItemIndex = 0;

    std::vector<std::string> mainNames = { "Start Game", "History", "About", "Exit" };
    std::vector<std::string> diffNames = { "3 x 3 (Easy)", "4 x 4 (Normal)", "5 x 5 (Hard)", "Back" };

    auto createButtons = [&](std::vector<std::string>& names, std::vector<sf::Text>& items) {
        for (int i = 0; i < (int)names.size(); i++) {
            sf::Text text(font, names[i], 40);
            text.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White);
            sf::FloatRect tr = text.getLocalBounds();
            text.setOrigin({tr.position.x + tr.size.x / 2.0f,
                            tr.position.y + tr.size.y / 2.0f});
            text.setPosition({width / 2.0f, height / (names.size() + 1) * (i + 1)});
            items.push_back(text);
        }
    };

    createButtons(mainNames, mainMenuItems);
    createButtons(diffNames, difficultyItems);
}

void Menu::draw(sf::RenderWindow& window, GameState state) {
    auto& items = (state == GameState::MENU) ? mainMenuItems : difficultyItems;
    for (auto& item : items) window.draw(item);
}

int Menu::getClickedItem(sf::Vector2f mousePos, GameState state) {
    auto& items = (state == GameState::MENU) ? mainMenuItems : difficultyItems;
    for (int i = 0; i < (int)items.size(); i++) {
        if (items[i].getGlobalBounds().contains(mousePos)) return i;
    }
    return -1;
}

void Menu::moveUp(GameState state) {
    auto& items = (state == GameState::MENU) ? mainMenuItems : difficultyItems;
    if (selectedItemIndex - 1 >= 0) {
        items[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        items[selectedItemIndex].setFillColor(sf::Color::Yellow);
    }
}

void Menu::moveDown(GameState state) {
    auto& items = (state == GameState::MENU) ? mainMenuItems : difficultyItems;
    if (selectedItemIndex + 1 < (int)items.size()) {
        items[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        items[selectedItemIndex].setFillColor(sf::Color::Yellow);
    }
}
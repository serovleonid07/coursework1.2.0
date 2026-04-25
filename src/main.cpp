#include "GameLogic.h"
#include "Render.h"
#include <iostream>

int main() {
    try {
        GameLogic game(4);
        Render ui;
        ui.run(game);
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Критическая ошибка: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Неизвестная ошибка." << std::endl;
        return 2;
    }
}
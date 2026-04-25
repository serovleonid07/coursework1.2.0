#include "GameLogic.h"
#include <algorithm>
#include <random>

GameLogic::GameLogic(int boardSize) : size(boardSize), moves(0) {
    field.resize(size * size);
    for (int i = 0; i < size * size; ++i) field[i] = (i + 1) % (size * size);
    emptyPos = size * size - 1;
}

bool GameLogic::isSolvable(const std::vector<int>& p) const {
    int inversions = 0;
    for (int i = 0; i < p.size(); ++i) {
        if (p[i] == 0) continue;
        for (int j = i + 1; j < p.size(); ++j) {
            if (p[j] != 0 && p[i] > p[j]) inversions++;
        }
    }
    if (size % 2 != 0) return inversions % 2 == 0;
    int row = emptyPos / size + 1;
    return (inversions + row) % 2 == 0;
}

void GameLogic::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());

    do {
        std::shuffle(field.begin(), field.end(), g);
        for (int i = 0; i < (int)field.size(); i++) {
            if (field[i] == 0) emptyPos = i;
        }
    } while (!isSolvable(field) || isSolved());
    moves = 0;
}

bool GameLogic::move(int pos) {
    int row = pos / size, col = pos % size;
    int emptyRow = emptyPos / size, emptyCol = emptyPos % size;

    if (std::abs(row - emptyRow) + std::abs(col - emptyCol) == 1) {
        std::swap(field[pos], field[emptyPos]);
        emptyPos = pos;
        moves++;
        return true;
    }
    return false;
}

bool GameLogic::isSolved() const {
    for (int i = 0; i < field.size() - 1; ++i) {
        if (field[i] != i + 1) return false;
    }
    return true;
}

void GameLogic::init(int newSize) {
    size = newSize;
    field.clear();
    field.resize(size * size);
    for (int i = 0; i < size * size; ++i) {
        field[i] = (i + 1) % (size * size);
    }
    emptyPos = size * size - 1;
    moves = 0;
}
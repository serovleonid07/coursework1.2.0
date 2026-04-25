#pragma once
#include <vector>

class GameLogic {
private:
    int size;
    std::vector<int> field;
    int emptyPos;
    int moves;
public:
    GameLogic(int boardSize = 4);
    void init(int newSize);
    void shuffle();
    bool move(int pos);
    bool isSolved() const;
    bool isSolvable(const std::vector<int>& p) const;
    int getTile(int index) const { return field[index]; }
    int getSize() const { return size; }
    int getMoves() const { return moves; }
    void resetMoves() { moves = 0; }
};
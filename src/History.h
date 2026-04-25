#pragma once
#include <string>
#include <vector>

struct GameRecord {
    std::string dateTime;
    int boardSize;
    int moves;
};

class History {
public:
    static void addRecord(int size, int moves);
    static std::vector<GameRecord> loadRecords();
    static void clearHistory();
    static std::string formatRecord(const GameRecord& rec);
};
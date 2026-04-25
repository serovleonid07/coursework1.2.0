#include "History.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

const std::string HISTORY_FILE = "history.txt";

static std::string getCurrentDateTime() {
    auto now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void History::addRecord(int size, int moves) {
    std::ofstream out(HISTORY_FILE, std::ios::app);
    if (!out) {
        std::cerr << "Error: cannot open history file for writing.\n";
        return;
    }
    out << getCurrentDateTime() << "|" << size << "|" << moves << "\n";
}

std::vector<GameRecord> History::loadRecords() {
    std::vector<GameRecord> records;
    std::ifstream in(HISTORY_FILE);
    if (!in) return records;
    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        GameRecord rec;
        std::string token;
        if (std::getline(ss, token, '|')) rec.dateTime = token;
        if (std::getline(ss, token, '|')) rec.boardSize = std::stoi(token);
        if (std::getline(ss, token, '|')) rec.moves = std::stoi(token);
        records.push_back(rec);
    }
    std::reverse(records.begin(), records.end());
    return records;
}

std::string History::formatRecord(const GameRecord& rec) {
    return rec.dateTime + "  |  Board " + std::to_string(rec.boardSize) + "x" + std::to_string(rec.boardSize) +
           "  |  Moves: " + std::to_string(rec.moves);
}

void History::clearHistory() {
    std::ofstream out(HISTORY_FILE, std::ios::trunc);
    out.close();
}
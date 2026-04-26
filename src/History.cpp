#include "History.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

const std::string HISTORY_FILE = "C:/Users/user/coursework1.2.0/history.txt";

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
        std::cerr << "ERROR: Cannot open " << HISTORY_FILE << " for writing." << std::endl;
        return;
    }
    out << getCurrentDateTime() << "|" << size << "|" << moves << "\n";
    out.close();
    std::cout << "Saved: " << size << "x" << size << " moves=" << moves << std::endl;
}

std::vector<GameRecord> History::loadRecords() {
    std::vector<GameRecord> records;
    std::ifstream in(HISTORY_FILE);
    if (!in) {
        std::cout << "No history file yet." << std::endl;
        return records;
    }
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
    in.close();
    std::reverse(records.begin(), records.end()); // последние сверху
    std::cout << "Loaded " << records.size() << " records." << std::endl;
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
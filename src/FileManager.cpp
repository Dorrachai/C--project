#include "../include/FileManager.h"
#include <fstream>
#include <sstream>

void FileManager::saveGameData(const std::string& data) {
    std::ofstream file("data/game_data.txt");
    if (file.is_open()) {
        file << data;
        file.close();
    }
}

std::string FileManager::loadGameData() {
    std::ifstream file("data/game_data.txt");
    std::string data;
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        data = buffer.str();
        file.close();
    }
    return data;
}

void FileManager::savePlayerStats(const std::vector<std::string>& stats) {
    std::ofstream file("data/player_stats.txt");
    if (file.is_open()) {
        for (const auto& line : stats) {
            file << line << "\n";
        }
        file.close();
    }
}

std::vector<std::string> FileManager::loadPlayerStats() {
    std::vector<std::string> stats;
    std::ifstream file("data/player_stats.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            stats.push_back(line);
        }
        file.close();
    }
    return stats;
}

int FileManager::loadTotalWins(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return 0;
    }
    int val = 0;
    in >> val;
    return (in.fail() ? 0 : val);
}

void FileManager::saveTotalWins(int totalWins, const std::string& path) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        return;
    }
    out << totalWins << "\n";
}

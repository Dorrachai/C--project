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
        std::string line;
        while (std::getline(file, line)) {
            data += line + "\n";
        }
        file.close();
    }
    return data;
}

void FileManager::savePlayerStats(const std::vector<std::string>& stats) {
    std::ofstream file("data/player_stats.txt");
    if (file.is_open()) {
        for (const std::string& stat : stats) {
            file << stat << std::endl;
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

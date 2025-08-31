#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

class FileManager {
public:
    void saveGameData(const std::string& data);
    std::string loadGameData();
    void savePlayerStats(const std::vector<std::string>& stats);
    std::vector<std::string> loadPlayerStats();

    int loadTotalWins(const std::string& path = "data/total_wins.txt");
    void saveTotalWins(int totalWins, const std::string& path = "data/total_wins.txt");
};

#endif

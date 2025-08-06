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
};

#endif

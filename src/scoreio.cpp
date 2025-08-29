
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "scoreio.h"


namespace fs = std::filesystem;


namespace ScoreIO{

    const fs::path PATH = fs::path(PROJECT_ROOT_PATH) / "assets" / ".scores.txt";

    std::vector<Entry> load_scores(){

        if (!fs::exists(PATH)) {
            // Create an empty file
            std::ofstream file(PATH);
            if (!file) {
                    std::cerr << "Failed to create score file at: " << PATH << "\n";
            } else {
                return {};
            }
        } 

        std::ifstream file(PATH);
        std::vector<Entry> entries;
        
        if (!file.is_open()) {
            std::cerr << "Could not open score file: " << PATH << std::endl;
            return {};
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            Entry entry;
            if (iss >> entry.name >> entry.score) {
                entries.push_back(entry);
            }
            else {
                std::cerr << "Warning: failed to parse line: " << line << std::endl;
            }
        }
        return entries;
    }

    void save_score(const Entry& new_entry){

        std::vector<Entry> entries = load_scores();
        entries.push_back(new_entry);
        std::sort(entries.begin(), entries.end(), std::greater<Entry>());
        if (entries.size() > 20) {
            entries.resize(20); // Keep top 20 scores
        }

        // Save back to file (overwrite)
        std::ofstream file(PATH, std::ios::trunc);  // `trunc` clears the file
        if (!file) {
            std::cerr << "Failed to open score file for writing.\n";
            return;
        }

        for (const Entry& entry : entries) {
            file << entry.name << " " << entry.score << '\n';
        }
    }

    void print_scores() {
        std::vector<Entry> entries = load_scores();
        std::cout << "---- High Scores ----\n";
        for (const Entry& entry : entries) {
            std::cout << entry.name << ": " << entry.score << '\n';
        }
        std::cout << "---------------------\n";
    }
}


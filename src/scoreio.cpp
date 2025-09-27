
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "scoreio.h"
#include "utils.h"


namespace fs = std::filesystem;


namespace ScoreIO{

    const fs::path PATH = fs::path(PROJECT_ROOT_PATH) / "assets" / "scores.txt";

    std::vector<std::vector<std::string>> load_scores(){

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
        std::vector<std::vector<std::string>> scores;
        
        if (!file.is_open()) {
            std::cerr << "Could not open score file: " << PATH << std::endl;
            return {};
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::vector<std::string> entry = {"null","null"}; // if null displays, know there was parsing error
            if (iss >> entry[0] >> entry[1]) {
                StringUtils::replace(entry[0], "_", " "); // reverse the underscore replacement
                scores.push_back(entry);
            }
            else {
                std::cerr << "Warning: failed to parse line: " << line << std::endl;
            }
        }
        std::cout << "Loaded " << scores.size() << " scores from file." << scores[0][0] << std::endl; //debug
        return scores;
    }

    // Overload to handle struct Entry
    std::vector<Entry> load_entries(){

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

    void save_score(Entry& new_entry){
        // replace spaces with underscores for file storage
        StringUtils::replace(new_entry.name, " ", "_");
        std::vector<Entry> entries = load_entries();
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
        std::vector<Entry> entries = load_entries();
        std::cout << "---- High Scores ----\n";
        for (const Entry& entry : entries) {
            std::cout << entry.name << ": " << entry.score << '\n';
        }
        std::cout << "---------------------\n";
    }
}


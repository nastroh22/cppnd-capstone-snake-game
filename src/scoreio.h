#ifndef SCOREIO_H
#define SCOREIO_H
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace ScoreIO {
    struct Entry {
        std::string name;
        int score;

        bool operator>(const Entry& other) const {
            return score > other.score; // For descending order
        }
    };
    struct DisplayEntry {
        std::string name;
        std::string score;
    };
    extern const fs::path PATH; 
    std::vector<std::vector<std::string>> load_scores();
    std::vector<Entry> load_entries();
    void save_score(Entry& new_entry);
    void print_scores();
}
#endif
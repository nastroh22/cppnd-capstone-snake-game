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
    extern const fs::path PATH; 
    std::vector<Entry> load_scores();
    void save_score(const Entry& new_entry);
    void print_scores();
}
#endif
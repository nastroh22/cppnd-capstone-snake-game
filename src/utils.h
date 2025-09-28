#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <thread>
#include <optional>
#include <atomic>
#include <memory>

#include "SDL.h"


/*********************  SDL Render Helpers *******************************************/
namespace RenderUtils{

    inline SDL_Texture* InitTexture(SDL_Renderer* const renderer, const std::string& path) {
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if (!surface) {
            std::cerr << "Failed to load BMP file: " << path  << " " << SDL_GetError() << std::endl;
            return nullptr;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cerr << "Failed to create texture: " <<  path  << " " << SDL_GetError() << std::endl;
            // Handle error or set to nullptr
        }
        else {
            std::cout << "Loaded Texture from " << path << std::endl;
        }
        SDL_FreeSurface(surface);
        return texture;
    };

    //minimal representation of all game items that Renderer can parse to render
    struct Item {std::string name; int x; int y;};

    // Try this approach of reading in textures directly from a map of key/filepath pairs
    template<typename T>
    inline std::unordered_map<T, SDL_Texture*>
    loadTexturesFromMap(SDL_Renderer* renderer, const std::unordered_map<T, std::string>& files) {
        std::unordered_map<T, SDL_Texture*> textures;

        for (const auto& [key, path] : files) {
            SDL_Texture* tex = InitTexture(renderer, path);
            if (tex) {
                textures[key] = tex;
            } else {
                std::cerr << "Texture load failed for '" << key << "' from path: " << path << "\n";
            }
        }
        return textures;
    }
    template<typename KeyType>
    inline void freeTextureMap(std::unordered_map<KeyType,SDL_Texture*> &texture_map) {
        for (auto& [key, texture] : texture_map) {
            if (texture) {
                SDL_DestroyTexture(texture);
                texture = nullptr; // avoid dangling pointer
            }
        }
        texture_map.clear();
    }
    template<size_t N>
    inline std::array<SDL_Texture*, N>
    loadTexturesFromArray(SDL_Renderer* renderer, const std::array<std::string, N>& files) {
        std::array<SDL_Texture*, N> textures = {nullptr};

        for (size_t i = 0; i < files.size(); ++i) {
            SDL_Texture* tex = InitTexture(renderer, files[i]);
            if (tex) {
                textures[i] = tex;
            } else {
                std::cerr << "Texture load failed for index " << i << " from path: " << files[i] << "\n";
            }
        }
        return textures;
    }
    template<size_t N>
    inline void freeTextureArray(std::array<SDL_Texture*, N>& textures) {
        for (auto& texture : textures) {
            if (texture) {
                SDL_DestroyTexture(texture);
                std::cout<<" Freed texture"<<std::endl;
                texture = nullptr; // avoid dangling pointer
            }
        }
    }

    inline void drawBorder(SDL_Renderer* renderer, SDL_Rect rect, int thickness, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        // Top
        SDL_Rect top = {rect.x, rect.y, rect.w, thickness};
        SDL_RenderFillRect(renderer, &top);

        // Bottom
        SDL_Rect bottom = {rect.x, rect.y + rect.h - thickness, rect.w, thickness};
        SDL_RenderFillRect(renderer, &bottom);

        // Left
        SDL_Rect left = {rect.x, rect.y, thickness, rect.h};
        SDL_RenderFillRect(renderer, &left);

        // Right
        SDL_Rect right = {rect.x + rect.w - thickness, rect.y, thickness, rect.h};
        SDL_RenderFillRect(renderer, &right);
    }
}

/*********************  String Helpers  *******************************************/
namespace StringUtils{
    static void replace(std::string& str, const std::string& from, const std::string& to) {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
    }

    static std::string strip(const std::string& str) {
        size_t start = 0;
        size_t end = str.length();

        // Find first non-whitespace character
        while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
            ++start;
        }

        // Find last non-whitespace character
        while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
            --end;
        }
        return str.substr(start, end - start);
    }
}

/*********************  Score Utils *******************************************/
namespace ScoreIO{
    
    namespace fs = std::filesystem;
    const fs::path PATH = fs::path(PROJECT_ROOT_PATH) / "assets" / "scores.txt";

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

    //load score "vectors"
    inline std::vector<std::vector<std::string>> load_scores(){

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
    inline std::vector<Entry> load_entries(){

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

    inline void save_score(Entry& new_entry){
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

    inline void print_scores() {
        std::vector<Entry> entries = load_entries();
        std::cout << "---- High Scores ----\n";
        for (const Entry& entry : entries) {
            std::cout << entry.name << ": " << entry.score << '\n';
        }
        std::cout << "---------------------\n";
    }
}

/* 
    MessageQueue Def 
******************************************************************************************
*/
template <typename T>
class MessageQueue {
public:
    MessageQueue(std::shared_ptr<std::atomic<bool>> shutdown_flag) : _shutdown_flag(shutdown_flag) {};
    ~MessageQueue() = default;

    // feels more intuitive to actually call this "receive" since it is called by the consumer
    std::optional<T> send() {
        std::unique_lock<std::mutex> lock(_mutex);
        _cond1.wait(lock, [this] { 
            return (!_messages.empty() || _shutdown_flag->load());}); // wait until not empty or shutdown is true
        if (_shutdown_flag->load()){
            std::cout << "Shutdown in send q received? " << std::endl;
            _cond2.notify_all();
            return std::nullopt;
        }
        T msg = std::move(_messages.back());
        _messages.pop_back();
        _cond2.notify_one();
        return msg;
    };

    void receive(T&& msg) {
        std::unique_lock<std::mutex> lock(_mutex);
        _cond2.wait(lock, [this] { 
            return (_messages.size() <= _maxSize || _shutdown_flag->load());});
        if (_shutdown_flag->load()){
            std::cout << "Shutdown in receive q received? " << std::endl;
            _cond1.notify_all(); // (Found bug) No need for it to be empty, just notify all to shutdown
            return;
        }
        _messages.push_front(std::move(msg));
        _cond1.notify_one(); // notify client that a new message is available
    };

    void size(){
        std::lock_guard<std::mutex> lock(_mutex);
        std::cout << "Queue Size: " << _messages.size() << std::endl;
    };

    void clear(){
        std::lock_guard<std::mutex> lock(_mutex);
        _messages.clear();
    };

    void shutdown(){
        // std::lock_guard<std::mutex> lock(_mutex);
        // _shutdown_flag->store(true); // redundant I think 
        _messages.clear();
        _cond1.notify_all();
        _cond2.notify_all();
    };

private:
    std::mutex _mutex;
    std::deque<T> _messages;
    std::condition_variable _cond1;
    std::condition_variable _cond2;
    size_t _maxSize = 5;
    std::shared_ptr<std::atomic<bool>> _shutdown_flag;

};

#endif 


/* 
---- More Utils to Consider ------
// Probabluy Keep AppendRoot although ../assets path should be fine
inline std::string appendRoot(std::string path){
    return std::string(PROJECT_ROOT_PATH) + path;
}
*/
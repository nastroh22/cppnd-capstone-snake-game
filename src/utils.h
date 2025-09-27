#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <iostream>
#include "SDL.h"


// Refactor this to "render_utils" instead (or something) -- probably include text here as well
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

// namespace ScoreUtils{
//     //Todo
//     int *parser = nullptr;
// }

#endif 

/* More Utils to Consider: 


// Probabluy Keep AppendRoot although ../assets path should be fine
inline std::string appendRoot(std::string path){
    return std::string(PROJECT_ROOT_PATH) + path;
}

*/
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "SDL.h"


// Refactor this to "render_utils" instead (or something) -- probably include text here as well
namespace RenderUtils{

    static SDL_Texture* InitTexture(SDL_Renderer* const renderer, const std::string& path) {
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


    // Try this approach of reading in textures directly from a map of key/filepath pairs
   inline std::unordered_map<std::string, SDL_Texture*>
    loadTexturesFromMap(SDL_Renderer* renderer, const std::unordered_map<std::string, std::string>& files) {
        std::unordered_map<std::string, SDL_Texture*> textures;

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


    static void drawBorder(SDL_Renderer* renderer, SDL_Rect rect, int thickness, SDL_Color color) {
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

    // Refactor this to "render_utils" instead (or something) -- probably include text here as well
    class Hawk {
        public:
            Hawk(SDL_Renderer* const renderer) {
                hawk_texture = InitTexture(renderer, "../assets/hawk_000.bmp");
            }
            ~Hawk() {
                if (hawk_texture) SDL_DestroyTexture(hawk_texture);
            }
            SDL_Texture* get() const { return hawk_texture; }

        private:
            SDL_Texture *hawk_texture = nullptr;
    };
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

#endif 

/* More Utils to Consider: 


inline std::string appendRoot(std::string path){
    return std::string(PROJECT_ROOT_PATH) + path;
}

std::unordered_map<CharacterEnum, std::array<std::string, 3>> characterSpriteFiles = {
    {Sammy,  {appendRoot("/assets/snake_green_head.bmp"), 
              appendRoot("/assets/snake_green_blob.bmp"),
              appendRoot("/assets/snake_green_xx.bmp")}
    },
    {Cindy,  {appendRoot("/assets/snake_yellow_head.bmp"), 
              appendRoot("/assets/snake_yellow_blob.bmp"),
              appendRoot("/assets/snake_yellow_xx.bmp")}
    },
};

// these might be specific to snake but can use common utils:

void Snake::InitHeadTexture(SDL_Renderer* renderer, const std::string& path) {
    if (snake_head_texture) SDL_DestroyTexture(snake_head_texture); // free existing resource
    snake_head_texture = InitTexture(renderer, path);
}

void Snake::InitBodyTexture(SDL_Renderer* renderer, const std::string& path) {
    if (snake_body_texture) SDL_DestroyTexture(snake_dies_texture); // free existing resource
    snake_body_texture = InitTexture(renderer, path);
}

void Snake::InitDiesTexture(SDL_Renderer* renderer, const std::string& path) {
    if (snake_dies_texture) SDL_DestroyTexture(snake_dies_texture); // free existing resource
    snake_dies_texture = InitTexture(renderer, path);
}


*/
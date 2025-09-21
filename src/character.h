#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "SDL.h"


// Refactor this to "render_utils" instead (or something) -- probably include text here as well
namespace Characters{

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
            std::cout << "Loaded Character Texture from " << path << std::endl;
        }
        SDL_FreeSurface(surface);
        std::cout << "Texture pointer here : " << texture << std::endl;
        //   int w = 0, h = 0;
        //   SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        //   std::cout << "Raw loaded texture is size: " << w << " x " << h << "\n";
        return texture;
    };

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
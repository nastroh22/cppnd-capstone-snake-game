#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "SDL.h"


namespace Characters{

    SDL_Texture* InitTexture(SDL_Renderer* const renderer, const std::string& path) {
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
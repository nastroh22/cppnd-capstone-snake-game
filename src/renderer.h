#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "snake.h"
#include "food.h"
#include <unordered_map>
// #include <SDL2/SDL_image.h>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake, 
              const Food *food, 
              std::string &render_item,
              const SDL_Point ai_location = SDL_Point{0,0},
              SDL_Texture *ai_texture = nullptr); //old way: SDL_Point const &food
  void UpdateWindowTitle(int score, int fps);
  SDL_Renderer* get() const { return sdl_renderer; } // needed so I can init textures in Food class



 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Texture *sdl_texture;

  // std::vector<SDL_Texture*> food_textures;
  // SDL_Texture *hawk_texture; //enemy texture

  std::unordered_map<std::string, SDL_Texture*> _item_textures;



  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
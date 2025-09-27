#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "snake.h"
#include "food.h"
#include <unordered_map>
#include <array>
// #include <SDL2/SDL_image.h>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake, 
              const Food *food, 
              RenderUtils::Item &item,
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
  std::array<SDL_Texture*, Assets::HAWK_ANIMATION_FRAMES> _hawk_textures; // cycle through map to animate character
  std::array<SDL_Texture*, Assets::HAWK_ANIMATION_FRAMES> _flipped_hawk_textures; // cycle through map to animate character
  std::array<SDL_Texture*, Assets::HAWK_ANIMATION_FRAMES>::iterator _hawk_it; // init in constructor
  int _flap_frame_count = 0;
  bool _flip_hawk_direction = false;
  int _prev_x = 0;


  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
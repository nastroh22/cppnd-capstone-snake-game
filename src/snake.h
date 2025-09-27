#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <unordered_map>
#include <array>

#include "SDL.h"
#include "constants.h"
#include "utils.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  ~Snake(){
    std::cout<<" Freeing Snake Textures"<<std::endl;
    RenderUtils::freeTextureArray(_texture_map);
  };

  //pubic methods
  void Update(SDL_Point const &ai_location);
  void GrowBody();
  bool SnakeCell(int x, int y);
  void InitTextures(SDL_Renderer* renderer, CharacterEnum character) {
      _texture_map = RenderUtils::loadTexturesFromArray(
          renderer,
          characterFileMap.at(character)
      );
  }
  SDL_Texture *get_head_texture() const { return _texture_map[0]; }
  SDL_Texture *get_body_texture() const { return _texture_map[1]; } // TODO - make separate body texture?
  SDL_Texture *get_ko_texture() const { return _texture_map[2]; }
  
  //public attributes
  std::vector<SDL_Point> body;
  Direction direction = Direction::kUp;
  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;

 protected:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, SDL_Point const &ai_location);
  std::array<SDL_Texture*,NUM_CHARACTER_TEXTURES> _texture_map; // head, dies, body

  bool growing{false};
  int grid_width;
  int grid_height;

};

#endif
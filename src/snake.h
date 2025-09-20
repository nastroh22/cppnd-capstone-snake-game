#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include <unordered_map>
#include <array>

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  ~Snake(){
    if (snake_head_texture) SDL_DestroyTexture(snake_head_texture);
    if (snake_body_texture) SDL_DestroyTexture(snake_body_texture);
    if (snake_dies_texture) SDL_DestroyTexture(snake_dies_texture);
  };

  void Update(SDL_Point const &ai_location);

  void GrowBody();
  bool SnakeCell(int x, int y);

  SDL_Texture* InitTexture(SDL_Renderer* renderer, const std::string& path);
  void InitHeadTexture(SDL_Renderer* renderer, const std::string& path);
  void InitBodyTexture(SDL_Renderer* renderer, const std::string& path);
  void InitDiesTexture(SDL_Renderer* renderer, const std::string& path);

  SDL_Texture *get_head_texture() const { return snake_head_texture; }
  SDL_Texture *get_dies_texture() const { return snake_dies_texture; }
  SDL_Texture *get_body_texture() const { return snake_body_texture; } // TODO - make separate body texture?

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 protected:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, SDL_Point const &ai_location);

  SDL_Texture *snake_head_texture = nullptr;
  SDL_Texture *snake_body_texture = nullptr;
  SDL_Texture *snake_dies_texture = nullptr;

  bool growing{false};
  int grid_width;
  int grid_height;
};

// class Sammy : Snake{
//   public:
//       Sammy(int grid_width, int grid_height) : Snake(grid_width, grid_height)
//       {
//           // Initialize textures here, assuming you have access to an SDL_Renderer
//           snake_head_texture = InitTexture(renderer, "../assets/sprites/snake_green_head.bmp";);
//           snake_dies_texture = InitTexture(renderer, "../assets/sprites/snake_green_xx.bmp"); 
//       }; 

//       ~Sammy(){
//           if (snake_head_texture) SDL_DestroyTexture(snake_head_texture);
//           if (snake_dies_texture) SDL_DestroyTexture(snake_dies_texture);
//       };
//   };

// class Cindy : Snake{
//   public:
//       Cindy(int grid_width, int grid_height) : Snake(grid_width, grid_height)
//       {
//           // Initialize textures here, assuming you have access to an SDL_Renderer
//           snake_head_texture = InitTexture(renderer, "../assets/sprites/snake_yellow_head.bmp";);
//           snake_dies_texture = InitTexture(renderer, "../assets/sprites/snake_yellow_xx.bmp"); 
//       }; 

//       ~Cindy(){
//           if (snake_head_texture) SDL_DestroyTexture(snake_head_texture);
//           if (snake_dies_texture) SDL_DestroyTexture(snake_dies_texture);
//       };
//   };

// Character Options
enum CharacterEnum {
    Sammy,
    Cindy
};

extern std::unordered_map<CharacterEnum, std::array<std::string, 3>> characterSpriteFiles;

#endif
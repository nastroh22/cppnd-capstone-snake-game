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

  Snake(int grid_width, int grid_height, std::string name="Sammy")
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {
          std::cout << "Creating Snake Object for " << name <<std::endl;
          // For Safety, init the snake textures with NUll ptrs, user must explicitly call InitTextures
          _texture_map.fill(nullptr);
      }

  ~Snake(){
    std::cout<<" Freeing Snake Textures for " << name <<std::endl;
    RenderUtils::freeTextureArray(_texture_map);
  };

  //pubic methods
  void Update(SDL_Point const &ai_location);
  void SetHead(int x, int y) {
      head_x = static_cast<float>(x);
      head_y = static_cast<float>(y);
  }
  // SDL_Point GetHead() const { return SDL_Point{head_x, head_y}; };
  void GrowBody();
  bool SnakeCell(int x, int y);
  void InitTextures(SDL_Renderer* renderer, CharacterEnum character) {
      _texture_map = RenderUtils::loadTexturesFromArray(
          renderer,
          characterFileMap.at(character)
      );
      std::cout << "Loaded Snake Textures for " << CHARACTER_NAMES[static_cast<int>(character)] << std::endl;
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
  std::string name = "None";

  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, SDL_Point const &ai_location);

  void ChooseDirection(SDL_Point &target_point){
    // Automate control inputs to the snake based on a planner's desired point
    int d_x = target_point.x - head_x;
    int d_y = target_point.y - head_y;

    Direction desired_vertical_move = (d_y > 0) ? Direction::kDown : Direction::kUp;
    Direction desired_horizontal_move = (d_x > 0) ? Direction::kRight : Direction::kLeft;
    Direction opposite_horizontal_move = (desired_horizontal_move == Direction::kRight ) ? Direction::kLeft : Direction::kRight;
    Direction opposite_vertical_move = (desired_vertical_move == Direction::kUp ) ? Direction::kDown : Direction::kUp;
    
    // select greater of two moves
    if (std::abs(d_x) > std::abs(d_y)) {
      // prevent 180 turns
      if (opposite_horizontal_move != desired_horizontal_move){
          direction = desired_horizontal_move;
      }
      else{
          direction = desired_vertical_move;
      }  
    }
    else { 
      // vertical move
      if (desired_vertical_move != opposite_vertical_move){
          direction = desired_vertical_move;
      }
      else{
          direction = desired_horizontal_move;
      }
    }
  }

 protected:
  void UpdateHead();
  // void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, SDL_Point const &ai_location);
  std::array<SDL_Texture*,NUM_CHARACTER_TEXTURES> _texture_map; // head, dies, body

  bool growing{false};
  int grid_width;
  int grid_height;

};

#endif
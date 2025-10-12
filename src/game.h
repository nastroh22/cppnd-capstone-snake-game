#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "utils.h"
#include "enemy/planner.h"
#include "snake.h"
#include "constants.h"

class Game {
 public: // optionally init the Game Object with a copy of queues (TODO?)
  Game(std::size_t grid_width, std::size_t grid_height, std::string characterName); 
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration, 
           MessageQueue<SDL_Point> *pubq,
           MessageQueue<SDL_Point> *subq,
           std::shared_ptr<std::atomic<bool>> shutdown_flag);
 
  int GetScore() const {return score;}
  int GetSize() const {return snake.size;}
  void InitCharacter(Renderer &renderer, CharacterEnum character) { 
    std::cout << "Initializing Game Character Textures " << std::endl;
    std::cout << &renderer << std::endl;
    snake.InitTextures(renderer.get(),character); 
  }; 
  

 private:
  //state
  Snake snake;
  SDL_Point ai_location; // store last location from AI (TODO -- check if needed?)
  RenderUtils::Item itemStruct {"apple",0,0};
  std::vector<RenderUtils::Bomb> _bombs;
  int score{0};

  //item controls
  void RemoveBomb();
  void PlaceBomb();
  void PlaceItem();
  bool snakeCollides(int x, int y);
  bool itemCollides(int x, int y);
  bool anyCollides(int x, int y);
  bool bombCollides(int x, int y);
  void Update(SDL_Point const &ai_location);

  //random engines
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_int_distribution<int> bomb_timer;
  std::uniform_real_distribution<float> random_real;
  std::discrete_distribution<> item_choice;
  std::string _render_item; // current item to render
};

#endif
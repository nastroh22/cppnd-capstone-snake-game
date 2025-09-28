#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
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
  Game(std::size_t grid_width, std::size_t grid_height); 
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration, 
           MessageQueue<SDL_Point> *pubq,
           MessageQueue<SDL_Point> *subq,
           std::shared_ptr<std::atomic<bool>> shutdown_flag);
 
  int GetScore() const;
  int GetSize() const;
  std::string GetPlayerName();
  void InitCharacter(Renderer &renderer, CharacterEnum character) { 
      snake.InitTextures(renderer.get(),character); 
  }; 

 private:
  Snake snake;
  SDL_Point ai_location; // store last location from AI (TODO -- check if needed?)
  RenderUtils::Item itemStruct {"dot",0,0};

  int score{0};
  void PlaceItem();
  void Update(SDL_Point const &ai_location);

  //rng's
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_real_distribution<float> random_food;
  std::discrete_distribution<> item_choice;
  std::string _render_item; // current item to render
};

#endif
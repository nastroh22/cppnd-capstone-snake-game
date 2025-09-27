#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"
#include <memory>
#include "queue.h"
#include "enemy/planner.h"
#include "snake.h"
#include "constants.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height); // optionally init the Game Object with a copy of queues
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration, 
           MessageQueue<SDL_Point> *pubq,
           MessageQueue<SDL_Point> *subq,
           std::shared_ptr<std::atomic<bool>> shutdown_flag,
           CharacterEnum character,
           SDL_Texture *ai_texture);
 
  int GetScore() const;
  int GetSize() const;
  // SDL_Point GetAiMove() {};
  std::string GetPlayerName();

 private:
  Snake snake;
  Food *food;  // SDL_Point food;
  std::vector<std::unique_ptr<Food>> foods;
  SDL_Point ai_location; // store last location from AI

  RenderUtils::Item itemStruct {"dot",0,0};
  RenderUtils::Item snakeStruct {"snake",0,0}; //Snake has more complexity, so keep class but dynamically update it's render file
  RenderUtils::Item hawkStruct {"hawk",0,0};

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_real_distribution<float> random_food;
  std::discrete_distribution<> item_choice;
  std::string _render_item; // current item to render

  int score{0};

  void PlaceItem();
  void Update(SDL_Point const &ai_location);
};

#endif
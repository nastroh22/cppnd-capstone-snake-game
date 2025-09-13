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
#include "ai/planner.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height); // optionally init the Game Object with a copy of queues
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration, 
           MessageQueue<SDL_Point> *pubq = nullptr,
           MessageQueue<SDL_Point> *subq = nullptr);
          //  Planner *planner = nullptr); // I think just need the queue for now
  int GetScore() const;
  int GetSize() const;
  // SDL_Point GetAiMove() {};
  std::string GetPlayerName();

 private:
  Snake snake;
  Food *food;  // SDL_Point food;
  std::vector<std::unique_ptr<Food>> foods;
  SDL_Point ai_location; // store last location from AI

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_real_distribution<float> random_food;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif
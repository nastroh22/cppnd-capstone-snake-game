#include <iostream>
#include <memory>
#include <optional>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "game.h"
#include "constants.h"
#include "snake.h"


Game::Game(std::size_t grid_width, std::size_t grid_height, std::string characterName = "Sammy")
    : snake(grid_width, grid_height, characterName),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      bomb_timer(Assets::MIN_BOMB_FRAMES, Assets::MAX_BOMB_FRAMES),
      random_real(0, 1),
      item_choice(Assets::itemProbs.begin(), Assets::itemProbs.end())
{
  // TODO tempted to store queues as class variables
  _bombs.reserve(Assets::MAX_BOMB_COUNT);
  for (int i = 0; i < Assets::MAX_BOMB_COUNT; ++i){
    _bombs.emplace_back("bomb",0,0,0,false,0);
  }
  // at least one bomb to start
  _bombs[0].is_active = true; 
  _bombs[0].x = random_w(engine);
  _bombs[0].y = random_w(engine);
  _bombs[0].timeout = bomb_timer(engine);
}


void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration,
               MessageQueue<SDL_Point> *subscriberq,
               MessageQueue<SDL_Point> *publisherq,
               std::shared_ptr<std::atomic<bool>> shutdown_flag)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  ai_location = SDL_Point{20,20}; // initialize to same as constructor
  PlaceItem(); // initial random location


  while (running) {
    frame_start = SDL_GetTicks();

    // Actually GETS move from AI
    std::optional<SDL_Point> msg = subscriberq->send(); 
    if (msg.has_value()) {
        ai_location = msg.value();
    }

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(ai_location);
    renderer.Render(snake, itemStruct, _bombs, ai_location);

    if (snake.is_immune) {
      snake.alive = true; // ensure lives if immune
    }

    // Keep track of how long each loop through the input/update/render cycle takes.
    frame_end = SDL_GetTicks();
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // SENDS player loc to AI, giving it time to calc next move
    SDL_Point temp = SDL_Point{static_cast<int>(snake.head_x), static_cast<int>(snake.head_y)};
    publisherq->receive(std::move(temp));
    
    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }

    if (snake.alive == false) {
      // Shutdown the ai thread  with shutdown flag, stop queues if waiting
      shutdown_flag->store(true);
      std::this_thread::sleep_for(std::chrono::milliseconds(5)); // ensure time for clean shutdown
      subscriberq->shutdown();
      publisherq->shutdown();
      // planner.stop(); // break planner loop if running
      return;
    }
  }
}

void Game::Update(SDL_Point const &ai_location) {
  if (!snake.alive) return;

  snake.Update(ai_location);

  RemoveBomb();
  PlaceBomb();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (itemStruct.x == new_x && itemStruct.y == new_y) {
    score++;
    if (itemStruct.name == "star") {
      snake.is_immune = true;
      snake.immunity_timer = Assets::SNAKE_IMMUNITY_FRAMES;
    }
    PlaceItem();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  // check immunity
  if (snake.immunity_timer > 0) {
    snake.immunity_timer--;
  } else {
    snake.is_immune = false;
    snake.immunity_timer = 0; // redundant but safe
  }

  /*NOTE: collision logic is split between snake update and here. 
      A bit clunky too. Assuming if head does not hit bomb, body won't either */
  if (bombCollides(new_x, new_y)){
    snake.alive = false;
  }

}

void Game::RemoveBomb() {
  for (auto &bomb: _bombs) {
    if (bomb.is_active) {
      bomb.frame_count++;
      if (bomb.frame_count >= bomb.timeout) {
        bomb.is_active = false;
        bomb.frame_count = 0;
        // std::cout << "Bomb Deactivated at " << bomb.x << "," << bomb.y << std::endl; //debug
      }
    }
  }
}

void Game::PlaceBomb() {
  std::string name = Assets::itemNames[item_choice(engine)];
  if (name != "bomb") {
    return;
  }
  for (auto &bomb: _bombs){
    if (bomb.is_active) {
      continue; // skip active bombs, should also handle max capacity automatically
    }
    while (true) {
      bomb.x = random_w(engine);
      bomb.y = random_h(engine);
      bomb.timeout = bomb_timer(engine);
      if (!anyCollides(bomb.x,bomb.y)) {
        bomb.is_active = true;
        return;
      }
    }
  }
}


void Game::PlaceItem() { 
  // random choice
  while (true) {
    itemStruct.name = Assets::itemNames[item_choice(engine)];
    if (itemStruct.name != "bomb"){
      break; // separate logic for placing bombs
    } 
  }
  int x, y;
  while (true) {
    // random location
    x = random_w(engine);
    y = random_h(engine);
    // can't collide
    if (!snakeCollides(x, y) && !bombCollides(x, y)) {
      itemStruct.x = x;
      itemStruct.y = y;
      return;
    }
  }
}

bool Game::snakeCollides(int x, int y) {
  if (snake.SnakeCell(x, y)){
    return true;
  }
  return false;
}

bool Game::bombCollides(int x, int y){
  for (auto &bomb : _bombs){
    if ((bomb.is_active) && (bomb.x == x && bomb.y == y)){
        return true;
      }
    }
  return false;
}

bool Game::itemCollides(int x, int y){
  if (itemStruct.x == x && itemStruct.y == y){
    return true;
  }
  return false;
}

bool Game::anyCollides(int x, int y){
  if (snakeCollides(x,y) || bombCollides(x,y) || itemCollides(x,y)){
    return true;
  }
  return false;
}


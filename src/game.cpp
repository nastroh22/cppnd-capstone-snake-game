#include <iostream>
#include <memory>
#include <optional>

#include "SDL.h"
#include "game.h"
#include "constants.h"
#include "snake.h"


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_food(0, 1),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      item_choice(Assets::itemProbs.begin(), Assets::itemProbs.end())
  {
  // TODO I am also tempted to store queues as class variables
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
  SDL_Point ai_location = SDL_Point{20,20}; // initialize to same as constructor
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
    renderer.Render(snake, itemStruct, ai_location);

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
    int send_x = static_cast<int>(snake.head_x);
    int send_y = static_cast<int>(snake.head_x);
    publisherq->receive(std::move(
      SDL_Point{static_cast<int>(snake.head_x), 
                static_cast<int>(snake.head_y)}));
    
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

void Game::PlaceItem() { 
  // random choice
  itemStruct.name = Assets::itemNames[item_choice(engine)];
  int x, y;
  while (true) {
    // random location
    x = random_w(engine);
    y = random_h(engine);

    // can't collide
    if (!snake.SnakeCell(x, y)) {
      itemStruct.x = x;
      itemStruct.y = y;
      return;
    }
  }
}

void Game::Update(SDL_Point const &ai_location) {
  if (!snake.alive) return;

  snake.Update(ai_location);

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (itemStruct.x == new_x && itemStruct.y == new_y) {
    score++;
    PlaceItem();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

std::string Game::GetPlayerName(){
    std::string playerName;

    std::cout << "====================================\n";
    std::cout << "         Snake-GAME TERMINAL MENU   \n";
    std::cout << "====================================\n";
    std::cout << "Enter Your Name and Hit Enter to Play:\n> ";

    std::getline(std::cin, playerName);

    while (playerName.empty()) {
        std::cout << "Name cannot be empty. Please enter your name:\n> ";
        std::getline(std::cin, playerName);
    }

    std::cout << "\nWelcome, " << playerName << "! Starting the game...\n";
    return playerName;
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
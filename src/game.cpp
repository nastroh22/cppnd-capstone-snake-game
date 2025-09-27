#include "game.h"
#include <iostream>
#include "SDL.h"
#include "food.h"
#include <memory>
#include <optional>
#include "snake.h"
#include <constants.h>


// Note: possible use case of a template:
template <typename... FoodTypes>
void AddFoods(std::vector<std::unique_ptr<Food>>& foods) {
    (foods.emplace_back(std::make_unique<FoodTypes>()), ...);
}

//TODO Same Pattern For Character Textures ?? 

// NOTE: another pattern (for unequal number of food types is iterate over number of each type, emplacing in constructor)
// Another nice pattern would be to use a shared pointer on the surface 
// then render a bunch of dots using a shared resource (possibly for pacman game)


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_food(0, 1),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      item_choice(Assets::itemProbs.begin(), Assets::itemProbs.end())
  {

  // Pass Renderer here to init textures at construction
  // I am also tempted to store queues as class variables

  AddFoods<Banana, Cherries, Dot, Star, Bomb>(foods);
  PlaceItem();


}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration,
               MessageQueue<SDL_Point> *subscriberq,
               MessageQueue<SDL_Point> *publisherq,
               std::shared_ptr<std::atomic<bool>> shutdown_flag,
               CharacterEnum character,
               SDL_Texture* ai_texture)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  // Game Viz Inits Unique Textures (could move to main perhaps)
  // for (auto& food_ptr : foods) {food_ptr->init_texture(renderer.get());} // Move to constructor
  SDL_Point ai_location = SDL_Point{20,20}; // initialize to same as constructor
  snake.InitHeadTexture(renderer.get(), characterSpriteFiles[character][0]);
  snake.InitBodyTexture(renderer.get(), characterSpriteFiles[character][1]);
  snake.InitDiesTexture(renderer.get(), characterSpriteFiles[character][2]);
  //TODO probably move the hawk texture into this as well (??)

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
    renderer.Render(snake, food, itemStruct, ai_location, ai_texture);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle takes.
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

    //NOTE: I think the fix here is just make sure send and receive run every loop before breaking
    // to allow condition vars to send
    // However, another (cleaner?) solution is to write a shutdown function in the queu that notifies all conditions
    // after the shutdown flag is set
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
  std::cout << "Exiting Update?" << std::endl;
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
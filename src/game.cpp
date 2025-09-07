#include "game.h"
#include <iostream>
#include "SDL.h"
#include "food.h"
#include <memory>


// Note: possible use case of a template:
template <typename... FoodTypes>
void AddFoods(std::vector<std::unique_ptr<Food>>& foods) {
    (foods.emplace_back(std::make_unique<FoodTypes>()), ...);
}
// NOTE: another pattern (for unequal number of food types is iterate over number of each type, emplacing in constructor)
// Another nice pattern would be to use a shared pointer on the surface 
// then render a bunch of dots using a shared resource (possibly for pacman game)



Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_food(0, 1),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)){

  AddFoods<Banana, Cherries, Dot>(foods);
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  // Init Unique Textures (could move to main perhaps)
  for (auto& food_ptr : foods) {food_ptr->init_texture(renderer.get());}

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);
    if (snake.alive == false) {
        return;
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.

    // NOTE: add random food selection step
    float rand = random_food(engine);
    int food_index;
    if (rand > 0.66) {
        food_index = 0; // Banana
    } else if (rand > 0.33) {
        food_index = 1; // Cherry
    } else {
        food_index = 2; // Dot
    }
    food = foods[food_index].get();
    if (!snake.SnakeCell(x, y)) {
      // food->position.x = x;
      // food->position.y = y;
      food->set_position(x, y);
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food->get_x() == new_x && food->get_y() == new_y) {
    score++;
    PlaceFood();
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
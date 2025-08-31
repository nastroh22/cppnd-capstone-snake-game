#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "scoreio.h"
#include "menu.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  // later : update project paths: ScoreIO::PROJECT_PAT
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  MenuManager menu(renderer.get()); // todo rename get() to be more descriptive
  
  // Text text(renderer.get(), "../assets/fonts/comic_sans_ms.ttf", 64, "Start Game", {255, 255, 255, 255});
  // text.display(renderer.get(), 10, 10);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  bool start = menu.display();

  if (!start){ return 0;} // exit game logic }

  // TODO: figure it out in the morning ... DO NOT run this WHILE loop -- will crash!!
  // while (true){
  //   text.display(renderer.get(), 220, 200); // display start text
  // }
  std::string Player = game.GetPlayerName();
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  ScoreIO::Entry new_entry{Player, game.GetScore()};
  ScoreIO::save_score(new_entry);
  ScoreIO::print_scores(); // return to menu logic

  return 0;
}
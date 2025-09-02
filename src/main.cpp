#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "scoreio.h"
#include "menu.h"
#include "constants.h"

int main() {

  // later : update project paths: ScoreIO::PROJECT_PAT
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  MenuManager menu(renderer.get()); // todo rename get() to be more descriptive
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  bool start = menu.display();
  std::cout << "Start ? " << start << std::endl;
  if (!start){ return 0;} // exit game logic }

  // std::string Player = game.GetPlayerName();
  std::string Player = "Bob";

  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  ScoreIO::Entry new_entry{Player, game.GetScore()}; // NOTE: names can't contain spaces bug (TODO fix)
  ScoreIO::save_score(new_entry);
  ScoreIO::print_scores(); // return to menu logic

  return 0;
}
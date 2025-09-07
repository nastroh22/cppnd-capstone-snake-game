#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "scoreio.h"
#include "menu.h"
#include "constants.h"
#include <thread>
#include <chrono>

enum class GameState { MENU, PLAYING, EXIT };

int main() {

  GameState state = GameState::MENU;
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  MenuManager menu(renderer.get());
  Controller controller;
  std::string Player = "Player1";
  bool start = false;

  
  while (state != GameState::EXIT) {
    switch (state) {
      
      case GameState::MENU: {
        std::cout << "Displaying Menu " << std::endl;
        start = menu.display();
        Player = menu.getPlayerName();
        std::cout << "Start ? : " << start << std::endl;
        if (start){ 
            state = GameState::PLAYING;
            std::cout << "Player Name: " << Player << std::endl;
         } else {
             std::cout << "Should Exit : " << std::endl;
            state = GameState::EXIT;
        }
        break;
      }

      case GameState::PLAYING: {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        {
          Game game(kGridWidth, kGridHeight);
          game.Run(controller, renderer, kMsPerFrame);
          ScoreIO::Entry new_entry{Player, game.GetScore()};
          ScoreIO::save_score(new_entry);
        }
        state = GameState::MENU;
        std::cout << "Play again? " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        break;
      }
      
      case GameState::EXIT:
        state = GameState::EXIT;
        break;
      
      default:
        state = GameState::EXIT;

    }
  }

  std::cout << "Game has terminated successfully!\n";
  ScoreIO::print_scores(); // return to menu logic

  return 0;
}
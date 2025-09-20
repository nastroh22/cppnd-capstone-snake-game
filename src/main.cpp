#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "scoreio.h"
#include "menu.h"
#include "constants.h"
#include <thread>
#include <future>
#include <chrono>
#include "ai/run.h"
#include "queue.h"
#include "SDL.h"
#include <atomic>
#include "snake.h"
#include "character.h"

enum class GameState { MENU, PLAYING, EXIT };

int main() {

  GameState state = GameState::MENU;
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  MenuManager menu(renderer.get());
  Controller controller;
  std::string Player = "Player1";
  bool start = false;


  //Set up AI position queue
  std::shared_ptr<std::atomic<bool>> shutdown_flag = std::make_shared<std::atomic<bool>>(false);
  std::shared_ptr<MessageQueue<SDL_Point>> aiq = std::make_shared<MessageQueue<SDL_Point>>(shutdown_flag);
  std::shared_ptr<MessageQueue<SDL_Point>> playerq = std::make_shared<MessageQueue<SDL_Point>>(shutdown_flag);
  Planner planner = Planner(aiq.get(),playerq.get(), shutdown_flag);
  CharacterEnum character = CharacterEnum::Sammy;
  Characters::Hawk hawk(renderer.get());
  
  
  
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
        shutdown_flag->store(false);
      
        planner.start(); 
        std::cout << "Launching planner thread " << std::endl;
        std::future<bool> f = std::async(std::launch::async, [&planner](){return AI::run(planner);});
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        {
          Game game(kGridWidth, kGridHeight);
          game.Run(controller, renderer, kMsPerFrame, aiq.get(), playerq.get(), shutdown_flag, character, hawk.get());
          shutdown_flag->store(true); // for manual quit path
          aiq->clear(); playerq->clear();
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          planner.stop();
          bool test = f.get(); // TODO: notify AI thread that the game is over 
          std::cout << "Enemy thread is off: " << test << std::endl;
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
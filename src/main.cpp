#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <atomic>

#include "constants.h"
// #include "queue.h"
#include "controller.h"
#include "game.h"
#include "snake.h"
#include "renderer.h"
#include "utils.h"
#include "enemy/planner.h"
#include "menu/menu.h"
#include "SDL.h"


enum class GameState { MENU, PLAYING, EXIT };

int main() {

  // stateF
  GameState state = GameState::MENU;
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  MenuManager menu(renderer.get());
  Controller controller;
  std::string Player = "Player1";
  std::string characterName = "Sammy";
  bool start = false;

  // Position Queues and Planner
  std::shared_ptr<std::atomic<bool>> shutdown_flag = std::make_shared<std::atomic<bool>>(false);
  std::shared_ptr<MessageQueue<SDL_Point>> aiq = std::make_shared<MessageQueue<SDL_Point>>(shutdown_flag);
  std::shared_ptr<MessageQueue<SDL_Point>> playerq = std::make_shared<MessageQueue<SDL_Point>>(shutdown_flag);
  Planner planner(aiq.get(), playerq.get(), shutdown_flag);


  // main loop
  while (state != GameState::EXIT) {
    switch (state) {
      
      case GameState::MENU: {
        start = menu.display();
        Player = menu.getPlayerName();
        characterName = menu.getCharacterSelection();
        if (start){ 
            state = GameState::PLAYING;
            std::cout << "Character Name? " << characterName << std::endl;
         } else {
             std::cout << "Should Exit : " << std::endl;
            state = GameState::EXIT;
        }
        //TODO: let Menu go out of scope here ??
        break;
      }

      case GameState::PLAYING: {

        // get character textures:
        CharacterEnum character = characterEnumMap.at(characterName);

        
        shutdown_flag->store(false);
        planner.start(); // maybe move the thread launching into this function
        
        std::cout << "Launching planner thread " << std::endl;
        // std::future<bool> f = std::async(std::launch::async, [&planner](){return AI::run(planner);});
        std::future<bool> f = std::async(std::launch::async, &Planner::run, &planner);
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
        
        {
          Game game(kGridWidth, kGridHeight);
          game.InitCharacter(renderer, character); // init files from player's selected character
          // game.initQueues(std::move(playerq.get()), std::move(aiq.get())); //TODO: game instance manages all related state
          std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
          game.Run(controller,  
                  renderer,    
                  kMsPerFrame, 
                  aiq.get(),    // planner positions
                  playerq.get(), // player positions
                  shutdown_flag // kills planner thread
            );
        
          // some shutdown steps (TODO: Destroy Menu (and confirm no seg faults))
          shutdown_flag->store(true); // for manual quit path
          // aiq->clear(); playerq->clear(); // Moved these calls inside game.Run();
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          planner.stop(); // std::cout << "Enemy thread is off: " << test << std::endl;
          std::cout << "flag: " << std::boolalpha << shutdown_flag->load() << std::endl;
          bool test = f.get(); // TODO: this could be causing the bug ?
          ScoreIO::Entry new_entry{Player, game.GetScore()};
          ScoreIO::save_score(new_entry);
        }

        state = GameState::MENU; // TODO:
        // reconstruct menu (not sure what is actually more efficient but idea is to test destruction and to free menu textures from memory)
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        // TODO: add intermediate screen to show score, and maybe replay option
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

  return 0;
}
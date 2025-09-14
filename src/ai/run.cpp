#include "planner.h"
// #include "./renderer.h"
#include <iostream>
#include "run.h"
#include <thread>
#include <atomic>


bool AI::run(Planner &planner) {
    // std::cout << "Running AI Planner Test: " << std::this_thread::get_id() << std::endl;
    SDL_Point move;
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Starting Planner Loop " << planner.is_running() << std::endl;
    while (planner.is_running()) { 
        // NOTE: how this is now, really very little use for threads, since it is basically sequential logic
        //TODO hope to demonstrate more performance using A* algorithm on it's own thread
        std::this_thread::sleep_for(std::chrono::milliseconds(7)); // avoid burning out
        move = planner.getNextMove();
        planner.publishMove();
        planner.checkPubqSize();
        std::cout << "Move Published: " << move.x << ", " << move.y << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        planner.checkSubqSize();
        // std::cout << "Subscribing to new goal " << std::endl;
        planner.subscribeGoal(); // get player position from queue
        std::cout << "Received New Goal "; planner.printGoalPoint();
    }

    return false;
};

// Next Step is to set up another queue to communicate moves back to main thread
// Or perhaps the queue just needs to be internal, actually

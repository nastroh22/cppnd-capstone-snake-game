#pragma once

#include "./queue.h"
#include "SDL.h"
#include <vector>
#include <algorithm>
#include "./constants.h"
#include <memory>
#include <atomic>
#include <optional>

// NOTE: Renderer is using a subgrid, which could make A* more feasible actually (future version)
// kGridWidth, kGridHeight


// TODO: probably move "Hawk" here

// Possible extension with the "Fox" character as well 

// TODO: and another extension would be multiple hawks or 'swarms' (which glide across screen in some random pattern, then exit)


// NOTE: maybe store the hawk texture here instead
class Planner {
    public:
        Planner() = delete;
        Planner(MessageQueue<SDL_Point> *const pubq, 
                MessageQueue<SDL_Point> *const subq,
                std::shared_ptr<std::atomic<bool>> _shutdown_flag) :
                _publisher(pubq), _subscriber(subq), _shutdown_flag(_shutdown_flag) {};
        ~Planner() = default;

        SDL_Point getNextMove();
        void publishMove() {
            if (_shutdown_flag->load()){
                _running = false; // break loop
                return;
            }
            _publisher->receive(std::move(_next_move));
        };
        void subscribeGoal() {
            if (_shutdown_flag->load()){
                _running = false; // break loop
                return;
            }
            std::optional<SDL_Point> temp = _subscriber->send(); 
            if (!temp.has_value()){ // do nothing, but should only happen in shutdown
                std::cout << "Planner did not receive goal, continue with last value" << std::endl;
                return;
            }
            // NOTE: move this multiply into the game logic instead (perhaps)?
            goal.x = temp.value().x;  //* (kScreenWidth/ kGridWidth); // or deal compeltely in grid units here
            goal.y = temp.value().y;  //* (kScreenHeight/ kGridHeight);
        };
        bool start() {_running = true; resetPosition(); return _running;}; // NOTE: can probably just coordinate with shutdown flag
        bool stop() {_running = false; return _running;};
        bool is_running() const {return _running;};
        SDL_Point Patrol();
        SDL_Point FollowLine();
        void printGoalPoint() const {std::cout << "Goal : " << goal.x << ", " << goal.y << std::endl;};
        void checkPubqSize() const {std::cout << "Publisher Queue Size: "; _publisher->size();};
        void checkSubqSize() const {std::cout << "Subscriber Queue Size: "; _subscriber->size();};
        void resetPosition() { x = 1.0; y = 1.0;}; // reset to corner
        bool run(); // main loop
        void debugRunFlag() const {std::cout << "Running? " << _running << std::endl;};

    private:
        // Update to shared pointer eventually? (NOTE: threads make less sense for simple behavior)
        MessageQueue<SDL_Point> *const _publisher; // For broadcasting location
        MessageQueue<SDL_Point> *const _subscriber; // For receiving goal
        SDL_Point _next_move;

        // dummy moves
        size_t _counter;
        double x = 1.0;
        double y = 1.0;

        bool _running = true;
        int delta_x = 0;
        int delta_y = 0;
        float speed{0.06f}; // 60% of the snake speed // TODO move everything to constants.h
        SDL_Point goal = SDL_Point{2,2}; // Location of the player
        std::shared_ptr<std::atomic<bool>> _shutdown_flag; // stop logic
        SDL_Texture *_hawk_texture = nullptr; // store hawk texture for rendering

        
};
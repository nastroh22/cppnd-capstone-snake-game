#pragma once

#include "./queue.h"
#include "SDL.h"
#include <vector>
#include <algorithm>
#include "./constants.h"
#include <memory>
#include <atomic>
#include <optional>

// NOTE: Rendenrer is using a subgrid, which will make A* more feasible actually
// kGridWidth, kGridHeight


class Node {

    public:
        Node() = default;
        ~Node() = default;

        SDL_Point position;
        Node * parent = nullptr;
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0;
        bool visited = false;
        std::vector<Node *> neighbors;

        float distance(const Node& other) const {
            return std::sqrt(std::pow((position.x - other.position.x), 2) + std::pow((position.y - other.position.y), 2));
        }
        bool operator>= (const Node &other){
            return ((h_value + g_value) >= (other.h_value + other.g_value));
        };
    //private:
};

bool inline Compare(Node *a , Node *b){
        return ((a->h_value + a->g_value) >= (b->h_value + b->g_value));
};


class Planner {
    public:
        Planner() = delete;
        Planner(MessageQueue<SDL_Point> *const pubq, 
                MessageQueue<SDL_Point> *const subq,
                std::shared_ptr<std::atomic<bool>> _shutdown_flag) :
                _publisher(pubq), _subscriber(subq), _shutdown_flag(_shutdown_flag) {};
        ~Planner() = default;

        SDL_Point getNextMove();
        void publishMove() {_publisher->receive(std::move(_next_move));};
        void subscribeGoal() {
            std::optional<SDL_Point> temp = _subscriber->send(); 
            if (_shutdown_flag->load()){
                _running = false; // break loop
                return;
            }
            if (!temp.has_value()){ // do nothing, but should only happen in shutdown
                std::cout << "Planner did not receive goal, continue with last value" << std::endl;
                return;
            }
            // NOTE: move this multiply into the game logic instead (perhaps)?
            goal.x = temp.value().x * (kScreenWidth/ kGridWidth); // or deal compeltely in grid units here
            goal.y = temp.value().y * (kScreenHeight/ kGridHeight);
        };
        bool on() {_running = true; return _running;}; // NOTE: can probably just coordinate with shutdown flag
        bool off() {_running = false; return _running;};
        bool is_running() const {return _running;};
        SDL_Point Patrol();
        SDL_Point FollowLine();
        void printGoalPoint() const {std::cout << "Goal : " << goal.x << ", " << goal.y << std::endl;};
        void checkPubqSize() const {std::cout << "Publisher Queue Size: "; _publisher->size();};
        void checkSubqSize() const {std::cout << "Subscriber Queue Size: "; _subscriber->size();};

        // A* helper methods
        // float calculateHValue(Node const *node) {float h = end_node->distance(*node); return h;};
        // void Planner::AddNeighbors(Model::Node *current_node);
        // Node *Planner::NextNode();
        // std::vector<Node> ConstructFinalPath(Node *current_node);
        
        // void AStarSearch(){
        //     Node *current_node = start_node;
        //     while (true){
        //         AddNeighbors(current_node);
        //         current_node = NextNode();
        //         if (current_node == end_node){break;}
        //     }
        // };

    private:
        // Update to shared pointer eventually? (NOTE: threads make less sense for simple behavior)
        MessageQueue<SDL_Point> *const _publisher; // For broadcasting location
        MessageQueue<SDL_Point> *const _subscriber; // For receiving goal
        SDL_Point _next_move;

        // dummy moves
        size_t _counter;
        int x = 20;
        int y = 20;

        bool _running = true;
        int delta_x = 0;
        int delta_y = 0;
        SDL_Point goal = SDL_Point{50,300}; // Location of the player
        std::shared_ptr<std::atomic<bool>> _shutdown_flag; // stop logic

        // TODO: Implement message queues
        // TODO: A* stuff
        // Node start_node;
        // Node end_node;
        // Node current_node;
        // std::vector<Node *> open_list; // TODO (possibly?): change to priority queue
};
#include "planner.h"
#include <iostream>
#include <cmath>

SDL_Point Planner::getNextMove() {
    // Dummy logic for now, move in square pattern
    _next_move = FollowLine();
    return _next_move;
};

SDL_Point Planner::Patrol() {
    // Patrol logic here, square pattern
    // For simplicity, let's say the enemy moves in a square pattern
    if (_counter % 100 == 0) {delta_x = 2; delta_y = 0;};
    if (_counter % 100 == 25) {delta_x = 0; delta_y = 2;};
    if (_counter % 100 == 50) {delta_x = -2; delta_y = 0;};
    if (_counter % 100 == 75) {delta_x = 0; delta_y = -2;};
    x+=delta_x; y+=delta_y;
    _counter ++;
    return SDL_Point{x,y};
}

std::pair<int, int> inline NormalizeSpeed(int dx, int dy, float speed = 2.0f) {
    // Handle zero direction edge case
    if (dx == 0 && dy == 0)
        return {0, 0}; // or throw / handle however you want

    // Step 1: normalize to unit vector
    double len = std::sqrt(dx * dx + dy * dy);
    double ux = dx / len;
    double uy = dy / len;

    // Step 2: scale to length 2
    double scaled_x = ux * speed;
    double scaled_y = uy * speed;

    // std::cout << "Normalized Vector ? : " << scaled_x << ", " << scaled_y << " | ";

    // Step 3: round to nearest integer
    int vx = std::round(scaled_x);
    int vy = std::round(scaled_y);

    // Optional: Re-check magnitude (vx, vy) and adjust if needed
    // But this gets very close to norm 2 with correct direction

    return {vx, vy};
}


SDL_Point Planner::FollowLine() {
    int d_x = (goal.x - x);
    int d_y = (goal.y - y);
    std::cout << "Deltas ? : " << d_x << ", " << d_y << " | ";
    // delta_x = std::min(std::max(delta_x, -2), 2); // clamp to [-2, 2]
    // delta_y = std::min(std::max(delta_y, -2), 2); // clamp to [-2, 2]
    std::pair<int,int> vec = NormalizeSpeed(d_x, d_y, 1.5f); // 17 standard allows: auto [x, y] = vectorWithNormTwo(5, 2);
    x+=vec.first; 
    y+=vec.second;
    // currPos.x += vec.first; 
    // currPos.y += vec.first;
    // std::cout << "Velocity ? : " << vec.first << ", " << vec.second << " | ";
    return SDL_Point{x,y};
}


/************ Changed my Mind Again (maybe), Might Use A* Afterall ********************
Going to use a much simpler algorithm to save on time and space for an A* search of the whole space
But will still use a basic A* implementation now that can be generalized later 
*/

/* Algorithm
    1. Draw a line to the enemy
    2. Draw a rectangle around the enemy, let's say 20x20 ( a sophisticated enemy will include screen wrap)
    3. Find where the line intersects the rectangle
    4. Make the intersection point the goal for the enemy
    5. Construct path an buffer it in the queue
    6. Planner thread will construct a new path from the front of the queue (last to be entered)
    7. Play with parameters to see performance (q length, grid size, refresh rate etc.)
*/

// TODO:
// void Planner::AddNeighbors(Model::Node *current_node) {
//     // RouteModel::Node current = *current_node;
//     current_node->FindNeighbors();
//     for (auto node : current_node->neighbors){
//         node->g_value = current_node->g_value + current_node->distance(*node);
//         node->h_value = CalculateHValue(node);
//         node->parent = current_node;
//         open_list.push_back(node);
//         node->visited = true;
//     };
//     current_node->visited = true;
// }

// Node* Planner::NextNode() {
//     std::sort(open_list.begin(), open_list.end(),
//         [](Node *a , Node *b){return Compare(a,b);});
//     Node *next =  open_list.back();
//     open_list.pop_back();
//     return next;
// }

// std::vector<Node> Planner::ConstructFinalPath(RouteModel::Node *current_node) {
//     // Create path_found vector
//     std::vector<Node> path_found; // TODO: make a class property
//     distance = 0.0f;
//     while (true){ 
//         path_found.push_back(*current_node);
//         distance += current_node->distance(*(current_node->parent));
//         current_node = current_node->parent;
//         if (current_node == start_node){
//             path_found.push_back(*current_node);
//             break;
//         }
//     }
//     std::reverse(path_found.begin(), path_found.end());    
//     return path_found;
// }



///TIMEOUT Example:
// T receiveWithTimeout(std::chrono::milliseconds timeout, const T& default_value) {
//     std::unique_lock<std::mutex> uLock(_mutex);

//     // wait_for returns false if timeout occurs
//     bool got_message = _cond.wait_for(uLock, timeout, [this] {
//         return !_messages.empty();
//     });

//     if (!got_message) {
//         return default_value; // Timeout happened
//     }

//     T msg = std::move(_messages.back());
//     _messages.pop_back();
//     return msg;
// }
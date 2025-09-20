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
    return SDL_Point{static_cast<int>(x),static_cast<int>(y)};
}

std::pair<double, double> inline NormalizeSpeed(int dx, int dy, float speed = 2.0f) {
    // Handle zero direction edge case
    if (dx == 0 && dy == 0)
        return {0, 0}; // or throw / handle however you want
    
    // std::cout << "Deltas ? : " << dx << ", " << dy << "speed "<< speed << " | ";

    // Step 1: normalize to unit vector
    double len = std::sqrt(dx * dx + dy * dy);
    double ux = dx / len;
    double uy = dy / len;

    // Step 2: scale to length 2
    double vx = ux * speed;
    double vy = uy * speed;

    // std::cout << "Normalized Vector ? : " << scaled_x << ", " << scaled_y << " | ";

    return {vx, vy};
}


SDL_Point Planner::FollowLine() {
    int d_x = (goal.x - x); // NOTE: maybe this could be float
    int d_y = (goal.y - y);

    std::pair<double, double> vec = NormalizeSpeed(d_x, d_y, speed); // 17 standard allows: auto [x, y] = vectorWithNormTwo(5, 2);
    std::cout << "Deltas ? : " << vec.first << ", " << vec.second << " | ";
    x+=vec.first; 
    y+=vec.second;
    std::cout << "New Position ? : " << x << ", " << y << " | ";
    
    return SDL_Point{static_cast<int>(x),static_cast<int>(y)};
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
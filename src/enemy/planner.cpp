#include <iostream>
#include <cmath>
#include "planner.h"

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

    // Step 1: normalize to unit vector
    double len = std::sqrt(dx * dx + dy * dy);
    double ux = dx / len;
    double uy = dy / len;

    // Step 2: scale to length 2
    double vx = ux * speed;
    double vy = uy * speed;

    return {vx, vy};
}


SDL_Point Planner::FollowLine() {
    int d_x = (goal.x - x);
    int d_y = (goal.y - y);
    std::pair<double, double> vec = NormalizeSpeed(d_x, d_y, speed); // 17 standard allows: auto [x, y] = vectorWithNormTwo(5, 2);
    x+=vec.first; 
    y+=vec.second;
    return SDL_Point{static_cast<int>(x),static_cast<int>(y)};
}

bool Planner::run() {
    while (_running) { 
        SDL_Point move = getNextMove();
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); // avoid burning out
        publishMove(); // send planner move
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        subscribeGoal(); // get player pos
    }
    return false;
};
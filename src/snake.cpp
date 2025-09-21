#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update(SDL_Point const &ai_location) {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell, ai_location);
  }

}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell, SDL_Point const &ai_location) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  int radius = 1; // make configurable? Hit box
  const auto isCollision = [radius,ai_location](SDL_Point const &pt) -> bool {
    return std::abs(ai_location.x - pt.x) <= radius &&
           std::abs(ai_location.y - pt.y) <= radius;
  };
  if (isCollision(current_head_cell)) {
    alive = false;
  }
  for (auto const &item : body) {
    if ((item.x == ai_location.x && item.y == ai_location.y) || isCollision(item))
    {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

SDL_Texture* Snake::InitTexture(SDL_Renderer* renderer, const std::string& path) {
  SDL_Surface* surface = SDL_LoadBMP(path.c_str());
  if (!surface) {
    std::cerr << "Failed to load snake BMP: " << path  << " " << SDL_GetError() << std::endl;
    return nullptr;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    std::cerr << "Failed to create  texture: " <<  path  << " " << SDL_GetError() << std::endl;
    // Handle error or set to nullptr
  }
  else {
    std::cout << "Loaded Snake Texture from " << path << std::endl;
  }
  SDL_FreeSurface(surface);
  std::cout << "Texture pointer here : " << texture << std::endl;
  int w = 0, h = 0;
  SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
  std::cout << "Raw loaded texture is size: " << w << " x " << h << "\n";
  return texture;
}

void Snake::InitHeadTexture(SDL_Renderer* renderer, const std::string& path) {
    if (snake_head_texture) SDL_DestroyTexture(snake_head_texture); // free existing resource
    snake_head_texture = InitTexture(renderer, path);
}

void Snake::InitBodyTexture(SDL_Renderer* renderer, const std::string& path) {
    if (snake_body_texture) SDL_DestroyTexture(snake_dies_texture); // free existing resource
    snake_body_texture = InitTexture(renderer, path);
}

void Snake::InitDiesTexture(SDL_Renderer* renderer, const std::string& path) {
    if (snake_dies_texture) SDL_DestroyTexture(snake_dies_texture); // free existing resource
    snake_dies_texture = InitTexture(renderer, path);
}



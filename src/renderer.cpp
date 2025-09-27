#include <iostream>
#include <string>
// #include <algorithm> // for std::for_each
#include "renderer.h"
#include "utils.h"
#include "constants.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  
  //NOTE: not that big of a deal, just pass copies of blocks, maybe do this later
  // auto init_blocks = [&]() {
  //     SDL_Rect block;
  //     block.w = screen_width / grid_width;
  //     block.h = screen_height / grid_height;
  //     return block;
  // };

  // std::for_each({&_snake_block, &_item_block, &_hawk_block}, 
  //               [&](SDL_Rect *b){*b = init_blocks();});

  
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  if (TTF_Init() == -1) {
    std::cerr << "SDL_ttf could not initialize.\n";
    std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
    // Optionally handle failure, e.g., exit or fallback
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Static Items and Enemy Textures:
  _item_textures = RenderUtils::loadTexturesFromMap(
    sdl_renderer,
    Assets::itemTextureMap
  );
  _hawk_textures = RenderUtils::loadTexturesFromArray(
    sdl_renderer,
    Assets::hawkTextureFiles
  );
  _flipped_hawk_textures = RenderUtils::loadTexturesFromArray(
    sdl_renderer,
    Assets::hawkTextureFilesLeft
  );
  _hawk_it = _hawk_textures.begin();                               
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  RenderUtils::freeTextureMap(_item_textures);
  RenderUtils::freeTextureArray(_hawk_textures);
  RenderUtils::freeTextureArray(_flipped_hawk_textures);
  SDL_Quit();
}

void Renderer::RenderItem(SDL_Rect block, RenderUtils::Item const &item) {
  block.x = item.x * block.w;
  block.y = item.y * block.h;
  SDL_RenderCopy(sdl_renderer, 
    _item_textures[item.name], nullptr, &block); 
};

void Renderer::animateHawk(SDL_Rect const &block) {
  _flap_frame_count++;
  if (_flap_frame_count >= Assets::FLAP_RATE) {
    _hawk_idx++;
    _flap_frame_count = 0;
  } // update rate
  if (block.x < _prev_x) {
    _flip_hawk_direction = true;
  } else if (block.x > _prev_x) {
    _flip_hawk_direction = false;
  } else {
    ; // do nothing
  }
  // determine direction based on x movement
  if (_hawk_idx == Assets::HAWK_ANIMATION_FRAMES-1) {
    _hawk_idx = 0; // reset to start
  }
  if (_flip_hawk_direction){
    _hawk_it = _flipped_hawk_textures.begin() + _hawk_idx;
  } else {
    _hawk_it = _hawk_textures.begin() + _hawk_idx;
  }
};

void Renderer::RenderHawk(SDL_Rect block, SDL_Point const &ai_location) {
    // AI Texture
  block.x = ai_location.x * block.w; 
  block.y = ai_location.y * block.h;
  block.w *= 1.45;
  block.h *= 1.45; //scale size a bit
  animateHawk(block); // update iterator
  SDL_RenderCopy(sdl_renderer, *_hawk_it, nullptr, &block);
  _prev_x = block.x; // store previous x position for direction check
};

void Renderer::RenderSnake(SDL_Rect block, Snake const &snake) {
  // Render snake's body
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderCopy(sdl_renderer, snake.get_body_texture(), nullptr, &block);
  }
  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w ;
  block.y = static_cast<int>(snake.head_y) * block.h ;
  if (snake.alive) {
    SDL_RenderCopy(sdl_renderer, snake.get_head_texture(), nullptr, &block);
  } else {
    SDL_RenderCopy(sdl_renderer, snake.get_ko_texture(), nullptr, &block);
  }
};

void Renderer::Render(
    Snake const &snake, 
    RenderUtils::Item &item,
    const SDL_Point hawk_location) 
{ 
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  //Render Textures
  RenderItem(block, item);
  RenderHawk(block, hawk_location);
  RenderSnake(block, snake);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

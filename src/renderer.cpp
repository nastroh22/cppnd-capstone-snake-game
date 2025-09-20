#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
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

  // create a texture
  SDL_Surface *surface = SDL_LoadBMP("../assets/pactiles/banana.bmp");
  if (!surface) {
    std::cerr << "Failed to load BMP: " << SDL_GetError() << std::endl;
    // Handle error or exit
  }
  sdl_texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  SDL_FreeSurface(surface);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);

  // free texture resource
  if (sdl_texture) {
    SDL_DestroyTexture(sdl_texture);
    sdl_texture = nullptr;
  }

  SDL_Quit();
}

void Renderer::Render(
    Snake const &snake, 
    const Food *food, 
    const SDL_Point ai_location,
    SDL_Texture* ai_texture) 
{ 
  SDL_Rect block;
  SDL_Rect enemy;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food->get_x() * block.w;
  block.y = food->get_y() * block.h;
  // std::cout << "Food position: (" << food.x << ", " << food.y << ")\n";
  // SDL_RenderFillRect(sdl_renderer, &block);

  // Step 1: Try to Render Png Files instead
  // SDL_Surface *surface = IMG_LOAD("assets/pactiles/dot.bmp");
  // SDL_Surface *surface SDL_BMPtoSurface("assets/pactiles/dot.bmp");
  // NOTE: TODO: install and link SDL_image to load PNGs
  //bmp version:

  // SDL_Rect dstRect;
  // dstRect.x = x_position;  // update this each frame to move texture horizontally
  // dstRect.y = y_position;  // update this each frame to move texture vertically
  // dstRect.w = width;       // width of your texture (can query surface or texture)
  // dstRect.h = height;      // height of your texture
  
  // Item texture
  block.x = food->get_x() * block.w;
  block.y = food->get_y() * block.h;
  SDL_RenderCopy(sdl_renderer, food->get_texture(), nullptr, &block); // "block" is already an SDL_rect


  // AI Texture
  enemy.w = block.w; 
  enemy.h = block.h;
  enemy.x = ai_location.x * block.w; 
  enemy.y = ai_location.y * block.h;
  enemy.w *= 1.45;
  enemy.h *= 1.45; //scale size a bit
  SDL_RenderCopy(sdl_renderer, ai_texture, nullptr, &enemy);

  // Render snake's body
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderCopy(sdl_renderer, snake.get_body_texture(), nullptr, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w ;
  block.y = static_cast<int>(snake.head_y) * block.h ;
  std::cout << block.w << " "<< block.h << std::endl;
  if (snake.alive) {
    // NOTE: possible TODO: figure out how to make sammy a bit larger
    // std::cout << "Should Render Snake Head Texture " << snake.get_head_texture() << std::endl;
    SDL_RenderCopy(sdl_renderer, snake.get_head_texture(), nullptr, &block);
    // SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_RenderCopy(sdl_renderer, snake.get_dies_texture(), nullptr, &block);
    // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  // SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

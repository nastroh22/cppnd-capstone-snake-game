#include "SDL.h"
#include "menu.h"
#include "SDL_ttf.h"
#include <string>
#include <iostream>


// ********************* Text Defs  *************************** //
Text::Text(
    SDL_Renderer *renderer,
    const std::string &font_path,
    int font_size,
    const std::string &message_text,
    SDL_Color color)
{
    _text_texture = loadFont(renderer, font_path, font_size, message_text, color);
    if (!_text_texture) {
        std::cerr << "Failed to load text texture." << std::endl;
        //todo raise exception
    }
    SDL_QueryTexture(_text_texture, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
}
void Text::display(SDL_Renderer *renderer, int x, int y) const
{
    _text_rect.x = x;
    _text_rect.y = y;
    SDL_RenderCopy(renderer, _text_texture, nullptr, &_text_rect);
    std::cout << "Displayed Text" << std::endl;
}


SDL_Texture *Text::loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &message_text, SDL_Color& color)
{
    TTF_Font *font = TTF_OpenFont( font_path.c_str(), font_size);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    auto text_surface = TTF_RenderText_Solid(font, message_text.c_str(), color);

    if (!text_surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return nullptr;
    }
    
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
    }
    std::cout << "Loaded Font" << std::endl;

    SDL_FreeSurface(text_surface);
    return text_texture;
}

//  ********************* Button Defs  *************************** //

void Button::Render(SDL_Renderer* renderer){
    //clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
    SDL_RenderClear(renderer);

    // draw color
    SDL_SetRenderDrawColor(renderer, _buttonColor.r,  _buttonColor.g,  _buttonColor.b,  _buttonColor.a);
    SDL_RenderFillRect(renderer, &_buttonRect);

    
    // draw border
    SDL_SetRenderDrawColor(renderer, _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
    SDL_RenderDrawRect(renderer, &_buttonRect);

    // text render
    _text.display(renderer, 
                _buttonRect.x + (_buttonRect.w - _text.getWidth()) / 2,
                _buttonRect.y + (_buttonRect.h - _text.getHeight()) / 2
    );

    // Pretty sure I only need to call this once in main loop
    // SDL_RenderPresent(renderer);
    // SDL_Delay(16); // ~60fps
}

//  ********************* Menu Defs  *************************** //

// Menu::Menu(SDL_Renderer *renderer) {};

void Menu::Render() {
    for (const auto& button : _buttons) {
        //buttons should render their text
        button->Render(_renderer);
    }
    // TODO: background
}

// Trim this to have buttons return states
MenuState Menu::queryButtons(const SDL_Event& e) {
    
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return MenuState::NONE; // or some other default state
    }
    SDL_Point mousePoint = {e.button.x, e.button.y };
    for (const auto& button : _buttons) {
        if (button->wasClicked(mousePoint)) {
            buttonClicked = true;
            return button->onClick();
        }
    }
    return MenuState::NONE;
}


// MenuState Menu::handleEvent() {

//     MenuState temp = MenuState::NONE;
//     while (!buttonClicked) {
//         while (SDL_PollEvent(&e)) {
//             if (e.type == SDL_QUIT) {
//                 return false;
//             }
//             temp = queryButtons(e); // reference could lead to nullptr error ?
//         }
//         // Render startButton
//         startButton.Render(renderer);
//         scoreButton.Render(renderer);

//         SDL_RenderPresent(renderer);
//         SDL_Delay(16); // ~60fps
//     }
//     return temp;
// }

MainMenu::MainMenu(SDL_Renderer* renderer) //tehchnically can use internal _renderer member
    : Menu(renderer) {
        // Some vector stuff just for understanding
        std::cout << "size: " << _buttons.size() << " capacity: " << _buttons.capacity() << std::endl;
        _buttons.reserve(2); // reserve space for 2 buttons
        std::cout << "After reserve, size: " << _buttons.size() << " capacity: " << _buttons.capacity() << std::endl;
        // Add specific buttons for MainMenu if needed
        _buttons.emplace_back(std::make_unique<PlayButton>(renderer)); //TODO can use base reference _renderer avoid another copy 
        _buttons.emplace_back(std::make_unique<ScoreButton>(renderer)); 
        std::cout << "After emplace, size: " << _buttons.size() << " capacity: " << _buttons.capacity() << std::endl;

        // Todo: create background texture (although might do this in base)
}

ScoreMenu::ScoreMenu(SDL_Renderer *renderer) : Menu(renderer) 
{
    _buttons.reserve(1);
    _buttons.emplace_back(std::make_unique<BackButton>(renderer));
}


// bool Menu::ShowMenu(SDL_Renderer *renderer){
//     SDL_Event e;

//     while (!buttonClicked){
//         while (SDL_PollEvent(&e)) {
//             if (e.type == SDL_QUIT) {
//                 return false;
//             }

//             if (e.type == SDL_MOUSEBUTTONDOWN) {
//                 int mouseX = e.button.x;
//                 int mouseY = e.button.y;

//                 const SDL_Point mousePoint = { mouseX, mouseY };

//                 if (SDL_PointInRect(&mousePoint, &startButton)) {
//                     std::cout << "Button clicked!" << std::endl;
//                     bool clicked = ToggleButtonClicked();
//                     std::cout << "Button state: " << (clicked ? "Clicked" : "Not Clicked") << std::endl;
//                     return true;
//                     // Call your button's action here
//                 } elif (SDL_PointInRect(&mousePoint, &scoreButton)){
//                     bool clicked = ToggleButtonClicked();
//                     // State = Score Menu
//                 // } elif (SDL_PointInRect(&mousePoint, &quitButton)){
//                 //     return false; // exit game
//                 } else {
//                     continue;
//                 }
//             }
//         }
//         // Render startButton
//         startButton.Render(renderer);
//         scoreButton.Render(renderer);

//         SDL_RenderPresent(renderer);
//         SDL_Delay(16); // ~60fps
//     }
//     return false;
// }



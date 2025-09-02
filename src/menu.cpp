#include "SDL.h"
#include "menu.h"
#include "SDL_ttf.h"
#include <string>
#include <iostream>
#include <algorithm> 

//***************************** Helpe ***************************************** */

//TODO: Make this polymorphic class function  (Button derives from Window)
static void drawBorder(SDL_Renderer* renderer, SDL_Rect rect, int thickness, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Top
    SDL_Rect top = {rect.x, rect.y, rect.w, thickness};
    SDL_RenderFillRect(renderer, &top);

    // Bottom
    SDL_Rect bottom = {rect.x, rect.y + rect.h - thickness, rect.w, thickness};
    SDL_RenderFillRect(renderer, &bottom);

    // Left
    SDL_Rect left = {rect.x, rect.y, thickness, rect.h};
    SDL_RenderFillRect(renderer, &left);

    // Right
    SDL_Rect right = {rect.x + rect.w - thickness, rect.y, thickness, rect.h};
    SDL_RenderFillRect(renderer, &right);
}

// ***************************** Text Defs  *********************************** //
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
    std::cout << "Loaded Text: " << message_text << std::endl;
}
void Text::display(SDL_Renderer *renderer, int x, int y) const
{
    _text_rect.x = x;
    _text_rect.y = y;
    SDL_RenderCopy(renderer, _text_texture, nullptr, &_text_rect);
    // std::cout << "Displayed Text" << message << std::endl;
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
// ***************************** Window Defs  *********************************** //
void Window::Render(SDL_Renderer* renderer){
    // draw color
    SDL_SetRenderDrawColor(renderer, _windowColor.r,  _windowColor.g,  _windowColor.b,  _windowColor.a);
    SDL_RenderFillRect(renderer, &_windowRect);

    // draw border
    // SDL_SetRenderDrawColor(renderer, _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
    // SDL_RenderDrawRect(renderer, &_windowRect);
    drawBorder(renderer, _windowRect, 2, _borderColor); // thickness of 2

    // render title
    _title.display(renderer, 
        _windowRect.x + (_windowRect.w - _title.getWidth()) / 2,
        _windowRect.y + 10 // some padding from top
    );

    // render dynamic text if applicable (NOTE: need to clean this text)
    // Better Approach is interior window "DynamicText" object that can be updated or rather "InputWindow"
    if (_playerInput) {
        Text playerText(renderer, "../assets/fonts/comic_sans_ms.ttf", 24, *_playerInput, DEFAULT_TEXT_COLOR);
        playerText.display(renderer, 
            _windowRect.x + 20, // some padding from left
            _windowRect.y + 60 // below title
        );
    }
}


//  ***************************** Button Defs  *********************************** //

void Button::toggleHover(const SDL_Point& mousePoint) {
    if (SDL_PointInRect(&mousePoint, &_buttonRect)) {
        // Change border color on hover
        _borderColor = HOVER_BORDER_COLOR;
        _border_thickness = 4; //thicker border
    } else {
        // Revert to default border color
        _borderColor = DEFAULT_BORDER_COLOR;
        _border_thickness = 2;
    }
}


// TODO : Make a Template That Can Render Button or Window
void Button::Render(SDL_Renderer* renderer){

    // draw color
    SDL_SetRenderDrawColor(renderer, _buttonColor.r,  _buttonColor.g,  _buttonColor.b,  _buttonColor.a);
    SDL_RenderFillRect(renderer, &_buttonRect);

    // draw border
    drawBorder(renderer, _buttonRect, _border_thickness, _borderColor); // thickness of 2

    // render text
    _text.display(renderer, 
        _buttonRect.x + (_buttonRect.w - _text.getWidth()) / 2,
        _buttonRect.y + (_buttonRect.h - _text.getHeight()) / 2
    );
}

//  ***************************** Menu Defs *********************************** //
void Menu::Render() {
    _window->Render(_renderer); // window first so buttons are on top
    for (const auto& button : _buttons) {
        button->Render(_renderer); //buttons call render text
    }
}

MenuState Menu::queryButtons(const SDL_Event& e) {
    // Check is Hovering
    int x, y; 
    SDL_GetMouseState(&x, &y);
    SDL_Point mousePoint = {x, y}; // std::algorithm just for practice
    std::for_each(_buttons.begin(), _buttons.end(), 
        [mousePoint](const auto &button) {button->toggleHover(mousePoint);});

    // Check is Selected
    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return MenuState::NONE; // or some other default state
    }
    SDL_Point mouseClick = {e.button.x, e.button.y };
    for (auto& button : _buttons) {
        if (button->wasClicked(mouseClick)) {
            buttonClicked = true;
            //debug
            std::cout << "Click! : " << static_cast<int>(button->onClick()) << " "; 
            button->printLabel();
            return button->onClick();
        }
    }
    return MenuState::NONE;
}

MainMenu::MainMenu(SDL_Renderer* renderer) //tehchnically can use internal _renderer member
    : Menu(renderer) {
        // Some vector stuff just for understanding
        // std::cout << "size: " << _buttons.size() << " capacity: " << _buttons.capacity() << std::endl;
        _buttons.reserve(4); // reserve space for 2 buttons
        // std::cout << "After reserve, size: " << _buttons.size() << " capacity: " << _buttons.capacity() << std::endl;
        _buttons.emplace_back(std::make_unique<PlayButton>(renderer)); //TODO can use base reference _renderer avoid another copy 
        _buttons.emplace_back(std::make_unique<ScoreButton>(renderer)); 
        _buttons.emplace_back(std::make_unique<CharacterButton>(renderer));
        _buttons.emplace_back(std::make_unique<QuitButton>(renderer)); 
        // std::cout << "After emplace, size: " << _buttons.size() << " capacity: " << _buttons.capacity() << std::endl;

        std::cout << "Intialized Buttons" << std::endl;
        _buttons[0]->printProperties();
        _buttons[1]->printProperties();
    
        // Construct Window:
        _window = std::make_unique<Window>(renderer, "Welcome to Snake Game", WINDOW_COLOR, WINBORDER_COLOR, WIN_POSITION);
}

ScoreMenu::ScoreMenu(SDL_Renderer *renderer) : Menu(renderer) 
{
    _buttons.reserve(1);
    _buttons.emplace_back(std::make_unique<BackButton>(renderer));
    // NOTE: leaving additional params for now in case want to customize each window style
    _window = std::make_unique<Window>(renderer, "High Scores", WINDOW_COLOR, WINBORDER_COLOR, WIN_POSITION);
}

PlayerEntryMenu::PlayerEntryMenu(SDL_Renderer *renderer) : Menu(renderer) 
{
    _buttons.reserve(2);
    _buttons.emplace_back(std::make_unique<BackButton>(renderer)); // TODO: Make an "Enter" or "Ok" button
    _buttons.emplace_back(std::make_unique<StartButton>(renderer)); // TODO: Make an "Enter" or "Ok" button
    _window = std::make_unique<Window>(renderer, "Enter Player Name: ", WINDOW_COLOR, WINBORDER_COLOR, WIN_POSITION, &_player_name);
    // _text_entry = std::make_unique<Window>(renderer, "Enter Player Name", WINDOW_COLOR, WINBORDER_COLOR, WIN_POSITION);
}
// SDL_Renderer *renderer


// TODO: clean this, right now just wrapping queryButtons()
// MenuState PlayerEntryMenu::query_buttons(SDL_Event &event){
//     getNameInput(&event);
// }


MenuState PlayerEntryMenu::getNameInput(const SDL_Event &event){
    
    if (event.type == SDL_QUIT) {
        // Handle quit
    }
    else if (event.type == SDL_TEXTINPUT) {
        // Append event.text.text to your string buffer
        _player_name += event.text.text;
        std::cout << "Keystroke: " << _player_name << std::endl;
    }
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && _player_name.size() > 0) {
            _player_name.pop_back();  // Handle backspace
            std::cout << "After backspace: " << _player_name << std::endl;
        }
    }
    return MenuState::NONE; // or some other default state
}


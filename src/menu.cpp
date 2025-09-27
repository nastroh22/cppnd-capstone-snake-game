#include <string>
#include <iostream>
#include <algorithm> 
#include <cassert>

#include "SDL.h"
#include "menu.h"
#include "SDL_ttf.h"
#include "utils.h" //move to utils


// void InputWindow::Render(SDL_Renderer* renderer){
//     SDL_SetRenderDrawColor(renderer, _windowColor.r,  _windowColor.g,  _windowColor.b,  _windowColor.a);
//     SDL_RenderFillRect(renderer, &_windowRect);
//     RenderUtils::drawBorder(renderer, _windowRect, 1, _borderColor);
//     // (TODO?) Better Approach is interior window "DynamicText" object that can be updated or rather "InputWindow"
//     if ((_playerInput) && !_playerInput->empty()) {
//         _inputText.displayDynamic(renderer, 
//             _windowRect.x + 20, // some padding from left
//             _windowRect.y + 20, // below title
//             *_playerInput // TODO: make this a refnerence instead ?
//         );
//     }
// }

//  ***************************************** Button Defs  **************************************************** //
void Button::toggleHover(const SDL_Point& mousePoint) {
    
    if (_freeze_border) { return; } // don't change hover state if frozen
    
    if (SDL_PointInRect(&mousePoint, &_buttonRect)) {
        // Change border color on hover
        _borderColor = HOVER_BORDER_COLOR;
        _borderThickness = 4; //thicker border
    } 
    else {
        // Revert to default border color
        _borderColor = DEFAULT_BORDER_COLOR;
        _borderThickness = 2;
    }
}

void Button::select() {
    _borderColor = SELECT_BORDER_COLOR;
    _freeze_border = true; // keep border on selected button
}

void Button::unselect() {
    _borderColor = HOVER_BORDER_COLOR;
    _freeze_border = false;
}

// TODO : Make a Template That Can Render Button or Window
void Button::Render(SDL_Renderer* renderer){

    // draw color
    SDL_SetRenderDrawColor(renderer, _buttonColor.r,  _buttonColor.g,  _buttonColor.b,  _buttonColor.a);
    SDL_RenderFillRect(renderer, &_buttonRect);

    // draw border
    RenderUtils::drawBorder(renderer, _buttonRect, _borderThickness, _borderColor); // thickness of 2

    // render text
    _text.display(renderer, _textX, _textY);
}

void ImageButton::Render(SDL_Renderer* renderer){

    // draw color
    SDL_SetRenderDrawColor(renderer, _buttonColor.r,  _buttonColor.g,  _buttonColor.b,  _buttonColor.a);
    SDL_RenderFillRect(renderer, &_buttonRect);

    // draw border
    RenderUtils::drawBorder(renderer, _buttonRect, _borderThickness, _borderColor); // thickness of 2

    int title_offset = 15;
    setY(_buttonRect.y + 10); // text y position (redunant)
    _text.display(renderer, _textX, _buttonRect.y + title_offset); // render text

    // render image centered
    if (_image_texture) {
        int img_w, img_h;
        SDL_QueryTexture(_image_texture, nullptr, nullptr, &img_w, &img_h);
        int img_x = _buttonRect.x + (_buttonRect.w - img_w) / 2;
        int img_y = _buttonRect.y + (_buttonRect.h - img_h) / 2 + title_offset; // adjust for title
        SDL_Rect destRect = {img_x, img_y, img_w, img_h};
        SDL_RenderCopy(renderer, _image_texture, nullptr, &destRect);
    } else {
        std::cerr << "Warning: ImageButton has no image texture to render." << std::endl;
    }
}

// following functions toggle the offset of the container's underlying TableWindow
MenuState ScoreDownButton::onClick(Menu* container) const {
        std::cout << "ScoreDown Clicked again ? " << std::endl; //debug
        if (container) {
            container->toggleOffset(PageToggle::DOWN);
        } else {
            std::cout << "Warning: ScoreDownButton did not receive container pointer " << container << std::endl;
        }
        return MenuState::NONE;
};

MenuState ScoreUpButton::onClick(Menu* container) const {
    std::cout << "ScoreUp Clicked again ? " << std::endl; //debug
    if (container) {
        container->toggleOffset(PageToggle::UP);
    } else {
        std::cout << "Warning: ScoreUpButton did not receive container pointer " << container << std::endl;
    }
    return MenuState::NONE;
};

MenuState ScoreBackButton::onClick(Menu* container) const {
    if (container) {
        container->toggleOffset(PageToggle::TOP);
    } else {
        std::cout << "Warning: ScoreUpButton did not receive container pointer " << container << std::endl;
    }
    return MenuState::BACK;
};

MenuState CharacterSelectButton::onClick(Menu* container) const {
    if (container) {
        container->setCharacterSelection(_characterName); // TODO: make this more robust
    } else {
        std::cout << "Warning: CharacterSelectButton did not receive container pointer " << container << std::endl;
    }
    return MenuState::NONE;
}



//  ***************************** Menu Defs *************************************************** //
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
            Button *temp = button.get();
            toggleSelectedButton(temp); 
            return button->onClick(this);
        }
    }
    return MenuState::NONE;
}
//tehchnically can use internal _renderer member... possible TODO
MainMenu::MainMenu(SDL_Renderer* renderer) 
    : Menu(renderer) // Main switches instantly, so disable select effect
    {
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
        _disableSelectEffect = true; // Main switches instantly, so disable effect
}

ScoreMenu::ScoreMenu(SDL_Renderer *renderer) : Menu(renderer) 
{
    _buttons.reserve(3);
    _buttons.emplace_back(std::make_unique<ScoreUpButton>(renderer));
    _buttons.emplace_back(std::make_unique<ScoreDownButton>(renderer));
    _buttons.emplace_back(std::make_unique<ScoreBackButton>(renderer));
    // NOTE: leaving additional params for now in case want to customize each window style
    _window = std::make_unique<Window>(renderer, "High Scores!", SCORE_WINDOW_COLOR, SCORE_WINDOW_BORDER_COLOR, WIN_POSITION);
    _scoreTable = std::make_unique<TableWindow>(SCORE_TABLE_RECT, SCORE_CELL_COLOR, SCORE_CELL_BORDER_COLOR, SCORE_TEXT_COLOR, 5, 2);
    _scoreTable->buildGrid(renderer); // TODO: implement
    _disableSelectEffect = true; 
}
void ScoreMenu::Render() {
    Menu::Render(); // render base window and buttons
    _scoreTable->Render(_renderer); // render score table on top
};



// TODO: again, can make a lot of this shared functionality more polymorphic
void CharacterMenu::generateGridDimensions() 
{

    std::cout << "Number of Characters: " << NUM_CHARACTERS << std::endl; //debug
    int grid_rows = (NUM_CHARACTERS + NUM_CHARACTER_GRID_COLUMNS - 1) / NUM_CHARACTER_GRID_COLUMNS; // ceiling division
     _characterRects.reserve(NUM_CHARACTERS);
    // int const grid_cols = 3;
    // int const grid_rows = (NUM_CHARACTERS + grid_cols - 1) / grid_cols; // ceiling division
    int const cell_width = (WIN_POSITION.w - (NUM_CHARACTER_GRID_COLUMNS + 1) * CHARACTER_BUTTON_PADDING_X) / NUM_CHARACTER_GRID_COLUMNS;
    // int const cell_height = (WIN_POSITION.h - (NUM_CHARACTER_GRID_COLUMNS + 1) * CHARACTER_BUTTON_PADDING_Y) / grid_rows;
    int const cell_height = cell_width; // Default to square cells

    for (int i = 0; i < NUM_CHARACTERS; ++i) {
        int row = i / NUM_CHARACTER_GRID_COLUMNS;
        int col = i % NUM_CHARACTER_GRID_COLUMNS;
        SDL_Rect rect = {
            WIN_POSITION.x + CHARACTER_BUTTON_PADDING_X + col * (cell_width + CHARACTER_BUTTON_PADDING_X),
            WIN_POSITION.y + CHARACTER_BUTTON_PADDING_Y + row * (cell_height + CHARACTER_BUTTON_PADDING_Y) + CHARACTER_TOP_OFFSET,
            cell_width,
            cell_height
        };
        _characterRects.push_back(rect);
        std::cout << "Character Rect " << i << ": (" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << ")\n"; //debug
    }
}

CharacterMenu::CharacterMenu(SDL_Renderer *renderer) : Menu(renderer){ 

    generateGridDimensions();
    assert(_characterRects.size() == NUM_CHARACTERS && "Something off with num characters grid gen"); // sanity check
    // assert(CHARACTER_GRID_COLUMNS <= characterNames.size() && 
    // "(CHARACTER_GRID_COLUMNS) Use at least many columns as characters for better formatting results")

    _buttons.reserve(NUM_CHARACTERS + 1);
    _buttons.emplace_back(std::make_unique<BackButton>(_renderer, BACK_BUTTON_COLOR, CHARACTER_BACK_BUTTON_RECT)); // TODO (Maybe): Make an "Enter" or "Ok" button
    for ( int i = 0; i < NUM_CHARACTERS; ++i) {
        _buttons.emplace_back(
            std::make_unique<CharacterSelectButton>(_renderer, i, _characterRects[i])
        );
    }

    _window = std::make_unique<Window>(renderer, "Select Your Character:", WINDOW_COLOR, WINBORDER_COLOR, WIN_POSITION);

}

PlayerEntryMenu::PlayerEntryMenu(SDL_Renderer *renderer) : Menu(renderer) 
{
    _buttons.reserve(2);
    _buttons.emplace_back(std::make_unique<BackButton>(renderer)); // TODO: Make an "Enter" or "Ok" button
    _buttons.emplace_back(std::make_unique<StartButton>(renderer)); // TODO: Make an "Enter" or "Ok" button
    _window = std::make_unique<Window>(renderer, "Enter Player Name: ", WINDOW_COLOR, WINBORDER_COLOR, WIN_POSITION);
    // _textEntry = std::make_unique<InputWindow>(renderer, NAME_WIN_COLOR, NAME_WIN_BORDER_COLOR, NAME_WIN_POSITION, &_playerName);
    
    //TODO: rename Cell to DynamicWindow or something (or Cell to InputWindow)
    _textEntry = std::make_unique<Cell>(
        renderer,
        _playerName, //Initial Text
        NAME_WIN_COLOR,
        NAME_WIN_BORDER_COLOR,
        NAME_WIN_POSITION, 
        28,
        NAME_WIN_TEXT_COLOR
        // TODO: make dynamic based on window size
    );
    _disableSelectEffect = true; 
}
void PlayerEntryMenu::Render() {
    Menu::Render(); // render base window and buttons
    toggleCursor(); // handle cursor blinking
    //NOTE: probably want to (TODO) make a separate display string to avoid erroneous |'s added to name

    // std::string temp = _playerName + _cursor;
    _textEntry->UpdateText(_playerName + _cursor); // update text with current name (and cursor if visible)
    _textEntry->leftJustify(); // align text to left
    _textEntry->Render(_renderer); // render input window on top
}

std::string PlayerEntryMenu::getPlayerName() {
    StringUtils::replace(_playerName, "|", ""); // remove cursor if present
    return StringUtils::strip(_playerName); // remove leading/trailing whitespace
}

MenuState PlayerEntryMenu::getNameInput(const SDL_Event &event){

    if (event.type == SDL_QUIT) {
        // Handle quit
    }
    else if (event.type == SDL_TEXTINPUT) {
        // Append event.text.text to string buffer
        _playerName += event.text.text;
        std::cout << "Keystroke: " << _playerName << std::endl;
    }
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && _playerName.size() > 0) {
            _playerName.pop_back();  // Handle backspace
            if (_playerName.length() == 0) {
                _playerName = " "; // Add cursor if name is empty
            }
            std::cout << "After backspace: " << _playerName << std::endl;
        }
    }
    return MenuState::NONE; // or some other default state
};

void PlayerEntryMenu::toggleCursor()
{
    Uint32 current_time = SDL_GetTicks();
    if (current_time -  _lastToggleTime > CURSOR_BLINK_INTERVAL_MS) {
        _cursor = (_cursor == '|') ? ' ' : '|';
        _lastToggleTime = current_time;
    }
}


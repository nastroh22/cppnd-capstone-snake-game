#include <string>
#include <iostream>
#include <algorithm> 
#include <cassert>

#include "SDL.h"
#include "SDL_ttf.h"
#include "menu.h"
#include "./utils.h" //move to utils


// ***************************** Custom Button Behaviors *************************************************** //
// following functions toggle the offset of the container's underlying Table
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
        _window = std::make_unique<Window>(
            renderer, 
            MainConst.TITLE_TEXT,
            MainConst.WINDOW_COLOR, 
            MainConst.WINBORDER_COLOR, 
            WINDOW_POSITION,
            MainConst.TITLE_FONT_SIZE
        );
        _disableSelectEffect = true; // Main switches instantly, so disable effect
}

ScoreMenu::ScoreMenu(SDL_Renderer *renderer) : Menu(renderer) 
{
    _buttons.reserve(3);
    _buttons.emplace_back(std::make_unique<ScoreUpButton>(renderer));
    _buttons.emplace_back(std::make_unique<ScoreDownButton>(renderer));
    _buttons.emplace_back(std::make_unique<ScoreBackButton>(renderer));
    // NOTE: leaving additional params for now in case want to customize each window style
    _window = std::make_unique<Window>(
        renderer,
        ScoreConst.TITLE_TEXT,
        ScoreConst.SCORE_WINDOW_COLOR, 
        ScoreConst.SCORE_WINDOW_BORDER_COLOR, 
        WINDOW_POSITION,
        ScoreConst.TITLE_FONT_SIZE,
        ScoreConst.TITLE_TEXT_COLOR,
        ScoreConst.TITLE_FONT,
        ScoreConst.TOP_TITLE_OFFSET
    );
    std::cout << ScoreConst.TABLE_FONT_NAME << "<-- Table Font Name" << std::endl;
    _scoreTable = std::make_unique<Table>(
        SCORE_TABLE_RECT, 
        ScoreConst.SCORE_CELL_COLOR, 
        ScoreConst.SCORE_CELL_BORDER_COLOR, 
        ScoreConst.SCORE_TEXT_COLOR,
        ScoreConst.TABLE_FONT_NAME,
        ScoreConst.NUM_ROWS,
        ScoreConst.NUM_COLS
    );
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
    int grid_rows = (NUM_CHARACTERS + CharConst.NUM_COLUMNS - 1) / CharConst.NUM_COLUMNS; // ceiling division
     _characterRects.reserve(NUM_CHARACTERS);
    int const cell_width = (WINDOW_POSITION.w - (CharConst.NUM_COLUMNS + 1) * CharConst.BUTTON_PADDING_X) / CharConst.NUM_COLUMNS;
    int const cell_height = cell_width; // Default square cells
    for (int i = 0; i < NUM_CHARACTERS; ++i) {
        int row = i / CharConst.NUM_COLUMNS;
        int col = i % CharConst.NUM_COLUMNS;
        SDL_Rect rect = {
            WINDOW_POSITION.x + CharConst.BUTTON_PADDING_X + col * (cell_width + CharConst.BUTTON_PADDING_X),
            WINDOW_POSITION.y + CharConst.BUTTON_PADDING_Y + row * (cell_height + CharConst.BUTTON_PADDING_Y) + CharConst.TITLE_OFFSET,
            cell_width,
            cell_height
        };
        _characterRects.push_back(rect);
    }
}

CharacterMenu::CharacterMenu(SDL_Renderer *renderer) : Menu(renderer)
{ 
    generateGridDimensions();
    assert(_characterRects.size() == NUM_CHARACTERS && "Something off with num characters grid gen"); // sanity check
    _buttons.reserve(NUM_CHARACTERS + 1);
    _buttons.emplace_back(
        std::make_unique<CharBackButton>(
            _renderer, 
            CharConst.BACK_BUTTON_COLOR, 
            CharConst.BACK_BUTTON_RECT
        )
    ); // TODO (Maybe): Make this "Enter" or "Ok" 
    for ( int i = 0; i < NUM_CHARACTERS; ++i) {
        _buttons.emplace_back(
            std::make_unique<CharacterSelectButton>(_renderer, i, _characterRects[i])
        );
    }  
    _window = std::make_unique<Window>(
        renderer, 
        CharConst.TITLE_TEXT, 
        CharConst.WINDOW_COLOR, 
        CharConst.WINBORDER_COLOR, 
        WINDOW_POSITION, //calculated
        CharConst.TITLE_FONT_SIZE,
        CharConst.TITLE_TEXT_COLOR,
        CharConst.TITLE_FONT,
        CharConst.TOP_TITLE_OFFSET
    );
}

PlayerEntryMenu::PlayerEntryMenu(SDL_Renderer *renderer) : Menu(renderer) 
{
    _buttons.reserve(2);
    _buttons.emplace_back(std::make_unique<BackButton>(renderer)); // TODO: Make an "Enter" or "Ok" button
    _buttons.emplace_back(std::make_unique<StartButton>(renderer)); // TODO: Make an "Enter" or "Ok" button
    _window = std::make_unique<Window>(
        renderer, 
        PlayerConst.TITLE_TEXT, 
        PlayerConst.WINDOW_COLOR, 
        PlayerConst.WINBORDER_COLOR, 
        WINDOW_POSITION,
        PlayerConst.TITLE_FONT_SIZE,
        PlayerConst.TITLE_TEXT_COLOR,
        PlayerConst.TITLE_FONT,
        PlayerConst.TITLE_OFFSET

    );    
    _textEntry = std::make_unique<DynamicWindow>(
        renderer,
        _playerName, //Initial Text
        PlayerConst.NAME_WINDOW_COLOR,
        PlayerConst.NAME_WINDOW_BORDER_COLOR,
        NAME_WINDOW_POSITION,  //calculated values
        PlayerConst.NAME_TEXT_FONT_SIZE,
        PlayerConst.NAME_WINDOW_TEXT_COLOR,
        PlayerConst.NAME_TEXT_FONT
    );
    _disableSelectEffect = true; 
}
void PlayerEntryMenu::Render() {
    Menu::Render();
    toggleCursor();
    _textEntry->UpdateText(_playerName + _cursor); 
    _textEntry->leftJustify();
    _textEntry->Render(_renderer);
}

std::string PlayerEntryMenu::getPlayerName() {
    //StringUtils::replace(_playerName, "|", ""); // remove cursor if present (TODO: shouldn't be necessary now)
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
    if (current_time -  _lastToggleTime > PlayerConst.CURSOR_BLINK_INTERVAL_MS) {
        _cursor = (_cursor == '|') ? ' ' : '|';
        _lastToggleTime = current_time;
    }
}


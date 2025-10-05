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
    : Menu(renderer), _renderer(renderer) // Main switches instantly, so disable select effect
    {
        _buttons.reserve(4); // reserve space for 4 buttons
        _buttons.emplace_back(std::make_unique<PlayButton>(renderer)); 
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
            MainConst.SAMMY_WINDOW_COLOR, 
            MainConst.SAMMY_BORDER_COLOR, 
            WINDOW_POSITION,
            MainConst.TITLE_FONT_SIZE,
            MainConst.SAMMY_TITLE_COLOR // Init with the Sammy Theme
        );
        _disableSelectEffect = true; // Main switches instantly, so disable effect

        // animation effect
        SDL_Rect window_dims = _window->getDimensions();
        SDL_Rect button_dims = _buttons[_buttons.size()-1]->getDims(); // assuming all buttons same size
        std::cout << "Which button for route ?? "; _buttons[_buttons.size()-1]->printLabel();
        std::cout << "Window Dims: " << window_dims.x << " " << window_dims.y << " " << window_dims.w << " " << window_dims.h << std::endl;
        std::cout << "Button Dims: " << button_dims.x << " " << button_dims.y << " " << button_dims.w << " " << button_dims.h << std::endl;
        

        auto map_to_grid = [&](SDL_Point point) {
            std::cout << "Map Constants: " << (kGridWidth/kScreenWidth) << " " << (kGridHeight/kScreenHeight) << std::endl;
            return SDL_Point{
                static_cast<int>((point.x * kGridWidth)/kScreenWidth),
                static_cast<int>((point.y * kGridHeight)/kScreenHeight)
            };
        };
        
        SDL_Point start = {
            (window_dims.x + button_dims.x)/2 + 16,
            (window_dims.y + window_dims.h)/3
        };
        SDL_Point end = {
            ((window_dims.x + window_dims.w) + (button_dims.x + button_dims.w))/2,
            (window_dims.y + window_dims.h)/3
        };
        SDL_Point start_bottom = {
            start.x,
            ((button_dims.y + button_dims.h) + (window_dims.y + window_dims.h))/2 + 20
        };
        SDL_Point end_bottom = { 
            end.x , start_bottom.y
        };
        SDL_Point left_top = {
            (start.x - 32),
            start.y 
        };
        SDL_Point left_bottom = {
            (start.x - 32),
            start_bottom.y 
        };
        SDL_Point right_top = {
            (end.x + 32),
            end.y 
        };
        SDL_Point right_bottom = {
            (end.x + 32),
            start_bottom.y 
        };


        // std::cout << "Screen Space Path" << std::endl;
        // std::cout << "Start Point: " << start.x << " " << start.y << std::endl;
        // std::cout << "Left Point: " << left.x << " " << left.y << std::endl;
        // std::cout << "Right Point: " << right.x << " " << right.y << std::endl;
        // std::cout << "End Point: " << end.x << " " << end.y << std::endl;

        path.reserve(4);
        path.emplace_back(start);
        path.emplace_back(left_top);
        path.emplace_back(left_bottom);
        path.emplace_back(end_bottom);
        path.emplace_back(end);
        path.emplace_back(right_top);
        path.emplace_back(right_bottom);
        path.emplace_back(start_bottom);


        for_each(path.begin(), path.end(), [&](auto &point) {point = map_to_grid(point);});
        std::cout << "Grid Space Path" << std::endl;
        std::cout << "Start Point: " << path[0].x << " " <<  path[0].y << std::endl;
        std::cout << "Left Point: " <<  path[1].x << " " << path[1].y << std::endl;
        std::cout << "Right Point: " <<  path[2].x << " " << path[2].y << std::endl;
        std::cout << "End Point: " <<  path[3].x << " " << path[3].y << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(5)); // pause for effect
        
        // For Body Rendering it is better to updates in the reduced "grid" space

        // store Textures for animation
        _sammyTexture = RenderUtils::InitTexture(renderer, characterFileMap.at(static_cast<CharacterEnum>(0))[0]);    
        _sammyBody = RenderUtils::InitTexture(renderer, characterFileMap.at(static_cast<CharacterEnum>(0))[1]);
        _sandyTexture = RenderUtils::InitTexture(renderer, characterFileMap.at(static_cast<CharacterEnum>(1))[0]); 
        _sandyBody = RenderUtils::InitTexture(renderer, characterFileMap.at(static_cast<CharacterEnum>(1))[1]); 

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


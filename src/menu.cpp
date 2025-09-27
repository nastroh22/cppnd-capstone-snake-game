#include "SDL.h"
#include "menu.h"
#include "SDL_ttf.h"
#include <string>
#include <iostream>
#include <algorithm> 
#include "scoreio.h"
#include <cassert>

//***************************** Helper ***************************************** */

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

std::string strip(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    // Find first non-whitespace character
    while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    // Find last non-whitespace character
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
    }

    return str.substr(start, end - start);
}

// ***************************** Text Defs  *********************************** //
Text::Text(
    SDL_Renderer *renderer,
    const std::string &font_path,
    int font_size,
    const std::string &message_text,
    SDL_Color color) : _color(color)
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

void Text::displayDynamic(SDL_Renderer *renderer, int x, int y, const std::string &message) {
    _text_rect.x = x;
    _text_rect.y = y;
    if (_text_texture) {
        SDL_DestroyTexture(_text_texture);
        _text_texture = nullptr;
    }
    _text_texture = loadFont(renderer, "../assets/fonts/comic_sans_ms.ttf", 24, message, _color); // TODO - store color and font_size
    SDL_QueryTexture(_text_texture, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
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
    // std::cout << "Loaded Font" << std::endl;

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
        _windowRect.y + 10
    );
}

// NOTE: can actually deprecate this custom render and just store this loagic at class level with _text_x, _text_y
void Cell::Render(SDL_Renderer* renderer){
    // draw color
    SDL_SetRenderDrawColor(renderer, _windowColor.r,  _windowColor.g,  _windowColor.b,  _windowColor.a);
    SDL_RenderFillRect(renderer, &_windowRect);

    // draw border
    drawBorder(renderer, _windowRect, 2, _borderColor); // thickness of 2
    if (_shouldUpdate) {
        _title.displayDynamic(renderer, _text_x, _text_y, _cellText);
        _shouldUpdate = false;
    } else {
        _title.display(renderer, _text_x, _text_y);
    }
}

void InputWindow::Render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, _windowColor.r,  _windowColor.g,  _windowColor.b,  _windowColor.a);
    SDL_RenderFillRect(renderer, &_windowRect);
    drawBorder(renderer, _windowRect, 1, _borderColor); // thickness of 2
    // render dynamic text if applicable (NOTE: need to clean this text)
    // Better Approach is interior window "DynamicText" object that can be updated or rather "InputWindow"
    // std::cout << "Check Width" << _windowRect.x + 20 << " " << _windowRect.y + 60 << std::endl; //debug
    if ((_playerInput) && !_playerInput->empty()) {
        // std::cout << "Should Render " << *_playerInput << std::endl; //debug
        // std::cout <<  _windowRect.x << std::endl;
        std::cout << _playerInput->empty() << std::endl;
        _inputText.displayDynamic(renderer, 
            _windowRect.x + 20, // some padding from left
            _windowRect.y + 20, // below title
            *_playerInput
        );
    }
}

//  ***************************** Button Defs  *********************************** //
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
    drawBorder(renderer, _buttonRect, _borderThickness, _borderColor); // thickness of 2

    // render text
    _text.display(renderer, _textX, _textY);
}

void ImageButton::Render(SDL_Renderer* renderer){

    // draw color
    SDL_SetRenderDrawColor(renderer, _buttonColor.r,  _buttonColor.g,  _buttonColor.b,  _buttonColor.a);
    SDL_RenderFillRect(renderer, &_buttonRect);

    // draw border
    drawBorder(renderer, _buttonRect, _borderThickness, _borderColor); // thickness of 2

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



// ***************************** Table Defs *********************************** //
void TableWindow::buildGrid(SDL_Renderer *renderer) {
    // TODO: this is not yet general, builds in assumptions about scoreTable (i.e. data type, cols=2)
    // possibly use overloading

    int const cellWidth = _tableRect.w / _cols;
    int const cellHeight = _tableRect.h / _rows; // TODO: make scrollable
    gridData = ScoreIO::load_scores(); 


    // gridData.clear(); // should only be built once, but in case
    // gridData.resize(_rows, std::vector<std::string>(_cols, "")); // initialize with empty strings

    gridSpec.clear(); // should only be built once, but in case
    gridSpec.resize(_rows);
    std::cout << "Inside buildGrid, gridSpec size: " << gridSpec.size() << gridData.size() << std::endl; //debug

    for (int i = 0; i < _rows; ++i) {
        
        std::cout << "tries to get griddata" << std::endl; //debug
        std::vector<std::string> entry = gridData[i];
        gridSpec[i].reserve(_cols); // avoid multiple allocations
        
        for (int j = 0; j < _cols; ++j) {
            // std::string initial_text = (j == 0) ? 
            //     entry.name : std::to_string(entry.score);
            SDL_Rect cellRect = { //tight layout
                _tableRect.x + j * cellWidth,
                _tableRect.y + i * cellHeight,
                cellWidth,
                cellHeight
            };
            gridSpec[i].emplace_back(
                renderer, 
                gridData[i][j], // initial text
                _cellColor,
                _cellBorderColor, 
                cellRect, 
                _textFontSize, 
                _textColor
            );
            if (j % 2 == 0) {gridSpec[i][j].leftJustify();} // name column
        }
    }
    for (const auto& item : gridData) {
        std::cout << "Score Data: " << item[0] << ", " << item[1] << std::endl; //debug
    }
}

void TableWindow::UpdateCells(int offset){
    for (int i = 0; i < _rows; ++i) {
        for (int c = 0; c < _cols; ++c) {
            int r = i + offset;
            gridSpec[i][c].UpdateText(gridData[r][c]);
        }
    }
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
    std::cout << "Mouse Click Lag? " << mouseClick.x << " " << mouseClick.y << std::endl;
    for (auto& button : _buttons) {
        if (button->wasClicked(mouseClick)) {
            buttonClicked = true;
            Button *temp = button.get();
            toggleSelectedButton(temp); 
            button->printLabel();
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
    _textEntry = std::make_unique<InputWindow>(renderer, NAME_WIN_COLOR, NAME_WIN_BORDER_COLOR, NAME_WIN_POSITION, &_playerName);
    _disableSelectEffect = true; 
}
void PlayerEntryMenu::Render() {
    Menu::Render(); // render base window and buttons
    _textEntry->Render(_renderer); // render input window on top
}

std::string PlayerEntryMenu::getPlayerName() const {
    return strip(_playerName); // remove leading/trailing whitespace
}

MenuState PlayerEntryMenu::getNameInput(const SDL_Event &event){
    std::cout << "Getting Name Input: " << _playerName << std::endl; //debug
    if (event.type == SDL_QUIT) {
        // Handle quit
    }
    else if (event.type == SDL_TEXTINPUT) {
        // Append event.text.text to your string buffer
        _playerName += event.text.text;
        std::cout << "Keystroke: " << _playerName << std::endl;
    }
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && _playerName.size() > 0) {
            _playerName.pop_back();  // Handle backspace
            std::cout << "After backspace: " << _playerName << std::endl;
        }
    }
    return MenuState::NONE; // or some other default state
}


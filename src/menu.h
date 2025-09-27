#pragma once

// #include <array>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "constants.h" //include Screen Dimensions
#include "scoreio.h"
#include "utils.h" 
#include "SDL.h"
#include "SDL_ttf.h"



/* "Dynamic Constants"
------------------------------- */

//Window Positions
static int border_width = static_cast<int>(.15*kScreenWidth);
static int border_height = static_cast<int>(.10*kScreenHeight);
const SDL_Rect WIN_POSITION = {
    border_width, 
    border_height,
    static_cast<int>(kScreenWidth - 2*border_width), 
    static_cast<int>(kScreenHeight-2*border_height)
};
constexpr SDL_Color WINDOW_COLOR = FOREST;
constexpr SDL_Color WINBORDER_COLOR = MOSS;

// const SDL_Rect NAME_WIN_POSITION= {
//     static_cast<int>(.05*kScreenWidth),
//     static_cast<int>(.10*kScreenHeight),
//     static_cast<int>(.90*kScreenWidth),
//     static_cast<int>(.15*kScreenHeight)
// };
const SDL_Rect NAME_WIN_POSITION = {
    static_cast<int>(1.2*border_width), 
    static_cast<int>(2*border_height),
    static_cast<int>(kScreenWidth - 2.4*border_width),
    80
};

// NOTE: button rectangles generated dynamically based on screen size and number of characters
const int NUM_CHARACTERS = static_cast<size_t>(sizeof(CHARACTER_NAMES) / sizeof(CHARACTER_NAMES[0]));


// "IIFE" (Immediately Invoked Function Expression), lambda is void so no assignment // could also move to apprpriate spot
static auto init = []() {
    for  (int i = 0; i < NUM_CHARACTERS; ++i){
        characterEnumMap[CHARACTER_NAMES[i]] = static_cast<CharacterEnum>(i);
    }; 
    return 0;
}();

const SDL_Rect SCORE_TABLE_RECT = {
    WIN_POSITION.x + 10,
    WIN_POSITION.y + 60, // need room for Text
    static_cast<int>(WIN_POSITION.w - 20), 
    static_cast<int>(WIN_POSITION.h - 160) // pad 100 for buttons
};
constexpr SDL_Color SCORE_UP_BUTTON_COLOR = MINT;
constexpr SDL_Color SCORE_DOWN_BUTTON_COLOR = DEEPBLUE;

const int score_button_pad = 10;
const int score_button_width = static_cast<int>(SCORE_TABLE_RECT.w/3) - score_button_pad; // pad for spacing

// Positions are rel calcs, leave as is 
const SDL_Rect SCORE_UP_BUTTON_RECT = {
        SCORE_TABLE_RECT.x, 
        SCORE_TABLE_RECT.y + SCORE_TABLE_RECT.h + 10, // below table
        score_button_width, 
        80
};
const SDL_Rect SCORE_DOWN_BUTTON_RECT = {
    SCORE_TABLE_RECT.x + score_button_width + score_button_pad, 
    SCORE_TABLE_RECT.y + SCORE_TABLE_RECT.h + 10, 
    score_button_width, 
    80
};
const SDL_Rect SCORE_BACK_BUTTON_RECT = {
    SCORE_TABLE_RECT.x + 2*(score_button_width + score_button_pad),
    SCORE_TABLE_RECT.y + SCORE_TABLE_RECT.h + 10,
    score_button_width,
    80
};


// Character Menu Positions
// constexpr SDL_Rect CHARACTER_BUTTON_RECT = {220, 350, 200, 75};

// TODO : Font Options
// maybe an even cooler feature... add themes (future)

//TODO: String Utils ?
// #include <algorithm>
// #include <string>
// #include <cctype>

// std::string to_lower(const std::string& input) {
//     std::string result = input;
//     std::transform(result.begin(), result.end(), result.begin(),
//                    [](unsigned char c) { return std::tolower(c); });
//     return result;
// }


// State Enum (convenience)
enum class MenuState {
    MAIN_MENU,
    SCORE_MENU,
    CHARACTER_MENU,
    PLAYER_NAME,
    PLAY,
    BACK,
    NONE,
    QUIT
};

// Rule of 5 Macro:
#define DISABLE_COPY_ENABLE_MOVE(ClassName)            \
    ClassName(const ClassName&) = delete;              \
    ClassName& operator=(const ClassName&) = delete;   \
    ClassName(ClassName&&) noexcept = default;         \
    ClassName& operator=(ClassName&&) noexcept = default;

// Generic Text Object, based on youtube tutorial. Nice pattern, reusable for buttons.
class Text {
    SDL_Color _color = DEFAULT_TEXT_COLOR; // default color
    public:
        Text(SDL_Renderer *renderer,const std::string &font_path, int font_size, const std::string &message_text, SDL_Color color);
        void loadFont(SDL_Renderer *renderer,const std::string &font_path, int font_size);
        SDL_Texture *makeTexture(SDL_Renderer *renderer, const std::string &message_text, SDL_Color& color);
        void display(SDL_Renderer *renderer, int x, int y) const;
        void displayDynamic(SDL_Renderer *renderer, int x, int y, const std::string &message);
        ~Text(){ 
            std::cout << "Destroying Text Object" << std::endl;
            SDL_DestroyTexture(_text_texture);
            TTF_CloseFont(_font); //NOTE: One of the fonts becomes nullptr (I think from PlayerEntryMenu)
        }; //free textures
        int getWidth() const { return _text_rect.w;}
        int getHeight() const { return _text_rect.h;}
    
        // Rule of 5 (since Text manages an SDL_Texture resource)
        Text(const Text&) = delete;            // prevent accidental copies
        Text& operator=(const Text&) = delete; // prevent accidental copies
        Text(Text&& other) noexcept             // move constructor
            : _text_texture(other._text_texture), _text_rect(other._text_rect), _color(other._color) {
            other._text_texture = nullptr; // null out other's pointer to avoid double free
        }
        Text& operator=(Text&& other) noexcept { // move assignment
            if (this != &other) {
                SDL_DestroyTexture(_text_texture); // free existing resource
                _text_texture = other._text_texture;
                _text_rect = other._text_rect;
                _color = other._color;
                other._text_texture = nullptr; // null out other's pointer to avoid double free
            }
            return *this;
        }

    private:
        SDL_Texture *_text_texture;
        mutable SDL_Rect _text_rect = {0,0,20,20}; // for positioning text, mutable so that display can be const
        TTF_Font *_font = nullptr;
};

// **************** Window Compoenents ********************************************************
class Window {
    protected:
        SDL_Rect _windowRect; // for positioning button
        SDL_Color _windowColor; // for button color
        SDL_Color _borderColor = DEFAULT_BORDER_COLOR; // could make customizable
        Text _title;
        // Text _content; // could be vector of texts for multiple lines
    
    public:
        Window(SDL_Renderer* renderer, const std::string& title, SDL_Color win_color, 
                SDL_Color border_color, SDL_Rect rect, int text_font_size = 32, SDL_Color text_color = TITLE_COLOR) : 
                    _windowRect(rect), _windowColor(win_color), _borderColor(border_color),
                        _title(renderer, "../assets/fonts/comic_sans_ms.ttf", text_font_size, title, text_color) {};
        ~Window() = default; // will call _text destructor by default
        
        DISABLE_COPY_ENABLE_MOVE(Window);


        void virtual Render(SDL_Renderer* renderer);
};

// TODO: a Window and a cell are very similar, could refactor to avoid code duplication
class Cell : public Window {
    int _text_x, _text_y;
    std::string _cellText;
    bool _shouldUpdate = false;

    public:
        Cell(SDL_Renderer* renderer, const std::string& text, SDL_Color cell_color, 
                SDL_Color border_color, SDL_Rect rect, int text_font_size = 24, SDL_Color text_color = SCORE_TEXT_COLOR) :
                    Window(renderer, text, cell_color, border_color, rect, text_font_size, text_color), _cellText(text) 
                    {
                        std::cout << "Created Cell with text: " << text << std::endl;
                        _text_x = _windowRect.x + (_windowRect.w - _title.getWidth()) / 2;
                        _text_y = _windowRect.y + (_windowRect.h - _title.getHeight()) / 2;
                    };
        ~Cell() = default; // will call _text destructor by default
        DISABLE_COPY_ENABLE_MOVE(Cell);
        virtual void Render(SDL_Renderer* renderer) override;
        void UpdateText(const std::string &new_text) {
            _cellText = new_text; _shouldUpdate = true;
        }
        void leftJustify() {_text_x = _windowRect.x + 15 ;}
        void rightJustify() {_text_x = _windowRect.x + _windowRect.w - _title.getWidth() - 15 ;}
        void setX(int x) {_text_x = x;}
        void setY(int y) {_text_y = y;}
        int getX() const { return _text_x; }
        int getY() const { return _text_y; }
        void setTextPos(int x, int y) { _text_x = x; _text_y = y; }
        // Next TODO: either reduce Text size (if goes out of bounds or replace with ... and return option for player to view it)
};

enum PageToggle {UP, DOWN, TOP, NONE};

class TableWindow  {
    // A Grid of Cells with Static Text (as opposed to InputWindow, which is dynamic)
    // although since switch to dynamic, could possibly be refactored with just unique functions 
    int const _rows;
    int const _cols;
    SDL_Rect const _tableRect; 
    SDL_Color const _cellColor; 
    SDL_Color const _cellBorderColor; 
    SDL_Color const _textColor;
    int const _textFontSize = 24; 
    std::vector<std::vector<Cell>> gridSpec;
    std::vector<std::vector<std::string>> gridData;
    int _offset = 0; //for scrolling

    public:
        TableWindow(SDL_Rect tableDims, SDL_Color cellColor = WHITE, SDL_Color cellBorderColor = BLACK, SDL_Color cellTextColor = WHITE, int rows=5, int cols=2) :
            _tableRect(tableDims), _cellColor(cellColor), _cellBorderColor(cellBorderColor), _textColor(cellTextColor), _rows(rows), _cols(cols) {};
        ~TableWindow() = default;
        // DISABLE_COPY_ENABLE_MOVE(TableWindow);
        void buildGrid(SDL_Renderer *renderer);
        void UpdateCells(int start_index); 
        void Render(SDL_Renderer* renderer) {
            for (auto& row : gridSpec) {
                for (auto& cell : row) {
                    cell.Render(renderer);
                }
            }
        }
        int getRows() const { return _rows; }
        void toggleOffset(PageToggle direction) {
            if (direction == UP && _offset > 0) {
                _offset -= _rows;
                UpdateCells(_offset);
            } else if (direction == DOWN && (_offset + _rows) < static_cast<int>(gridData.size())) {
                _offset += _rows;
                UpdateCells(_offset);
            }
            else if (direction == TOP) {
                resetOffset();
            }
            else{
                std::cout << "(TableWindow) Cannot scroll further in that direction: " 
                    << _offset <<  " " << gridData.size() << " " <<  direction << std::endl;
            }
        }
        void resetOffset() { _offset = 0; UpdateCells(_offset);}
};

//TODO: Possibly refactor this (reuse Cell), because a reference should achieve the same thing than explictly passing pointer
class InputWindow : public Window {
    const std::string* _playerInput;
    Text _inputText;
    public:
        InputWindow(SDL_Renderer* renderer, SDL_Color win_color, SDL_Color border_color, SDL_Rect rect, const std::string* dynamic) :
            Window(renderer, "", win_color, border_color, rect),
            _playerInput(dynamic),
             _inputText(
                renderer,
                "../assets/fonts/comic_sans_ms.ttf", 
                28, 
                dynamic ? *dynamic : "", // avoids null deref
                NAME_WIN_TEXT_COLOR)
            {
                if (!dynamic) {
                    std::cerr << "Warning: InputWindow initialized with null player input string pointer." << std::endl;
                }
            };
        // void Render(SDL_Renderer* renderer) override; // could override to add cursor
        ~InputWindow() = default; // will call _text destructor by default
        // DISABLE_COPY_ENABLE_MOVE(InputWindow);
        void Render(SDL_Renderer* renderer) override;
};

// TODO: Possibly Inherit Button From Window 
// IDEA: new class TextWindow where Button should inherit rather than this (slighlty convoluted) way 
// class TextWindow : public Button {
//     // A Button that looks like a Window (for table cells)
//     public:
//     TextWindow(SDL_Renderer* renderer, MenuState label, SDL_Color color, SDL_Rect rect, const std::string& text)
//         : Button(renderer, label, color, rect, text) {label = text;}
//     ~TextWindow() = default;
//     bool wasClicked(const SDL_Point& mouse_point) const = delete;
//     bool isHovered(const SDL_Point& mouse_point) const = delete;
//     void toggleHover(const SDL_Point& mouse_point);
// }



// ******************** Base Button Type ************************************************************ 
// TODO : A Button Can Inherit From a Cell
class Menu; // forward declaration needed
class Button {

protected:
    Text _text;
    const SDL_Rect _buttonRect; // for positioning button
    const SDL_Color _buttonColor; // for button color
    const MenuState _return_state = MenuState::NONE; // default state
    SDL_Color _borderColor = DEFAULT_BORDER_COLOR; // non-const to enable hover effect
    std::string label = "Button";
    int _borderThickness = 2; // could make customizable
    bool _freeze_border = false; // to keep border on selected button
    int _textX, _textY; // for text positioning


// I think the protected constructor pattern works to make sure no abstract button can be created // TODO re-use for "Menu"
    Button(SDL_Renderer* renderer, MenuState label, SDL_Color color, SDL_Rect rect, const std::string& text)
        : _return_state(label), _buttonColor(color), _buttonRect(rect), 
        _text(renderer, "../assets/fonts/comic_sans_ms.ttf", 24, text, DEFAULT_TEXT_COLOR) 
        {
           // default centered text
           _textX =  _buttonRect.x + (_buttonRect.w - _text.getWidth()) / 2;
           _textY = _buttonRect.y + (_buttonRect.h - _text.getHeight()) / 2;
        }

public:
    virtual ~Button() = default; // will call _text destructor by default

    virtual void Render(SDL_Renderer* renderer); //virtual ?
    bool wasClicked(const SDL_Point& mouse_point) const { 
        return SDL_PointInRect(&mouse_point, &_buttonRect);
    }
    bool isHovered(const SDL_Point& mouse_point) const { 
        return SDL_PointInRect(&mouse_point, &_buttonRect);
    }
    void toggleHover(const SDL_Point& mouse_point);
    void setX(int x) { _textX = x; }
    void setY(int y) { _textY = y; }

    void unselect();
    void select();
    
    virtual MenuState onClick(Menu* container = nullptr) const { return _return_state; } // buttons don't mutate their own state, but can mutate container state

    // for debugging / check formats
    void printProperties() const {
        std::cout << "Button Label: " << label << "\n";
        std::cout << "Button Rect: (" << _buttonRect.x << ", " << _buttonRect.y << ", " << _buttonRect.w << ", " << _buttonRect.h << ")\n";
        std::cout << "Button Color: (" << static_cast<int>(_buttonColor.r) << ", " 
                  << static_cast<int>(_buttonColor.g) << ", " 
                  << static_cast<int>(_buttonColor.b) << ", " 
                  << static_cast<int>(_buttonColor.a) << ")\n";
        std::cout << "Return State: " << static_cast<int>(_return_state) << "\n"; // cast enum to int for display
    }
    void printLabel() const { std::cout << "Button Label: " << label << std::endl; }

};

class ImageButton : public Button 
{
    std::string _asset_path;
    SDL_Texture* _image_texture = nullptr;

    public:
    ImageButton(SDL_Renderer* renderer, MenuState label, SDL_Color color, SDL_Rect rect, const std::string& text, std::string image_asset_path)
        : Button(renderer, label, color, rect, text), _asset_path(std::move(image_asset_path))
    {
        // load image texture
        _image_texture = RenderUtils::InitTexture(renderer, _asset_path); // figure out a better way here
    }
    ~ImageButton() override {
        if (_image_texture) {
            SDL_DestroyTexture(_image_texture);
            _image_texture = nullptr;
        }
    }
    // DISABLE_COPY_ENABLE_MOVE(ImageButton);
    // MenuState onClick(Menu* container = nullptr) const override;
    void Render(SDL_Renderer* renderer) override;

};

// ********************** Custom Buttons ************************************************************ 
class PlayButton : public Button {
public: // TODo: Rename the Top Menu Button to "Start"
    PlayButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::PLAYER_NAME, PLAY_BUTTON_COLOR, PLAY_BUTTON_RECT, "Play!") {label = "Play";}
    // DISABLE_COPY_ENABLE_MOVE(PlayButton);
    ~PlayButton() = default;
    
};

class ScoreButton : public Button {
public:
    ScoreButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::SCORE_MENU, SCORE_BUTTON_COLOR, SCORE_BUTTON_RECT, "High Scores") {label = "Score";}
    // DISABLE_COPY_ENABLE_MOVE(ScoreButton);
    ~ScoreButton() = default;
};

class QuitButton : public Button {
public:
    QuitButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::QUIT, QUIT_BUTTON_COLOR, QUIT_BUTTON_RECT, "Quit") {}
    // DISABLE_COPY_ENABLE_MOVE(QuitButton);
    ~QuitButton() = default;
};

class CharacterButton : public Button {
public:
    CharacterButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::CHARACTER_MENU, CHAR_BUTTON_COLOR, CHAR_BUTTON_RECT, "Characters") {}
    // DISABLE_COPY_ENABLE_MOVE(CharacterButton);
    ~CharacterButton() = default;
};

class BackButton : public Button {
public:
    BackButton(SDL_Renderer* renderer, SDL_Color color = BACK_BUTTON_COLOR, SDL_Rect rect = BACK_BUTTON_RECT)
        : Button(renderer, MenuState::BACK, color, rect, "Back") {}
    // DISABLE_COPY_ENABLE_MOVE(BackButton);
    ~BackButton() = default;
};

class StartButton : public Button {
public:
    StartButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::PLAY, START_BUTTON_COLOR, START_BUTTON_RECT, "Start") {label = "Play";}
    // DISABLE_COPY_ENABLE_MOVE(StartButton);
    ~StartButton() = default;
};

class ScoreUpButton : public Button {
public:
    ScoreUpButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::BACK, SCORE_UP_BUTTON_COLOR, SCORE_UP_BUTTON_RECT, "Up") {label = "Up";}
    // DISABLE_COPY_ENABLE_MOVE(StartButton);
    ~ScoreUpButton() = default;
    MenuState onClick(Menu* container) const override; // custom behavior
};

class ScoreDownButton : public Button {
public:
    ScoreDownButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::BACK, SCORE_DOWN_BUTTON_COLOR, SCORE_DOWN_BUTTON_RECT, "Down") {label = "Down";}
    // DISABLE_COPY_ENABLE_MOVE(StartButton);
    ~ScoreDownButton() = default;
    MenuState onClick(Menu* container) const override;
};

class ScoreBackButton : public Button {
public:
    ScoreBackButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::BACK, BACK_BUTTON_COLOR, SCORE_BACK_BUTTON_RECT, "Back") {}
    // DISABLE_COPY_ENABLE_MOVE(BackButton);
    ~ScoreBackButton() = default;
    MenuState onClick(Menu* container) const override;
};

class CharacterSelectButton : public ImageButton {
    int _characterIndex = 0; // index in character array
    std::string _characterName; // or simply use the label
    public:
    CharacterSelectButton(SDL_Renderer* renderer, int char_index, SDL_Rect rect)
        : ImageButton(renderer, MenuState::NONE, CHARACTER_COLORS[char_index], rect, 
            CHARACTER_NAMES[char_index], characterFileMap.at(static_cast<CharacterEnum>(char_index))[0]), 
                _characterName(CHARACTER_NAMES[char_index])
        {
            label = CHARACTER_NAMES[char_index];
        }
        //CHARACTER_BUTTON_BORDER_COLOR -> NOTE/TODO: This is a dummy since hard-coded at button level currently
    ~CharacterSelectButton() override {};
    MenuState onClick(Menu* container) const override;

};


// ********************** Base Menu Type ************************************************************ 
class Menu {
public:
    explicit Menu(SDL_Renderer *renderer) : _renderer(renderer) {} ;
    virtual ~Menu() = default;
    DISABLE_COPY_ENABLE_MOVE(Menu);

    virtual void Render();
    // virtual void handleEvent(const SDL_Event& e);
    virtual MenuState queryButtons(const SDL_Event& e);
    virtual MenuState getNameInput(const SDL_Event& e) {return MenuState::NONE;}; // just make generic getTextInput
    virtual std::string getPlayerName() {return "";};
    virtual void toggleCursor() {};
    virtual std::string getCharacterSelection() const {return "";};
    virtual void setCharacterSelection(const std::string& character){}; // just need this in parent so CharacterMenu can override

    virtual Menu* next() { return nullptr; } //not using ? todo:deprecate

    // Optional hook when menu becomes active
    virtual void enter() {}
    virtual void toggleOffset(PageToggle direction){}; // just need this in parent so ScoreMenu can override

    virtual void resetButtons(){
        for (auto &button : _buttons){
            button->unselect();
        }
    }


    // Some of these could be protected?
    void toggleSelectedButton(Button *newButton) {
        if (_disableSelectEffect) { return; }
        if (_selectedButton){ //nullptr guard
            _selectedButton->unselect();
        }
        if (_selectedButton == newButton) {
            _selectedButton->unselect();
            _selectedButton = nullptr;
        }
        else{
            std::cout << "Should Set the New Button: " << newButton << std::endl;
            _selectedButton = newButton;
            _selectedButton->select(); 
            std::cout << "Selected?: " << _selectedButton << std::endl;
        }
    }


protected:
    SDL_Texture* _background = nullptr; // Could use a smart pointer with custom deleter
    bool buttonClicked = false;
    SDL_Renderer *_renderer = nullptr; // for creating textures, etc.

    std::unique_ptr<Window> _window; // automate free textures
    std::vector<std::unique_ptr<Button>> _buttons;
    Button* _selectedButton = nullptr; // keep track of selected button for border highlight
    bool _disableSelectEffect = false; // optional: if menu doen't use select effect
};


// ******************* Custom Menus ************************************************************
class MainMenu : public Menu {
    public:
    MainMenu(SDL_Renderer* renderer);
    ~MainMenu(){
         std::cout << "Main Menu Destructor Called" << std::endl;
    };
    // DISABLE_COPY_ENABLE_MOVE(MainMenu);
};

// TODO : Currently unsafe Text creation since dynamic -- memory leak possible - need to restructure
class PlayerEntryMenu : public Menu {
    public:
    PlayerEntryMenu(SDL_Renderer* renderer);
    ~PlayerEntryMenu(){
        std::cout << "Player Entry Menu Destructor Called" << std::endl;
    };
    // DISABLE_COPY_ENABLE_MOVE(PlayerEntryMenu);
    std::string getPlayerName() override ;
    MenuState getNameInput(const SDL_Event& e) override;
    void toggleCursor() override;
    void Render() override;
    // MenuState queryButtons(const SDL_Event& e) override { return getNameInput(e);};

    private:
    std::string _playerName = DEFAULT_PLAYER_NAME; // default name
    std::unique_ptr<InputWindow> _textEntry; // automate free textures
    Uint32 _lastToggleTime = 0; // move to be inherent to a dialog box? (TODO)
    bool cursor_visible = true;
    // Window _text_entry;
};


class ScoreMenu : public Menu {
    public:
    ScoreMenu(SDL_Renderer* renderer);
    ~ScoreMenu() {
        std::cout << "Score Menu Destructor Called" << std::endl;
    }; // unique_pointer will auto free
    DISABLE_COPY_ENABLE_MOVE(ScoreMenu);
    void Render() override;
    int pageSize() const { return _scoreTable->getRows(); } // could make dynamic
    void toggleOffset(PageToggle direction) override {
        _scoreTable->toggleOffset(direction);
    };

    private:
    std::unique_ptr<TableWindow> _scoreTable; 
    // NOTE: score manager maybe could use a new thread to get scores from file
    // wouldn't really be that helfpful for small file, but to demonstrate the concept (use Promise/Future)
};

// TODO
class CharacterMenu : public Menu {

    std::string _selectedCharacter = CHARACTER_NAMES[0]; // default
    int _num_characters = 0;
    std::vector<SDL_Rect> _characterRects;

    // NOTE: simple first approach, just create two image buttons
    // future flexibility, reuse the TableWindow class to accept multiple data types

    public:
    CharacterMenu(SDL_Renderer* renderer);
    ~CharacterMenu(){
        std::cout << "Character Menu Destructor Called" << std::endl;
    }; // responsible for destroying image textures
    // DISABLE_COPY_ENABLE_MOVE(CharacterMenu);


    // void Render() override; // or maybe override not needed
    void setCharacterSelection(const std::string& character) override { _selectedCharacter = character; }
    std::string getCharacterSelection() const override { return _selectedCharacter; }


    private:
    void generateGridDimensions();

};


// *************** Menu Logic Driver ************************************************************
class MenuManager {
    /* Manage display by moving currentMenu pointer, render 
        menus once, destroy when Manager goes out of scope.  */

    const std::unique_ptr<Menu> mainMenu;
    const std::unique_ptr<Menu> scoreMenu;
    const std::unique_ptr<Menu> nameInput;
    const std::unique_ptr<Menu> characterMenu;
    SDL_Renderer* _renderer = nullptr;

    Menu* _currentMenu = nullptr;
    Menu* _prevMenu = nullptr; 
    MenuState _state = MenuState::MAIN_MENU;
    std::string _playerName = "Player1"; // default name
    std::string _selectedCharacter = CHARACTER_NAMES[0]; // default character

    //TODO(?) store _prev

    public:

    explicit MenuManager(SDL_Renderer* renderer) : // note to self: using "explicit"
        _renderer(renderer),
        mainMenu(std::make_unique<MainMenu>(renderer)),
        scoreMenu(std::make_unique<ScoreMenu>(renderer)),
        nameInput(std::make_unique<PlayerEntryMenu>(renderer)),
        characterMenu(std::make_unique<CharacterMenu>(renderer))
    {
        _currentMenu = mainMenu.get(); // entry
        std::cout << "Main Pointer : " << mainMenu.get() << std::endl; //debug
        std::cout << "Score Pointer : " << scoreMenu.get() << std::endl; //debug

    };
    ~MenuManager() 
    {
        std::cout << "MenuManager Destructor Called" << std::endl;
    }; 
    //TODO any unique logic needed here?
    std::string getPlayerName() const { return _playerName; }
    std::string getCharacterSelection() const { return _selectedCharacter; }

    void switchMenu() {
        if (_state == MenuState::NONE) { 
            return ;
        }
        switch (_state){
            case MenuState::MAIN_MENU:
                _currentMenu = mainMenu.get();
                std::cout << "Switch: "<< _currentMenu << std::endl;
                break;
            case MenuState::SCORE_MENU:
                _currentMenu = scoreMenu.get();
                std::cout << "Switch: "<< _currentMenu << std::endl;
                break;
            case MenuState::PLAYER_NAME:
                // _currentMenu = playerNameMenu.get(); // TODO
                _currentMenu = nullptr;
                _currentMenu = nameInput.get();
                break;
            case MenuState::PLAY:
                std::cout << "Switch Play: "<< _currentMenu << std::endl;
                break;
            case MenuState::CHARACTER_MENU:
                _currentMenu = characterMenu.get();
                break;
            case MenuState::BACK:
                _currentMenu->resetButtons();
                _currentMenu = mainMenu.get();
                std::cout << "Switch: "<< _currentMenu << std::endl;
                break;
            case MenuState::QUIT:
                _currentMenu = nullptr; // or some exit logic
                break;
            default:
                _currentMenu = nullptr;
                break;
        }
    }

    void Render(SDL_Renderer* renderer) {
        if (_currentMenu) {
            _currentMenu->Render();
        }
    }
 
    // maybe generalize the menus to have a general get data/input function
    MenuState handleEvent(const SDL_Event& e) {
        
        if (_currentMenu == nameInput.get()){
            nameInput->getNameInput(e); 
            // nameInput->toggleCursor(); // to handle cursor blink
            // if (nameInput->getPlayerName().size() >= 2) {
            //     _playerName = nameInput->getPlayerName();
            // }
        }
    
        if (_currentMenu == characterMenu.get()) {
            _selectedCharacter =  _currentMenu->getCharacterSelection();
            // _currentMenu->toggleSelectedButton();
            //possibly sleep to ensure proper selection...although probably unecessary
        }
        
        return _currentMenu->queryButtons(e);
    }

    bool display() {
        bool running = true;
        SDL_Event e;
        _currentMenu = mainMenu.get();
        _state = MenuState::MAIN_MENU;

        while (running && _currentMenu) {
   
            if (_state == MenuState::PLAY) { return true;}  //start game

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false; // exit game
                }
                _state = handleEvent(e);
                if (_state == MenuState::QUIT) {
                    return false; // may be redundant ? 
                } 
                switchMenu(); // wonder how this will work if PollEvent is still non-empty
            }
            // could bundle into another "Render" call ?
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); // black background
            SDL_RenderClear(_renderer);
            SDL_SetRenderDrawColor(_renderer, 
                            BACKGROUND_COLOR.r, 
                            BACKGROUND_COLOR.g, 
                            BACKGROUND_COLOR.b, 
                            BACKGROUND_COLOR.a);
            SDL_RenderClear(_renderer); // Clear screen
            Render(_renderer); // Render current menu
            SDL_RenderPresent(_renderer);  // Update Screen
            SDL_Delay(16); // ~60fps
        }
        return false; // default exit point
    }
};
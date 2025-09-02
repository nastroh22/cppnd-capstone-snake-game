#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
// #include <array>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "constants.h"


//Copies of Screen Constants 



// see: https://www.color-hex.com/color-palette/59812 (good resource on rgb colors)
constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color TAN = {255, 219, 172, 150}; // darker : (241,194,125)
constexpr SDL_Color FOREST = {39, 62, 6, 200};
constexpr SDL_Color ARMY = {75,92,9, 250};
constexpr SDL_Color OLIVE = {106, 118, 12, 255};
constexpr SDL_Color MOSS = {134, 141, 7, 255};
constexpr SDL_Color SPRING = {157, 174, 17, 250};
constexpr SDL_Color DEEPBLUE = {1, 31, 75, 255};
constexpr SDL_Color BLACK = {0, 0, 0, 255};
constexpr SDL_Color GREY = {192, 197, 206, 200};
constexpr SDL_Color RETRO_PEACH = {255,226,138,250};
constexpr SDL_Color BRICK = {191,0,0,255};


// Text Colors
constexpr SDL_Color DEFAULT_TEXT_COLOR = WHITE;
constexpr SDL_Color HIGHLIGHT_TEXT_COLOR = DEEPBLUE;
constexpr SDL_Color TITLE_COLOR = RETRO_PEACH;

// Button Backgrounds
constexpr SDL_Color PLAY_BUTTON_COLOR = OLIVE;
constexpr SDL_Color SCORE_BUTTON_COLOR = ARMY;
constexpr SDL_Color QUIT_BUTTON_COLOR = RETRO_PEACH;
constexpr SDL_Color BACK_BUTTON_COLOR = MOSS;
constexpr SDL_Color DEFAULT_BORDER_COLOR = WHITE;
constexpr SDL_Color HOVER_BORDER_COLOR = TAN; // for hover effect

constexpr SDL_Color BACKGROUND_COLOR = BLACK;
constexpr SDL_Color WINDOW_COLOR = FOREST;
constexpr SDL_Color WINBORDER_COLOR = MOSS;

//Window Positions
static int border_width = static_cast<int>(.15*kScreenWidth);
static int border_height = static_cast<int>(.10*kScreenHeight);
const SDL_Rect WIN_POSITION = {
    border_width, 
    border_height,
    static_cast<int>(kScreenWidth - 2*border_width), 
    static_cast<int>(kScreenHeight-2*border_height)
};


// Main Button Positions 
constexpr SDL_Rect PLAY_BUTTON_RECT = {220, 150, 200, 75};
constexpr SDL_Rect CHAR_BUTTON_RECT = {220, 250, 200, 75};  // x,y,w,h
constexpr SDL_Rect SCORE_BUTTON_RECT = {220, 350, 200, 75};
constexpr SDL_Rect QUIT_BUTTON_RECT = {220, 450, 200, 75};


// Score Menu Positions
constexpr SDL_Rect BACK_BUTTON_RECT = {220, 250, 200, 75};

// Character Menu Positions
// constexpr SDL_Rect CHARACTER_BUTTON_RECT = {220, 350, 200, 75};

// TODO : Font Options
// maybe an even cooler feature... add themes (future)


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

// Generic Text Object, based on youtube tutorial. Nice pattern, reusable for buttons.
class Text {
    public:
        Text(SDL_Renderer *renderer,const std::string &font_path, int font_size, const std::string &message_text, SDL_Color color);
        SDL_Texture *loadFont(SDL_Renderer *renderer,const std::string &font_path, int font_size, const std::string &message_text, SDL_Color& color);
        void display(SDL_Renderer *renderer, int x, int y) const;
        ~Text(){ SDL_DestroyTexture(_text_texture); }; //free textures
        int getWidth() const { return _text_rect.w;}
        int getHeight() const { return _text_rect.h;}
    
    private:
        SDL_Texture *_text_texture;
        mutable SDL_Rect _text_rect = {0,0,20,20}; // for positioning text, mutable so that display can be const
};

// TODO: Make Button Derivce from Window so as to capture identical render function (perhaps)
class Window {
    protected:
        SDL_Rect _windowRect; // for positioning button
        SDL_Color _windowColor; // for button color
        SDL_Color _borderColor = DEFAULT_BORDER_COLOR; // could make customizable
        Text _title;
        const std::string* _playerInput = nullptr; // for text entry window
        // Text _content; // could be vector of texts for multiple lines
    
    public:
        Window(SDL_Renderer* renderer, const std::string& title, SDL_Color win_color, SDL_Color border_color ,SDL_Rect rect, const std::string* dynamic = nullptr) :
            _windowRect(rect), _windowColor(win_color), _borderColor(border_color), _playerInput(dynamic),
            _title(renderer, "../assets/fonts/comic_sans_ms.ttf", 32, title, TITLE_COLOR)
            {};
        
        void Render(SDL_Renderer* renderer);
        // void drawBorder(SDL_renderer* renderer, int thickness);

        ~Window() = default; // will call _text destructor by default
};


// Base Button Type
class Button {
protected:
    Text _text;
    const SDL_Rect _buttonRect; // for positioning button
    const SDL_Color _buttonColor; // for button color
    const MenuState _return_state = MenuState::NONE; // default state
    SDL_Color _borderColor = DEFAULT_BORDER_COLOR; // non-const to enable hover effect
    std::string label = "Button";
    int _border_thickness = 2; // could make customizable

/*TODO's: 
    Options: create options to customize test colors and Fonts
*/

// I think the protected constructor pattern works to make sure no abstract button can be created // TODO re-use for "Menu"
    Button(SDL_Renderer* renderer, MenuState label, SDL_Color color, SDL_Rect rect, const std::string& text)
        :_return_state(label), _buttonColor(color), _buttonRect(rect), 
        _text(renderer, "../assets/fonts/comic_sans_ms.ttf", 24, text, DEFAULT_TEXT_COLOR) {}

public:
    void Render(SDL_Renderer* renderer); //virtual ?
    bool wasClicked(const SDL_Point& mouse_point) const { return SDL_PointInRect(&mouse_point, &_buttonRect);}
    bool isHovered(const SDL_Point& mouse_point) const { return SDL_PointInRect(&mouse_point, &_buttonRect);}
    void toggleHover(const SDL_Point& mouse_point);
    MenuState onClick() const { return _return_state; } // maybe useful for debugging
    // void drawBorder(SDL_renderer* renderer, int thickness);

    virtual ~Button() = default; // will call _text destructor by default

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

// Button Types
class PlayButton : public Button {
public: // TODo: Rename the Top Menu Button to "Start"
    PlayButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::PLAYER_NAME, PLAY_BUTTON_COLOR, PLAY_BUTTON_RECT, "Play!") {label = "Play";}
    // void onClick() override {
    //     // Another pattern would be to return custom state here
    // }
};

class ScoreButton : public Button {
public:
    ScoreButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::SCORE_MENU, SCORE_BUTTON_COLOR, SCORE_BUTTON_RECT, "High Scores") {label = "Score";}
};

class QuitButton : public Button {
public:
    QuitButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::QUIT, QUIT_BUTTON_COLOR, QUIT_BUTTON_RECT, "Quit") {}
};

class CharacterButton : public Button {
public:
    CharacterButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::CHARACTER_MENU, TAN, CHAR_BUTTON_RECT, "Characters") {}
};

class BackButton : public Button {
public:
    BackButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::BACK, BACK_BUTTON_COLOR, BACK_BUTTON_RECT, "Back") {}
};

class StartButton : public Button {
public:
    StartButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::PLAY, BACK_BUTTON_COLOR, QUIT_BUTTON_RECT, "Start") {}
};

// Base Menu Type
class Menu {
public:
    explicit Menu(SDL_Renderer *renderer) : _renderer(renderer) {} ;
    virtual ~Menu() = default;

    virtual void Render();
    // virtual void handleEvent(const SDL_Event& e);
    virtual MenuState queryButtons(const SDL_Event& e);
    virtual MenuState getNameInput(const SDL_Event& e) {return MenuState::NONE;}; // just make generic getTextInput
    virtual std::string getPlayerName() const {return "";};

    virtual Menu* next() { return nullptr; } //not using ? todo:deprecate

    // Optional hook when menu becomes active
    virtual void enter() {}

protected:
    SDL_Texture* _background = nullptr; // Could use a smart pointer with custom deleter
    bool buttonClicked = false;
    SDL_Renderer *_renderer = nullptr; // for creating textures, etc.

    std::unique_ptr<Window> _window; // automate free textures
    std::vector<std::unique_ptr<Button>> _buttons;
};

class MainMenu : public Menu {
    public:
    MainMenu(SDL_Renderer* renderer);
    ~MainMenu() = default;
};

class ScoreMenu : public Menu {
    public:
    ScoreMenu(SDL_Renderer* renderer);
    ~ScoreMenu() = default;
};


// TODO : Currently unsafe Text creation since dynamic -- memory leak possible - need to restructure
class PlayerEntryMenu : public Menu {
    public:
    PlayerEntryMenu(SDL_Renderer* renderer);
    ~PlayerEntryMenu() = default;
    std::string getPlayerName() const override { return _player_name; };
    MenuState getNameInput(const SDL_Event& e) override;
    // MenuState queryButtons(const SDL_Event& e) override { return getNameInput(e);};

    private:
    std::string _player_name;
    // Window _text_entry;
};

// NOTE: score manager maybe could use a new thread to get scores from file
// wouldn't really be that helfpful for small file, but to demonstrate the concept


class MenuManager {
    /* Manage display by moving currentMenu pointer, render 
        menus once, destroy when Manager goes out of scope.  */

    const std::unique_ptr<Menu> mainMenu;
    const std::unique_ptr<Menu> scoreMenu;
    const std::unique_ptr<Menu> nameInput;
    SDL_Renderer* _renderer = nullptr;

    Menu* _currentMenu = nullptr;
    Menu* _prevMenu = nullptr; 
    MenuState _state = MenuState::MAIN_MENU;

    //TODO: store _prev

    public:

    explicit MenuManager(SDL_Renderer* renderer) :
        _renderer(renderer),
        mainMenu(std::make_unique<MainMenu>(renderer)),
        scoreMenu(std::make_unique<ScoreMenu>(renderer)),
        nameInput(std::make_unique<PlayerEntryMenu>(renderer))
    {
        _currentMenu = mainMenu.get(); // start at main menu , not totally sure about explicit 
        std::cout << "Main Pointer : " << mainMenu.get() << std::endl; //debug
        std::cout << "Score Pointer : " << scoreMenu.get() << std::endl; //debug

    };
    ~MenuManager() = default; //TODO any unique logic needed here?

    void switchMenu() {

        // std::cout << "Menu States: " << _new  << " " << _state << std::endl; // TODO: make pritnable, overload MenuState <<
        // std::cout << "State Change : " << static_cast<int>(_new)  << " -> " << static_cast<int>(_state) << std::endl; // TODO: make pritnable, overload MenuState <<
        if (_state == MenuState::NONE) { return ;}

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
                std::cout << "Player Name Menu Not Implemented Yet!" << std::endl;
                break;
            case MenuState::PLAY:
                _currentMenu = nullptr;
                std::cout << "Switch Play: "<< _currentMenu << std::endl;
                break;
            case MenuState::CHARACTER_MENU:
                // _currentMenu = characterMenu.get(); // TODO
                _currentMenu = nullptr;
                break;
            case MenuState::BACK:
                // Note: for simple menu like this, just default to Main
                // _currentMenu = _prevMenu; // but could use _prevMenu logic here
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
        //possible reset logic
    }

    void Render(SDL_Renderer* renderer) {
        if (_currentMenu) {_currentMenu->Render();}
    }

    //unecessary(??) wrapper 
    MenuState handleEvent(const SDL_Event& e) {
        if (_currentMenu != nameInput.get()) {
            return _currentMenu->queryButtons(e);
        }
        nameInput->getNameInput(e);
        if (nameInput->getPlayerName().size() >= 2) {
            std::cout << "Player Name Entered: " << nameInput->getPlayerName() << std::endl;
           return _currentMenu->queryButtons(e);
        }
        return MenuState::NONE;
    }

    bool display() {
        bool running = true;
        SDL_Event e;

        while (running && _currentMenu) {
   
            if (_state == MenuState::PLAY) {
                std::cout << "Menu State Here ? " << static_cast<int>(_state) << std::endl; //debug
                return true;
            } //start game

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false; // exit game
                }
                _state = _currentMenu->queryButtons(e);
                if (_state == MenuState::QUIT) {
                    std::cout << "Not here ?" << std::endl;
                    return false; // exit game (may be redundant), should quit from any menu (TODO add "Back Logic")
                } 
                switchMenu(); // wonder how this will work if PollEvent is still non-empty
            }
            if (_state == MenuState::PLAY) { return true ;} // putting it here for now bc of rendering errors
            //could bundle into another "Render" call
            //clear screen
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); // black background
            SDL_RenderClear(_renderer);
            SDL_SetRenderDrawColor(_renderer, BACKGROUND_COLOR.r,BACKGROUND_COLOR.g,BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);  // Clear screen
            SDL_RenderClear(_renderer);
            Render(_renderer); // Render current menu
            SDL_RenderPresent(_renderer);  // Update Screen
            SDL_Delay(16); // ~60fps
        }
        return false; // default exit
    }
};


// TODO: MORNING -- menu background/ Score Menu Style / Connect to ScoreIo / glue all logic together / wokr on compiling and cleaning / get menu to work in basic form / Try Scores Menu 
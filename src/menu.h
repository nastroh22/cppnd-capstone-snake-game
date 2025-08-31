#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
// #include <array>
#include <string>
#include <vector>
#include <memory>


// see: https://www.color-hex.com/color-palette/59812 (good resource on rgb colors)
constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color OLIVE = {106, 118, 12, 255};
constexpr SDL_Color ARMY = {134, 141, 7, 255};
constexpr SDL_Color TAN = {255, 219, 172, 150}; // darker : (241,194,125)
constexpr SDL_Color FOREST = {39, 62, 6, 200};
constexpr SDL_Color DEEPBLUE = {1, 31, 75, 255};
constexpr SDL_Color BLACK = {0, 0, 0, 255};


// Colors 
constexpr SDL_Color DEFAULT_TEXT_COLOR = WHITE;
constexpr SDL_Color PLAY_BUTTON_COLOR = OLIVE;
constexpr SDL_Color SCORE_BUTTON_COLOR = FOREST;
constexpr SDL_Color BACKGROUND_COLOR = TAN;
constexpr SDL_Color QUIT_BUTTON_COLOR = ARMY;
constexpr SDL_Color BACK_BUTTON_COLOR = ARMY;
constexpr SDL_Color DEFAULT_BORDER_COLOR = WHITE;


// Main Menu Positions
constexpr SDL_Rect PLAY_BUTTON_RECT = {220, 150, 200, 75};
constexpr SDL_Rect SCORE_BUTTON_RECT = {220, 250, 200, 75};
constexpr SDL_Rect QUIT_BUTTON_RECT = {220, 350, 200, 75};


// Score Menu Positions
constexpr SDL_Rect BACK_BUTTON_RECT = {220, 400, 200, 75};

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
        int getWidth() const { return _width;}
        int getHeight() const { return _height;}
    
    private:
        SDL_Texture *_text_texture;
        mutable SDL_Rect _text_rect = {0,0,20,20}; // for positioning text, mutable so that display can be const
        int _height = 20;
        int _width = 20;
};

// Base Button Class
class Button {
protected:
    Text _text;
    const SDL_Rect _buttonRect; // for positioning button
    const SDL_Color _buttonColor; // for button color
    const MenuState _return_state = MenuState::NONE; // default state
    const SDL_Color _borderColor = DEFAULT_BORDER_COLOR; // could make customizable

/*TODO's: 
    Options: create options to customize test colors and Fonts
*/

// I think the protected constructor pattern works to make sure no abstract button can be created // TODO re-use for "Menu"
    Button(SDL_Renderer* renderer, MenuState label, SDL_Color color, SDL_Rect rect, const std::string& text)
        :_return_state(label), _buttonColor(color), _buttonRect(rect), 
        _text(renderer, "../assets/fonts/comic_sans_ms.ttf", 24, text, SDL_Color{255,255,255,255}) {}

public:
    void Render(SDL_Renderer* renderer); //virtual ?
    bool wasClicked(const SDL_Point& mouse_point) const { return SDL_PointInRect(&mouse_point, &_buttonRect);}
    MenuState onClick() const { return _return_state; } // maybe useful for debugging

    virtual ~Button() = default; // will call _text destructor by default
};

// Button Types
class PlayButton : public Button {
public:
    PlayButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::PLAY, PLAY_BUTTON_COLOR, PLAY_BUTTON_RECT, "Play") {}

    // void onClick() override {
    //     // Another pattern would be to return custom state here
    // }
};

class ScoreButton : public Button {
public:
    ScoreButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::SCORE_MENU, SCORE_BUTTON_COLOR, SCORE_BUTTON_RECT, "High Scores") {}
};

class QuitButton : public Button {
public:
    QuitButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::QUIT, QUIT_BUTTON_COLOR, QUIT_BUTTON_RECT, "Quit") {}
};

class BackButton : public Button {
public:
    BackButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::BACK, BACK_BUTTON_COLOR, BACK_BUTTON_RECT, "Back") {}
};


//TODO Deprecate
// class Menu {
//     public:
//         Menu() = default;
//         ~Menu() = default;
//         // bool ShowMenu(SDL_Renderer* renderer);
//         // void Render();
//         // void HandleEvent(SDL_Event* e);
//         bool IsButtonClicked() const { return buttonClicked; }
//         bool ToggleButtonClicked() { buttonClicked = !buttonClicked; return buttonClicked; }
    
//     // optional get mouse hover state (and add effects)
    

//     private:
//         Button startButton = {320-150,320-50,300,100};
//         Button scoreButton = {320-150,420-50,300,100};
//         bool buttonClicked = false;
    
//         // std::array<SDL_Rect, 1> buttons;
//         // std::array<Color, 5> colors = {
//         //     BLUE, RED, GREEN, YELLOW, WHITE
//         // };

//         // TODO: ITEMS
//         // std::array<MenuItem, 2> items = {ScoreItem, CharacterItem}; // N = number of menu items
//         // MenuItem* current_item;
//         // MenuItem  ScoreItem; // MenuItem
//         // MenuItem CharacterItem;
// };


class Menu {
public:
    explicit Menu(SDL_Renderer *renderer) : _renderer(renderer) {}; //possibly render backdrop here // must explictly construct the base Menu
    virtual ~Menu() = default;

    virtual void Render();
    // virtual void handleEvent(const SDL_Event& e);
    MenuState queryButtons(const SDL_Event& e);

    virtual Menu* next() { return nullptr; } //not using ? todo:deprecate

    // Optional hook when menu becomes active
    virtual void enter() {}

protected:
    SDL_Texture* _background = nullptr; // Could use a smart pointer with custom deleter
    bool buttonClicked = false;
    SDL_Renderer *_renderer = nullptr; // for creating textures, etc.

    std::vector<std::unique_ptr<Button>> _buttons;

    // std::unique_ptr<Window> _internalWindow = nullptr; // optional "popup" or panel TODO 

    // Utility: shared render function for buttons, window, etc.
    // void renderComponents(SDL_Renderer* renderer);
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
// NOTE: score manager maybe could use a new thread to get scores from file
// wouldn't really be that helfpful for small file, but to demonstrate the concept


class MenuManager {
    /* Manage display by moving currentMenu pointer, render 
        menus once, destroy when Manager goes out of scope.  */

    const std::unique_ptr<Menu> mainMenu;
    const std::unique_ptr<Menu> scoreMenu;
    SDL_Renderer* _renderer = nullptr;

    Menu* _currentMenu = nullptr;
    MenuState _state = MenuState::MAIN_MENU;

    public:

    explicit MenuManager(SDL_Renderer* renderer) :
        _renderer(renderer),
        mainMenu(std::make_unique<MainMenu>(renderer)),
        scoreMenu(std::make_unique<ScoreMenu>(renderer))
    {
        _currentMenu = mainMenu.get(); // start at main menu , not totally sure about explicit 
    };
    ~MenuManager() = default; //TODO any unique logic needed here?

    void switchMenu(MenuState _new) {

        // std::cout << "Menu States: " << _new  << " " << _state << std::endl; // NOTE: need to overload << for MenuState

        if (_new == _state || _new == MenuState::NONE) { return ;}

        switch (_new){
            case MenuState::MAIN_MENU:
                _currentMenu = mainMenu.get();
                break;
            case MenuState::SCORE_MENU:
                _currentMenu = scoreMenu.get();
                break;
            case MenuState::PLAYER_NAME:
                // _currentMenu = playerNameMenu.get(); // TODO
                _currentMenu = nullptr;
                break;
            case MenuState::PLAY:
            // case MenuState::CHARACTER_MENU:
            //     _currentMenu = characterMenu.get();
            //     break;
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

    //unecessary wrapper 
    MenuState handleEvent(const SDL_Event& e) {
        return _currentMenu->queryButtons(e);
    }

    bool display() {
        bool running = true;
        SDL_Event e;

        while (running && _currentMenu) {

            if (_state == MenuState::PLAY) {return true;} //start game

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false; // exit game
                }
                _state = _currentMenu->queryButtons(e);
                if (_state == MenuState::QUIT) {
                    return false; // exit game (may be redundant), should quit from any menu (TODO add "Back Logic")
                } 
                switchMenu(_state); // wonder how this will work if PollEvent is still non-empty
            }
            //could bundle into another "Render" call
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
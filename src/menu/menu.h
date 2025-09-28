#pragma once

// #include <array>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "SDL.h"
#include "SDL_ttf.h"
#include "components.h"
#include "./constants.h"
#include "./utils.h" 


/* NOTE: defs start with custom buttons since these are closely tied with their respective menus
            the rest of the base elements can be found in "components.h"
*/

// ********************** Custom Buttons ************************************************************ 
class PlayButton : public Button {
public: // TODo: Rename the Top Menu Button to "Start"
    PlayButton(SDL_Renderer* renderer)
        : Button(
            renderer, 
            MenuState::PLAYER_NAME, 
            MMConst.PLAY_BUTTON_COLOR, 
            MMConst.PLAY_BUTTON_RECT, "Play!") {label = "Play";}
    
    // DISABLE_COPY_ENABLE_MOVE(PlayButton);
    ~PlayButton() = default;
    
};

class ScoreButton : public Button {
public:
    ScoreButton(SDL_Renderer* renderer)
        : Button(
            renderer, 
            MenuState::SCORE_MENU, 
            MMConst.SCORE_BUTTON_COLOR,
            MMConst.SCORE_BUTTON_RECT, "High Scores") {label = "Score";}
    ~ScoreButton() = default;
};

class QuitButton : public Button {
public:
    QuitButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::QUIT, 
        MMConst.QUIT_BUTTON_COLOR, 
        MMConst.QUIT_BUTTON_RECT, "Quit") {}
    ~QuitButton() = default;
};

class CharacterButton : public Button {
public:
    CharacterButton(SDL_Renderer* renderer)
        : Button(renderer, 
            MenuState::CHARACTER_MENU, 
            MMConst.CHAR_BUTTON_COLOR, 
            MMConst.CHAR_BUTTON_RECT, 
            "Characters") {}
    // DISABLE_COPY_ENABLE_MOVE(CharacterButton);
    ~CharacterButton() = default;
};

class BackButton : public Button {
public:
    BackButton(
        SDL_Renderer* renderer, 
        SDL_Color color = PEConst.BACK_BUTTON_COLOR, 
        SDL_Rect rect = PEConst.BACK_BUTTON_RECT
    ) :
        Button(renderer, MenuState::BACK, color, rect, "Back") {}
    ~BackButton() = default;
};

class StartButton : public Button {
public:
    StartButton(SDL_Renderer* renderer)
        : Button(
            renderer, 
            MenuState::PLAY, 
            PEConst.START_BUTTON_COLOR, 
            PEConst.START_BUTTON_RECT, "Start") {label = "Play";}
    ~StartButton() = default;
};

class ScoreUpButton : public Button {
public:
    ScoreUpButton(SDL_Renderer* renderer) :
        Button(renderer, 
            MenuState::BACK, 
            ScoreConst.SCORE_UP_BUTTON_COLOR, 
            SCORE_UP_BUTTON_RECT, "Up" ) {label = "Up";}
    
    ~ScoreUpButton() = default;
    MenuState onClick(Menu* container) const override; // custom behavior
};

class ScoreDownButton : public Button {
public:
    ScoreDownButton(SDL_Renderer* renderer)
        : Button(renderer, 
            MenuState::BACK, 
            ScoreConst.SCORE_DOWN_BUTTON_COLOR,
            SCORE_DOWN_BUTTON_RECT, "Down") {label = "Down";}
   
    ~ScoreDownButton() = default;
    MenuState onClick(Menu* container) const override;
};

class ScoreBackButton : public Button {
public:
    ScoreBackButton(SDL_Renderer* renderer)
        : Button(renderer, 
            MenuState::BACK, 
            BACK_BUTTON_COLOR, 
            SCORE_BACK_BUTTON_RECT, "Back") {}

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
    virtual void toggleCursor() {/* dummy def, used only by PlayerEntryMenu */};
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

    //TODO: could be protected?
    // void toggleSelectedButton(Button *newButton) {
    //     if (_disableSelectEffect) { return; }
    //     if (_selectedButton){ //nullptr guard
    //         _selectedButton->unselect();
    //     }
    //     if (_selectedButton == newButton) {
    //         _selectedButton->unselect();
    //         _selectedButton = nullptr;
    //     }
    //     else{
    //         std::cout << "Should Set the New Button: " << newButton << std::endl;
    //         _selectedButton = newButton;
    //         _selectedButton->select(); 
    //         std::cout << "Selected?: " << _selectedButton << std::endl;
    //     }
    // }


protected:
    SDL_Texture* _background = nullptr; // Could use a smart pointer with custom deleter
    bool buttonClicked = false;
    SDL_Renderer *_renderer = nullptr; // for creating textures, etc.

    std::unique_ptr<Window> _window; // automate free textures
    std::vector<std::unique_ptr<Button>> _buttons;
    Button* _selectedButton = nullptr; // keep track of selected button for border highlight
    bool _disableSelectEffect = false; // optional: if menu doen't use select effect
    char _cursor = '|'; // for text input menus

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
};


// ******************* Custom Menus ************************************************************
class MainMenu : public Menu {
    public:
    MainMenu(SDL_Renderer* renderer);
    ~MainMenu(){
         std::cout << "Main Menu Destructor Called" << std::endl;
    };
};

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
    std::string _playerName = PEConst.DEFAULT_PLAYER_NAME; 
    std::unique_ptr<DynamicWindow> _textEntry; 
    Uint32 _lastToggleTime = 0;
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
    std::unique_ptr<Table> _scoreTable; 
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


// *************** State Management and Navigation Logic  ************************************************************
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
            if (nameInput->getPlayerName().size() >= 2) {
                _playerName = nameInput->getPlayerName();
            } // TODO: playerName constraints ?
        }
        if (_currentMenu == characterMenu.get()) {
            _selectedCharacter =  _currentMenu->getCharacterSelection();
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
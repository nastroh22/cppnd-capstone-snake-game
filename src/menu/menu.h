#pragma once

// #include <array>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <future>
#include <iostream>
#include <unordered_map>
#include <tuple>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_ttf.h"
#include "components.h"
#include "./constants.h"
#include "./utils.h" 
#include "enemy/planner.h"
#include "snake.h" // repurpose snake functions to render full body

/* NOTE: defs start with custom buttons since these are closely tied with their respective menus
            the rest of the base elements can be found in "components.h"
*/
// FUTURE TODO: create generic "button spec" then emplace back all buttons from a vector of specs (to reduce boilerplate)
// ********************** Custom Buttons ************************************************************ 
class PlayButton : public Button {
public: // TODo: Rename the Top Menu Button to "Start"
    PlayButton(SDL_Renderer* renderer)
        : Button(
            renderer, 
            MenuState::PLAYER_NAME, 
            MainConst.PLAY_BUTTON_COLOR, 
            MainConst.PLAY_BUTTON_RECT,
            MainConst.PLAY_BUTTON_LABEL,
            MainConst.BUTTON_FONT,
            MainConst.BUTTON_FONT_SIZE,
            MainConst.BUTTON_TEXT_COLOR,
            MainConst.BUTTON_BORDER_COLOR,
            MainConst.HOVER_COLOR) {}
    
    // DISABLE_COPY_ENABLE_MOVE(PlayButton);
    ~PlayButton() = default;
};

class ScoreButton : public Button {
public:
    ScoreButton(SDL_Renderer* renderer)
        : Button(
            renderer, 
            MenuState::SCORE_MENU, 
            MainConst.SCORE_BUTTON_COLOR,
            MainConst.SCORE_BUTTON_RECT, 
            MainConst.SCORE_BUTTON_LABEL,
            MainConst.BUTTON_FONT,
            MainConst.BUTTON_FONT_SIZE,
            MainConst.BUTTON_TEXT_COLOR,
            MainConst.BUTTON_BORDER_COLOR,
            MainConst.HOVER_COLOR
            ) {}
    ~ScoreButton() = default;
};

class CharacterButton : public Button {
public:
    CharacterButton(SDL_Renderer* renderer)
        : Button(renderer, 
            MenuState::CHARACTER_MENU, 
            MainConst.CHAR_BUTTON_COLOR, 
            MainConst.CHAR_BUTTON_RECT,
            MainConst.CHAR_BUTTON_LABEL,
            MainConst.BUTTON_FONT,
            MainConst.BUTTON_FONT_SIZE,
            MainConst.BUTTON_TEXT_COLOR,
            MainConst.BUTTON_BORDER_COLOR,
            MainConst.HOVER_COLOR) {}

    ~CharacterButton() = default;
};

class QuitButton : public Button {
public:
    QuitButton(SDL_Renderer* renderer)
        : Button(renderer, MenuState::QUIT, 
        MainConst.QUIT_BUTTON_COLOR, 
        MainConst.QUIT_BUTTON_RECT,
        MainConst.QUIT_BUTTON_LABEL,
        MainConst.BUTTON_FONT,
        MainConst.BUTTON_FONT_SIZE,
        MainConst.BUTTON_TEXT_COLOR,
        MainConst.BUTTON_BORDER_COLOR,
        MainConst.HOVER_COLOR) {}
    ~QuitButton() = default;
};

class BackButton : public Button {
public:
    BackButton(
        SDL_Renderer* renderer, 
        SDL_Color color = PlayerConst.BACK_BUTTON_COLOR, 
        SDL_Rect rect = PlayerConst.BACK_BUTTON_RECT
    ) :
        Button(renderer, 
            MenuState::BACK, color, rect, 
            "<< Back", "public_pixel", 28, WHITE, WHITE, YELLOW) {}
    ~BackButton() = default;
};

class StartButton : public Button {
public:
    StartButton(SDL_Renderer* renderer)
        : Button(
            renderer, 
            MenuState::PLAY, 
            PlayerConst.START_BUTTON_COLOR, 
            PlayerConst.START_BUTTON_RECT, 
            PlayerConst.START_BUTTON_LABEL,
            "public_pixel", 28, WHITE, WHITE, YELLOW) {}
    ~StartButton() = default;
};

class ScoreUpButton : public Button {
public:
    ScoreUpButton(SDL_Renderer* renderer) :
        Button(renderer, 
            MenuState::BACK, 
            ScoreConst.SCORE_UP_BUTTON_COLOR, 
            SCORE_UP_BUTTON_RECT, 
            "▲", "dejavu_sans", 36, WHITE, WHITE, ScoreConst.HOVER_COLOR ) {} // "▲"
    
    ~ScoreUpButton() = default;
    MenuState onClick(Menu* container) const override; // custom behavior
};

class ScoreDownButton : public Button {
public:
    ScoreDownButton(SDL_Renderer* renderer)
        : Button(renderer, 
            MenuState::BACK, 
            ScoreConst.SCORE_DOWN_BUTTON_COLOR,
            SCORE_DOWN_BUTTON_RECT, 
            "▼", "dejavu_sans", 36, WHITE, WHITE, ScoreConst.HOVER_COLOR  ) {}  // "▼"
   
    ~ScoreDownButton() = default;
    MenuState onClick(Menu* container) const override;
};

class ScoreBackButton : public Button {
public:
    ScoreBackButton(SDL_Renderer* renderer)
        : Button(renderer, 
            MenuState::BACK, 
            ScoreConst.SCORE_BACK_BUTTON_COLOR, 
            SCORE_BACK_BUTTON_RECT, 
            "◄◄", "dejavu_sans", 36, WHITE, WHITE, ScoreConst.HOVER_COLOR ) {} //◄

    ~ScoreBackButton() = default;
    MenuState onClick(Menu* container) const override;
};

class CharacterSelectButton : public ImageButton {
    // int _characterIndex = 0; // index in character array (not using)
    std::string _characterName; // or simply use the label
    public:
    CharacterSelectButton(SDL_Renderer* renderer, int char_index, SDL_Rect rect)
        : ImageButton(renderer, MenuState::NONE, CHARACTER_COLORS[char_index], rect, 
            CHARACTER_NAMES[char_index], characterFileMap.at(static_cast<CharacterEnum>(char_index))[0], 
                "comic_sans", 28, CharConst.BUTTON_TEXT_COLOR, CharConst.BUTTON_BORDER_COLOR, CharConst.HOVER_COLOR),  _characterName(CHARACTER_NAMES[char_index])
        {
            label = CHARACTER_NAMES[char_index];
        }
    ~CharacterSelectButton() override {};
    MenuState onClick(Menu* container) const override;
};

class CharBackButton : public Button {
public:
    CharBackButton(
        SDL_Renderer* renderer, 
        SDL_Color color = PlayerConst.BACK_BUTTON_COLOR, 
        SDL_Rect rect = PlayerConst.BACK_BUTTON_RECT
    ) :
        Button(renderer, 
            MenuState::BACK, color, rect, 
            "<< Main", 
            "public_pixel", 
            28, 
            WHITE, 
            CharConst.BUTTON_BORDER_COLOR, 
            CharConst.HOVER_COLOR) {}
    ~CharBackButton() = default;
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

    virtual SDL_Texture *getCharacterTexture() { return nullptr;};
    virtual SDL_Texture *getBodyTexture() { return nullptr;};
    virtual Snake *getSnake() { return nullptr;};
    virtual SDL_Point animateMain() {return {0,0};}; // so MainMenu can override
    virtual std::vector<SDL_Point> resetAnimation() {std::vector<SDL_Point> path = {{0,0}}; return path;}; 

    virtual Menu* next() { return nullptr; } //not using ? todo:deprecate

    // Optional hook when menu becomes active
    virtual void enter() {}
    virtual void setTheme(int x) {};
    virtual void toggleOffset(PageToggle direction){}; // just need this in parent so ScoreMenu can override

    virtual void resetButtons(){
        for (auto &button : _buttons){
            button->unselect();
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
            _selectedButton = newButton;
            _selectedButton->select(); 
        }
    }
};


// ******************* Custom Menus ************************************************************
class MainMenu : public Menu {
    public:
    MainMenu(SDL_Renderer* renderer);
    ~MainMenu(){
        std::cout << "Main Menu Destructor Called" << std::endl;
        SDL_DestroyTexture(_sammyTexture);
        SDL_DestroyTexture(_sandyTexture);
        SDL_DestroyTexture(_sammyBody);
        SDL_DestroyTexture(_sandyBody);
    };

    protected:
    int _theme = 0; // 0 for sammy, 1 for sandy
    SDL_Renderer* _renderer;  // going to store a copy here only so can do the setTheme effect
    std::vector<SDL_Point> path;
    size_t _next_path_point = 0;
    int _path_direction = 1;

    SDL_Texture* _sammyTexture = nullptr; // texture for character image
    SDL_Texture* _sandyTexture = nullptr; // texture for character image
    SDL_Texture* _sammyBody = nullptr; // texture for character image
    SDL_Texture* _sandyBody = nullptr; // texture for character image

    
    SDL_Texture *getCharacterTexture() override {
        if (_theme == 0 ){
            return _sammyTexture; 
        }
        else{
            return _sandyTexture;
        }   
    }
    SDL_Texture *getBodyTexture() override {
        if (_theme == 0 ){
            return _sammyBody; 
        }
        else{
            return _sandyBody;
        }   
    }

    // For Fun, animation on another thread
    SDL_Point animateMain() override {
        _next_path_point ++;
        if (_next_path_point == path.size()) {
            // _path_direction = -1;
            _next_path_point = 0; // loop back to start
        }
        // std::cout << "Next Path Point Index " << _next_path_point << std::endl;
        // std::cout << "Next Path Point  " << path[_next_path_point].x << " " << path[_next_path_point].y << std::endl;

        return path[_next_path_point]; // return copy
    }

    std::vector<SDL_Point> resetAnimation() override {
        _next_path_point = 1; // default give it the first "goal" point
        return path;
    }

    // For Fun, multiple themes
    void setTheme(int theme) override {
        if (_theme == theme) {
            return;
        }
        else{ 
            _theme = theme;
        }
        if (theme == 0) {
            _window->setBorderColor(MainConst.SAMMY_BORDER_COLOR);
            _window->setWindowColor(MainConst.SAMMY_WINDOW_COLOR);
            _window->remakeTitle(
                _renderer, 
                MainConst.SAMMY_TITLE_COLOR,
                MainConst.TITLE_FONT_SIZE,
                MainConst.TITLE_FONT,
                MainConst.TITLE_TEXT
            );
        
            for (auto &button : _buttons){

                button->setBorderColor(MainConst.BUTTON_BORDER_COLOR);
                button->setHoverColor(MainConst.HOVER_COLOR);


                button->remakeText(
                    _renderer,
                    MainConst.BUTTON_TEXT_COLOR,
                    MainConst.BUTTON_FONT_SIZE,
                    MainConst.BUTTON_FONT,
                    button->label // keep same label
                );


                if (button->label == MainConst.PLAY_BUTTON_LABEL) {
                    button->setColor(MainConst.PLAY_BUTTON_COLOR);
                }
                else if (button->label == MainConst.CHAR_BUTTON_LABEL) {
                    button->setColor(MainConst.CHAR_BUTTON_COLOR);
                }
                else if (button->label == MainConst.SCORE_BUTTON_LABEL) {
                    button->setColor(MainConst.SCORE_BUTTON_COLOR);
                }
                else if (button->label == MainConst.QUIT_BUTTON_LABEL) {
                    button->setColor(MainConst.QUIT_BUTTON_COLOR);
                }
            }
        }
        else {
            _window->setBorderColor(MainConst.SANDY_BORDER_COLOR);
            _window->setWindowColor(MainConst.SANDY_WINDOW_COLOR);
            _window->remakeTitle(
                _renderer, 
                MainConst.SANDY_TITLE_COLOR,
                MainConst.TITLE_FONT_SIZE,
                MainConst.TITLE_FONT,
                MainConst.TITLE_TEXT
            );

            // Update Button Colors
            for (auto &button : _buttons){
                button->setBorderColor(MainConst.SANDY_BUTTON_BORDER_COLOR);
                button->setHoverColor(MainConst.SANDY_BUTTON_HOVER_COLOR);

                button->remakeText(
                    _renderer,
                    MainConst.SANDY_BUTTON_TEXT_COLOR,
                    MainConst.BUTTON_FONT_SIZE,
                    MainConst.BUTTON_FONT,
                    button->label // keep same label
                );
                
                std::cout << "Update Button!" << button->label << std::endl;
                if (button->label == MainConst.PLAY_BUTTON_LABEL) {
                    button->setColor(MainConst.SANDY_PLAY_BUTTON_COLOR);
                }
                else if (button->label == MainConst.CHAR_BUTTON_LABEL) {
                    button->setColor(MainConst.SANDY_CHAR_BUTTON_COLOR);
                }
                else if (button->label == MainConst.SCORE_BUTTON_LABEL) {
                    button->setColor(MainConst.SANDY_SCORE_BUTTON_COLOR);
                }
                else if (button->label == MainConst.QUIT_BUTTON_LABEL) {
                    button->setColor(MainConst.SANDY_QUIT_BUTTON_COLOR);
                }
            }
        }
    }
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
    std::string _playerName = PlayerConst.DEFAULT_PLAYER_NAME; 
    std::unique_ptr<DynamicWindow> _textEntry; 
    Uint32 _lastToggleTime = 0;
    // bool cursor_visible = true; // not using
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

class CharacterMenu : public Menu {

    std::string _selectedCharacter = CHARACTER_NAMES[0]; // default
    // int _num_characters = 0; // not used
    std::vector<SDL_Rect> _characterRects;

    public:
    CharacterMenu(SDL_Renderer* renderer);
    ~CharacterMenu(){
        std::cout << "Character Menu Destructor Called" << std::endl;
    }; // responsible for destroying image textures

    void setCharacterSelection(const std::string& character) override { _selectedCharacter = character; }
    std::string getCharacterSelection() const override { return _selectedCharacter; }


    private:
    void generateGridDimensions();

};

// class GameOver : public Menu {

//     std::string _selectedCharacter = CHARACTER_NAMES[0]; // default
//     int _num_characters = 0;
//     std::vector<SDL_Rect> _characterRects;

//     public:
//     GameOver(SDL_Renderer* renderer){};
//     ~GameOver(){
//         std::cout << "Game Over Menu Destructor Called" << std::endl;
//     }; // responsible for destroying image textures

//     void setCharacterSelection(const std::string& character) override { _selectedCharacter = character; }
//     std::string getCharacterSelection() const override { return _selectedCharacter; }


//     private:
//     void generateGridDimensions();

// };


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
    // Menu* _prevMenu = nullptr;  // Not Using
    MenuState _state = MenuState::MAIN_MENU;
    // MenuState _prevState = MenuState::NONE; // should help coordinating async
    std::string _playerName = PlayerConst.DEFAULT_PLAYER_NAME; 
    std::string _selectedCharacter = CHARACTER_NAMES[0]; //default to Sammy

    // bool main_theme_sammy = true; // 0 for sammy theme, 1 for cindy theme ( not using )
    bool _launchPlanner; // flag to start planner when return to main menu

    public:

    explicit MenuManager(SDL_Renderer* renderer) : //note to self: using "explicit"?
        mainMenu(std::make_unique<MainMenu>(renderer)),
        scoreMenu(std::make_unique<ScoreMenu>(renderer)),
        nameInput(std::make_unique<PlayerEntryMenu>(renderer)),
        characterMenu(std::make_unique<CharacterMenu>(renderer)),
        _renderer(renderer)
    {
        _currentMenu = mainMenu.get(); // entry
        // std::cout << "Main Pointer : " << mainMenu.get() << std::endl; //debug
        // std::cout << "Score Pointer : " << scoreMenu.get() << std::endl; //debug

    };
    ~MenuManager() 
    {
        std::cout << "MenuManager Destructor Called" << std::endl;
    }; 
    //TODO any unique logic needed here?
    std::string getPlayerName() const { return _playerName; }
    std::string getCharacterSelection() const { return _selectedCharacter; }

    void switchMenu() {
        // sleeping .1 of second for some smoothness on transitions
        if (_state == MenuState::NONE) { 
            return;
        }
        switch (_state){
            case MenuState::MAIN_MENU:
                _currentMenu = mainMenu.get();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                // std::cout << "Switch: "<< _currentMenu << std::endl;
                break;
            case MenuState::SCORE_MENU:
                _currentMenu = scoreMenu.get();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                // std::cout << "Switch: "<< _currentMenu << std::endl;
                break;
            case MenuState::PLAYER_NAME:
                _currentMenu = nameInput.get();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                break;
            case MenuState::PLAY:
                // std::cout << "Switch Play: "<< _currentMenu << std::endl;
                break;
            case MenuState::CHARACTER_MENU:
                _currentMenu = characterMenu.get();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                break;
            case MenuState::BACK:
                _currentMenu->resetButtons();
                _currentMenu = mainMenu.get();
                _state = MenuState::MAIN_MENU; // in this simple menu, back always goes to main
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                break;
            case MenuState::QUIT:
                _currentMenu = nullptr;
                break;
            default:
                _currentMenu = nullptr;
                break;
        }
    }

    void Render(SDL_Renderer* renderer) {
        if (mainMenu) {
            
            if (_selectedCharacter == CHARACTER_NAMES[0]) {
                mainMenu->setTheme(0); //Sammy
            }
            else {
                mainMenu->setTheme(1);
            }
        }
        if (_currentMenu) {
            _currentMenu->Render();
        }
    }

    MenuState handleEvent(const SDL_Event& e) {
        if (_currentMenu == nameInput.get()){
            nameInput->getNameInput(e); 
            if (nameInput->getPlayerName().size() >= 2) {
                _playerName = nameInput->getPlayerName();
            } // TODO: playerName constraints?
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

        // a flag to make animation optional
        bool _shouldAnimate = MainConst.ANIMATE_MAIN_MENU; 

        // animation state
        _launchPlanner = _shouldAnimate;
        SDL_Rect _dest;
        SDL_Point _send;
        SDL_Point next_point; //memory for animation goal
        std::future<bool> future;
        SDL_Point pos; // render position
        std::vector<SDL_Rect> body;
        size_t body_size = 30;
        body.reserve(body_size);
        SDL_Point _charDims = {24,24};
        Snake snake(kGridWidth, kGridHeight); // dummy snake for rendering body
        snake.name = "Sammy";
        SDL_Point dummy_loc = {-10,-10}; // no enemy in this case, init off grid

        // Main and Manager share ownership
        std::shared_ptr<std::atomic<bool>> flag = std::make_shared<std::atomic<bool>>(false);
        std::shared_ptr<MessageQueue<SDL_Point>> animateq = std::make_shared<MessageQueue<SDL_Point>>(flag);
        std::shared_ptr<MessageQueue<SDL_Point>> managerq = std::make_shared<MessageQueue<SDL_Point>>(flag);

        Planner cycler(
            managerq.get(),    // cycler publishes to manager
            animateq.get(),   // cycler subscribes from animate
            flag
        );
        
        //sync these speeds (hacky but whatever)
        cycler.setSpeed(0.15f); 
        snake.speed = 0.15f;
        cycler.stop(); // ensure in waiting state at init

        // reusable shutdown lambda for threads
        auto shutdown_planner = [&]() {
            flag->store(true);
            cycler.stop();        // stop internal loop
            managerq->shutdown();  // flush queues
            animateq->shutdown();
            future.get();         // join         
        };

        //TODO: move these to utils
        // grow body (repurpose snake functions)
        auto init_body = [&snake, &body_size, &dummy_loc](
                SDL_Point const head, size_t const vertical_offset=0, size_t const horizontal_offset=0)
            {
            // init body in grid space, head should alreday be properly scaled
            snake.body.clear();
            SDL_Point prev_head_cell = head;
            for (size_t i = 1; i < body_size+1; i++) {
                SDL_Point curr_cell = {
                    prev_head_cell.x - static_cast<int>(i*horizontal_offset), 
                    prev_head_cell.y - static_cast<int>(i*vertical_offset)
                };
                snake.GrowBody();
                snake.UpdateBody(curr_cell, prev_head_cell, dummy_loc);
                prev_head_cell = curr_cell;
            }
            // std::cout << snake.body.size() << " Body Size After Init " << std::endl;
        };

        auto map_to_screen = [&](SDL_Point point) {
            return SDL_Point{
                static_cast<int>((point.x * kScreenWidth)/kGridWidth),
                static_cast<int>((point.y * kScreenHeight)/kGridHeight)
            };
        };

        while (running && _currentMenu) {

            // If Play, exit the menu
            if (_state == MenuState::PLAY) { 
                std::cout<< "We are playing!! " << cycler.is_running() << std::endl; 
                return true;
            }  //start game

            //main render func needs to be able get next move from animator
            // when should it publish new goal points?, signal telling when ?
            // checks length queue perhaps and when < 2, sends next point


            // captures moment of switching to main 
            if (_state == MenuState::MAIN_MENU && _shouldAnimate && !_launchPlanner) {
                flag->store(false);
                _launchPlanner = true;
                // std::cout << "Should Launch? " << _launchPlanner << std::endl;
            }

            // shutdown cycler if no longer at main
            if (_currentMenu != mainMenu.get() && cycler.is_running() && _shouldAnimate) {
                shutdown_planner();
                // std::cout << "Cycle Thread Stopped " << cycler.is_running()  << std::endl;
            }

            // Finally, poll events
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT || _state == MenuState::QUIT) {
                    running = false; // exit menu (should exit anyway from nullptr)
                    break;
                }
                _state = handleEvent(e);
                switchMenu(); // captures button press
                std::cout << "Menu State: " << static_cast<int>(_state) << std::endl;
                std::cout << "Event: type=" << e.type << std::endl;
            }
            if (!running) {break;} // extra break flag


            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); //background
            SDL_RenderClear(_renderer);
            SDL_SetRenderDrawColor(_renderer, 
                            BACKGROUND_COLOR.r, 
                            BACKGROUND_COLOR.g, 
                            BACKGROUND_COLOR.b, 
                            BACKGROUND_COLOR.a);
            SDL_RenderClear(_renderer); // Clear screen
            Render(_renderer); // Render current menu

            // if at main and should launch, launch once
            if (_currentMenu == mainMenu.get() &&  _launchPlanner && _shouldAnimate && !cycler.is_running()) {
                // reset initial positions here ?? Animation may do something funky without full proper reset
                std::vector<SDL_Point> path = mainMenu->resetAnimation();
                SDL_Point start = path[0];
                SDL_Point goal = path[1];
                next_point = goal;
                _send = next_point;
                cycler.setPosition(start.x, start.y);
                snake.SetHead(start.x,start.y);
                init_body(start, 0, 0); // reset body
                snake.direction = Snake::Direction::kLeft; // reset direction
                
                // flush/init queues:
                managerq->clear(); animateq->clear(); flag->store(false);
                managerq->receive(std::move(start)); // send point to the manager
                animateq->receive(std::move(goal)); // receive first goal point at the animator
                cycler.start(start.x, start.y); // set run flag to true
                future = std::async(std::launch::async, &Planner::run, &cycler);
                _launchPlanner = false;
                flag->store(false);
                // std::cout << "Menu Animation Thread Launched " << _launchPlanner <<  std::endl; // Debug
            }

            // Render Animation Step:
            if (_currentMenu == mainMenu.get() && _shouldAnimate) {
                
                // Get new point if current goal is reached 
                // DEBUG
                // std::cout << "Pos: " << pos.x  << " " << pos.y <<  " Goal: " << next_point.x<< " "<< next_point.y << std::endl;
                // std::cout << "Head: " << snake.head_x  << " " << snake.head_y << std::endl;
                // std::cout << cycler.Goal().x << " " << cycler.Goal().y << std::endl;
                // std::cout << &next_point <<  std::endl;
                if ((std::abs(snake.head_x - next_point.x) < (1.01*snake.speed) ) && (std::abs(snake.head_y - next_point.y) < (1.01*snake.speed))) {
                    next_point = mainMenu->animateMain();
                    // next_point = map_to_grid(next_point);
                    // std::cout << "New Goal Point: " << next_point.x << " " << next_point.y << std::endl;
                    if (_send.x < next_point.x) {
                        snake.direction = Snake::Direction::kRight;
                        // std::cout << "Direction Right " << std::endl;
                    }
                    else if (_send.x > next_point.x) {
                        snake.direction = Snake::Direction::kLeft;
                        // std::cout << "Direction Left " << std::endl;
                    }
                    else if (_send.y < next_point.y) {
                        snake.direction = Snake::Direction::kDown;
                        // std::cout << "Direction Down " << std::endl;
                    }
                    else if (_send.y > next_point.y) {
                        snake.direction = Snake::Direction::kUp;
                        // std::cout << "Direction Up " << std::endl;
                    }
                    else {
                        // should not happen
                        // std::cout << "Direction Not Changed " << std::endl;
                    }
            
                //Debug
                // std::cout << "What is pos from queue  (" << pos.x << " " << pos.y << ")  Goal: (" << next_point.x << " " << next_point.y <<")"
                //         " Prev: (" << _send.x << " " << _send.y << ")" << std::endl ;

                _send = next_point;
                
                }
                else {
                    // Because the "run" function subscribes every iteration,
                    // need to keep filling the queue, not the cleanest but should work
                    next_point = _send;
                }
                animateq->receive(std::move(next_point)); // send new goal, animator receives point


                // Get point for rendering
                std::optional<SDL_Point> msg = managerq->send(); // send point to the manager
                if (msg.has_value()) {
                    pos = msg.value();
                    // snake.ChooseDirection(pos); // derive direction from screen plan
                    snake.Update(dummy_loc); // should update head and body in grid space
                } else {
                    std::cout << " Did not receive menu animator point " << std::endl;
                }
                // Head, map back to screen space for rendering
                
                SDL_Point screen_head = map_to_screen(
                    {static_cast<int>(snake.head_x),
                     static_cast<int>(snake.head_y)}
                );
                _dest = {
                    screen_head.x, 
                    screen_head.y, 
                    _charDims.x, 
                    _charDims.y
                };
                SDL_RenderCopy(_renderer, mainMenu->getCharacterTexture(), nullptr, &_dest);

                // Body, map back to screen space for rendering
                SDL_Rect block;
                for (SDL_Point const &pt : snake.body) {
                    SDL_Point mapped = map_to_screen(pt);
                    block.x = mapped.x; //_charDims.x/2
                    block.y = mapped.y;
                    block.w = _charDims.x;
                    block.h = _charDims.y;
                    SDL_RenderCopy(_renderer, mainMenu->getBodyTexture(), nullptr, &block);
                }
            };
            SDL_RenderPresent(_renderer);  // Update Screen
            SDL_Delay(20); // ~60fps

        }


        // extra safety for hard quit path:
        if (cycler.is_running()) {
            shutdown_planner();
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // idk, saw a segfault once maybe this will help
        }

        return false; // default exit point, cycler and queues go out of scope 
    }
};
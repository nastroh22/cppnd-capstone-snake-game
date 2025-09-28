#pragma once

#include <cstddef>
#include <unordered_map>
#include <string>
#include <array>
#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"

// constexpr std::size_t kFramesPerSecond{60};
/*  Core Game Constants
------------------------------------------------------------------------------------------------*/
constexpr std::size_t kFramesPerSecond{60}; // For Debugging
constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
constexpr std::size_t kScreenWidth{640};
constexpr std::size_t kScreenHeight{640};
constexpr std::size_t kGridWidth{32};
constexpr std::size_t kGridHeight{32};


/* NAMED COLOR THEMES (can of course add whatever you'd like!)
see: https://www.color-hex.com/color-palette/59812 (good resource for rgb pallettes)
----------------------------------------------------------------------------------------------*/
//neutrals
constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color GREY = {192, 197, 206, 255};
constexpr SDL_Color SLATE = {112, 128, 144, 255};
constexpr SDL_Color BLACK = {0, 0, 0, 255};

//blues
constexpr SDL_Color DEEPBLUE = {1, 31, 75, 255};
constexpr SDL_Color RETRO_BLUE = {125, 249, 255, 250};

//earthy greens and browns
constexpr SDL_Color TAN = {255, 219, 172, 150}; // darker : (241,194,125)
constexpr SDL_Color FOREST = {39, 62, 6, 200};
constexpr SDL_Color ARMY = {75, 92, 9, 250};
constexpr SDL_Color OLIVE = {106, 118, 12, 255};
constexpr SDL_Color MOSS = {134, 141, 7, 255};
constexpr SDL_Color SPRING = {157, 174, 17, 250};
constexpr SDL_Color AVOCADO = {85, 131, 3, 255};
constexpr SDL_Color SIENNA = {212, 140, 87,255};
constexpr SDL_Color RUST = {185, 90, 17, 255};
constexpr SDL_Color GOLD = {183, 153, 6, 255};
constexpr SDL_Color DARKGOLD = {152, 114, 11, 255};

//yellows and reds
constexpr SDL_Color BRICK = {191, 0, 0, 255};
constexpr SDL_Color RED = {255, 0, 0, 255};
constexpr SDL_Color FIRE = {252, 78, 41, 255};
constexpr SDL_Color LEMON = {255, 244, 79, 255};
constexpr SDL_Color YELLOW = {255, 255, 0, 255};
constexpr SDL_Color RETRO_PEACH = {255, 226, 138, 250};

//Greens
constexpr SDL_Color NEON_GREEN = {12, 205, 23, 255};
constexpr SDL_Color GREEN = {0, 128, 0, 255};
constexpr SDL_Color BRIGHT_GREEN = {69, 221, 18, 255};
constexpr SDL_Color DARTMOUTH = {0, 121, 68, 255};

// Purples
constexpr SDL_Color LIGHT_MAGENTA = {221, 160, 221, 250} ;
constexpr SDL_Color PURPLE = {115, 79, 150, 250};
constexpr SDL_Color MAGENTA = {238, 130, 238, 250};
constexpr SDL_Color LAVENDER = {191, 148, 228, 250};


/* Defaults
------------------------------------------------------------------------------------------------*/
//windows
constexpr SDL_Color BACKGROUND_COLOR = BLACK;
constexpr SDL_Color TITLE_COLOR = LEMON;
constexpr SDL_Color DEFAULT_TEXT_COLOR = WHITE;
constexpr SDL_Color DEFAULT_WINDOW_COLOR = FOREST;
constexpr SDL_Color DEFAULT_WINBORDER_COLOR = MOSS;

// buttons
constexpr SDL_Color DEFAULT_HOVER_COLOR = RETRO_BLUE; //Lemon
constexpr SDL_Color DEFAULT_BORDER_COLOR = WHITE;
constexpr SDL_Color SELECT_BORDER_COLOR = FIRE; 
constexpr SDL_Color HIGHLIGHT_TEXT_COLOR = DEEPBLUE; // TODO: check in use ?
constexpr SDL_Color BACK_BUTTON_COLOR = RUST; // generic back button (kind of working like a default right now)
constexpr SDL_Color DEFAULT_BACK_BUTTON_COLOR = BACK_BUTTON_COLOR; // for consistency (clean)

constexpr float DEFAULT_HOVER_THICKNESS = 5;

//fonts
constexpr const char* DEFAULT_BUTTON_FONT = "public_pixel";  //public_pixel
constexpr const char* DEFAULT_TITLE_FONT = "rocket";
constexpr const char* DEFAULT_TABLE_FONT = "game_played";

constexpr int DEFAULT_BUTTON_FONT_SIZE = 20;
constexpr int DEFAULT_TITLE_FONT_SIZE = 32;
constexpr int DEFAULT_TITLE_OFFSET = 20;

constexpr double MENU_WIDTH_PROPORTION = .12;
constexpr double MENU_HEIGHT_PROPORTION = .085;



/* Customize Menus
---------------------------------------------------------------------------------------------- */
struct MainMenuConstants {

    std::string TITLE_TEXT = "Welcome to SSSnake!";
    int TITLE_FONT_SIZE = DEFAULT_TITLE_FONT_SIZE;

    const int button_width = 265;
    const int button_height = 85;
    const int button_x = static_cast<int>((kScreenWidth - button_width) / 2); // center
    const int button_y_padding = 15;
    const int starting_y = 150;

    // Future TODO: use button layout functions instead
    SDL_Rect PLAY_BUTTON_RECT =  {button_x,   starting_y,                                     button_width,  button_height};
    SDL_Rect CHAR_BUTTON_RECT =  {button_x,  (starting_y+button_y_padding+button_height),     button_width,  button_height};
    SDL_Rect SCORE_BUTTON_RECT = {button_x,  (starting_y+2*(button_y_padding+button_height)), button_width,  button_height};
    SDL_Rect QUIT_BUTTON_RECT =  {button_x,  (starting_y+3*(button_y_padding+button_height)), button_width,  button_height};

    std::string SCORE_BUTTON_LABEL = "Scoreboard";
    std::string CHAR_BUTTON_LABEL = "Characters";
    std::string QUIT_BUTTON_LABEL = "Quit";
    std::string PLAY_BUTTON_LABEL = "Play!";


    SDL_Color PLAY_BUTTON_COLOR = AVOCADO; //SPRING; //ARMY
    SDL_Color CHAR_BUTTON_COLOR = ARMY;
    SDL_Color SCORE_BUTTON_COLOR = MOSS; //GOLD, ARMY, DARKGOLD
    SDL_Color QUIT_BUTTON_COLOR = RUST;
    SDL_Color WINDOW_COLOR = DEFAULT_WINDOW_COLOR;
    SDL_Color WINBORDER_COLOR = DEFAULT_WINBORDER_COLOR;
    
    std::string BUTTON_FONT = "comic_sans";
    int BUTTON_FONT_SIZE = 32;
    SDL_Color BUTTON_TEXT_COLOR = WHITE;
    SDL_Color BUTTON_BORDER_COLOR = WHITE;
    SDL_Color HOVER_COLOR = YELLOW;
    
};


struct PlayerEntryConstants {
    
    //title text
    const std::string TITLE_TEXT = "Enter Your Name: ";
    int TITLE_FONT_SIZE = DEFAULT_TITLE_FONT_SIZE + 2;
    SDL_Color TITLE_TEXT_COLOR = YELLOW;
    std::string TITLE_FONT = DEFAULT_TITLE_FONT;
    int TITLE_OFFSET = 45; //to-do, full configuration

    //window and borders
    SDL_Color WINDOW_COLOR = DEEPBLUE;
    SDL_Color WINBORDER_COLOR = RETRO_BLUE;
    SDL_Color NAME_WINDOW_BORDER_COLOR = RETRO_BLUE; //MINT
    SDL_Color NAME_WINDOW_COLOR = WHITE;
    SDL_Color NAME_WINDOW_TEXT_COLOR = BLACK;
    int NAME_WINDOW_OFFSET = 70; // From default starting position

    //button sizes
    const int button_width = 250;
    const int button_height = 85;
    const int button_x = static_cast<int>((kScreenWidth - button_width) / 2); // center
    const int start_height = 300;
    const int pad_y = 20;

    SDL_Color START_BUTTON_COLOR = NEON_GREEN;
    SDL_Color BACK_BUTTON_COLOR = FIRE;
    SDL_Rect START_BUTTON_RECT = {button_x, start_height,  button_width,  button_height};
    SDL_Rect BACK_BUTTON_RECT =  {button_x, start_height + pad_y + button_height,  button_width, button_height};
    const std::string START_BUTTON_LABEL = "Start >>";
    const std::string BACK_BUTTON_LABEL = "<< Back";

    const std::string DEFAULT_PLAYER_NAME = "Player1";
    int NAME_TEXT_FONT_SIZE = 32;
    const std::string NAME_TEXT_FONT = "comic_sans";

    const int CURSOR_BLINK_INTERVAL_MS = 500;
};


struct ScoreMenuConstants {
    std::string TITLE_TEXT = "High Scores!";
    int TITLE_FONT_SIZE = DEFAULT_TITLE_FONT_SIZE;
    std::string TITLE_FONT = DEFAULT_TITLE_FONT;
    SDL_Color TITLE_TEXT_COLOR = YELLOW; // RETRO_BLUE
    const std::string TABLE_FONT_NAME ="rocket";  // "rocket", "game_played",  "comic_sans", "public_pixel"
    int TOP_TITLE_OFFSET = 15;

    SDL_Color SCORE_CELL_COLOR = BLACK;
    SDL_Color SCORE_TEXT_COLOR = YELLOW;  // NEON_GREEN
    SDL_Color SCORE_CELL_BORDER_COLOR = RETRO_BLUE;
    
    SDL_Color SCORE_WINDOW_COLOR = SLATE;
    SDL_Color SCORE_WINDOW_BORDER_COLOR = LEMON;
    
    SDL_Color SCORE_UP_BUTTON_COLOR = BRIGHT_GREEN; // GREEN, LAVENDER, SPRING, BRIGHT_GREEN
    SDL_Color SCORE_DOWN_BUTTON_COLOR = DEEPBLUE; 
    SDL_Color SCORE_BACK_BUTTON_COLOR = FIRE;

    SDL_Color BUTTON_BORDER_COLOR = WHITE;
    SDL_Color HOVER_COLOR = RETRO_BLUE;

    int NUM_ROWS = 5;
    int NUM_COLS = 2;
};


struct CharacterMenuConstants {
    std::string TITLE_TEXT = "Choose Character"; //hack to get multi-line
    int TITLE_FONT_SIZE = 34;
    std::string TITLE_FONT = DEFAULT_TITLE_FONT;
    int TOP_TITLE_OFFSET = 35;

    SDL_Color TITLE_TEXT_COLOR = LEMON; //RETRO_PEACH
    SDL_Color WINBORDER_COLOR = RETRO_BLUE;
    SDL_Color WINDOW_COLOR = BLACK;

    const int button_width = 250;
    const int button_height = 85;
    const int button_x = static_cast<int>((kScreenWidth - button_width) / 2); 


    SDL_Rect BACK_BUTTON_RECT = {button_x, 400, 250, 85};
    SDL_Color BACK_BUTTON_COLOR = DEFAULT_BACK_BUTTON_COLOR;
    
    SDL_Color BUTTON_BORDER_COLOR = WHITE;
    SDL_Color HOVER_COLOR = RETRO_BLUE;


 
    int TITLE_OFFSET = 65; // space for title
    int BUTTON_PADDING_X = 50;
    int BUTTON_PADDING_Y = 50;
    int NUM_COLUMNS = 2;
};
constexpr int CHARACTER_TOP_OFFSET = 75; //offset to leave room for title (char menu begins here)
constexpr SDL_Color CHARACTER_BUTTON_BORDER_COLOR = WHITE; // currently does nothing

/* Make structs
---------------------------------------------------------------------------------------------*/
inline const ScoreMenuConstants ScoreConst{};
inline const CharacterMenuConstants CharConst{};
inline const PlayerEntryConstants PlayerConst{};
inline const MainMenuConstants MainConst{};

inline void debug() {
    [=](){std::cout << "DEBUG CONSTANTS : " << ScoreConst.TABLE_FONT_NAME << std::endl;}();
}



/* Asset Files and Names
---------------------------------------------------------------------------------------------*/
enum CharacterEnum {Sammy, Sandy}; // arbitrary, but consistent naming
const std::string CHARACTER_NAMES[]= {"Sammy", "Sandy"}; //for display
constexpr SDL_Color CHARACTER_COLORS[] = {AVOCADO, GOLD};
static std::unordered_map<std::string, CharacterEnum> characterEnumMap;

// Bitmap files
constexpr int NUM_CHARACTER_TEXTURES = 3; // head, body, dead
const std::unordered_map<CharacterEnum, std::array<std::string, 3>> characterFileMap = 
{
    {CharacterEnum::Sammy, {
            "../assets/characters/snake_green_head.bmp", 
            "../assets/characters/snake_green_blob.bmp", 
            "../assets/characters/snake_green_xx.bmp"
        }
    },
    {CharacterEnum::Sandy, 
        {
            "../assets/characters/snake_yellow_head.bmp", 
            "../assets/characters/snake_yellow_blob.bmp", 
            "../assets/characters/snake_yellow_xx.bmp"
        }
    }
};

namespace Assets{

    inline const std::unordered_map<std::string, std::string> fontMap = {
        {"comic_sans", "../assets/fonts/comic_sans_ms.ttf"},
        {"rocket", "../assets/fonts/rocket_command.otf"},
        {"karmatic_arcade", "../assets/fonts/karmatic_arcade.ttf"},
        {"game_played", "../assets/fonts/game_played.ttf"},
        {"rocabe", "../assets/fonts/rocabe_trial_regular.otf"},
        {"public_pixel", "../assets/fonts/public_pixel.ttf"},
        {"dejavu_sans", "../assets/fonts/DejaVuSans.ttf"}
    };

    inline const std::unordered_map<std::string, std::string> itemTextureMap = {
        { "banana", "../assets/items/banana.bmp" },
        { "dot", "../assets/items/dot.bmp" },
        { "cherries", "../assets/items/cherry.bmp" },
        { "bomb", "../assets/items/bomb.bmp" },
        { "star", "../assets/items/star.bmp" }
        // Add more as needed
    };

    inline const std::array<std::string, 5> itemNames = {"dot", "cherries", "banana", "star", "bomb"};
    inline constexpr std::array<double, 5> itemProbs = {0.25, 0.25, 0.25, 0.15, 0.10}; // dot, cherry, banana, star, bomb
    inline constexpr std::array<double, 5> itemScores = {1.0, 1.0, 1.0, 10.0, -5.0}; 

    // hawk animation
    inline constexpr int HAWK_ANIMATION_FRAMES = 6;
    inline constexpr int FLAP_RATE = 20; //frames per flap, discrete control over animation speed
    inline const std::array<std::string, HAWK_ANIMATION_FRAMES> hawkTextureFiles = {
        "../assets/enemies/hawk_000.bmp",
        "../assets/enemies/hawk_001.bmp",
        "../assets/enemies/hawk_002.bmp",
        "../assets/enemies/hawk_003.bmp",
        "../assets/enemies/hawk_004.bmp",
        "../assets/enemies/hawk_006.bmp"
    };
    inline const std::array<std::string, HAWK_ANIMATION_FRAMES> hawkTextureFilesLeft = {
        "../assets/enemies/hawkL_000.bmp",
        "../assets/enemies/hawkL_001.bmp",
        "../assets/enemies/hawkL_002.bmp",
        "../assets/enemies/hawkL_003.bmp",
        "../assets/enemies/hawkL_004.bmp",
        "../assets/enemies/hawkL_006.bmp"
    };

    //add more playable characters

    // Future Extension: add more enemies. Make Configurable via "level state" variables.

}

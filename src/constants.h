#pragma once

#include <cstddef>
#include <unordered_map>
#include <string>
#include <array>
#include "SDL.h"
#include "SDL_ttf.h"

// constexpr std::size_t kFramesPerSecond{60};
constexpr std::size_t kFramesPerSecond{60}; // For Debugging
constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
constexpr std::size_t kScreenWidth{640};
constexpr std::size_t kScreenHeight{640};
constexpr std::size_t kGridWidth{32};
constexpr std::size_t kGridHeight{32};


// NAMED COLOR THEMES (can of course add whatever you'd like!)
// see: https://www.color-hex.com/color-palette/59812 (good resource for rgb pallettes)
constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color TAN = {255, 219, 172, 150}; // darker : (241,194,125)
constexpr SDL_Color FOREST = {39, 62, 6, 200};
constexpr SDL_Color ARMY = {75, 92, 9, 250};
constexpr SDL_Color OLIVE = {106, 118, 12, 255};
constexpr SDL_Color MOSS = {134, 141, 7, 255};
constexpr SDL_Color SPRING = {157, 174, 17, 250};
constexpr SDL_Color DEEPBLUE = {1, 31, 75, 255};
constexpr SDL_Color BLACK = {0, 0, 0, 255};
constexpr SDL_Color GREY = {192, 197, 206, 255};
constexpr SDL_Color SLATE = {112, 128, 144, 255};
constexpr SDL_Color RETRO_BLUE = {125, 249, 255, 250};
constexpr SDL_Color RETRO_PEACH = {255, 226, 138, 250};
constexpr SDL_Color BRICK = {191, 0, 0, 255};
constexpr SDL_Color MINT = {111, 203, 159, 255};
constexpr SDL_Color LEMON = {255, 244, 79, 255};
constexpr SDL_Color AVOCADO = {85, 131, 3, 255};
constexpr SDL_Color SIENNA = {212, 140, 87,255};
constexpr SDL_Color DARTMOUTH = {0, 121, 68, 255};
constexpr SDL_Color RUST = {185, 90, 17, 255};
constexpr SDL_Color DARKGOLD = {152, 114, 11, 255};
constexpr SDL_Color GOLD = {183, 153, 6, 255};
constexpr SDL_Color RED = {255, 0, 0, 255};
constexpr SDL_Color FIRE = {252, 78, 41, 255};



/* MENU COLOR THEMES
----------------------------------------------*/
// Text Colors

// enum Defaults
constexpr SDL_Color BACKGROUND_COLOR = BLACK;
constexpr SDL_Color TITLE_COLOR = LEMON;
constexpr SDL_Color DEFAULT_TEXT_COLOR = WHITE;
constexpr SDL_Color DEFAULT_WINDOW_COLOR = FOREST;
constexpr SDL_Color DEFAULT_WINBORDER_COLOR = MOSS;
constexpr SDL_Color DEFAULT_HOVER_COLOR = LEMON;
constexpr SDL_Color DEFAULT_BORDER_COLOR = WHITE;
constexpr SDL_Color SELECT_BORDER_COLOR = RETRO_BLUE; // TODO: check in use ?
constexpr SDL_Color HIGHLIGHT_TEXT_COLOR = DEEPBLUE; // TODO: check in use ?

constexpr SDL_Color BACK_BUTTON_COLOR = RUST; // generic back button (kind of working like a default right now)
constexpr SDL_Color DEFAULT_BACK_BUTTON_COLOR = BACK_BUTTON_COLOR; // for consistency
constexpr SDL_Color HOVER_BORDER_COLOR = LEMON;

struct MainMenuConstants {
    static inline constexpr SDL_Rect PLAY_BUTTON_RECT = {220, 150, 200, 75};
    static inline constexpr SDL_Rect CHAR_BUTTON_RECT = {220, 250, 200, 75};
    static inline constexpr SDL_Rect SCORE_BUTTON_RECT = {220, 350, 200, 75};
    static inline constexpr SDL_Rect QUIT_BUTTON_RECT = {220, 450, 200, 75};

    static inline constexpr SDL_Color PLAY_BUTTON_COLOR = MOSS ; //SPRING; //ARMY
    static inline constexpr SDL_Color CHAR_BUTTON_COLOR = OLIVE;
    static inline constexpr SDL_Color SCORE_BUTTON_COLOR = AVOCADO;
    static inline constexpr SDL_Color QUIT_BUTTON_COLOR = RUST;
};

inline constexpr MainMenuConstants MMConst{};


// Button Backgrounds (TODO Bundle into Menu Type Blocks)
// constexpr SDL_Color PLAY_BUTTON_COLOR = MOSS ; //SPRING; //ARMY
// constexpr SDL_Color CHAR_BUTTON_COLOR = OLIVE;
// constexpr SDL_Color SCORE_BUTTON_COLOR = AVOCADO;
// constexpr SDL_Color QUIT_BUTTON_COLOR = RUST;
// constexpr SDL_Color START_BUTTON_COLOR = MOSS;

struct PlayerEntryConstants {
    static inline constexpr SDL_Color WINDOW_COLOR = DEFAULT_WINDOW_COLOR;
    static inline constexpr SDL_Color WINBORDER_COLOR = DEFAULT_WINBORDER_COLOR;
    static inline constexpr SDL_Color NAME_WINDOW_BORDER_COLOR = MINT;
    static inline constexpr SDL_Color NAME_WINDOW_COLOR = WHITE;
    static inline constexpr SDL_Color NAME_WINDOW_TEXT_COLOR = BLACK;
    static inline constexpr int NAME_TEXT_FONT_SIZE = 28;

    static inline constexpr SDL_Color START_BUTTON_COLOR = MOSS;
    static inline constexpr SDL_Color BACK_BUTTON_COLOR = DEFAULT_BACK_BUTTON_COLOR;
    static inline constexpr SDL_Rect START_BUTTON_RECT = {220, 250, 200, 75};
    static inline constexpr SDL_Rect BACK_BUTTON_RECT = {220, 350, 200, 75};

    
    const std::string DEFAULT_PLAYER_NAME = "Player1";
    const int CURSOR_BLINK_INTERVAL_MS = 500;
};
inline const PlayerEntryConstants PEConst{};

// Start Menu Constants
// constexpr SDL_Rect START_BUTTON_RECT = {220, 250, 200, 75};
// constexpr SDL_Rect BACK_BUTTON_RECT = {220, 350, 200, 75};
// const std::string DEFAULT_PLAYER_NAME = "Player1";

struct ScoreMenuConstants {
    static inline constexpr SDL_Color SCORE_CELL_COLOR = BLACK;
    static inline constexpr SDL_Color SCORE_TEXT_COLOR = LEMON;
    static inline constexpr SDL_Color SCORE_CELL_BORDER_COLOR = RETRO_BLUE;
    static inline constexpr SDL_Color SCORE_WINDOW_COLOR = SLATE;
    static inline constexpr SDL_Color SCORE_WINDOW_BORDER_COLOR = LEMON;

    static inline constexpr SDL_Color SCORE_UP_BUTTON_COLOR = MINT;
    static inline constexpr SDL_Color SCORE_DOWN_BUTTON_COLOR = DEEPBLUE;
};
inline constexpr ScoreMenuConstants ScoreConst{};

// // Colors are customizable
// constexpr SDL_Color SCORE_CELL_COLOR = BLACK;
// constexpr SDL_Color SCORE_TEXT_COLOR = LEMON;
// constexpr SDL_Color SCORE_CELL_BORDER_COLOR = RETRO_BLUE;
// constexpr SDL_Color SCORE_WINDOW_COLOR = SLATE;
// constexpr SDL_Color SCORE_WINDOW_BORDER_COLOR = LEMON;


/* MENU COLOR THEMES
----------------------------------------------*/
// constexpr SDL_Color NAME_WIN_BORDER_COLOR = MINT;
// constexpr SDL_Color NAME_WIN_COLOR = WHITE;
// constexpr SDL_Color NAME_WIN_TEXT_COLOR = BLACK;


/* Menu Format Constants
------------------------------------------------*/
struct CharacterMenuConstants {
    SDL_Color WINBORDER_COLOR = DEFAULT_WINBORDER_COLOR;
    SDL_Color WINDOW_COLOR = DEFAULT_WINDOW_COLOR;

    SDL_Rect BACK_BUTTON_RECT = {220, 400, 200, 75};
    SDL_Color BACK_BUTTON_COLOR = DEFAULT_BACK_BUTTON_COLOR;
};
inline constexpr CharacterMenuConstants CharConst{};

constexpr int CHARACTER_TOP_OFFSET = 75; //offset to leave room for title (char menu begins here)
constexpr SDL_Rect CHARACTER_BACK_BUTTON_RECT = {220, 400, 200, 75};
constexpr int CHARACTER_BUTTON_PADDING_X = 50;
constexpr int CHARACTER_BUTTON_PADDING_Y = 50;
constexpr int NUM_CHARACTER_GRID_COLUMNS = 2; 
constexpr SDL_Color CHARACTER_BUTTON_BORDER_COLOR = WHITE; // currently does nothing




/* Character Renderings and Names
------------------------------------------------*/
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

    //add playable characters


    // Future Extensions, add more enemies. Make Configurable via level state variables.

}

#include "constants.h"
#include "SDL.h"
#include "utils.h"
// #include "menu.h"

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
        int _text_x, _text_y; // for text positioning
        std::string _cellText;
        bool _shouldUpdate = false;
        // Text _content; // could be vector of texts for multiple lines
    
    public:
        Window(SDL_Renderer* renderer, const std::string& title, SDL_Color win_color, 
                SDL_Color border_color, SDL_Rect rect, int text_font_size = 32, SDL_Color text_color = TITLE_COLOR) : 
                    _windowRect(rect), _windowColor(win_color), _borderColor(border_color),
                        _title(renderer, "../assets/fonts/comic_sans_ms.ttf", text_font_size, title, text_color) {};
        ~Window() = default; // will call _text destructor by default
        
        DISABLE_COPY_ENABLE_MOVE(Window);

        //formatting helpers
        void leftJustify() {_text_x = _windowRect.x + 15 ;}
        void rightJustify() {_text_x = _windowRect.x + _windowRect.w - _title.getWidth() - 15 ;}
        void setX(int x) {_text_x = x;}
        void setY(int y) {_text_y = y;}
        int getX() const { return _text_x; }
        int getY() const { return _text_y; }
        void setTextPos(int x, int y) { _text_x = x; _text_y = y; }
        void centerText(){
            _text_x = _windowRect.x + (_windowRect.w - _title.getWidth()) / 2;
            _text_y = _windowRect.y + (_windowRect.h - _title.getHeight()) / 2;
        }
        void titleText(){
            _text_x = _windowRect.x + (_windowRect.w - _title.getWidth()) / 2;
            _text_y = _windowRect.y + 10; // pad from top
        }

        void virtual Render(SDL_Renderer* renderer);
};

// TODO: a Window and a cell are very similar, could refactor to avoid code duplication
class Cell : public Window {
    std::string _cellText;
    bool _shouldUpdate = false;

    public:
        Cell(SDL_Renderer* renderer, const std::string& text, SDL_Color cell_color, 
                SDL_Color border_color, SDL_Rect rect, int text_font_size = 24, SDL_Color text_color = SCORE_TEXT_COLOR) :
                    Window(renderer, text, cell_color, border_color, rect, text_font_size, text_color), _cellText(text) 
                    {
                        centerText();
                        std::cout << "Cell created: " << text << " " <<  _text_y << _text_x <<std::endl;

                    };
        ~Cell() = default; // will call _text destructor by default
        DISABLE_COPY_ENABLE_MOVE(Cell);
        virtual void Render(SDL_Renderer* renderer) override;
        void UpdateText(const std::string &new_text) {
            _cellText = new_text; _shouldUpdate = true;
        }
        // Next TODO: either reduce Text size (if goes out of bounds or replace with ... and return option for player to view it)
};


enum PageToggle {UP, DOWN, TOP, NONE};

//"TextTable"
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
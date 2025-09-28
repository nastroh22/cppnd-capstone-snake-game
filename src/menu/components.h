#pragma once
#include "SDL.h"
#include "./utils.h"
#include "./constants.h"

// Character Menu Positions
// constexpr SDL_Rect CHARACTER_BUTTON_RECT = {220, 350, 200, 75};

// TODO : Font Options
// maybe an even cooler feature... add themes (future)


/* "Dynamic Constants"
------------------------------- */


//Window Positions
static int border_width = static_cast<int>(MENU_WIDTH_PROPORTION*kScreenWidth);
static int border_height = static_cast<int>(MENU_HEIGHT_PROPORTION*kScreenHeight);
const SDL_Rect WINDOW_POSITION = {
    border_width, 
    border_height,
    static_cast<int>(kScreenWidth - 2*border_width), 
    static_cast<int>(kScreenHeight-2*border_height)
};

const SDL_Rect NAME_WINDOW_POSITION = {
    static_cast<int>(1.2*border_width), 
    static_cast<int>(2*border_height) + PlayerConst.NAME_WINDOW_OFFSET,
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
    WINDOW_POSITION.x + 10,
    WINDOW_POSITION.y + 60, // need room for Text
    static_cast<int>(WINDOW_POSITION.w - 20), 
    static_cast<int>(WINDOW_POSITION.h - 160) // pad 100 for buttons
};

const int score_button_pad = 10;
const int score_button_width = static_cast<int>((SCORE_TABLE_RECT.w - 2*score_button_pad)/3); // pad for spacing

// Positions are rel calcs, leave as is 
const SDL_Rect SCORE_BACK_BUTTON_RECT = {
        SCORE_TABLE_RECT.x, 
        SCORE_TABLE_RECT.y + SCORE_TABLE_RECT.h + 10, // below table
        score_button_width, 
        80
};
const SDL_Rect SCORE_UP_BUTTON_RECT = {
    SCORE_TABLE_RECT.x + score_button_width + score_button_pad, 
    SCORE_TABLE_RECT.y + SCORE_TABLE_RECT.h + 10, 
    score_button_width, 
    80
};
const SDL_Rect SCORE_DOWN_BUTTON_RECT = {
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
        SDL_Color _borderColor; // could make customizable
        Text _title;
        int _title_offset;
        int _text_x, _text_y; // for text positioning
        bool _shouldUpdate = false;
        // Text _content; // could be vector of texts for multiple lines
    
    public:
        Window(SDL_Renderer* renderer, const std::string& title, SDL_Color win_color, 
                    SDL_Color border_color, SDL_Rect rect, int font_size = DEFAULT_TITLE_FONT_SIZE, 
                        SDL_Color text_color = TITLE_COLOR, std::string font_name = DEFAULT_TITLE_FONT, int title_offset = DEFAULT_TITLE_OFFSET) :
                            _windowRect(rect),  
                            _windowColor(win_color),  
                            _borderColor(border_color), 
                            _title_offset(title_offset),
                            _title(renderer, Assets::fontMap.at(font_name), font_size, title, text_color) {};
        
        ~Window() = default; // will call _text destructor by default
        
        DISABLE_COPY_ENABLE_MOVE(Window);

        //formatting helpers
        void leftJustify() {_text_x = _windowRect.x + 15 ;}
        void rightJustify() {_text_x = _windowRect.x + _windowRect.w - _title.getWidth() - 15 ;}
        void setX(int x) {_text_x = x;}
        void setY(int y) {_text_y = y;}
        int getX() const {return _text_x;}
        int getY() const {return _text_y;}
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

class DynamicWindow : public Window {
    std::string _cellText;
    bool _shouldUpdate = false;

    public:
        DynamicWindow(SDL_Renderer* renderer, const std::string& text, SDL_Color cell_color, 
                        SDL_Color border_color, SDL_Rect rect, int text_font_size = DEFAULT_BUTTON_FONT_SIZE, 
                            SDL_Color text_color = DEFAULT_TEXT_COLOR, std::string font_name = DEFAULT_BUTTON_FONT) :
                                Window(renderer, text, cell_color, border_color, rect, text_font_size, text_color, font_name), 
                                _cellText(text) 
                    {
                        centerText();
                        std::cout << "Cell created: " << text << " " <<  _text_y << _text_x <<std::endl;
                    };
        ~DynamicWindow() = default; // will call _text destructor by default
        DISABLE_COPY_ENABLE_MOVE(DynamicWindow);
        virtual void Render(SDL_Renderer* renderer) override;
        void UpdateText(const std::string &new_text) {_cellText = new_text; _shouldUpdate = true;}
        // ?? TODO: either reduce Text size (if goes out of bounds or replace with ... and return option for player to view it)
        // or enforce a limit on text length when updating
};

enum PageToggle {
    UP, 
    DOWN, 
    TOP, 
    NONE
};

class Table  {
    // A Grid of Cells with Dynamic Text (for loading/saving)
    int const _rows;
    int const _cols;
    SDL_Rect const _tableRect; 
    SDL_Color const _cellColor; 
    SDL_Color const _cellBorderColor; 
    SDL_Color const _textColor;
    int const _textFontSize = 24; 
    std::string const _textFontName;
    std::vector<std::vector<DynamicWindow>> gridSpec;
    std::vector<std::vector<std::string>> gridData;
    int _offset = 0; //for scrolling

    public:
        Table(SDL_Rect tableDims, SDL_Color cellColor = WHITE, SDL_Color cellBorderColor = BLACK, 
                SDL_Color cellTextColor = WHITE, std::string const cellTextFont = DEFAULT_TABLE_FONT, int rows=5, int cols=2) : 
                _tableRect(tableDims), 
                _cellColor(cellColor), 
                _cellBorderColor(cellBorderColor),
                _textColor(cellTextColor), 
                _textFontName(cellTextFont),
                _rows(rows), 
                _cols(cols) {};
        ~Table() = default;
        // DISABLE_COPY_ENABLE_MOVE(Table);
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
                std::cout << "(Table) Cannot scroll further in that direction: " 
                    << _offset <<  " " << gridData.size() << " " <<  direction << std::endl;
            }
        }
        void resetOffset() { _offset = 0; UpdateCells(_offset);}
};

// **************************************** Core Button Defs ******************************************************* //
class Menu; // forward declaration needed
enum class MenuState { // Buttons tied to MenuStates
    MAIN_MENU,
    SCORE_MENU,
    CHARACTER_MENU,
    PLAYER_NAME,
    PLAY,
    BACK,
    NONE,
    QUIT
};

class Button {

protected:
    Text _text;
    const SDL_Rect _buttonRect; // for positioning button
    const SDL_Color _buttonColor; // for button color
    const MenuState _return_state = MenuState::NONE; // default state
    SDL_Color _borderColor; // non-const to enable hover effect
    std::string label = "Button";
    int _borderThickness = 2; // could make customizable
    bool _freeze_border = false; // to keep border on selected button
    int _textX, _textY; // for text positioning
    SDL_Color _hover_color;


// I think the protected constructor pattern works to make sure no abstract button can be created // TODO re-use for "Menu"
    Button(SDL_Renderer* renderer, 
            MenuState label, 
            SDL_Color color, 
            SDL_Rect rect,
            const std::string& text, 
            std::string font_name = DEFAULT_BUTTON_FONT, 
            int font_size = DEFAULT_BUTTON_FONT_SIZE, 
            SDL_Color text_color = DEFAULT_TEXT_COLOR, 
            SDL_Color border_color = DEFAULT_BORDER_COLOR, 
            SDL_Color hover_color = DEFAULT_HOVER_COLOR) 
            :  
                _return_state(label), 
                _buttonColor(color), 
                _buttonRect(rect),
                _borderColor(border_color), 
                _hover_color(hover_color), 
                _text(renderer, Assets::fontMap.at(font_name), font_size, text, text_color) 
        { 
           _textX =  _buttonRect.x + (_buttonRect.w - _text.getWidth()) / 2;
           _textY = _buttonRect.y + (_buttonRect.h - _text.getHeight()) / 2;
        } // default centered text

public:
    virtual ~Button() = default; // will call _text destructor by default

    // State Checkers
    bool wasClicked(const SDL_Point& mouse_point) const { 
        return SDL_PointInRect(&mouse_point, &_buttonRect);
    }
    bool isHovered(const SDL_Point& mouse_point) const { 
        return SDL_PointInRect(&mouse_point, &_buttonRect);
    }
    // buttons don't mutate their own state, but can mutate container state
    virtual MenuState onClick(Menu* container = nullptr) const { return _return_state; } 

    // Hover and Select Effects
    void toggleHover(const SDL_Point& mousePoint) {
        if (_freeze_border) { 
            return; // no change if frozen/selected
        } 
        if (SDL_PointInRect(&mousePoint, &_buttonRect)) {
            // Change border color on hover
            _borderColor = _hover_color;
            _borderThickness = DEFAULT_HOVER_THICKNESS; //thicker border
        } 
        else {
            // Revert to default border color
            _borderColor = DEFAULT_BORDER_COLOR; // TODO: make this configurable as well
            _borderThickness = 2;
        }
    }
    void select() {
        _borderColor = SELECT_BORDER_COLOR;
        _freeze_border = true; // keep border on selected button
    }
    void unselect() {
        _borderColor = DEFAULT_HOVER_COLOR;
        _freeze_border = false;
    }

    // Setter Utils
    void setX(int x) { _textX = x; }
    void setY(int y) { _textY = y; }

    // Render Core Button
    virtual void Render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, _buttonColor.r,  _buttonColor.g,  _buttonColor.b,  _buttonColor.a);
        SDL_RenderFillRect(renderer, &_buttonRect);
        RenderUtils::drawBorder(renderer, _buttonRect, _borderThickness, _borderColor);
        _text.display(renderer, _textX, _textY);
    }


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
    // rendering is the only unique effect of ImageButtons
    void Render(SDL_Renderer* renderer) override {

        SDL_SetRenderDrawColor(renderer, _buttonColor.r,  _buttonColor.g,  _buttonColor.b,  _buttonColor.a);
        SDL_RenderFillRect(renderer, &_buttonRect);
        RenderUtils::drawBorder(renderer, _buttonRect, _borderThickness, _borderColor);

        int title_offset = 15;
        setY(_buttonRect.y + 10); // text y position (redunant)
        _text.display(renderer, _textX, _buttonRect.y + title_offset); 

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
};

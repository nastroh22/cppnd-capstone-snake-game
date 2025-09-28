#include "components.h"
#include "SDL.h"

// ****************************************** Text Defs  ************************************************ //
Text::Text(
    SDL_Renderer *renderer,
    const std::string &font_path,
    int font_size,
    const std::string &message_text,
    SDL_Color color) : _color(color)
{
    loadFont(renderer, font_path, font_size);
    _text_texture = makeTexture(renderer, message_text, _color);
    if (!_text_texture) {
        std::cerr << "Failed to load text texture." << std::endl;
    }
    SDL_QueryTexture(_text_texture, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
    std::cout << "Loaded Text: " << message_text << std::endl;
}
void Text::display(SDL_Renderer *renderer, int x, int y) const
{
    _text_rect.x = x;
    _text_rect.y = y;
    SDL_RenderCopy(renderer, _text_texture, nullptr, &_text_rect);
}

void Text::displayDynamic(SDL_Renderer *renderer, int x, int y, const std::string &message) {
    _text_rect.x = x;
    _text_rect.y = y;
    if (_text_texture) {
        // TTF_CloseFont(_font);
        SDL_DestroyTexture(_text_texture);
        _text_texture = nullptr;
    }
    _text_texture = makeTexture(renderer, message, _color); // TODO - store color and font_size
    SDL_QueryTexture(_text_texture, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
    SDL_RenderCopy(renderer, _text_texture, nullptr, &_text_rect);
}

void Text::loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size)
{
    _font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!_font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

SDL_Texture *Text::makeTexture(SDL_Renderer *renderer,  const std::string &message_text, SDL_Color& color)
{
    auto text_surface = TTF_RenderUTF8_Solid(_font, message_text.c_str(), color); //TTF_RenderText_Solid

    if (!text_surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(_font);
        _font = nullptr; // reassign to avoid dangling pointer
        return nullptr;
    }
    
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(text_surface);
    return text_texture;
}

// ************************************  Window Defs ***************************************************************** //
void Window::Render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, _windowColor.r,  _windowColor.g,  _windowColor.b,  _windowColor.a);
    SDL_RenderFillRect(renderer, &_windowRect);
    RenderUtils::drawBorder(renderer, _windowRect, 2, _borderColor); // thickness of 2
    _title.display(renderer, 
        _windowRect.x + (_windowRect.w - _title.getWidth()) / 2,
        _windowRect.y + _title_offset
    );
}

void DynamicWindow::Render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, _windowColor.r,  _windowColor.g,  _windowColor.b,  _windowColor.a);
    SDL_RenderFillRect(renderer, &_windowRect);
    // support dynamic text
    RenderUtils::drawBorder(renderer, _windowRect, 2, _borderColor); // thickness of 2
    if (_shouldUpdate) {
        _title.displayDynamic(renderer, _text_x, _text_y, _cellText); //title is the SDL_TTF interface
        _shouldUpdate = false;
    } else {
        _title.display(renderer, _text_x, _text_y);
    }
}

// **************************************** Table Defs ************************************************************** //
void Table::buildGrid(SDL_Renderer *renderer) {

    int const cellWidth = _tableRect.w / _cols;
    int const cellHeight = _tableRect.h / _rows; // TODO: make scrollable
    gridData = ScoreIO::load_scores(); 

    gridSpec.clear(); // should only be built once, but in case
    gridSpec.resize(_rows);
    std::cout << "Inside buildGrid, gridSpec size: " << gridSpec.size() << gridData.size() << std::endl; //debug
    std::cout << _textFontName << std::endl; //debug
    // int width_adjust = cellWidth;
    int width_adjust = 0; //tight layout
    int prev_width = 0;

    for (int i = 0; i < _rows; ++i) {
    
        std::vector<std::string> entry = gridData[i];
        gridSpec[i].reserve(_cols); // avoid multiple allocations

        for (int j = 0; j < _cols; ++j) {
            
            width_adjust = (j % 2 == 0) ? cellWidth+70 : cellWidth-70; // small hack for 2-column

            SDL_Rect cellRect = { //tight layout
                _tableRect.x + j * prev_width,
                _tableRect.y + i * cellHeight,
                width_adjust,
                cellHeight
            };
            gridSpec[i].emplace_back(
                renderer, 
                gridData[i][j], // initial text
                _cellColor,
                _cellBorderColor, 
                cellRect, 
                _textFontSize, 
                _textColor,
                _textFontName
            );
            if (j % 2 == 0) {gridSpec[i][j].leftJustify();} // name column
            prev_width = width_adjust;
        }
    }
}

void Table::UpdateCells(int offset){
    for (int i = 0; i < _rows; ++i) {
        for (int c = 0; c < _cols; ++c) {
            int r = i + offset;
            gridSpec[i][c].UpdateText(gridData[r][c]);
        }
    }
}


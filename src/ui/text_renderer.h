#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <string>
#include <map>

struct TextTexture {
    GLuint textureID;
    int width;
    int height;
};

class TextRenderer {
private:
    TTF_Font* font;
    std::map<std::string, TextTexture> textureCache;
    
public:
    TextRenderer();
    ~TextRenderer();
    
    bool init(const std::string& fontPath, int fontSize);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    TextTexture createTextTexture(const std::string& text, SDL_Color color);
    void drawTexture(const TextTexture& texture, int x, int y);
    void cleanup();
    
    int getTextWidth(const std::string& text);
    int getTextHeight();
};

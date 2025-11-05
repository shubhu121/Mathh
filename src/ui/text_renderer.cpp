#include "text_renderer.h"
#include <iostream>

TextRenderer::TextRenderer() : font(nullptr) {}

TextRenderer::~TextRenderer() {
    cleanup();
}

bool TextRenderer::init(const std::string& fontPath, int fontSize) {
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }
    
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
        return false;
    }
    
    return true;
}

TextTexture TextRenderer::createTextTexture(const std::string& text, SDL_Color color) {
    TextTexture result = {0, 0, 0};
    
    if (!font || text.empty()) {
        return result;
    }
    
    // Render UTF-8 text to surface
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "TTF_RenderUTF8_Blended failed: " << TTF_GetError() << std::endl;
        return result;
    }
    
    // Create OpenGL texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Determine format
    GLenum format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    result.textureID = texture;
    result.width = surface->w;
    result.height = surface->h;
    
    SDL_FreeSurface(surface);
    
    return result;
}

void TextRenderer::drawTexture(const TextTexture& texture, int x, int y) {
    if (texture.textureID == 0) return;
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, texture.textureID);
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + texture.width, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + texture.width, y + texture.height);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + texture.height);
    glEnd();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void TextRenderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    // Check cache
    std::string cacheKey = text + "_" + std::to_string(color.r) + "_" + 
                          std::to_string(color.g) + "_" + std::to_string(color.b);
    
    auto it = textureCache.find(cacheKey);
    if (it != textureCache.end()) {
        drawTexture(it->second, x, y);
        return;
    }
    
    // Create and cache new texture
    TextTexture texture = createTextTexture(text, color);
    if (texture.textureID != 0) {
        textureCache[cacheKey] = texture;
        drawTexture(texture, x, y);
    }
}

int TextRenderer::getTextWidth(const std::string& text) {
    if (!font || text.empty()) return 0;
    
    int w = 0;
    TTF_SizeUTF8(font, text.c_str(), &w, nullptr);
    return w;
}

int TextRenderer::getTextHeight() {
    if (!font) return 0;
    return TTF_FontHeight(font);
}

void TextRenderer::cleanup() {
    // Delete all cached textures
    for (auto& pair : textureCache) {
        glDeleteTextures(1, &pair.second.textureID);
    }
    textureCache.clear();
    
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    TTF_Quit();
}

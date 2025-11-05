#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>

class Renderer {
private:
    SDL_Window* window;
    SDL_GLContext glContext;
    int windowWidth;
    int windowHeight;
    
public:
    Renderer();
    ~Renderer();
    
    bool init(const char* title, int width, int height);
    void clear(float r, float g, float b);
    void present();
    void cleanup();
    
    int getWidth() const { return windowWidth; }
    int getHeight() const { return windowHeight; }
};

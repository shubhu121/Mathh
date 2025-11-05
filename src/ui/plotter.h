#pragma once
#include "../engine/ast.h"
#include <GL/glew.h>
#include <vector>

class Plotter {
private:
    int width;
    int height;
    float xMin, xMax;
    float yMin, yMax;
    
    void drawAxes();
    void drawGrid();
    float screenX(float x) const;
    float screenY(float y) const;
    
public:
    Plotter(int w, int h);
    
    void setViewport(float xmin, float xmax, float ymin, float ymax);
    void plotFunction(const ASTNode* func, float r, float g, float b, float lineWidth = 2.0f);
    void plotFunctionFilled(const ASTNode* func, float a, float b, float r, float g, float bl, float alpha = 0.3f);
    void render();
};

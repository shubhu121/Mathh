#include "plotter.h"
#include <cmath>
#include <algorithm>

Plotter::Plotter(int w, int h) 
    : width(w), height(h), xMin(-10), xMax(10), yMin(-10), yMax(10) {}

void Plotter::setViewport(float xmin, float xmax, float ymin, float ymax) {
    xMin = xmin;
    xMax = xmax;
    yMin = ymin;
    yMax = ymax;
}

float Plotter::screenX(float x) const {
    return (x - xMin) / (xMax - xMin) * width;
}

float Plotter::screenY(float y) const {
    return height - (y - yMin) / (yMax - yMin) * height;
}

void Plotter::drawGrid() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(1.0f);
    
    // Vertical grid lines
    for (int i = (int)xMin; i <= (int)xMax; i++) {
        float sx = screenX(i);
        glBegin(GL_LINES);
            glVertex2f(sx, 0);
            glVertex2f(sx, height);
        glEnd();
    }
    
    // Horizontal grid lines
    for (int i = (int)yMin; i <= (int)yMax; i++) {
        float sy = screenY(i);
        glBegin(GL_LINES);
            glVertex2f(0, sy);
            glVertex2f(width, sy);
        glEnd();
    }
}

void Plotter::drawAxes() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(2.0f);
    
    // X-axis
    float y0 = screenY(0);
    glBegin(GL_LINES);
        glVertex2f(0, y0);
        glVertex2f(width, y0);
    glEnd();
    
    // Y-axis
    float x0 = screenX(0);
    glBegin(GL_LINES);
        glVertex2f(x0, 0);
        glVertex2f(x0, height);
    glEnd();
}

void Plotter::plotFunction(const ASTNode* func, float r, float g, float b, float lineWidth) {
    if (!func) return;
    
    glColor3f(r, g, b);
    glLineWidth(lineWidth);
    
    glBegin(GL_LINE_STRIP);
    
    int numPoints = width * 2; // More points for smoother curves
    for (int i = 0; i < numPoints; i++) {
        float x = xMin + (xMax - xMin) * i / (float)numPoints;
        float y = func->evaluate(x);
        
        // Clamp y to avoid rendering issues
        if (std::isfinite(y) && y >= yMin && y <= yMax) {
            glVertex2f(screenX(x), screenY(y));
        }
    }
    
    glEnd();
}

void Plotter::plotFunctionFilled(const ASTNode* func, float a, float b, float r, float g, float bl, float alpha) {
    if (!func) return;
    
    // Clamp bounds to viewport
    a = std::max(a, xMin);
    b = std::min(b, xMax);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, bl, alpha);
    
    // Draw filled area under curve
    glBegin(GL_TRIANGLE_STRIP);
    
    int numPoints = 200; // Sufficient points for smooth fill
    for (int i = 0; i <= numPoints; i++) {
        float x = a + (b - a) * i / (float)numPoints;
        float y = func->evaluate(x);
        
        if (std::isfinite(y)) {
            // Clamp y
            float yTop = std::min(std::max(y, yMin), yMax);
            float yBottom = std::max(0.0f, yMin); // Bottom at y=0 or yMin
            
            // Draw from x-axis to function
            glVertex2f(screenX(x), screenY(yBottom));
            glVertex2f(screenX(x), screenY(yTop));
        }
    }
    
    glEnd();
    glDisable(GL_BLEND);
    
    // Draw boundary lines
    glColor3f(r, g, bl);
    glLineWidth(2.0f);
    
    // Left bound
    float yLeft = func->evaluate(a);
    if (std::isfinite(yLeft)) {
        glBegin(GL_LINES);
            glVertex2f(screenX(a), screenY(0));
            glVertex2f(screenX(a), screenY(std::min(std::max(yLeft, yMin), yMax)));
        glEnd();
    }
    
    // Right bound
    float yRight = func->evaluate(b);
    if (std::isfinite(yRight)) {
        glBegin(GL_LINES);
            glVertex2f(screenX(b), screenY(0));
            glVertex2f(screenX(b), screenY(std::min(std::max(yRight, yMin), yMax)));
        glEnd();
    }
}

void Plotter::render() {
    drawGrid();
    drawAxes();
}

#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include "engine/parser.h"
#include "engine/differentiator.h"
#include "engine/integrator.h"
#include "engine/simplifier.h"
#include "ui/renderer.h"
#include "ui/text_renderer.h"
#include "ui/plotter.h"

enum class Mode {
    MENU,
    DIFFERENTIATION,
    INDEFINITE_INTEGRATION,
    DEFINITE_INTEGRATION
};

// Default example expressions for differentiation
const char* defaultDiffExpressions[] = {
    "sin(x^2)",
    "x^3 + 2*x",
    "(x^3 + 2*x)*cos(x)",
    "ln(x)*x^2",
    "exp(x)*sin(x)"
};
const int numDefaultDiffExpressions = 5;

// Default example expressions for integration
const char* defaultIntegralExpressions[] = {
    "x^2",
    "x^3 + 2*x",
    "sin(x)",
    "cos(x)",
    "exp(x)"
};
const int numDefaultIntegralExpressions = 5;

int main(int argc, char* argv[]) {
    // Initialize renderer
    Renderer renderer;
    if (!renderer.init("Mathematics Engine - Calculus Toolkit", 1280, 720)) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return 1;
    }
    
    // Initialize text renderer
    TextRenderer textRenderer;
    if (!textRenderer.init("assets/fonts/NotoSansMath-Regular.ttf", 20)) {
        std::cerr << "Failed to initialize text renderer" << std::endl;
        return 1;
    }
    
    // Colors
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color green = {100, 255, 100, 255};
    SDL_Color yellow = {255, 255, 100, 255};
    SDL_Color cyan = {100, 200, 255, 255};
    SDL_Color orange = {255, 180, 100, 255};
    SDL_Color purple = {200, 150, 255, 255};
    SDL_Color gray = {150, 150, 150, 255};
    
    // Application state
    Mode currentMode = Mode::MENU;
    int menuSelection = 0;
    
    // Shared variables
    Parser parser;
    std::unique_ptr<ASTNode> ast;
    std::unique_ptr<ASTNode> result;
    std::vector<DifferentiationStep> diffSteps;
    std::vector<IntegrationStep> integSteps;
    bool parseSuccess = false;
    std::string errorMsg;
    std::string currentExpression = "";
    
    // Differentiation mode variables
    int currentDiffExpressionIndex = 0;
    
    // Integration mode variables
    int currentIntegralExpressionIndex = 0;
    bool boundsInputMode = false; // For definite integration
    std::string lowerBoundStr = "0";
    std::string upperBoundStr = "1";
    bool editingLowerBound = true;
    double definiteResult = 0.0;
    
    // Input mode variables
    bool inputMode = false;
    std::string userInput = "";
    
    // UI variables
    int scrollOffset = 0;
    Plotter plotter(600, 300);
    plotter.setViewport(-5, 5, -5, 5);
    
    // Lambda to process differentiation
    auto processDifferentiation = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            Differentiator diff;
            result = diff.differentiate(ast.get());
            diffSteps = diff.getSteps();
            
            result = Simplifier::simplify(std::move(result));
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process indefinite integration
    auto processIndefiniteIntegration = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            Integrator integ;
            result = integ.integrate(ast.get());
            integSteps = integ.getSteps();
            
            result = Simplifier::simplify(std::move(result));
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process definite integration
    auto processDefiniteIntegration = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            double a = std::stod(lowerBoundStr);
            double b = std::stod(upperBoundStr);
            
            Integrator integ;
            definiteResult = integ.evaluateDefinite(ast.get(), a, b);
            integSteps = integ.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Console output
    std::cout << "\n+===================================+\n";
    std::cout << "|  Mathematics Engine - Calculus   |\n";
    std::cout << "+===================================+\n\n";
    
    // Main loop
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_TEXTINPUT) {
                if (inputMode) {
                    userInput += event.text.text;
                }
                else if (boundsInputMode) {
                    if (editingLowerBound) {
                        lowerBoundStr += event.text.text;
                    } else {
                        upperBoundStr += event.text.text;
                    }
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                // Menu mode
                if (currentMode == Mode::MENU) {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_1:
                            currentMode = Mode::DIFFERENTIATION;
                            currentExpression = defaultDiffExpressions[0];
                            scrollOffset = 0;
                            processDifferentiation();
                            std::cout << "Switched to DIFFERENTIATION mode\n";
                            break;
                        case SDLK_2:
                            currentMode = Mode::INDEFINITE_INTEGRATION;
                            currentExpression = defaultIntegralExpressions[0];
                            scrollOffset = 0;
                            processIndefiniteIntegration();
                            std::cout << "Switched to INDEFINITE INTEGRATION mode\n";
                            break;
                        case SDLK_3:
                            currentMode = Mode::DEFINITE_INTEGRATION;
                            currentExpression = defaultIntegralExpressions[0];
                            scrollOffset = 0;
                            boundsInputMode = true;
                            editingLowerBound = true;
                            SDL_StartTextInput();
                            std::cout << "Switched to DEFINITE INTEGRATION mode\n";
                            break;
                        case SDLK_UP:
                            menuSelection = std::max(0, menuSelection - 1);
                            break;
                        case SDLK_DOWN:
                            menuSelection = std::min(2, menuSelection + 1);
                            break;
                        case SDLK_RETURN:
                            if (menuSelection == 0) {
                                currentMode = Mode::DIFFERENTIATION;
                                currentExpression = defaultDiffExpressions[0];
                                scrollOffset = 0;
                                processDifferentiation();
                            } else if (menuSelection == 1) {
                                currentMode = Mode::INDEFINITE_INTEGRATION;
                                currentExpression = defaultIntegralExpressions[0];
                                scrollOffset = 0;
                                processIndefiniteIntegration();
                            } else if (menuSelection == 2) {
                                currentMode = Mode::DEFINITE_INTEGRATION;
                                currentExpression = defaultIntegralExpressions[0];
                                scrollOffset = 0;
                                boundsInputMode = true;
                                editingLowerBound = true;
                                SDL_StartTextInput();
                            }
                            break;
                    }
                }
                // Handle input mode across all calculus modes
                else if (inputMode) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (!userInput.empty()) {
                            currentExpression = userInput;
                            scrollOffset = 0;
                            
                            if (currentMode == Mode::DIFFERENTIATION) {
                                processDifferentiation();
                            } else if (currentMode == Mode::INDEFINITE_INTEGRATION) {
                                processIndefiniteIntegration();
                            } else if (currentMode == Mode::DEFINITE_INTEGRATION) {
                                processDefiniteIntegration();
                            }
                        }
                        inputMode = false;
                        SDL_StopTextInput();
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !userInput.empty()) {
                        userInput = userInput.substr(0, userInput.length() - 1);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        inputMode = false;
                        userInput = "";
                        SDL_StopTextInput();
                    }
                }
                // Handle bounds input for definite integration
                else if (boundsInputMode && currentMode == Mode::DEFINITE_INTEGRATION) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        boundsInputMode = false;
                        SDL_StopTextInput();
                        processDefiniteIntegration();
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        editingLowerBound = !editingLowerBound;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (editingLowerBound && !lowerBoundStr.empty()) {
                            lowerBoundStr = lowerBoundStr.substr(0, lowerBoundStr.length() - 1);
                        } else if (!editingLowerBound && !upperBoundStr.empty()) {
                            upperBoundStr = upperBoundStr.substr(0, upperBoundStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        boundsInputMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Normal calculus mode controls
                else {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            // Return to menu
                            currentMode = Mode::MENU;
                            scrollOffset = 0;
                            std::cout << "Returned to MENU\n";
                            break;
                        case SDLK_RETURN:
                            inputMode = true;
                            userInput = "";
                            SDL_StartTextInput();
                            break;
                        case SDLK_b:
                            // B key for bounds (definite integration only)
                            if (currentMode == Mode::DEFINITE_INTEGRATION) {
                                boundsInputMode = true;
                                editingLowerBound = true;
                                SDL_StartTextInput();
                            }
                            break;
                        case SDLK_SPACE:
                            if (currentMode == Mode::DIFFERENTIATION) {
                                currentDiffExpressionIndex = (currentDiffExpressionIndex + 1) % numDefaultDiffExpressions;
                                currentExpression = defaultDiffExpressions[currentDiffExpressionIndex];
                                scrollOffset = 0;
                                processDifferentiation();
                            } else {
                                currentIntegralExpressionIndex = (currentIntegralExpressionIndex + 1) % numDefaultIntegralExpressions;
                                currentExpression = defaultIntegralExpressions[currentIntegralExpressionIndex];
                                scrollOffset = 0;
                                if (currentMode == Mode::INDEFINITE_INTEGRATION) {
                                    processIndefiniteIntegration();
                                } else {
                                    processDefiniteIntegration();
                                }
                            }
                            break;
                        case SDLK_UP:
                            scrollOffset = std::max(0, scrollOffset - 30);
                            break;
                        case SDLK_DOWN:
                            scrollOffset += 30;
                            break;
                    }
                }
            }
        }
        
        // Render
        renderer.clear(0.05f, 0.05f, 0.1f);
        
        int y = 20 - scrollOffset;
        int leftMargin = 20;
        int lineHeight = 35;
        
        // === MENU MODE ===
        if (currentMode == Mode::MENU) {
            textRenderer.renderText("+===============================================+", leftMargin, y, cyan);
            y += lineHeight;
            textRenderer.renderText("|     MATHEMATICS ENGINE - Calculus Toolkit    |", leftMargin, y, cyan);
            y += lineHeight;
            textRenderer.renderText("+===============================================+", leftMargin, y, cyan);
            y += lineHeight + 20;
            
            textRenderer.renderText("Select Operation:", leftMargin, y, yellow);
            y += lineHeight + 10;
            
            // Menu option 1
            std::string opt1 = (menuSelection == 0) ? "> 1. Differentiation (d/dx)" : "  1. Differentiation (d/dx)";
            SDL_Color color1 = (menuSelection == 0) ? green : white;
            textRenderer.renderText(opt1, leftMargin + 40, y, color1);
            y += lineHeight;
            textRenderer.renderText("     Find derivatives with step-by-step solutions", leftMargin + 60, y, gray);
            y += lineHeight + 10;
            
            // Menu option 2
            std::string opt2 = (menuSelection == 1) ? "> 2. Indefinite Integration (∫ f(x) dx)" : "  2. Indefinite Integration (∫ f(x) dx)";
            SDL_Color color2 = (menuSelection == 1) ? green : white;
            textRenderer.renderText(opt2, leftMargin + 40, y, color2);
            y += lineHeight;
            textRenderer.renderText("     Find antiderivatives + C", leftMargin + 60, y, gray);
            y += lineHeight + 10;
            
            // Menu option 3
            std::string opt3 = (menuSelection == 2) ? "> 3. Definite Integration (∫[a,b] f(x) dx)" : "  3. Definite Integration (∫[a,b] f(x) dx)";
            SDL_Color color3 = (menuSelection == 2) ? green : white;
            textRenderer.renderText(opt3, leftMargin + 40, y, color3);
            y += lineHeight;
            textRenderer.renderText("     Calculate area under curve with bounds", leftMargin + 60, y, gray);
            y += lineHeight + 30;
            
            textRenderer.renderText("Controls:", leftMargin, y, yellow);
            y += lineHeight;
            textRenderer.renderText("  • Press 1, 2, or 3 to select", leftMargin + 20, y, white);
            y += lineHeight;
            textRenderer.renderText("  • UP/DOWN arrows to navigate", leftMargin + 20, y, white);
            y += lineHeight;
            textRenderer.renderText("  • ENTER to confirm", leftMargin + 20, y, white);
            y += lineHeight;
            textRenderer.renderText("  • ESC to quit", leftMargin + 20, y, white);
        }
        // === DIFFERENTIATION MODE ===
        else if (currentMode == Mode::DIFFERENTIATION) {
            textRenderer.renderText("Differentiation Mode - d/dx", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type equation: " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string inputLine = "Input: f(x) = " + currentExpression;
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !diffSteps.empty()) {
                textRenderer.renderText("--- Differentiation Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < diffSteps.size(); i++) {
                    const auto& step = diffSteps[i];
                    std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                    textRenderer.renderText(stepHeader, leftMargin, y, white);
                    y += lineHeight;
                    textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                    y += lineHeight + 5;
                }
                
                y += 10;
                textRenderer.renderText("--- Simplified Result ---", leftMargin, y, yellow);
                y += lineHeight;
                std::string finalResult = "f'(x) = " + result->toString();
                textRenderer.renderText(finalResult, leftMargin, y, green);
                y += lineHeight + 20;
                
                // Plot
                int plotX = 650;
                int plotY = 100;
                
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, renderer.getWidth(), renderer.getHeight(), 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();
                
                glViewport(plotX, plotY, 600, 300);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0, 600, 300, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                
                plotter.render();
                plotter.plotFunction(ast.get(), 0.3f, 0.6f, 1.0f);
                plotter.plotFunction(result.get(), 1.0f, 0.3f, 0.3f);
                
                glViewport(0, 0, renderer.getWidth(), renderer.getHeight());
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                
                textRenderer.renderText("Graph:", plotX, plotY - 25, yellow);
                textRenderer.renderText("f(x) - Blue", plotX, plotY + 310, cyan);
                textRenderer.renderText("f'(x) - Red", plotX + 150, plotY + 310, {255, 100, 100, 255});
                
                textRenderer.renderText("ENTER: custom | SPACE: next example | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === INDEFINITE INTEGRATION MODE ===
        else if (currentMode == Mode::INDEFINITE_INTEGRATION) {
            textRenderer.renderText("Indefinite Integration Mode - ∫ f(x) dx", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type equation: " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string inputLine = "Input: f(x) = " + currentExpression;
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !integSteps.empty()) {
                textRenderer.renderText("--- Integration Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < integSteps.size(); i++) {
                    const auto& step = integSteps[i];
                    std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                    textRenderer.renderText(stepHeader, leftMargin, y, white);
                    y += lineHeight;
                    textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                    y += lineHeight + 5;
                }
                
                y += 10;
                textRenderer.renderText("--- Result (Antiderivative) ---", leftMargin, y, yellow);
                y += lineHeight;
                std::string finalResult = "∫ f(x) dx = " + result->toString() + " + C";
                textRenderer.renderText(finalResult, leftMargin, y, green);
                y += lineHeight + 20;
                
                // Plot section for indefinite integration
                int plotX = 650;
                int plotY = 100;
                
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, renderer.getWidth(), renderer.getHeight(), 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();
                
                glViewport(plotX, plotY, 600, 300);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0, 600, 300, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                
                plotter.render();
                
                // Plot antiderivative F(x) in green (first, so it's behind)
                if (result) {
                    plotter.plotFunction(result.get(), 0.3f, 1.0f, 0.3f);
                }
                
                // Plot original function f(x) in blue (on top)
                if (ast) {
                    plotter.plotFunction(ast.get(), 0.3f, 0.6f, 1.0f, 2.5f);
                }
                
                // Restore viewport
                glViewport(0, 0, renderer.getWidth(), renderer.getHeight());
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                
                textRenderer.renderText("Graph:", plotX, plotY - 25, yellow);
                textRenderer.renderText("f(x) - Blue", plotX, plotY + 310, cyan);
                textRenderer.renderText("F(x) = ∫f(x)dx - Green", plotX + 150, plotY + 310, {100, 255, 100, 255});
                
                textRenderer.renderText("ENTER: custom | SPACE: next example | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === DEFINITE INTEGRATION MODE ===
        else if (currentMode == Mode::DEFINITE_INTEGRATION) {
            textRenderer.renderText("Definite Integration Mode - ∫[a,b] f(x) dx", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (boundsInputMode) {
                textRenderer.renderText("Set Integration Bounds:", leftMargin, y, yellow);
                y += lineHeight;
                
                std::string lowerPrompt = (editingLowerBound ? "> " : "  ") + std::string("Lower bound (a): ") + lowerBoundStr + (editingLowerBound ? "_" : "");
                textRenderer.renderText(lowerPrompt, leftMargin + 20, y, editingLowerBound ? green : white);
                y += lineHeight;
                
                std::string upperPrompt = (!editingLowerBound ? "> " : "  ") + std::string("Upper bound (b): ") + upperBoundStr + (!editingLowerBound ? "_" : "");
                textRenderer.renderText(upperPrompt, leftMargin + 20, y, !editingLowerBound ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
                y += lineHeight + 15;
            } else if (inputMode) {
                std::string inputPrompt = "Type equation: " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string inputLine = "Input: f(x) = " + currentExpression;
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight;
                
                std::string boundsLine = "Bounds: [" + lowerBoundStr + ", " + upperBoundStr + "]";
                textRenderer.renderText(boundsLine, leftMargin, y, orange);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !integSteps.empty()) {
                textRenderer.renderText("--- Integration Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < integSteps.size(); i++) {
                    const auto& step = integSteps[i];
                    std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                    textRenderer.renderText(stepHeader, leftMargin, y, white);
                    y += lineHeight;
                    textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                    y += lineHeight + 5;
                }
                
                y += 10;
                textRenderer.renderText("--- Final Result ---", leftMargin, y, yellow);
                y += lineHeight;
                std::string finalResult = "∫[" + lowerBoundStr + "," + upperBoundStr + "] f(x) dx = " + std::to_string(definiteResult);
                textRenderer.renderText(finalResult, leftMargin, y, green);
                y += lineHeight + 20;
                
                // Plot section for definite integration
                int plotX = 650;
                int plotY = 100;
                
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, renderer.getWidth(), renderer.getHeight(), 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();
                
                glViewport(plotX, plotY, 600, 300);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0, 600, 300, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                
                plotter.render();
                
                // Draw shaded area under curve
                double a = std::stod(lowerBoundStr);
                double b = std::stod(upperBoundStr);
                plotter.plotFunctionFilled(ast.get(), a, b, 0.3f, 0.8f, 1.0f, 0.4f);
                
                // Plot function f(x) in blue (over the fill)
                plotter.plotFunction(ast.get(), 0.2f, 0.5f, 1.0f, 2.5f);
                
                // Restore viewport
                glViewport(0, 0, renderer.getWidth(), renderer.getHeight());
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                
                textRenderer.renderText("Graph:", plotX, plotY - 25, yellow);
                textRenderer.renderText("f(x) - Blue curve", plotX, plotY + 310, cyan);
                textRenderer.renderText("Shaded area = " + std::to_string(definiteResult), plotX + 180, plotY + 310, {150, 220, 255, 255});
                
                textRenderer.renderText("ENTER: custom | B: change bounds | SPACE: next | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        
        renderer.present();
        SDL_Delay(16);
    }
    
    textRenderer.cleanup();
    renderer.cleanup();
    
    return 0;
}

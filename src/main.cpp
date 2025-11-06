#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include "engine/parser.h"
#include "engine/differentiator.h"
#include "engine/integrator.h"
#include "engine/simplifier.h"
#include "engine/limit_calculator.h"
#include "engine/matrix_operations.h"
#include "engine/latex_exporter.h"
#include "ui/renderer.h"
#include "ui/text_renderer.h"
#include "ui/plotter.h"

enum class Mode {
    MENU,
    DIFFERENTIATION,
    INDEFINITE_INTEGRATION,
    DEFINITE_INTEGRATION,
    LIMITS,
    MATRIX_MULTIPLICATION
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

// Default example expressions for limits
const char* defaultLimitExpressions[] = {
    "(x^2 - 4)/(x - 2)",
    "sin(x)/x",
    "(1 - cos(x))/x",
    "x^2",
    "1/x"
};
const int numDefaultLimitExpressions = 5;
const double defaultLimitPoints[] = {2.0, 0.0, 0.0, 0.0, 0.0};
const LimitType defaultLimitTypes[] = {
    LimitType::FINITE,
    LimitType::FINITE,
    LimitType::FINITE,
    LimitType::POSITIVE_INFINITY,
    LimitType::POSITIVE_INFINITY
};

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
    
    // Limits mode variables
    int currentLimitExpressionIndex = 0;
    std::vector<LimitStep> limitSteps;
    double limitResult = 0.0;
    double limitPoint = 2.0;
    LimitType limitType = LimitType::FINITE;
    bool limitConfigMode = false; // Full limit configuration mode
    bool limitPointInputMode = false;
    std::string limitPointStr = "2.0";
    int limitTypeSelection = 0; // 0=finite, 1=+inf, 2=-inf
    int limitConfigField = 0; // 0=type, 1=point (if finite)
    
    // Matrix multiplication mode variables
    std::vector<MatrixStep> matrixSteps;
    Matrix* matrixA = nullptr;
    Matrix* matrixB = nullptr;
    Matrix* matrixResult = nullptr;
    bool matrixDimensionInputMode = false;
    bool matrixValueInputMode = false;
    std::string matrixRowsAStr = "2";
    std::string matrixColsAStr = "3";
    std::string matrixRowsBStr = "3";
    std::string matrixColsBStr = "2";
    int matrixInputField = 0; // 0=rowsA, 1=colsA, 2=rowsB, 3=colsB
    int currentMatrixRow = 0;
    int currentMatrixCol = 0;
    bool editingMatrixA = true; // true=A, false=B
    std::string matrixValueStr = "0";
    
    // Input mode variables
    bool inputMode = false;
    std::string userInput = "";
    
    // UI variables
    int scrollOffset = 0;
    Plotter plotter(600, 300);
    plotter.setViewport(-5, 5, -5, 5);
    
    // LaTeX export variables
    LaTeXExporter latexExporter;
    std::string exportMessage = "";
    int exportMessageTimer = 0;
    std::string lastExportedFile = "";
    
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
    
    // Lambda to process limits
    auto processLimit = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            LimitCalculator limCalc;
            limitResult = limCalc.calculateLimit(ast.get(), limitPoint, limitType);
            limitSteps = limCalc.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process matrix multiplication
    auto processMatrixMultiplication = [&]() {
        try {
            if (!matrixA || !matrixB) {
                throw std::runtime_error("Matrices not initialized");
            }
            
            MatrixOperations matOps;
            
            // Check if multiplication is possible
            if (!matOps.canMultiply(*matrixA, *matrixB)) {
                errorMsg = matOps.getErrorMessage(*matrixA, *matrixB);
                parseSuccess = false;
                return;
            }
            
            // Perform multiplication
            if (matrixResult) {
                delete matrixResult;
            }
            matrixResult = new Matrix(matOps.multiply(*matrixA, *matrixB));
            matrixSteps = matOps.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to handle LaTeX export
    auto exportToLaTeX = [&]() {
        std::string filename;
        bool success = false;
        
        switch (currentMode) {
            case Mode::DIFFERENTIATION:
                if (parseSuccess && result) {
                    filename = "differentiation_solution.tex";
                    success = latexExporter.exportDifferentiation(
                        currentExpression, diffSteps, result.get(), filename
                    );
                }
                break;
                
            case Mode::INDEFINITE_INTEGRATION:
                if (parseSuccess && result) {
                    filename = "integration_solution.tex";
                    success = latexExporter.exportIndefiniteIntegration(
                        currentExpression, integSteps, result.get(), filename
                    );
                }
                break;
                
            case Mode::DEFINITE_INTEGRATION:
                if (parseSuccess) {
                    filename = "definite_integration_solution.tex";
                    success = latexExporter.exportDefiniteIntegration(
                        currentExpression, integSteps, definiteResult,
                        std::stod(lowerBoundStr), std::stod(upperBoundStr), filename
                    );
                }
                break;
                
            case Mode::LIMITS:
                if (parseSuccess && !limitSteps.empty()) {
                    filename = "limit_solution.tex";
                    success = latexExporter.exportLimit(
                        currentExpression, limitSteps, limitResult,
                        limitPoint, limitType, filename
                    );
                }
                break;
                
            case Mode::MATRIX_MULTIPLICATION:
                if (matrixA && matrixB && matrixResult && !matrixSteps.empty()) {
                    filename = "matrix_multiplication_solution.tex";
                    success = latexExporter.exportMatrixMultiplication(
                        *matrixA, *matrixB, *matrixResult, matrixSteps, filename
                    );
                }
                break;
                
            default:
                break;
        }
        
        if (success) {
            lastExportedFile = filename;
            exportMessage = "Exported to " + filename;
            exportMessageTimer = 180; // Show for 3 seconds at 60fps
            
            // Optionally try to compile to PDF
            std::cout << "LaTeX file saved: " << filename << std::endl;
            std::cout << "Attempting PDF compilation..." << std::endl;
            if (latexExporter.compileToPDF(filename)) {
                exportMessage += " and compiled to PDF";
                std::cout << "PDF compilation successful!" << std::endl;
            } else {
                std::cout << "PDF compilation failed (pdflatex not found or error occurred)" << std::endl;
            }
        } else {
            exportMessage = "Export failed - no solution to export";
            exportMessageTimer = 180;
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
                else if (limitConfigMode && limitConfigField == 1) {
                    // Typing custom point value (only when field is on point input)
                    limitPointStr += event.text.text;
                }
                else if (limitPointInputMode) {
                    limitPointStr += event.text.text;
                }
                else if (matrixDimensionInputMode) {
                    if (matrixInputField == 0) {
                        matrixRowsAStr += event.text.text;
                    } else if (matrixInputField == 1) {
                        matrixColsAStr += event.text.text;
                    } else if (matrixInputField == 2) {
                        matrixRowsBStr += event.text.text;
                    } else if (matrixInputField == 3) {
                        matrixColsBStr += event.text.text;
                    }
                }
                else if (matrixValueInputMode) {
                    matrixValueStr += event.text.text;
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
                        case SDLK_4:
                            currentMode = Mode::LIMITS;
                            currentExpression = defaultLimitExpressions[0];
                            limitPoint = defaultLimitPoints[0];
                            limitType = defaultLimitTypes[0];
                            scrollOffset = 0;
                            processLimit();
                            std::cout << "Switched to LIMITS mode\n";
                            break;
                        case SDLK_5:
                            currentMode = Mode::MATRIX_MULTIPLICATION;
                            scrollOffset = 0;
                            matrixDimensionInputMode = true;
                            matrixInputField = 0;
                            SDL_StartTextInput();
                            std::cout << "Switched to MATRIX MULTIPLICATION mode\n";
                            break;
                        case SDLK_UP:
                            menuSelection = std::max(0, menuSelection - 1);
                            break;
                        case SDLK_DOWN:
                            menuSelection = std::min(4, menuSelection + 1);
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
                            } else if (menuSelection == 3) {
                                currentMode = Mode::LIMITS;
                                currentExpression = defaultLimitExpressions[0];
                                limitPoint = defaultLimitPoints[0];
                                limitType = defaultLimitTypes[0];
                                scrollOffset = 0;
                                processLimit();
                            } else if (menuSelection == 4) {
                                currentMode = Mode::MATRIX_MULTIPLICATION;
                                scrollOffset = 0;
                                matrixDimensionInputMode = true;
                                matrixInputField = 0;
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
                // Handle limit configuration mode (type + point selection)
                else if (limitConfigMode && currentMode == Mode::LIMITS) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        // Apply the configured limit
                        try {
                            if (limitConfigField == 1 && limitType == LimitType::FINITE) {
                                // Parse the custom point
                                limitPoint = std::stod(limitPointStr);
                            }
                            limitConfigMode = false;
                            SDL_StopTextInput();
                            processLimit();
                        } catch (...) {
                            errorMsg = "Invalid limit point";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        // Toggle between type selection and point input
                        if (limitType == LimitType::FINITE) {
                            limitConfigField = (limitConfigField == 0) ? 1 : 0;
                            if (limitConfigField == 1) {
                                SDL_StartTextInput();
                            } else {
                                SDL_StopTextInput();
                            }
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                        // Change limit type (only when on type field)
                        if (limitConfigField == 0) {
                            if (event.key.keysym.sym == SDLK_UP) {
                                limitTypeSelection = (limitTypeSelection == 0) ? 2 : limitTypeSelection - 1;
                            } else {
                                limitTypeSelection = (limitTypeSelection + 1) % 3;
                            }
                            
                            // Update limit type
                            if (limitTypeSelection == 0) {
                                limitType = LimitType::FINITE;
                            } else if (limitTypeSelection == 1) {
                                limitType = LimitType::POSITIVE_INFINITY;
                            } else {
                                limitType = LimitType::NEGATIVE_INFINITY;
                            }
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && limitConfigField == 1) {
                        if (!limitPointStr.empty()) {
                            limitPointStr = limitPointStr.substr(0, limitPointStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        limitConfigMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle limit point input (legacy simple mode)
                else if (limitPointInputMode && currentMode == Mode::LIMITS) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            if (limitType == LimitType::FINITE) {
                                limitPoint = std::stod(limitPointStr);
                            }
                            limitPointInputMode = false;
                            SDL_StopTextInput();
                            processLimit();
                        } catch (...) {
                            errorMsg = "Invalid limit point";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !limitPointStr.empty()) {
                        limitPointStr = limitPointStr.substr(0, limitPointStr.length() - 1);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        limitPointInputMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle matrix dimension input
                else if (matrixDimensionInputMode && currentMode == Mode::MATRIX_MULTIPLICATION) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            int rowsA = std::stoi(matrixRowsAStr);
                            int colsA = std::stoi(matrixColsAStr);
                            int rowsB = std::stoi(matrixRowsBStr);
                            int colsB = std::stoi(matrixColsBStr);
                            
                            if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
                                throw std::invalid_argument("Dimensions must be positive");
                            }
                            
                            // Initialize matrices
                            if (matrixA) delete matrixA;
                            if (matrixB) delete matrixB;
                            if (matrixResult) delete matrixResult;
                            
                            matrixA = new Matrix(rowsA, colsA);
                            matrixB = new Matrix(rowsB, colsB);
                            matrixResult = nullptr;
                            
                            // Move to value input mode
                            matrixDimensionInputMode = false;
                            matrixValueInputMode = true;
                            editingMatrixA = true;
                            currentMatrixRow = 0;
                            currentMatrixCol = 0;
                            matrixValueStr = "0";
                            // Keep text input active
                        } catch (const std::exception& e) {
                            errorMsg = std::string("Error: ") + e.what();
                            parseSuccess = false;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        matrixInputField = (matrixInputField + 1) % 4;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (matrixInputField == 0 && !matrixRowsAStr.empty()) {
                            matrixRowsAStr = matrixRowsAStr.substr(0, matrixRowsAStr.length() - 1);
                        } else if (matrixInputField == 1 && !matrixColsAStr.empty()) {
                            matrixColsAStr = matrixColsAStr.substr(0, matrixColsAStr.length() - 1);
                        } else if (matrixInputField == 2 && !matrixRowsBStr.empty()) {
                            matrixRowsBStr = matrixRowsBStr.substr(0, matrixRowsBStr.length() - 1);
                        } else if (matrixInputField == 3 && !matrixColsBStr.empty()) {
                            matrixColsBStr = matrixColsBStr.substr(0, matrixColsBStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        matrixDimensionInputMode = false;
                        SDL_StopTextInput();
                        currentMode = Mode::MENU;
                    }
                }
                // Handle matrix value input
                else if (matrixValueInputMode && currentMode == Mode::MATRIX_MULTIPLICATION) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            double value = std::stod(matrixValueStr);
                            
                            if (editingMatrixA) {
                                matrixA->set(currentMatrixRow, currentMatrixCol, value);
                                
                                // Move to next cell
                                currentMatrixCol++;
                                if (currentMatrixCol >= matrixA->cols) {
                                    currentMatrixCol = 0;
                                    currentMatrixRow++;
                                    if (currentMatrixRow >= matrixA->rows) {
                                        // Done with matrix A, move to matrix B
                                        editingMatrixA = false;
                                        currentMatrixRow = 0;
                                        currentMatrixCol = 0;
                                    }
                                }
                            } else {
                                matrixB->set(currentMatrixRow, currentMatrixCol, value);
                                
                                // Move to next cell
                                currentMatrixCol++;
                                if (currentMatrixCol >= matrixB->cols) {
                                    currentMatrixCol = 0;
                                    currentMatrixRow++;
                                    if (currentMatrixRow >= matrixB->rows) {
                                        // Done with both matrices
                                        matrixValueInputMode = false;
                                        SDL_StopTextInput();
                                        processMatrixMultiplication();
                                    }
                                }
                            }
                            
                            matrixValueStr = "0";
                        } catch (const std::exception& e) {
                            errorMsg = "Invalid number";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !matrixValueStr.empty()) {
                        matrixValueStr = matrixValueStr.substr(0, matrixValueStr.length() - 1);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        matrixValueInputMode = false;
                        SDL_StopTextInput();
                        currentMode = Mode::MENU;
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
                        case SDLK_l:
                        case SDLK_p:
                            // L/P key for limit configuration (limits only)
                            if (currentMode == Mode::LIMITS) {
                                limitConfigMode = true;
                                limitConfigField = 0; // Start with type selection
                                // Update limitTypeSelection to match current limitType
                                if (limitType == LimitType::FINITE) {
                                    limitTypeSelection = 0;
                                } else if (limitType == LimitType::POSITIVE_INFINITY) {
                                    limitTypeSelection = 1;
                                } else {
                                    limitTypeSelection = 2;
                                }
                                // Initialize point string
                                limitPointStr = std::to_string(limitPoint);
                                // Don't start text input yet - only when TAB to point field
                            }
                            break;
                        case SDLK_t:
                            // T key to quick toggle limit type (limits only)
                            if (currentMode == Mode::LIMITS) {
                                limitTypeSelection = (limitTypeSelection + 1) % 3;
                                if (limitTypeSelection == 0) {
                                    limitType = LimitType::FINITE;
                                    limitPointStr = std::to_string(limitPoint);
                                } else if (limitTypeSelection == 1) {
                                    limitType = LimitType::POSITIVE_INFINITY;
                                } else {
                                    limitType = LimitType::NEGATIVE_INFINITY;
                                }
                                processLimit();
                            }
                            break;
                        case SDLK_x:
                            // X key to export solution to LaTeX
                            if (currentMode != Mode::MENU) {
                                exportToLaTeX();
                            }
                            break;
                        case SDLK_SPACE:
                            if (currentMode == Mode::DIFFERENTIATION) {
                                currentDiffExpressionIndex = (currentDiffExpressionIndex + 1) % numDefaultDiffExpressions;
                                currentExpression = defaultDiffExpressions[currentDiffExpressionIndex];
                                scrollOffset = 0;
                                processDifferentiation();
                            } else if (currentMode == Mode::LIMITS) {
                                currentLimitExpressionIndex = (currentLimitExpressionIndex + 1) % numDefaultLimitExpressions;
                                currentExpression = defaultLimitExpressions[currentLimitExpressionIndex];
                                limitPoint = defaultLimitPoints[currentLimitExpressionIndex];
                                limitType = defaultLimitTypes[currentLimitExpressionIndex];
                                scrollOffset = 0;
                                processLimit();
                            } else if (currentMode == Mode::INDEFINITE_INTEGRATION || currentMode == Mode::DEFINITE_INTEGRATION) {
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
            y += lineHeight + 10;
            
            // Menu option 4
            std::string opt4 = (menuSelection == 3) ? "> 4. Limits (lim f(x))" : "  4. Limits (lim f(x))";
            SDL_Color color4 = (menuSelection == 3) ? green : white;
            textRenderer.renderText(opt4, leftMargin + 40, y, color4);
            y += lineHeight;
            textRenderer.renderText("     Calculate limits with step-by-step evaluation", leftMargin + 60, y, gray);
            y += lineHeight + 10;
            
            // Menu option 5
            std::string opt5 = (menuSelection == 4) ? "> 5. Matrix Multiplication" : "  5. Matrix Multiplication";
            SDL_Color color5 = (menuSelection == 4) ? green : white;
            textRenderer.renderText(opt5, leftMargin + 40, y, color5);
            y += lineHeight;
            textRenderer.renderText("     Multiply matrices with custom dimensions", leftMargin + 60, y, gray);
            y += lineHeight + 30;
            
            textRenderer.renderText("Controls:", leftMargin, y, yellow);
            y += lineHeight;
            textRenderer.renderText("  • Press 1-5 to select", leftMargin + 20, y, white);
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
                
                textRenderer.renderText("ENTER: custom | SPACE: next | X: export LaTeX/PDF | ESC: menu", 20, 690, gray);
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
                
                textRenderer.renderText("ENTER: custom | SPACE: next | X: export LaTeX/PDF | ESC: menu", 20, 690, gray);
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
                
                textRenderer.renderText("ENTER: custom | B: bounds | SPACE: next | X: export LaTeX/PDF | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === LIMITS MODE ===
        else if (currentMode == Mode::LIMITS) {
            textRenderer.renderText("Limits Mode - lim f(x)", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (limitConfigMode) {
                textRenderer.renderText("Configure Limit:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                // Limit type selection
                std::string typeLabel = (limitConfigField == 0) ? "> " : "  ";
                std::string typeText = "Limit Type: ";
                if (limitType == LimitType::FINITE) {
                    typeText += "x → " + std::to_string(limitPoint);
                } else if (limitType == LimitType::POSITIVE_INFINITY) {
                    typeText += "x → +∞";
                } else {
                    typeText += "x → -∞";
                }
                textRenderer.renderText(typeLabel + typeText, leftMargin + 20, y, limitConfigField == 0 ? green : white);
                y += lineHeight;
                
                // Show available types
                if (limitConfigField == 0) {
                    textRenderer.renderText("    [UP/DOWN to change: Finite / +∞ / -∞]", leftMargin + 40, y, gray);
                    y += lineHeight;
                }
                
                // Point input (only for finite limits)
                if (limitType == LimitType::FINITE) {
                    y += 5;
                    std::string pointLabel = (limitConfigField == 1) ? "> " : "  ";
                    std::string pointPrompt = "Limit Point: " + limitPointStr;
                    if (limitConfigField == 1) {
                        pointPrompt += "_";
                    }
                    textRenderer.renderText(pointLabel + pointPrompt, leftMargin + 20, y, limitConfigField == 1 ? green : white);
                    y += lineHeight;
                }
                
                y += 10;
                textRenderer.renderText("Controls:", leftMargin, y, yellow);
                y += lineHeight;
                textRenderer.renderText("  TAB - Switch fields", leftMargin + 20, y, gray);
                y += lineHeight;
                textRenderer.renderText("  UP/DOWN - Change type (when on Type field)", leftMargin + 20, y, gray);
                y += lineHeight;
                textRenderer.renderText("  ENTER - Apply and compute", leftMargin + 20, y, gray);
                y += lineHeight;
                textRenderer.renderText("  ESC - Cancel", leftMargin + 20, y, gray);
            } else if (limitPointInputMode) {
                textRenderer.renderText("Enter limit point:", leftMargin, y, yellow);
                y += lineHeight;
                std::string inputPrompt = "Point: " + limitPointStr + "_";
                textRenderer.renderText(inputPrompt, leftMargin + 20, y, green);
                y += lineHeight;
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else if (inputMode) {
                std::string inputPrompt = "Type equation: " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string inputLine = "Input: f(x) = " + currentExpression;
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight;
                
                LimitCalculator limCalc;
                std::string limitTypeStr = limCalc.limitTypeToString(limitPoint, limitType);
                std::string limitLine = "Limit: " + limitTypeStr;
                textRenderer.renderText(limitLine, leftMargin, y, orange);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !limitSteps.empty()) {
                textRenderer.renderText("--- Limit Calculation Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < limitSteps.size(); i++) {
                    const auto& step = limitSteps[i];
                    std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                    textRenderer.renderText(stepHeader, leftMargin, y, white);
                    y += lineHeight;
                    textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                    y += lineHeight + 5;
                }
                
                y += 10;
                textRenderer.renderText("--- Final Result ---", leftMargin, y, yellow);
                y += lineHeight;
                
                std::string resultStr;
                if (std::isnan(limitResult)) {
                    resultStr = "Limit does not exist or undefined";
                } else if (std::isinf(limitResult)) {
                    resultStr = (limitResult > 0) ? "+∞" : "-∞";
                } else {
                    resultStr = std::to_string(limitResult);
                }
                
                LimitCalculator limCalc;
                std::string finalResult = "lim [" + limCalc.limitTypeToString(limitPoint, limitType) + "] f(x) = " + resultStr;
                textRenderer.renderText(finalResult, leftMargin, y, green);
                y += lineHeight + 20;
                
                textRenderer.renderText("ENTER: custom | L/P: config | T: toggle | SPACE: next | X: export | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === MATRIX MULTIPLICATION MODE ===
        else if (currentMode == Mode::MATRIX_MULTIPLICATION) {
            textRenderer.renderText("Matrix Multiplication Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (matrixDimensionInputMode) {
                textRenderer.renderText("Enter Matrix Dimensions:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string rowsAPrompt = (matrixInputField == 0 ? "> " : "  ") + std::string("Matrix A rows: ") + matrixRowsAStr + (matrixInputField == 0 ? "_" : "");
                textRenderer.renderText(rowsAPrompt, leftMargin + 20, y, matrixInputField == 0 ? green : white);
                y += lineHeight;
                
                std::string colsAPrompt = (matrixInputField == 1 ? "> " : "  ") + std::string("Matrix A cols: ") + matrixColsAStr + (matrixInputField == 1 ? "_" : "");
                textRenderer.renderText(colsAPrompt, leftMargin + 20, y, matrixInputField == 1 ? green : white);
                y += lineHeight + 10;
                
                std::string rowsBPrompt = (matrixInputField == 2 ? "> " : "  ") + std::string("Matrix B rows: ") + matrixRowsBStr + (matrixInputField == 2 ? "_" : "");
                textRenderer.renderText(rowsBPrompt, leftMargin + 20, y, matrixInputField == 2 ? green : white);
                y += lineHeight;
                
                std::string colsBPrompt = (matrixInputField == 3 ? "> " : "  ") + std::string("Matrix B cols: ") + matrixColsBStr + (matrixInputField == 3 ? "_" : "");
                textRenderer.renderText(colsBPrompt, leftMargin + 20, y, matrixInputField == 3 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to continue, ESC to cancel)", leftMargin, y, gray);
            } else if (matrixValueInputMode) {
                std::string matrixName = editingMatrixA ? "Matrix A" : "Matrix B";
                textRenderer.renderText("Entering values for " + matrixName + ":", leftMargin, y, yellow);
                y += lineHeight;
                
                std::string prompt = "Element [" + std::to_string(currentMatrixRow) + "][" + std::to_string(currentMatrixCol) + "]: " + matrixValueStr + "_";
                textRenderer.renderText(prompt, leftMargin + 20, y, green);
                y += lineHeight;
                textRenderer.renderText("(Press ENTER to confirm, ESC to cancel)", leftMargin, y, gray);
                y += lineHeight + 15;
                
                // Show current matrix being filled
                Matrix* currentMatrix = editingMatrixA ? matrixA : matrixB;
                if (currentMatrix) {
                    textRenderer.renderText("Current " + matrixName + ":", leftMargin, y, orange);
                    y += lineHeight;
                    
                    for (int i = 0; i < currentMatrix->rows; i++) {
                        std::string row = "[ ";
                        for (int j = 0; j < currentMatrix->cols; j++) {
                            if (i < currentMatrixRow || (i == currentMatrixRow && j < currentMatrixCol)) {
                                row += std::to_string((int)currentMatrix->get(i, j));
                            } else if (i == currentMatrixRow && j == currentMatrixCol) {
                                row += "?";
                            } else {
                                row += "0";
                            }
                            if (j < currentMatrix->cols - 1) row += " ";
                        }
                        row += " ]";
                        textRenderer.renderText(row, leftMargin + 20, y, white);
                        y += lineHeight;
                    }
                }
            } else if (parseSuccess && !matrixSteps.empty()) {
                textRenderer.renderText("--- Matrix Multiplication Steps ---", leftMargin, y, yellow);
                y += lineHeight + 5;
                
                for (size_t i = 0; i < matrixSteps.size(); i++) {
                    const auto& step = matrixSteps[i];
                    textRenderer.renderText(step.description + ":", leftMargin, y, white);
                    y += lineHeight;
                    
                    // Split multi-line expressions
                    std::string expr = step.expression;
                    size_t pos = 0;
                    while ((pos = expr.find('\n')) != std::string::npos) {
                        std::string line = expr.substr(0, pos);
                        textRenderer.renderText("  " + line, leftMargin + 20, y, white);
                        y += lineHeight;
                        expr = expr.substr(pos + 1);
                    }
                    if (!expr.empty()) {
                        textRenderer.renderText("  " + expr, leftMargin + 20, y, white);
                        y += lineHeight;
                    }
                    y += 5;
                }
                
                textRenderer.renderText("ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
                y += lineHeight + 10;
                textRenderer.renderText("Press ESC to return to menu", leftMargin, y, gray);
            }
        }
        
        // Display export message if active
        if (exportMessageTimer > 0) {
            SDL_Color exportColor = {100, 255, 100, 255}; // Green
            textRenderer.renderText(exportMessage, 20, 650, exportColor);
            exportMessageTimer--;
        }
        
        renderer.present();
        SDL_Delay(16);
    }
    
    // Cleanup matrices
    if (matrixA) delete matrixA;
    if (matrixB) delete matrixB;
    if (matrixResult) delete matrixResult;
    
    textRenderer.cleanup();
    renderer.cleanup();
    
    return 0;
}

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
#include "engine/partial_derivative.h"
#include "engine/multivariate_integrator.h"
#include "engine/implicit_differentiation.h"
#include "engine/linear_transformation.h"
#include "engine/taylor_series.h"
#include "engine/parametric_curve.h"
#include "engine/laplace_transform.h"
#include "engine/fourier_series.h"
#include "engine/differential_equations.h"
#include "engine/vector_calculus.h"
#include "engine/complex_numbers.h"
#include "engine/sequences_series.h"
#include "engine/numerical_methods.h"
#include "engine/eigenvalues.h"
#include "engine/statistics.h"
#include "engine/polynomial_operations.h"
#include <iomanip>
#include <sstream>
#include "ui/renderer.h"
#include "ui/text_renderer.h"
#include "ui/plotter.h"

enum class Mode {
    MENU,
    DIFFERENTIATION,
    INDEFINITE_INTEGRATION,
    DEFINITE_INTEGRATION,
    LIMITS,
    MATRIX_MULTIPLICATION,
    PARTIAL_DERIVATIVES,
    DOUBLE_INTEGRATION,
    IMPLICIT_DIFFERENTIATION,
    LINEAR_TRANSFORMATION,
    TAYLOR_SERIES,
    PARAMETRIC_CURVE,
    LAPLACE_TRANSFORM,
    INVERSE_LAPLACE,
    FOURIER_SERIES,
    DIFFERENTIAL_EQUATIONS,
    VECTOR_CALCULUS,
    COMPLEX_NUMBERS,
    SEQUENCES_SERIES,
    NUMERICAL_METHODS,
    EIGENVALUES,
    STATISTICS,
    POLYNOMIAL_OPERATIONS
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

// Default example expressions for partial derivatives (multivariate functions)
const char* defaultPartialExpressions[] = {
    "x^2 + y^2",
    "x*y",
    "x^2*y + x*y^2",
    "sin(x)*cos(y)",
    "x^3 + 2*x*y + y^2"
};
const int numDefaultPartialExpressions = 5;

// Default example expressions for double integration
const char* defaultDoubleIntegralExpressions[] = {
    "x*y",
    "x^2 + y^2",
    "x + y",
    "2*x*y",
    "x*y^2"
};
const int numDefaultDoubleIntegralExpressions = 5;

// Default example expressions for implicit differentiation (F(x,y) = 0)
const char* defaultImplicitExpressions[] = {
    "x^2 + y^2",           // Circle: x² + y² = constant
    "x*y",                 // Hyperbola: xy = constant
    "x^2 - y^2",           // Hyperbola: x² - y² = constant
    "x^3 + y^3",           // Folium-like curve
    "sin(x) + cos(y)"      // Transcendental implicit equation
};
const int numDefaultImplicitExpressions = 5;

// Default example expressions for Taylor series
const char* defaultTaylorExpressions[] = {
    "sin(x)",    // Taylor series of sin(x)
    "cos(x)",    // Taylor series of cos(x)
    "exp(x)",    // Taylor series of e^x
    "ln(x)",     // Taylor series of ln(x) around x=1
    "x^3"        // Simple polynomial
};
const int numDefaultTaylorExpressions = 5;

// Default example parametric curves (x(t), y(t))
const char* defaultParametricX[] = {
    "t",         // Line
    "cos(t)",    // Circle
    "t",         // Parabola
    "2*cos(t)",  // Ellipse
    "t*cos(t)"   // Spiral
};
const char* defaultParametricY[] = {
    "t",         // Line
    "sin(t)",    // Circle
    "t^2",       // Parabola
    "sin(t)",    // Ellipse
    "t*sin(t)"   // Spiral
};
const int numDefaultParametricCurves = 5;

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
    SDL_Color gray = {150, 150, 150, 255};
    
    // Application state
    Mode currentMode = Mode::MENU;
    int menuSelection = 0;
    int menuScrollOffset = 0;  // For scrolling the menu
    
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
    
    // Partial derivatives mode variables
    int currentPartialExpressionIndex = 0;
    std::vector<PartialDerivativeStep> partialStepsX;
    std::vector<PartialDerivativeStep> partialStepsY;
    std::unique_ptr<ASTNode> partialResultX;
    std::unique_ptr<ASTNode> partialResultY;
    
    // Double integration mode variables
    int currentDoubleIntegralExpressionIndex = 0;
    std::vector<MultivariateIntegrationStep> doubleIntegSteps;
    std::string xLowerBoundStr = "0";
    std::string xUpperBoundStr = "1";
    std::string yLowerBoundStr = "0";
    std::string yUpperBoundStr = "1";
    bool doubleIntegBoundsInputMode = false;
    int doubleIntegBoundField = 0; // 0=x_lower, 1=x_upper, 2=y_lower, 3=y_upper
    double doubleIntegralResult = 0.0;
    
    // Implicit differentiation mode variables
    int currentImplicitExpressionIndex = 0;
    std::vector<ImplicitDifferentiationStep> implicitSteps;
    std::string implicitResult;
    
    // Linear transformation mode variables
    Matrix2D transformMatrix(1, 0, 0, 1);  // Identity matrix default
    Vector2D inputVector(1, 0);            // Default vector
    std::vector<TransformationStep> transformSteps;
    bool matrixInputModeTransform = false;
    bool vectorInputModeTransform = false;
    std::string transformMatrixInput[4] = {"1", "0", "0", "1"};  // a, b, c, d
    std::string transformVectorInput[2] = {"1", "0"};           // x, y
    int transformInputField = 0;  // 0-3 for matrix, 4-5 for vector
    
    // Taylor series mode variables
    int currentTaylorExpressionIndex = 0;
    std::vector<TaylorSeriesStep> taylorSteps;
    std::string taylorResult;
    double taylorCenter = 0.0;  // Expansion center (a)
    int taylorOrder = 4;        // Order of approximation
    std::string taylorCenterStr = "0";
    std::string taylorOrderStr = "4";
    bool taylorConfigMode = false;
    int taylorConfigField = 0;  // 0=center, 1=order
    
    // Parametric curve mode variables
    int currentParametricCurveIndex = 0;
    std::vector<ParametricCurveStep> parametricSteps;
    std::string currentXExpression = "";
    std::string currentYExpression = "";
    std::unique_ptr<ASTNode> astX;
    std::unique_ptr<ASTNode> astY;
    double tStart = 0.0;
    double tEnd = 6.28;  // 2π
    double tEval = 1.57; // π/2
    std::string tStartStr = "0";
    std::string tEndStr = "6.28";
    std::string tEvalStr = "1.57";
    bool parametricConfigMode = false;
    int parametricConfigField = 0;  // 0=tStart, 1=tEnd, 2=tEval
    bool parametricXInputMode = false;
    bool parametricYInputMode = false;
    
    // Complex Numbers mode variables
    std::vector<ComplexStep> complexSteps;
    double complexA1 = 3.0, complexB1 = 4.0;  // 3+4i
    bool complexInputMode = false;
    std::string complexA1Str = "3.0", complexB1Str = "4.0";
    int complexInputField = 0;  // 0=real, 1=imaginary
    
    // Sequences & Series mode variables
    std::vector<SequenceStep> sequenceSteps;
    double seqA = 2.0, seqD = 3.0;  // Arithmetic: a=2, d=3
    int seqN = 10;  // Number of terms
    bool seqInputMode = false;
    std::string seqAStr = "2.0", seqDStr = "3.0", seqNStr = "10";
    int seqInputField = 0;  // 0=a, 1=d, 2=n
    
    // Numerical Methods mode variables
    std::vector<NumericalStep> numericalSteps;
    double numX0 = 1.0;  // Initial guess for Newton-Raphson
    int numIterations = 10;
    double numTolerance = 0.0001;
    bool numConfigMode = false;
    std::string numX0Str = "1.0";
    
    // Eigenvalues mode variables
    std::vector<EigenStep> eigenSteps;
    double eigenA = 2.0, eigenB = 1.0;
    double eigenC = 1.0, eigenD = 2.0;  // Matrix [[2,1],[1,2]]
    bool eigenInputMode = false;
    std::string eigenAStr = "2.0", eigenBStr = "1.0", eigenCStr = "1.0", eigenDStr = "2.0";
    int eigenInputField = 0;  // 0=a, 1=b, 2=c, 3=d
    
    // Statistics mode variables
    std::vector<StatisticsStep> statsSteps;
    std::vector<double> statsData = {2.0, 4.0, 6.0, 8.0, 10.0};
    bool statsInputMode = false;
    std::string statsDataStr = "2, 4, 6, 8, 10";
    
    // Polynomial Operations mode variables
    std::vector<PolynomialStep> polySteps;
    double polyA = 1.0, polyB = -5.0, polyC = 6.0;  // x^2 - 5x + 6 = 0 (roots: 2, 3)
    bool polyInputMode = false;
    std::string polyAStr = "1", polyBStr = "-5", polyCStr = "6";
    int polyInputField = 0;  // 0=a, 1=b, 2=c
    int polyExampleIndex = 0;
    
    // Laplace Transform mode variables
    std::vector<LaplaceStep> laplaceSteps;
    
    // Fourier Series mode variables
    std::vector<FourierStep> fourierSteps;
    double fourierPeriod = 6.28;  // 2π
    int fourierTerms = 5;
    
    // Differential Equations mode variables
    std::vector<DifferentialEquationStep> diffEqSteps;
    
    // Vector Calculus mode variables
    std::vector<VectorCalculusStep> vectorSteps;
    
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
    
    // Lambda to process partial derivatives
    auto processPartialDerivatives = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            // Compute partial derivative with respect to x
            PartialDerivative partialX;
            partialResultX = partialX.differentiate(ast.get(), DiffVariable::X);
            partialStepsX = partialX.getSteps();
            partialResultX = Simplifier::simplify(std::move(partialResultX));
            
            // Compute partial derivative with respect to y
            PartialDerivative partialY;
            partialResultY = partialY.differentiate(ast.get(), DiffVariable::Y);
            partialStepsY = partialY.getSteps();
            partialResultY = Simplifier::simplify(std::move(partialResultY));
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process double integration
    auto processDoubleIntegration = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            double x_lower = std::stod(xLowerBoundStr);
            double x_upper = std::stod(xUpperBoundStr);
            double y_lower = std::stod(yLowerBoundStr);
            double y_upper = std::stod(yUpperBoundStr);
            
            MultivariateIntegrator multiInteg;
            doubleIntegralResult = multiInteg.doubleIntegrate(ast.get(), x_lower, x_upper, y_lower, y_upper);
            doubleIntegSteps = multiInteg.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process implicit differentiation
    auto processImplicitDifferentiation = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            ImplicitDifferentiator implicitDiff;
            implicitResult = implicitDiff.computeImplicitDerivative(ast.get());
            implicitSteps = implicitDiff.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process linear transformation
    auto processLinearTransformation = [&]() {
        try {
            // Parse matrix inputs
            transformMatrix.a = std::stod(transformMatrixInput[0]);
            transformMatrix.b = std::stod(transformMatrixInput[1]);
            transformMatrix.c = std::stod(transformMatrixInput[2]);
            transformMatrix.d = std::stod(transformMatrixInput[3]);
            
            // Parse vector inputs
            inputVector.x = std::stod(transformVectorInput[0]);
            inputVector.y = std::stod(transformVectorInput[1]);
            
            LinearTransformation linearTransform;
            linearTransform.analyzeTransformation(transformMatrix, inputVector);
            transformSteps = linearTransform.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Taylor series
    auto processTaylorSeries = [&]() {
        try {
            ast = parser.parse(currentExpression);
            
            taylorCenter = std::stod(taylorCenterStr);
            taylorOrder = std::stoi(taylorOrderStr);
            
            TaylorSeriesCalculator taylorCalc;
            taylorResult = taylorCalc.computeTaylorSeries(ast.get(), taylorCenter, taylorOrder);
            taylorSteps = taylorCalc.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process parametric curve
    auto processParametricCurve = [&]() {
        try {
            astX = parser.parse(currentXExpression);
            astY = parser.parse(currentYExpression);
            
            tStart = std::stod(tStartStr);
            tEnd = std::stod(tEndStr);
            tEval = std::stod(tEvalStr);
            
            ParametricCurveAnalyzer paramAnalyzer;
            paramAnalyzer.analyzeParametricCurve(astX.get(), astY.get(), tStart, tEnd, tEval);
            parametricSteps = paramAnalyzer.getSteps();
            
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Complex Numbers
    auto processComplexNumbers = [&]() {
        try {
            ComplexNumberCalculator complexCalc;
            complexCalc.analyzeComplexNumber(complexA1, complexB1);
            complexSteps = complexCalc.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Sequences & Series
    auto processSequences = [&]() {
        try {
            SequencesSeriesCalculator seqCalc;
            seqCalc.analyzeArithmetic(seqA, seqD, seqN);
            sequenceSteps = seqCalc.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Numerical Methods
    auto processNumericalMethods = [&]() {
        try {
            NumericalMethods numMethods;
            numMethods.newtonRaphson(ast.get(), numX0, numIterations, numTolerance);
            numericalSteps = numMethods.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Eigenvalues
    auto processEigenvalues = [&]() {
        try {
            EigenvalueCalculator eigenCalc;
            eigenCalc.analyze2x2Matrix(eigenA, eigenB, eigenC, eigenD);
            eigenSteps = eigenCalc.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Statistics
    auto processStatistics = [&]() {
        try {
            StatisticsCalculator statsCalc;
            statsCalc.analyzeDataSet(statsData);
            statsSteps = statsCalc.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Polynomial Operations
    auto processPolynomialOperations = [&]() {
        try {
            PolynomialOperations polyOps;
            polyOps.solveQuadratic(polyA, polyB, polyC);
            polySteps = polyOps.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Laplace Transform
    auto processLaplaceTransform = [&]() {
        try {
            LaplaceTransform laplaceCalc;
            std::string result = laplaceCalc.computeLaplaceTransform(currentExpression);
            laplaceSteps = laplaceCalc.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Inverse Laplace Transform
    auto processInverseLaplace = [&]() {
        try {
            LaplaceTransform laplaceCalc;
            std::string result = laplaceCalc.computeInverseLaplace(currentExpression);
            laplaceSteps = laplaceCalc.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Fourier Series
    auto processFourierSeries = [&]() {
        try {
            FourierSeriesCalculator fourierCalc;
            std::string result = fourierCalc.computeFourierSeries(ast.get(), fourierPeriod, fourierTerms);
            fourierSteps = fourierCalc.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Differential Equations
    auto processDifferentialEquations = [&]() {
        try {
            DifferentialEquationSolver deqSolver;
            std::string result = deqSolver.solveDifferentialEquation(currentExpression);
            diffEqSteps = deqSolver.getSteps();
            parseSuccess = true;
            errorMsg.clear();
        } catch (const std::exception& e) {
            parseSuccess = false;
            errorMsg = std::string("Error: ") + e.what();
        }
    };
    
    // Lambda to process Vector Calculus  
    auto processVectorCalculus = [&]() {
        try {
            VectorCalculusEngine vecCalc;
            // For gradient of scalar field f(x,y) - compute partial derivatives manually
            Differentiator diff;
            PartialDerivative partialDiff;
            
            // Compute ∂f/∂x
            auto df_dx = diff.differentiate(ast.get());
            
            // Compute ∂f/∂y (using partial derivative with y variable)
            auto df_dy = partialDiff.differentiate(ast.get(), DiffVariable::Y);
            
            VectorCalculusStep step1;
            step1.description = "=== Gradient of Scalar Field ===";
            step1.expression = "f(x,y) = " + currentExpression;
            vectorSteps.push_back(step1);
            
            VectorCalculusStep step2;
            step2.description = "Gradient formula:";
            step2.expression = "∇f = <∂f/∂x, ∂f/∂y>";
            vectorSteps.push_back(step2);
            
            VectorCalculusStep step3;
            step3.description = "Partial derivatives:";
            step3.expression = "∂f/∂x = " + df_dx->toString() + "\n∂f/∂y = " + df_dy->toString();
            vectorSteps.push_back(step3);
            
            // Evaluate at point (1,1)
            double grad_x = df_dx->evaluate(1.0);
            double grad_y = df_dy->evaluate(1.0);
            
            VectorCalculusStep step4;
            step4.description = "Gradient at point (1, 1):";
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(3);
            oss << "∇f(1,1) = <" << grad_x << ", " << grad_y << ">";
            step4.expression = oss.str();
            vectorSteps.push_back(step4);
            
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
            else if (event.type == SDL_MOUSEWHEEL) {
                if (currentMode == Mode::MENU) {
                    // Scroll menu with mouse wheel
                    menuScrollOffset -= event.wheel.y * 30;
                    if (menuScrollOffset < 0) menuScrollOffset = 0;
                } else {
                    // Scroll steps with mouse wheel in other modes
                    scrollOffset -= event.wheel.y * 30;
                    if (scrollOffset < 0) scrollOffset = 0;
                }
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
                else if (doubleIntegBoundsInputMode) {
                    if (doubleIntegBoundField == 0) {
                        xLowerBoundStr += event.text.text;
                    } else if (doubleIntegBoundField == 1) {
                        xUpperBoundStr += event.text.text;
                    } else if (doubleIntegBoundField == 2) {
                        yLowerBoundStr += event.text.text;
                    } else if (doubleIntegBoundField == 3) {
                        yUpperBoundStr += event.text.text;
                    }
                }
                else if (matrixInputModeTransform) {
                    if (transformInputField >= 0 && transformInputField <= 3) {
                        transformMatrixInput[transformInputField] += event.text.text;
                    }
                }
                else if (vectorInputModeTransform) {
                    if (transformInputField >= 0 && transformInputField <= 1) {
                        transformVectorInput[transformInputField] += event.text.text;
                    }
                }
                else if (taylorConfigMode) {
                    if (taylorConfigField == 0) {
                        taylorCenterStr += event.text.text;
                    } else if (taylorConfigField == 1) {
                        taylorOrderStr += event.text.text;
                    }
                }
                else if (parametricXInputMode || parametricYInputMode) {
                    userInput += event.text.text;
                }
                else if (parametricConfigMode) {
                    if (parametricConfigField == 0) {
                        tStartStr += event.text.text;
                    } else if (parametricConfigField == 1) {
                        tEndStr += event.text.text;
                    } else if (parametricConfigField == 2) {
                        tEvalStr += event.text.text;
                    }
                }
                else if (complexInputMode) {
                    if (complexInputField == 0) {
                        complexA1Str += event.text.text;
                    } else if (complexInputField == 1) {
                        complexB1Str += event.text.text;
                    }
                }
                else if (seqInputMode) {
                    if (seqInputField == 0) {
                        seqAStr += event.text.text;
                    } else if (seqInputField == 1) {
                        seqDStr += event.text.text;
                    } else if (seqInputField == 2) {
                        seqNStr += event.text.text;
                    }
                }
                else if (numConfigMode) {
                    numX0Str += event.text.text;
                }
                else if (eigenInputMode) {
                    if (eigenInputField == 0) {
                        eigenAStr += event.text.text;
                    } else if (eigenInputField == 1) {
                        eigenBStr += event.text.text;
                    } else if (eigenInputField == 2) {
                        eigenCStr += event.text.text;
                    } else if (eigenInputField == 3) {
                        eigenDStr += event.text.text;
                    }
                }
                else if (statsInputMode) {
                    statsDataStr += event.text.text;
                }
                else if (polyInputMode) {
                    if (polyInputField == 0) {
                        polyAStr += event.text.text;
                    } else if (polyInputField == 1) {
                        polyBStr += event.text.text;
                    } else if (polyInputField == 2) {
                        polyCStr += event.text.text;
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
                        case SDLK_6:
                            currentMode = Mode::PARTIAL_DERIVATIVES;
                            currentExpression = defaultPartialExpressions[0];
                            scrollOffset = 0;
                            processPartialDerivatives();
                            std::cout << "Switched to PARTIAL DERIVATIVES mode\n";
                            break;
                        case SDLK_7:
                            currentMode = Mode::DOUBLE_INTEGRATION;
                            currentExpression = defaultDoubleIntegralExpressions[0];
                            scrollOffset = 0;
                            doubleIntegBoundsInputMode = true;
                            doubleIntegBoundField = 0;
                            SDL_StartTextInput();
                            std::cout << "Switched to DOUBLE INTEGRATION mode\n";
                            break;
                        case SDLK_8:
                            currentMode = Mode::IMPLICIT_DIFFERENTIATION;
                            currentExpression = defaultImplicitExpressions[0];
                            scrollOffset = 0;
                            processImplicitDifferentiation();
                            std::cout << "Switched to IMPLICIT DIFFERENTIATION mode\n";
                            break;
                        case SDLK_9:
                            currentMode = Mode::LINEAR_TRANSFORMATION;
                            scrollOffset = 0;
                            matrixInputModeTransform = true;
                            transformInputField = 0;
                            SDL_StartTextInput();
                            std::cout << "Switched to LINEAR TRANSFORMATION mode\n";
                            break;
                        case SDLK_UP:
                            menuSelection = std::max(0, menuSelection - 1);
                            break;
                        case SDLK_DOWN:
                            menuSelection = std::min(21, menuSelection + 1);
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
                            } else if (menuSelection == 5) {
                                currentMode = Mode::PARTIAL_DERIVATIVES;
                                currentExpression = defaultPartialExpressions[0];
                                scrollOffset = 0;
                                processPartialDerivatives();
                            } else if (menuSelection == 6) {
                                currentMode = Mode::DOUBLE_INTEGRATION;
                                currentExpression = defaultDoubleIntegralExpressions[0];
                                scrollOffset = 0;
                                doubleIntegBoundsInputMode = true;
                                doubleIntegBoundField = 0;
                                SDL_StartTextInput();
                            } else if (menuSelection == 7) {
                                currentMode = Mode::IMPLICIT_DIFFERENTIATION;
                                currentExpression = defaultImplicitExpressions[0];
                                scrollOffset = 0;
                                processImplicitDifferentiation();
                            } else if (menuSelection == 8) {
                                currentMode = Mode::LINEAR_TRANSFORMATION;
                                scrollOffset = 0;
                                matrixInputModeTransform = true;
                                transformInputField = 0;
                                SDL_StartTextInput();
                            } else if (menuSelection == 9) {
                                currentMode = Mode::TAYLOR_SERIES;
                                currentExpression = defaultTaylorExpressions[0];
                                scrollOffset = 0;
                                processTaylorSeries();
                            } else if (menuSelection == 10) {
                                currentMode = Mode::PARAMETRIC_CURVE;
                                currentXExpression = defaultParametricX[0];
                                currentYExpression = defaultParametricY[0];
                                scrollOffset = 0;
                                processParametricCurve();
                            } else if (menuSelection == 11) {
                                currentMode = Mode::LAPLACE_TRANSFORM;
                                currentExpression = "t^2";  // Default: L{t²}
                                scrollOffset = 0;
                                processLaplaceTransform();
                            } else if (menuSelection == 12) {
                                currentMode = Mode::INVERSE_LAPLACE;
                                currentExpression = "1/(s^2+1)";  // Default: L⁻¹{1/(s²+1)}
                                scrollOffset = 0;
                                processInverseLaplace();
                            } else if (menuSelection == 13) {
                                currentMode = Mode::FOURIER_SERIES;
                                currentExpression = "x";  // Default: f(x) = x
                                scrollOffset = 0;
                                ast = parser.parse(currentExpression);
                                processFourierSeries();
                            } else if (menuSelection == 14) {
                                currentMode = Mode::DIFFERENTIAL_EQUATIONS;
                                currentExpression = "dy/dx=x*y";  // Default: separable ODE
                                scrollOffset = 0;
                                processDifferentialEquations();
                            } else if (menuSelection == 15) {
                                currentMode = Mode::VECTOR_CALCULUS;
                                currentExpression = "x^2+y^2";  // Default: f(x,y) = x²+y²
                                scrollOffset = 0;
                                ast = parser.parse(currentExpression);
                                processVectorCalculus();
                            } else if (menuSelection == 16) {
                                currentMode = Mode::COMPLEX_NUMBERS;
                                scrollOffset = 0;
                                processComplexNumbers();
                            } else if (menuSelection == 17) {
                                currentMode = Mode::SEQUENCES_SERIES;
                                scrollOffset = 0;
                                processSequences();
                            } else if (menuSelection == 18) {
                                currentMode = Mode::NUMERICAL_METHODS;
                                currentExpression = "x^2-2";  // Default: find sqrt(2)
                                scrollOffset = 0;
                                ast = parser.parse(currentExpression);
                                processNumericalMethods();
                            } else if (menuSelection == 19) {
                                currentMode = Mode::EIGENVALUES;
                                scrollOffset = 0;
                                processEigenvalues();
                            } else if (menuSelection == 20) {
                                currentMode = Mode::STATISTICS;
                                scrollOffset = 0;
                                processStatistics();
                            } else if (menuSelection == 21) {
                                currentMode = Mode::POLYNOMIAL_OPERATIONS;
                                scrollOffset = 0;
                                processPolynomialOperations();
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
                            } else if (currentMode == Mode::PARTIAL_DERIVATIVES) {
                                processPartialDerivatives();
                            } else if (currentMode == Mode::DOUBLE_INTEGRATION) {
                                processDoubleIntegration();
                            } else if (currentMode == Mode::IMPLICIT_DIFFERENTIATION) {
                                processImplicitDifferentiation();
                            } else if (currentMode == Mode::TAYLOR_SERIES) {
                                processTaylorSeries();
                            } else if (currentMode == Mode::LAPLACE_TRANSFORM) {
                                processLaplaceTransform();
                            } else if (currentMode == Mode::INVERSE_LAPLACE) {
                                processInverseLaplace();
                            } else if (currentMode == Mode::FOURIER_SERIES) {
                                ast = parser.parse(currentExpression);
                                processFourierSeries();
                            } else if (currentMode == Mode::DIFFERENTIAL_EQUATIONS) {
                                processDifferentialEquations();
                            } else if (currentMode == Mode::VECTOR_CALCULUS) {
                                ast = parser.parse(currentExpression);
                                processVectorCalculus();
                            } else if (currentMode == Mode::NUMERICAL_METHODS) {
                                ast = parser.parse(currentExpression);
                                processNumericalMethods();
                            }
                            // Note: Parametric curve uses parametricXInputMode/parametricYInputMode instead
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
                // Handle double integration bounds input
                else if (doubleIntegBoundsInputMode && currentMode == Mode::DOUBLE_INTEGRATION) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        doubleIntegBoundsInputMode = false;
                        SDL_StopTextInput();
                        processDoubleIntegration();
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        doubleIntegBoundField = (doubleIntegBoundField + 1) % 4;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (doubleIntegBoundField == 0 && !xLowerBoundStr.empty()) {
                            xLowerBoundStr = xLowerBoundStr.substr(0, xLowerBoundStr.length() - 1);
                        } else if (doubleIntegBoundField == 1 && !xUpperBoundStr.empty()) {
                            xUpperBoundStr = xUpperBoundStr.substr(0, xUpperBoundStr.length() - 1);
                        } else if (doubleIntegBoundField == 2 && !yLowerBoundStr.empty()) {
                            yLowerBoundStr = yLowerBoundStr.substr(0, yLowerBoundStr.length() - 1);
                        } else if (doubleIntegBoundField == 3 && !yUpperBoundStr.empty()) {
                            yUpperBoundStr = yUpperBoundStr.substr(0, yUpperBoundStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        doubleIntegBoundsInputMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle linear transformation matrix input
                else if (matrixInputModeTransform && currentMode == Mode::LINEAR_TRANSFORMATION) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        // Move to vector input after matrix is entered
                        matrixInputModeTransform = false;
                        vectorInputModeTransform = true;
                        transformInputField = 0;
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        transformInputField = (transformInputField + 1) % 4;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (transformInputField >= 0 && transformInputField <= 3 && !transformMatrixInput[transformInputField].empty()) {
                            transformMatrixInput[transformInputField] = transformMatrixInput[transformInputField].substr(0, transformMatrixInput[transformInputField].length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        matrixInputModeTransform = false;
                        SDL_StopTextInput();
                        currentMode = Mode::MENU;
                    }
                }
                // Handle linear transformation vector input
                else if (vectorInputModeTransform && currentMode == Mode::LINEAR_TRANSFORMATION) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        vectorInputModeTransform = false;
                        SDL_StopTextInput();
                        processLinearTransformation();
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        transformInputField = (transformInputField + 1) % 2;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (transformInputField >= 0 && transformInputField <= 1 && !transformVectorInput[transformInputField].empty()) {
                            transformVectorInput[transformInputField] = transformVectorInput[transformInputField].substr(0, transformVectorInput[transformInputField].length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        vectorInputModeTransform = false;
                        SDL_StopTextInput();
                        currentMode = Mode::MENU;
                    }
                }
                // Handle Taylor series configuration
                else if (taylorConfigMode && currentMode == Mode::TAYLOR_SERIES) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        taylorConfigMode = false;
                        SDL_StopTextInput();
                        processTaylorSeries();
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        taylorConfigField = (taylorConfigField + 1) % 2;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (taylorConfigField == 0 && !taylorCenterStr.empty()) {
                            taylorCenterStr = taylorCenterStr.substr(0, taylorCenterStr.length() - 1);
                        } else if (taylorConfigField == 1 && !taylorOrderStr.empty()) {
                            taylorOrderStr = taylorOrderStr.substr(0, taylorOrderStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        taylorConfigMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle parametric curve X input
                else if (parametricXInputMode && currentMode == Mode::PARAMETRIC_CURVE) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (!userInput.empty()) {
                            currentXExpression = userInput;
                            userInput = "";
                            parametricXInputMode = false;
                            parametricYInputMode = true;
                            // Stay in text input mode for y(t)
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !userInput.empty()) {
                        userInput = userInput.substr(0, userInput.length() - 1);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        parametricXInputMode = false;
                        userInput = "";
                        SDL_StopTextInput();
                    }
                }
                // Handle parametric curve Y input
                else if (parametricYInputMode && currentMode == Mode::PARAMETRIC_CURVE) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (!userInput.empty()) {
                            currentYExpression = userInput;
                            userInput = "";
                            parametricYInputMode = false;
                            SDL_StopTextInput();
                            processParametricCurve();
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !userInput.empty()) {
                        userInput = userInput.substr(0, userInput.length() - 1);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        parametricYInputMode = false;
                        userInput = "";
                        SDL_StopTextInput();
                    }
                }
                // Handle parametric curve configuration
                else if (parametricConfigMode && currentMode == Mode::PARAMETRIC_CURVE) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        parametricConfigMode = false;
                        SDL_StopTextInput();
                        processParametricCurve();
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        parametricConfigField = (parametricConfigField + 1) % 3;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (parametricConfigField == 0 && !tStartStr.empty()) {
                            tStartStr = tStartStr.substr(0, tStartStr.length() - 1);
                        } else if (parametricConfigField == 1 && !tEndStr.empty()) {
                            tEndStr = tEndStr.substr(0, tEndStr.length() - 1);
                        } else if (parametricConfigField == 2 && !tEvalStr.empty()) {
                            tEvalStr = tEvalStr.substr(0, tEvalStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        parametricConfigMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle complex numbers input
                else if (complexInputMode && currentMode == Mode::COMPLEX_NUMBERS) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            complexA1 = std::stod(complexA1Str);
                            complexB1 = std::stod(complexB1Str);
                            complexInputMode = false;
                            SDL_StopTextInput();
                            processComplexNumbers();
                        } catch (...) {
                            errorMsg = "Invalid complex number input";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        complexInputField = (complexInputField + 1) % 2;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (complexInputField == 0 && !complexA1Str.empty()) {
                            complexA1Str = complexA1Str.substr(0, complexA1Str.length() - 1);
                        } else if (complexInputField == 1 && !complexB1Str.empty()) {
                            complexB1Str = complexB1Str.substr(0, complexB1Str.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        complexInputMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle sequences input
                else if (seqInputMode && currentMode == Mode::SEQUENCES_SERIES) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            seqA = std::stod(seqAStr);
                            seqD = std::stod(seqDStr);
                            seqN = std::stoi(seqNStr);
                            seqInputMode = false;
                            SDL_StopTextInput();
                            processSequences();
                        } catch (...) {
                            errorMsg = "Invalid sequence input";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        seqInputField = (seqInputField + 1) % 3;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (seqInputField == 0 && !seqAStr.empty()) {
                            seqAStr = seqAStr.substr(0, seqAStr.length() - 1);
                        } else if (seqInputField == 1 && !seqDStr.empty()) {
                            seqDStr = seqDStr.substr(0, seqDStr.length() - 1);
                        } else if (seqInputField == 2 && !seqNStr.empty()) {
                            seqNStr = seqNStr.substr(0, seqNStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        seqInputMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle numerical methods config
                else if (numConfigMode && currentMode == Mode::NUMERICAL_METHODS) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            numX0 = std::stod(numX0Str);
                            numConfigMode = false;
                            SDL_StopTextInput();
                            ast = parser.parse(currentExpression);
                            processNumericalMethods();
                        } catch (...) {
                            errorMsg = "Invalid initial guess";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !numX0Str.empty()) {
                        numX0Str = numX0Str.substr(0, numX0Str.length() - 1);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        numConfigMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle eigenvalues input
                else if (eigenInputMode && currentMode == Mode::EIGENVALUES) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            eigenA = std::stod(eigenAStr);
                            eigenB = std::stod(eigenBStr);
                            eigenC = std::stod(eigenCStr);
                            eigenD = std::stod(eigenDStr);
                            eigenInputMode = false;
                            SDL_StopTextInput();
                            processEigenvalues();
                        } catch (...) {
                            errorMsg = "Invalid matrix input";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        eigenInputField = (eigenInputField + 1) % 4;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (eigenInputField == 0 && !eigenAStr.empty()) {
                            eigenAStr = eigenAStr.substr(0, eigenAStr.length() - 1);
                        } else if (eigenInputField == 1 && !eigenBStr.empty()) {
                            eigenBStr = eigenBStr.substr(0, eigenBStr.length() - 1);
                        } else if (eigenInputField == 2 && !eigenCStr.empty()) {
                            eigenCStr = eigenCStr.substr(0, eigenCStr.length() - 1);
                        } else if (eigenInputField == 3 && !eigenDStr.empty()) {
                            eigenDStr = eigenDStr.substr(0, eigenDStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        eigenInputMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle statistics data input
                else if (statsInputMode && currentMode == Mode::STATISTICS) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            statsData.clear();
                            std::stringstream ss(statsDataStr);
                            std::string item;
                            while (std::getline(ss, item, ',')) {
                                statsData.push_back(std::stod(item));
                            }
                            statsInputMode = false;
                            SDL_StopTextInput();
                            processStatistics();
                        } catch (...) {
                            errorMsg = "Invalid data format (use: 1, 2, 3, 4)";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !statsDataStr.empty()) {
                        statsDataStr = statsDataStr.substr(0, statsDataStr.length() - 1);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        statsInputMode = false;
                        SDL_StopTextInput();
                    }
                }
                // Handle polynomial operations input
                else if (polyInputMode && currentMode == Mode::POLYNOMIAL_OPERATIONS) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        try {
                            polyA = std::stod(polyAStr);
                            polyB = std::stod(polyBStr);
                            polyC = std::stod(polyCStr);
                            polyInputMode = false;
                            SDL_StopTextInput();
                            processPolynomialOperations();
                        } catch (...) {
                            errorMsg = "Invalid polynomial coefficients";
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_TAB) {
                        polyInputField = (polyInputField + 1) % 3;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (polyInputField == 0 && !polyAStr.empty()) {
                            polyAStr = polyAStr.substr(0, polyAStr.length() - 1);
                        } else if (polyInputField == 1 && !polyBStr.empty()) {
                            polyBStr = polyBStr.substr(0, polyBStr.length() - 1);
                        } else if (polyInputField == 2 && !polyCStr.empty()) {
                            polyCStr = polyCStr.substr(0, polyCStr.length() - 1);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        polyInputMode = false;
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
                            if (currentMode == Mode::PARAMETRIC_CURVE) {
                                // For parametric curves, start with X input
                                parametricXInputMode = true;
                                userInput = "";
                                SDL_StartTextInput();
                            } else {
                                inputMode = true;
                                userInput = "";
                                SDL_StartTextInput();
                            }
                            break;
                        case SDLK_b:
                            // B key for bounds
                            if (currentMode == Mode::DEFINITE_INTEGRATION) {
                                boundsInputMode = true;
                                editingLowerBound = true;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::DOUBLE_INTEGRATION) {
                                doubleIntegBoundsInputMode = true;
                                doubleIntegBoundField = 0;
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
                        case SDLK_m:
                            // M key to re-enter matrix (linear transformation only)
                            if (currentMode == Mode::LINEAR_TRANSFORMATION) {
                                matrixInputModeTransform = true;
                                vectorInputModeTransform = false;
                                transformInputField = 0;
                                SDL_StartTextInput();
                            }
                            break;
                        case SDLK_v:
                            // V key to re-enter vector (linear transformation only)
                            if (currentMode == Mode::LINEAR_TRANSFORMATION) {
                                vectorInputModeTransform = true;
                                matrixInputModeTransform = false;
                                transformInputField = 0;
                                SDL_StartTextInput();
                            }
                            break;
                        case SDLK_c:
                            // C key to configure/custom input
                            if (currentMode == Mode::TAYLOR_SERIES) {
                                taylorConfigMode = true;
                                taylorConfigField = 0;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::PARAMETRIC_CURVE) {
                                parametricConfigMode = true;
                                parametricConfigField = 0;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::COMPLEX_NUMBERS) {
                                complexInputMode = true;
                                complexInputField = 0;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::SEQUENCES_SERIES) {
                                seqInputMode = true;
                                seqInputField = 0;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::NUMERICAL_METHODS) {
                                numConfigMode = true;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::EIGENVALUES) {
                                eigenInputMode = true;
                                eigenInputField = 0;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::STATISTICS) {
                                statsInputMode = true;
                                SDL_StartTextInput();
                            } else if (currentMode == Mode::POLYNOMIAL_OPERATIONS) {
                                polyInputMode = true;
                                polyInputField = 0;
                                SDL_StartTextInput();
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
                            } else if (currentMode == Mode::PARTIAL_DERIVATIVES) {
                                currentPartialExpressionIndex = (currentPartialExpressionIndex + 1) % numDefaultPartialExpressions;
                                currentExpression = defaultPartialExpressions[currentPartialExpressionIndex];
                                scrollOffset = 0;
                                processPartialDerivatives();
                            } else if (currentMode == Mode::DOUBLE_INTEGRATION) {
                                currentDoubleIntegralExpressionIndex = (currentDoubleIntegralExpressionIndex + 1) % numDefaultDoubleIntegralExpressions;
                                currentExpression = defaultDoubleIntegralExpressions[currentDoubleIntegralExpressionIndex];
                                scrollOffset = 0;
                                processDoubleIntegration();
                            } else if (currentMode == Mode::IMPLICIT_DIFFERENTIATION) {
                                currentImplicitExpressionIndex = (currentImplicitExpressionIndex + 1) % numDefaultImplicitExpressions;
                                currentExpression = defaultImplicitExpressions[currentImplicitExpressionIndex];
                                scrollOffset = 0;
                                processImplicitDifferentiation();
                            } else if (currentMode == Mode::TAYLOR_SERIES) {
                                currentTaylorExpressionIndex = (currentTaylorExpressionIndex + 1) % numDefaultTaylorExpressions;
                                currentExpression = defaultTaylorExpressions[currentTaylorExpressionIndex];
                                scrollOffset = 0;
                                processTaylorSeries();
                            } else if (currentMode == Mode::PARAMETRIC_CURVE) {
                                currentParametricCurveIndex = (currentParametricCurveIndex + 1) % numDefaultParametricCurves;
                                currentXExpression = defaultParametricX[currentParametricCurveIndex];
                                currentYExpression = defaultParametricY[currentParametricCurveIndex];
                                scrollOffset = 0;
                                processParametricCurve();
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
        
        int leftMargin = 20;
        int lineHeight = 28;
        int y;
        
        // === MENU MODE ===
        if (currentMode == Mode::MENU) {
            y = 20 - menuScrollOffset;  // Use menu scroll offset
            textRenderer.renderText("+===============================================+", leftMargin, y, cyan);
            y += lineHeight;
            textRenderer.renderText("|     MATHEMATICS ENGINE - Calculus Toolkit    |", leftMargin, y, cyan);
            y += lineHeight;
            textRenderer.renderText("+===============================================+", leftMargin, y, cyan);
            y += lineHeight + 20;
            
            textRenderer.renderText("Select Operation:", leftMargin, y, yellow);
            y += lineHeight + 5;
            
            // Menu option 1
            std::string opt1 = (menuSelection == 0) ? "> 1. Differentiation (d/dx)" : "  1. Differentiation (d/dx)";
            SDL_Color color1 = (menuSelection == 0) ? green : white;
            textRenderer.renderText(opt1, leftMargin + 40, y, color1);
            y += lineHeight;
            textRenderer.renderText("     Find derivatives with step-by-step solutions", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 2
            std::string opt2 = (menuSelection == 1) ? "> 2. Indefinite Integration (∫ f(x) dx)" : "  2. Indefinite Integration (∫ f(x) dx)";
            SDL_Color color2 = (menuSelection == 1) ? green : white;
            textRenderer.renderText(opt2, leftMargin + 40, y, color2);
            y += lineHeight;
            textRenderer.renderText("     Find antiderivatives + C", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 3
            std::string opt3 = (menuSelection == 2) ? "> 3. Definite Integration (∫[a,b] f(x) dx)" : "  3. Definite Integration (∫[a,b] f(x) dx)";
            SDL_Color color3 = (menuSelection == 2) ? green : white;
            textRenderer.renderText(opt3, leftMargin + 40, y, color3);
            y += lineHeight;
            textRenderer.renderText("     Calculate area under curve with bounds", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 4
            std::string opt4 = (menuSelection == 3) ? "> 4. Limits (lim f(x))" : "  4. Limits (lim f(x))";
            SDL_Color color4 = (menuSelection == 3) ? green : white;
            textRenderer.renderText(opt4, leftMargin + 40, y, color4);
            y += lineHeight;
            textRenderer.renderText("     Calculate limits with step-by-step evaluation", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 5
            std::string opt5 = (menuSelection == 4) ? "> 5. Matrix Multiplication" : "  5. Matrix Multiplication";
            SDL_Color color5 = (menuSelection == 4) ? green : white;
            textRenderer.renderText(opt5, leftMargin + 40, y, color5);
            y += lineHeight;
            textRenderer.renderText("     Multiply matrices with custom dimensions", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 6
            std::string opt6 = (menuSelection == 5) ? "> 6. Partial Derivatives (∂f/∂x, ∂f/∂y)" : "  6. Partial Derivatives (∂f/∂x, ∂f/∂y)";
            SDL_Color color6 = (menuSelection == 5) ? green : white;
            textRenderer.renderText(opt6, leftMargin + 40, y, color6);
            y += lineHeight;
            textRenderer.renderText("     Compute partial derivatives for functions of x and y", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 7
            std::string opt7 = (menuSelection == 6) ? "> 7. Double Integration (∫∫ f(x,y) dy dx)" : "  7. Double Integration (∫∫ f(x,y) dy dx)";
            SDL_Color color7 = (menuSelection == 6) ? green : white;
            textRenderer.renderText(opt7, leftMargin + 40, y, color7);
            y += lineHeight;
            textRenderer.renderText("     Calculate double integrals over rectangular regions", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 8
            std::string opt8 = (menuSelection == 7) ? "> 8. Implicit Differentiation (dy/dx)" : "  8. Implicit Differentiation (dy/dx)";
            SDL_Color color8 = (menuSelection == 7) ? green : white;
            textRenderer.renderText(opt8, leftMargin + 40, y, color8);
            y += lineHeight;
            textRenderer.renderText("     Find dy/dx for implicit equations F(x,y) = 0", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 9
            std::string opt9 = (menuSelection == 8) ? "> 9. Linear Transformation" : "  9. Linear Transformation";
            SDL_Color color9 = (menuSelection == 8) ? green : white;
            textRenderer.renderText(opt9, leftMargin + 40, y, color9);
            y += lineHeight;
            textRenderer.renderText("     Analyze 2D linear transformations and matrix properties", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 10
            std::string opt10 = (menuSelection == 9) ? "> 10. Taylor Series Expansion" : "  10. Taylor Series Expansion";
            SDL_Color color10 = (menuSelection == 9) ? green : white;
            textRenderer.renderText(opt10, leftMargin + 40, y, color10);
            y += lineHeight;
            textRenderer.renderText("     Compute Taylor/Maclaurin series with step-by-step expansion", leftMargin + 60, y, gray);
            y += lineHeight + 3;
            
            // Menu option 11
            std::string opt11 = (menuSelection == 10) ? "> 11. Parametric Curve Analysis" : "  11. Parametric Curve Analysis";
            SDL_Color color11 = (menuSelection == 10) ? green : white;
            textRenderer.renderText(opt11, leftMargin + 40, y, color11);
            y += lineHeight + 2;
            
            // Menu option 12
            std::string opt12 = (menuSelection == 11) ? "> 12. Laplace Transform" : "  12. Laplace Transform";
            SDL_Color color12 = (menuSelection == 11) ? green : white;
            textRenderer.renderText(opt12, leftMargin + 40, y, color12);
            y += lineHeight + 2;
            
            // Menu option 13
            std::string opt13 = (menuSelection == 12) ? "> 13. Inverse Laplace Transform" : "  13. Inverse Laplace Transform";
            SDL_Color color13 = (menuSelection == 12) ? green : white;
            textRenderer.renderText(opt13, leftMargin + 40, y, color13);
            y += lineHeight + 2;
            
            // Menu option 14
            std::string opt14 = (menuSelection == 13) ? "> 14. Fourier Series" : "  14. Fourier Series";
            SDL_Color color14 = (menuSelection == 13) ? green : white;
            textRenderer.renderText(opt14, leftMargin + 40, y, color14);
            y += lineHeight + 2;
            
            // Menu option 15
            std::string opt15 = (menuSelection == 14) ? "> 15. Differential Equations (1st Order)" : "  15. Differential Equations (1st Order)";
            SDL_Color color15 = (menuSelection == 14) ? green : white;
            textRenderer.renderText(opt15, leftMargin + 40, y, color15);
            y += lineHeight + 2;
            
            // Menu option 16
            std::string opt16 = (menuSelection == 15) ? "> 16. Vector Calculus (∇, div, curl)" : "  16. Vector Calculus (∇, div, curl)";
            SDL_Color color16 = (menuSelection == 15) ? green : white;
            textRenderer.renderText(opt16, leftMargin + 40, y, color16);
            y += lineHeight + 2;
            
            // Menu option 17
            std::string opt17 = (menuSelection == 16) ? "> 17. Complex Numbers" : "  17. Complex Numbers";
            SDL_Color color17 = (menuSelection == 16) ? green : white;
            textRenderer.renderText(opt17, leftMargin + 40, y, color17);
            y += lineHeight + 2;
            
            // Menu option 18
            std::string opt18 = (menuSelection == 17) ? "> 18. Sequences & Series" : "  18. Sequences & Series";
            SDL_Color color18 = (menuSelection == 17) ? green : white;
            textRenderer.renderText(opt18, leftMargin + 40, y, color18);
            y += lineHeight + 2;
            
            // Menu option 19
            std::string opt19 = (menuSelection == 18) ? "> 19. Numerical Methods" : "  19. Numerical Methods";
            SDL_Color color19 = (menuSelection == 18) ? green : white;
            textRenderer.renderText(opt19, leftMargin + 40, y, color19);
            y += lineHeight + 2;
            
            // Menu option 20
            std::string opt20 = (menuSelection == 19) ? "> 20. Eigenvalues & Eigenvectors" : "  20. Eigenvalues & Eigenvectors";
            SDL_Color color20 = (menuSelection == 19) ? green : white;
            textRenderer.renderText(opt20, leftMargin + 40, y, color20);
            y += lineHeight + 2;
            
            // Menu option 21
            std::string opt21 = (menuSelection == 20) ? "> 21. Statistics & Probability" : "  21. Statistics & Probability";
            SDL_Color color21 = (menuSelection == 20) ? green : white;
            textRenderer.renderText(opt21, leftMargin + 40, y, color21);
            y += lineHeight + 2;
            
            // Menu option 22
            std::string opt22 = (menuSelection == 21) ? "> 22. Polynomial Operations" : "  22. Polynomial Operations";
            SDL_Color color22 = (menuSelection == 21) ? green : white;
            textRenderer.renderText(opt22, leftMargin + 40, y, color22);
            y += lineHeight;
            textRenderer.renderText("     Solve quadratic equations with step-by-step solutions", leftMargin + 60, y, gray);
            y += lineHeight + 10;
            
            textRenderer.renderText("Controls:", leftMargin, y, yellow);
            y += lineHeight;
            textRenderer.renderText("  • UP/DOWN or Mouse Wheel: Navigate", leftMargin + 20, y, white);
            y += lineHeight;
            textRenderer.renderText("  • ENTER: Select  •  ESC: Quit", leftMargin + 20, y, white);
        }
        // === DIFFERENTIATION MODE ===
        else if (currentMode == Mode::DIFFERENTIATION) {
            y = 20 - scrollOffset;  // Use regular scroll offset for non-menu modes
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
            y = 20 - scrollOffset;
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
            y = 20 - scrollOffset;
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
            y = 20 - scrollOffset;
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
            y = 20 - scrollOffset;
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
        // === PARTIAL DERIVATIVES MODE ===
        else if (currentMode == Mode::PARTIAL_DERIVATIVES) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Partial Derivatives Mode - ∂f/∂x and ∂f/∂y", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type equation: " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string inputLine = "Input: f(x,y) = " + currentExpression;
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && partialResultX && partialResultY) {
                // Show partial derivative with respect to x
                textRenderer.renderText("--- Partial Derivative with respect to x ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < partialStepsX.size() && i < 5; i++) {
                    const auto& step = partialStepsX[i];
                    std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                    textRenderer.renderText(stepHeader, leftMargin, y, white);
                    y += lineHeight;
                    textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                    y += lineHeight + 5;
                }
                
                std::string resultX = "∂f/∂x = " + partialResultX->toString();
                textRenderer.renderText(resultX, leftMargin, y, green);
                y += lineHeight + 15;
                
                // Show partial derivative with respect to y
                textRenderer.renderText("--- Partial Derivative with respect to y ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < partialStepsY.size() && i < 5; i++) {
                    const auto& step = partialStepsY[i];
                    std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                    textRenderer.renderText(stepHeader, leftMargin, y, white);
                    y += lineHeight;
                    textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                    y += lineHeight + 5;
                }
                
                std::string resultY = "∂f/∂y = " + partialResultY->toString();
                textRenderer.renderText(resultY, leftMargin, y, green);
                y += lineHeight + 20;
                
                textRenderer.renderText("ENTER: custom | SPACE: next | X: export | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === DOUBLE INTEGRATION MODE ===
        else if (currentMode == Mode::DOUBLE_INTEGRATION) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Double Integration Mode - ∫∫ f(x,y) dy dx", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type equation: " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else if (doubleIntegBoundsInputMode) {
                textRenderer.renderText("Enter Integration Bounds:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string xLowerPrompt = (doubleIntegBoundField == 0 ? "> " : "  ") + std::string("x lower bound: ") + xLowerBoundStr + (doubleIntegBoundField == 0 ? "_" : "");
                textRenderer.renderText(xLowerPrompt, leftMargin + 20, y, doubleIntegBoundField == 0 ? green : white);
                y += lineHeight;
                
                std::string xUpperPrompt = (doubleIntegBoundField == 1 ? "> " : "  ") + std::string("x upper bound: ") + xUpperBoundStr + (doubleIntegBoundField == 1 ? "_" : "");
                textRenderer.renderText(xUpperPrompt, leftMargin + 20, y, doubleIntegBoundField == 1 ? green : white);
                y += lineHeight;
                
                std::string yLowerPrompt = (doubleIntegBoundField == 2 ? "> " : "  ") + std::string("y lower bound: ") + yLowerBoundStr + (doubleIntegBoundField == 2 ? "_" : "");
                textRenderer.renderText(yLowerPrompt, leftMargin + 20, y, doubleIntegBoundField == 2 ? green : white);
                y += lineHeight;
                
                std::string yUpperPrompt = (doubleIntegBoundField == 3 ? "> " : "  ") + std::string("y upper bound: ") + yUpperBoundStr + (doubleIntegBoundField == 3 ? "_" : "");
                textRenderer.renderText(yUpperPrompt, leftMargin + 20, y, doubleIntegBoundField == 3 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string inputLine = "Input: f(x,y) = " + currentExpression;
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight;
                
                std::string boundsLine = "Bounds: x ∈ [" + xLowerBoundStr + ", " + xUpperBoundStr + "], y ∈ [" + yLowerBoundStr + ", " + yUpperBoundStr + "]";
                textRenderer.renderText(boundsLine, leftMargin, y, white);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !doubleIntegSteps.empty()) {
                textRenderer.renderText("--- Double Integration Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < doubleIntegSteps.size(); i++) {
                    const auto& step = doubleIntegSteps[i];
                    std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                    textRenderer.renderText(stepHeader, leftMargin, y, white);
                    y += lineHeight;
                    textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                    y += lineHeight + 5;
                }
                
                y += 10;
                textRenderer.renderText("--- Result ---", leftMargin, y, yellow);
                y += lineHeight;
                std::string finalResult = "∫∫ f(x,y) dy dx = " + std::to_string(doubleIntegralResult);
                textRenderer.renderText(finalResult, leftMargin, y, green);
                y += lineHeight + 20;
                
                textRenderer.renderText("ENTER: custom | B: bounds | SPACE: next | X: export | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === IMPLICIT DIFFERENTIATION MODE ===
        else if (currentMode == Mode::IMPLICIT_DIFFERENTIATION) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Implicit Differentiation Mode - dy/dx", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type equation F(x,y): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string inputLine = "Input: F(x,y) = " + currentExpression + " = 0";
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !implicitSteps.empty()) {
                textRenderer.renderText("--- Implicit Differentiation Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < implicitSteps.size(); i++) {
                    const auto& step = implicitSteps[i];
                    if (!step.description.empty()) {
                        std::string stepHeader = "Step " + std::to_string(i + 1) + ": " + step.description;
                        textRenderer.renderText(stepHeader, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 5;
                    }
                }
                
                y += 10;
                textRenderer.renderText("--- Final Result ---", leftMargin, y, yellow);
                y += lineHeight;
                textRenderer.renderText(implicitResult, leftMargin, y, green);
                y += lineHeight + 20;
                
                textRenderer.renderText("ENTER: custom | SPACE: next example | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === LINEAR TRANSFORMATION MODE ===
        else if (currentMode == Mode::LINEAR_TRANSFORMATION) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Linear Transformation Mode - 2D Analysis", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (matrixInputModeTransform) {
                textRenderer.renderText("Enter Transformation Matrix [[a, b], [c, d]]:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string aPrompt = (transformInputField == 0 ? "> " : "  ") + std::string("a (top-left): ") + transformMatrixInput[0] + (transformInputField == 0 ? "_" : "");
                textRenderer.renderText(aPrompt, leftMargin + 20, y, transformInputField == 0 ? green : white);
                y += lineHeight;
                
                std::string bPrompt = (transformInputField == 1 ? "> " : "  ") + std::string("b (top-right): ") + transformMatrixInput[1] + (transformInputField == 1 ? "_" : "");
                textRenderer.renderText(bPrompt, leftMargin + 20, y, transformInputField == 1 ? green : white);
                y += lineHeight;
                
                std::string cPrompt = (transformInputField == 2 ? "> " : "  ") + std::string("c (bottom-left): ") + transformMatrixInput[2] + (transformInputField == 2 ? "_" : "");
                textRenderer.renderText(cPrompt, leftMargin + 20, y, transformInputField == 2 ? green : white);
                y += lineHeight;
                
                std::string dPrompt = (transformInputField == 3 ? "> " : "  ") + std::string("d (bottom-right): ") + transformMatrixInput[3] + (transformInputField == 3 ? "_" : "");
                textRenderer.renderText(dPrompt, leftMargin + 20, y, transformInputField == 3 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to continue to vector input, ESC to cancel)", leftMargin, y, gray);
            } else if (vectorInputModeTransform) {
                textRenderer.renderText("Enter Input Vector [x, y]:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string xPrompt = (transformInputField == 0 ? "> " : "  ") + std::string("x: ") + transformVectorInput[0] + (transformInputField == 0 ? "_" : "");
                textRenderer.renderText(xPrompt, leftMargin + 20, y, transformInputField == 0 ? green : white);
                y += lineHeight;
                
                std::string yPrompt = (transformInputField == 1 ? "> " : "  ") + std::string("y: ") + transformVectorInput[1] + (transformInputField == 1 ? "_" : "");
                textRenderer.renderText(yPrompt, leftMargin + 20, y, transformInputField == 1 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string matrixLine = "Transformation Matrix: " + transformMatrix.toString();
                textRenderer.renderText(matrixLine, leftMargin, y, green);
                y += lineHeight;
                
                std::string vectorLine = "Input Vector: " + inputVector.toString();
                textRenderer.renderText(vectorLine, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !transformSteps.empty()) {
                textRenderer.renderText("--- Linear Transformation Analysis ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < transformSteps.size(); i++) {
                    const auto& step = transformSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 5;
                    }
                }
                
                y += 10;
                textRenderer.renderText("Press M for new matrix, V for new vector, ESC for menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === TAYLOR SERIES MODE ===
        else if (currentMode == Mode::TAYLOR_SERIES) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Taylor Series Expansion Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type function f(x): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else if (taylorConfigMode) {
                textRenderer.renderText("Configure Taylor Series:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string centerPrompt = (taylorConfigField == 0 ? "> " : "  ") + std::string("Expansion center (a): ") + taylorCenterStr + (taylorConfigField == 0 ? "_" : "");
                textRenderer.renderText(centerPrompt, leftMargin + 20, y, taylorConfigField == 0 ? green : white);
                y += lineHeight;
                
                std::string orderPrompt = (taylorConfigField == 1 ? "> " : "  ") + std::string("Order (n): ") + taylorOrderStr + (taylorConfigField == 1 ? "_" : "");
                textRenderer.renderText(orderPrompt, leftMargin + 20, y, taylorConfigField == 1 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string inputLine = "Input: f(x) = " + currentExpression;
                textRenderer.renderText(inputLine, leftMargin, y, green);
                y += lineHeight;
                
                std::string configLine = "Center: a = " + taylorCenterStr + ", Order: n = " + taylorOrderStr;
                textRenderer.renderText(configLine, leftMargin, y, white);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !taylorSteps.empty()) {
                textRenderer.renderText("--- Taylor Series Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < taylorSteps.size(); i++) {
                    const auto& step = taylorSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 3;
                    }
                }
                
                y += 10;
                textRenderer.renderText("ENTER: custom | C: configure | SPACE: next | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === PARAMETRIC CURVE MODE ===
        else if (currentMode == Mode::PARAMETRIC_CURVE) {
            y = 20 - scrollOffset;  // Initialize y for this mode
            textRenderer.renderText("Parametric Curve Analysis Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (parametricXInputMode) {
                std::string inputPrompt = "Type x(t): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER for y(t), ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else if (parametricYInputMode) {
                std::string xLine = "x(t) = " + currentXExpression;
                textRenderer.renderText(xLine, leftMargin, y, green);
                y += lineHeight + 10;
                
                std::string inputPrompt = "Type y(t): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else if (parametricConfigMode) {
                textRenderer.renderText("Configure Parameter Range:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string tStartPrompt = (parametricConfigField == 0 ? "> " : "  ") + std::string("t start: ") + tStartStr + (parametricConfigField == 0 ? "_" : "");
                textRenderer.renderText(tStartPrompt, leftMargin + 20, y, parametricConfigField == 0 ? green : white);
                y += lineHeight;
                
                std::string tEndPrompt = (parametricConfigField == 1 ? "> " : "  ") + std::string("t end: ") + tEndStr + (parametricConfigField == 1 ? "_" : "");
                textRenderer.renderText(tEndPrompt, leftMargin + 20, y, parametricConfigField == 1 ? green : white);
                y += lineHeight;
                
                std::string tEvalPrompt = (parametricConfigField == 2 ? "> " : "  ") + std::string("t eval: ") + tEvalStr + (parametricConfigField == 2 ? "_" : "");
                textRenderer.renderText(tEvalPrompt, leftMargin + 20, y, parametricConfigField == 2 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string xLine = "x(t) = " + currentXExpression;
                textRenderer.renderText(xLine, leftMargin, y, green);
                y += lineHeight;
                
                std::string yLine = "y(t) = " + currentYExpression;
                textRenderer.renderText(yLine, leftMargin, y, green);
                y += lineHeight;
                
                std::string paramLine = "t ∈ [" + tStartStr + ", " + tEndStr + "], eval at t = " + tEvalStr;
                textRenderer.renderText(paramLine, leftMargin, y, white);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !parametricSteps.empty()) {
                textRenderer.renderText("--- Parametric Curve Analysis ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < parametricSteps.size(); i++) {
                    const auto& step = parametricSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 3;
                    }
                }
                
                y += 10;
                textRenderer.renderText("ENTER: custom | C: configure | SPACE: next | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === COMPLEX NUMBERS MODE ===
        else if (currentMode == Mode::COMPLEX_NUMBERS) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Complex Numbers Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (complexInputMode) {
                textRenderer.renderText("Enter Complex Number (a + bi):", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string realPrompt = (complexInputField == 0 ? "> " : "  ") + std::string("Real part (a): ") + complexA1Str + (complexInputField == 0 ? "_" : "");
                textRenderer.renderText(realPrompt, leftMargin + 20, y, complexInputField == 0 ? green : white);
                y += lineHeight;
                
                std::string imagPrompt = (complexInputField == 1 ? "> " : "  ") + std::string("Imaginary part (b): ") + complexB1Str + (complexInputField == 1 ? "_" : "");
                textRenderer.renderText(imagPrompt, leftMargin + 20, y, complexInputField == 1 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string z1 = "z1 = " + std::to_string(complexA1) + " + " + std::to_string(complexB1) + "i";
                textRenderer.renderText(z1, leftMargin, y, green);
                y += lineHeight + 15;
                
                if (parseSuccess && !complexSteps.empty()) {
                    textRenderer.renderText("--- Complex Number Analysis ---", leftMargin, y, yellow);
                    y += lineHeight;
                    
                    for (size_t i = 0; i < complexSteps.size(); i++) {
                        const auto& step = complexSteps[i];
                        if (!step.description.empty()) {
                            textRenderer.renderText(step.description, leftMargin, y, white);
                            y += lineHeight;
                        }
                        if (!step.expression.empty()) {
                            textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                            y += lineHeight + 3;
                        }
                    }
                    
                    y += 10;
                    textRenderer.renderText("C: custom input | ESC: menu", 20, 690, gray);
                } else if (!errorMsg.empty()) {
                    textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
                }
            }
        }
        // === SEQUENCES & SERIES MODE ===
        else if (currentMode == Mode::SEQUENCES_SERIES) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Sequences & Series Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (seqInputMode) {
                textRenderer.renderText("Enter Arithmetic Sequence Parameters:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string aPrompt = (seqInputField == 0 ? "> " : "  ") + std::string("First term (a): ") + seqAStr + (seqInputField == 0 ? "_" : "");
                textRenderer.renderText(aPrompt, leftMargin + 20, y, seqInputField == 0 ? green : white);
                y += lineHeight;
                
                std::string dPrompt = (seqInputField == 1 ? "> " : "  ") + std::string("Common difference (d): ") + seqDStr + (seqInputField == 1 ? "_" : "");
                textRenderer.renderText(dPrompt, leftMargin + 20, y, seqInputField == 1 ? green : white);
                y += lineHeight;
                
                std::string nPrompt = (seqInputField == 2 ? "> " : "  ") + std::string("Number of terms (n): ") + seqNStr + (seqInputField == 2 ? "_" : "");
                textRenderer.renderText(nPrompt, leftMargin + 20, y, seqInputField == 2 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string params = "Arithmetic: a=" + std::to_string(seqA) + ", d=" + std::to_string(seqD) + ", n=" + std::to_string(seqN);
                textRenderer.renderText(params, leftMargin, y, green);
                y += lineHeight + 15;
                
                if (parseSuccess && !sequenceSteps.empty()) {
                    textRenderer.renderText("--- Sequence Analysis ---", leftMargin, y, yellow);
                    y += lineHeight;
                    
                    for (size_t i = 0; i < sequenceSteps.size(); i++) {
                        const auto& step = sequenceSteps[i];
                        if (!step.description.empty()) {
                            textRenderer.renderText(step.description, leftMargin, y, white);
                            y += lineHeight;
                        }
                        if (!step.expression.empty()) {
                            textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                            y += lineHeight + 3;
                        }
                    }
                    
                    y += 10;
                    textRenderer.renderText("C: custom input | ESC: menu", 20, 690, gray);
                } else if (!errorMsg.empty()) {
                    textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
                }
            }
        }
        // === NUMERICAL METHODS MODE ===
        else if (currentMode == Mode::NUMERICAL_METHODS) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Numerical Methods Mode - Newton-Raphson", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type function f(x): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to set, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else if (numConfigMode) {
                textRenderer.renderText("Configure Initial Guess:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string x0Prompt = "> Initial guess (x0): " + numX0Str + "_";
                textRenderer.renderText(x0Prompt, leftMargin + 20, y, green);
                y += lineHeight + 10;
                
                textRenderer.renderText("(ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string func = "f(x) = " + currentExpression;
                textRenderer.renderText(func, leftMargin, y, green);
                y += lineHeight;
                std::string initial = "Initial guess: x0 = " + std::to_string(numX0);
                textRenderer.renderText(initial, leftMargin, y, white);
                y += lineHeight + 15;
                
                if (parseSuccess && !numericalSteps.empty()) {
                    textRenderer.renderText("--- Newton-Raphson Method ---", leftMargin, y, yellow);
                    y += lineHeight;
                    
                    for (size_t i = 0; i < numericalSteps.size(); i++) {
                        const auto& step = numericalSteps[i];
                        if (!step.description.empty()) {
                            textRenderer.renderText(step.description, leftMargin, y, white);
                            y += lineHeight;
                        }
                        if (!step.expression.empty()) {
                            textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                            y += lineHeight + 3;
                        }
                    }
                    
                    y += 10;
                    textRenderer.renderText("ENTER: custom | C: config x0 | ESC: menu", 20, 690, gray);
                } else if (!errorMsg.empty()) {
                    textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
                }
            }
        }
        // === EIGENVALUES MODE ===
        else if (currentMode == Mode::EIGENVALUES) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Eigenvalues & Eigenvectors Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (eigenInputMode) {
                textRenderer.renderText("Enter 2x2 Matrix Elements:", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string aPrompt = (eigenInputField == 0 ? "> " : "  ") + std::string("A[0][0]: ") + eigenAStr + (eigenInputField == 0 ? "_" : "");
                textRenderer.renderText(aPrompt, leftMargin + 20, y, eigenInputField == 0 ? green : white);
                y += lineHeight;
                
                std::string bPrompt = (eigenInputField == 1 ? "> " : "  ") + std::string("A[0][1]: ") + eigenBStr + (eigenInputField == 1 ? "_" : "");
                textRenderer.renderText(bPrompt, leftMargin + 20, y, eigenInputField == 1 ? green : white);
                y += lineHeight;
                
                std::string cPrompt = (eigenInputField == 2 ? "> " : "  ") + std::string("A[1][0]: ") + eigenCStr + (eigenInputField == 2 ? "_" : "");
                textRenderer.renderText(cPrompt, leftMargin + 20, y, eigenInputField == 2 ? green : white);
                y += lineHeight;
                
                std::string dPrompt = (eigenInputField == 3 ? "> " : "  ") + std::string("A[1][1]: ") + eigenDStr + (eigenInputField == 3 ? "_" : "");
                textRenderer.renderText(dPrompt, leftMargin + 20, y, eigenInputField == 3 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string matrixLine = "Matrix A = [[" + std::to_string(eigenA) + ", " + std::to_string(eigenB) + 
                                         "], [" + std::to_string(eigenC) + ", " + std::to_string(eigenD) + "]]";
                textRenderer.renderText(matrixLine, leftMargin, y, green);
                y += lineHeight + 15;
                
                if (parseSuccess && !eigenSteps.empty()) {
                    textRenderer.renderText("--- Eigenvalue Analysis ---", leftMargin, y, yellow);
                    y += lineHeight;
                    
                    for (size_t i = 0; i < eigenSteps.size(); i++) {
                        const auto& step = eigenSteps[i];
                        if (!step.description.empty()) {
                            textRenderer.renderText(step.description, leftMargin, y, white);
                            y += lineHeight;
                        }
                        if (!step.expression.empty()) {
                            textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                            y += lineHeight + 3;
                        }
                    }
                    
                    y += 10;
                    textRenderer.renderText("C: custom input | ESC: menu", 20, 690, gray);
                } else if (!errorMsg.empty()) {
                    textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
                }
            }
        }
        // === STATISTICS MODE ===
        else if (currentMode == Mode::STATISTICS) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Statistics & Probability Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (statsInputMode) {
                textRenderer.renderText("Enter Data (comma-separated):", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string dataPrompt = "> Data: " + statsDataStr + "_";
                textRenderer.renderText(dataPrompt, leftMargin + 20, y, green);
                y += lineHeight + 10;
                
                textRenderer.renderText("Example: 2, 4, 6, 8, 10", leftMargin + 20, y, gray);
                y += lineHeight;
                textRenderer.renderText("(ENTER to compute, ESC to cancel)", leftMargin, y, gray);
            } else {
                std::string dataStr = "Data: {";
                for (size_t i = 0; i < statsData.size(); i++) {
                    if (i > 0) dataStr += ", ";
                    dataStr += std::to_string((int)statsData[i]);
                }
                dataStr += "}";
                textRenderer.renderText(dataStr, leftMargin, y, green);
                y += lineHeight + 15;
                
                if (parseSuccess && !statsSteps.empty()) {
                    textRenderer.renderText("--- Statistical Analysis ---", leftMargin, y, yellow);
                    y += lineHeight;
                    
                    for (size_t i = 0; i < statsSteps.size(); i++) {
                        const auto& step = statsSteps[i];
                        if (!step.description.empty()) {
                            textRenderer.renderText(step.description, leftMargin, y, white);
                            y += lineHeight;
                        }
                        if (!step.expression.empty()) {
                            textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                            y += lineHeight + 3;
                        }
                    }
                    
                    y += 10;
                    textRenderer.renderText("C: custom input | ESC: menu", 20, 690, gray);
                } else if (!errorMsg.empty()) {
                    textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
                }
            }
        }
        // === POLYNOMIAL OPERATIONS MODE ===
        else if (currentMode == Mode::POLYNOMIAL_OPERATIONS) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Polynomial Operations - Quadratic Solver", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (polyInputMode) {
                textRenderer.renderText("Enter Quadratic Coefficients (ax^2 + bx + c = 0):", leftMargin, y, yellow);
                y += lineHeight + 10;
                
                std::string aPrompt = (polyInputField == 0 ? "> " : "  ") + std::string("Coefficient a (x^2): ") + polyAStr + (polyInputField == 0 ? "_" : "");
                textRenderer.renderText(aPrompt, leftMargin + 20, y, polyInputField == 0 ? green : white);
                y += lineHeight;
                
                std::string bPrompt = (polyInputField == 1 ? "> " : "  ") + std::string("Coefficient b (x): ") + polyBStr + (polyInputField == 1 ? "_" : "");
                textRenderer.renderText(bPrompt, leftMargin + 20, y, polyInputField == 1 ? green : white);
                y += lineHeight;
                
                std::string cPrompt = (polyInputField == 2 ? "> " : "  ") + std::string("Coefficient c (constant): ") + polyCStr + (polyInputField == 2 ? "_" : "");
                textRenderer.renderText(cPrompt, leftMargin + 20, y, polyInputField == 2 ? green : white);
                y += lineHeight + 10;
                
                textRenderer.renderText("(TAB to switch, ENTER to solve, ESC to cancel)", leftMargin, y, gray);
                y += lineHeight;
                textRenderer.renderText("Example: a=1, b=-5, c=6  =>  x^2 - 5x + 6 = 0", leftMargin, y, gray);
            } else {
                std::ostringstream eqOss;
                eqOss << std::fixed << std::setprecision(2);
                eqOss << "Equation: " << polyA << "x^2 ";
                if (polyB >= 0) eqOss << "+ ";
                eqOss << polyB << "x ";
                if (polyC >= 0) eqOss << "+ ";
                eqOss << polyC << " = 0";
                textRenderer.renderText(eqOss.str(), leftMargin, y, green);
                y += lineHeight + 15;
                
                if (parseSuccess && !polySteps.empty()) {
                    textRenderer.renderText("--- Step-by-Step Solution ---", leftMargin, y, yellow);
                    y += lineHeight;
                    
                    for (size_t i = 0; i < polySteps.size(); i++) {
                        const auto& step = polySteps[i];
                        if (!step.description.empty()) {
                            textRenderer.renderText(step.description, leftMargin, y, white);
                            y += lineHeight;
                        }
                        if (!step.expression.empty()) {
                            textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                            y += lineHeight + 3;
                        }
                    }
                    
                    y += 10;
                    textRenderer.renderText("C: custom input | ESC: menu", 20, 690, gray);
                } else if (!errorMsg.empty()) {
                    textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
                }
            }
        }
        // === LAPLACE TRANSFORM MODE ===
        else if (currentMode == Mode::LAPLACE_TRANSFORM) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Laplace Transform Mode - L{f(t)}", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type function f(t): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string func = "f(t) = " + currentExpression;
                textRenderer.renderText(func, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !laplaceSteps.empty() && !inputMode) {
                textRenderer.renderText("--- Laplace Transform ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < laplaceSteps.size(); i++) {
                    const auto& step = laplaceSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 3;
                    }
                }
                
                y += 10;
                textRenderer.renderText("ENTER: custom input | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === INVERSE LAPLACE MODE ===
        else if (currentMode == Mode::INVERSE_LAPLACE) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Inverse Laplace Transform Mode - L^-1{F(s)}", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type function F(s): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string func = "F(s) = " + currentExpression;
                textRenderer.renderText(func, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !laplaceSteps.empty() && !inputMode) {
                textRenderer.renderText("--- Inverse Laplace Transform ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < laplaceSteps.size(); i++) {
                    const auto& step = laplaceSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 3;
                    }
                }
                
                y += 10;
                textRenderer.renderText("ENTER: custom input | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === FOURIER SERIES MODE ===
        else if (currentMode == Mode::FOURIER_SERIES) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Fourier Series Mode", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type function f(x): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string func = "f(x) = " + currentExpression;
                textRenderer.renderText(func, leftMargin, y, green);
                y += lineHeight;
                std::string period = "Period: " + std::to_string(fourierPeriod) + ", Terms: " + std::to_string(fourierTerms);
                textRenderer.renderText(period, leftMargin, y, white);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !fourierSteps.empty() && !inputMode) {
                textRenderer.renderText("--- Fourier Series Expansion ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < fourierSteps.size(); i++) {
                    const auto& step = fourierSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 3;
                    }
                }
                
                y += 10;
                textRenderer.renderText("ENTER: custom input | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === DIFFERENTIAL EQUATIONS MODE ===
        else if (currentMode == Mode::DIFFERENTIAL_EQUATIONS) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Differential Equations Mode (1st Order)", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type equation: " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(e.g., dy/dx=x*y, Press ENTER to compute)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string eq = "Equation: " + currentExpression;
                textRenderer.renderText(eq, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !diffEqSteps.empty() && !inputMode) {
                textRenderer.renderText("--- Solution Steps ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < diffEqSteps.size(); i++) {
                    const auto& step = diffEqSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 3;
                    }
                }
                
                y += 10;
                textRenderer.renderText("ENTER: custom input | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
            }
        }
        // === VECTOR CALCULUS MODE ===
        else if (currentMode == Mode::VECTOR_CALCULUS) {
            y = 20 - scrollOffset;
            textRenderer.renderText("Vector Calculus Mode - Gradient", leftMargin, y, cyan);
            y += lineHeight + 10;
            
            if (inputMode) {
                std::string inputPrompt = "Type function f(x,y): " + userInput + "_";
                textRenderer.renderText(inputPrompt, leftMargin, y, yellow);
                textRenderer.renderText("(Press ENTER to compute gradient, ESC to cancel)", leftMargin, y + lineHeight, gray);
                y += lineHeight * 2 + 15;
            } else {
                std::string func = "f(x,y) = " + currentExpression;
                textRenderer.renderText(func, leftMargin, y, green);
                y += lineHeight + 15;
            }
            
            if (parseSuccess && !vectorSteps.empty() && !inputMode) {
                textRenderer.renderText("--- Vector Calculus Operations ---", leftMargin, y, yellow);
                y += lineHeight;
                
                for (size_t i = 0; i < vectorSteps.size(); i++) {
                    const auto& step = vectorSteps[i];
                    if (!step.description.empty()) {
                        textRenderer.renderText(step.description, leftMargin, y, white);
                        y += lineHeight;
                    }
                    if (!step.expression.empty()) {
                        textRenderer.renderText("  " + step.expression, leftMargin + 20, y, white);
                        y += lineHeight + 3;
                    }
                }
                
                y += 10;
                textRenderer.renderText("ENTER: custom input | ESC: menu", 20, 690, gray);
            } else if (!errorMsg.empty()) {
                textRenderer.renderText(errorMsg, leftMargin, y, {255, 100, 100, 255});
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

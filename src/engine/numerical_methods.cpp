#include "numerical_methods.h"
#include "differentiator.h"
#include <cmath>
#include <sstream>
#include <iomanip>

void NumericalMethods::newtonRaphson(const ASTNode* func, double x0, int maxIter, double tolerance) {
    steps.clear();
    
    NumericalStep step1;
    step1.description = "=== Newton-Raphson Method ===";
    step1.expression = "Finding root of f(x) = " + func->toString();
    steps.push_back(step1);
    
    NumericalStep step2;
    step2.description = "Initial guess:";
    step2.expression = "x(0) = " + std::to_string(x0);
    steps.push_back(step2);
    
    NumericalStep step3;
    step3.description = "Formula: x(n+1) = x(n) - f(x(n))/f'(x(n))";
    step3.expression = "";
    steps.push_back(step3);
    
    // Compute derivative
    Differentiator diff;
    auto fprime = diff.differentiate(func);
    
    NumericalStep step4;
    step4.description = "Derivative:";
    step4.expression = "f'(x) = " + fprime->toString();
    steps.push_back(step4);
    
    double x = x0;
    for (int i = 0; i < maxIter; i++) {
        double fx = func->evaluate(x);
        double fpx = fprime->evaluate(x);
        
        if (std::abs(fpx) < 1e-10) {
            NumericalStep errorStep;
            errorStep.description = "Error:";
            errorStep.expression = "Derivative too small, method fails";
            steps.push_back(errorStep);
            break;
        }
        
        double xnew = x - fx / fpx;
        
        NumericalStep stepI;
        stepI.description = "Iteration " + std::to_string(i+1) + ":";
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(8);
        oss << "x" << (i+1) << " = " << xnew << ", f(x" << (i+1) << ") = " << func->evaluate(xnew);
        stepI.expression = oss.str();
        steps.push_back(stepI);
        
        if (std::abs(xnew - x) < tolerance) {
            NumericalStep finalStep;
            finalStep.description = "=== Converged ===";
            std::ostringstream oss2;
            oss2 << std::fixed << std::setprecision(8);
            oss2 << "Root: x = " << xnew << " (in " << (i+1) << " iterations)";
            finalStep.expression = oss2.str();
            steps.push_back(finalStep);
            break;
        }
        
        x = xnew;
    }
}

void NumericalMethods::bisectionMethod(const ASTNode* func, double a, double b, int maxIter, double tolerance) {
    steps.clear();
    
    NumericalStep step1;
    step1.description = "=== Bisection Method ===";
    step1.expression = "Finding root of f(x) = " + func->toString();
    steps.push_back(step1);
    
    NumericalStep step2;
    step2.description = "Initial interval:";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(4);
    oss2 << "[" << a << ", " << b << "]";
    step2.expression = oss2.str();
    steps.push_back(step2);
    
    double fa = func->evaluate(a);
    double fb = func->evaluate(b);
    
    NumericalStep step3;
    step3.description = "Check: f(a) × f(b) < 0";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(4);
    oss3 << "f(" << a << ") = " << fa << ", f(" << b << ") = " << fb;
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    if (fa * fb > 0) {
        NumericalStep errorStep;
        errorStep.description = "Error:";
        errorStep.expression = "f(a) and f(b) must have opposite signs!";
        steps.push_back(errorStep);
        return;
    }
    
    double aOld = a, bOld = b;
    for (int i = 0; i < maxIter; i++) {
        double c = (aOld + bOld) / 2.0;
        double fc = func->evaluate(c);
        
        NumericalStep stepI;
        stepI.description = "Iteration " + std::to_string(i+1) + ":";
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(8);
        oss << "c = " << c << ", f(c) = " << fc;
        stepI.expression = oss.str();
        steps.push_back(stepI);
        
        if (std::abs(fc) < tolerance || (bOld - aOld) / 2.0 < tolerance) {
            NumericalStep finalStep;
            finalStep.description = "=== Converged ===";
            std::ostringstream oss2;
            oss2 << std::fixed << std::setprecision(8);
            oss2 << "Root: x = " << c << " (in " << (i+1) << " iterations)";
            finalStep.expression = oss2.str();
            steps.push_back(finalStep);
            break;
        }
        
        double faOld = func->evaluate(aOld);
        if (fc * faOld < 0) {
            bOld = c;
        } else {
            aOld = c;
        }
    }
}

void NumericalMethods::trapezoidalRule(const ASTNode* func, double a, double b, int n) {
    steps.clear();
    
    NumericalStep step1;
    step1.description = "=== Trapezoidal Rule ===";
    step1.expression = "∫[" + std::to_string(a) + "," + std::to_string(b) + "] " + func->toString() + " dx";
    steps.push_back(step1);
    
    NumericalStep step2;
    step2.description = "Number of subintervals: n = " + std::to_string(n);
    step2.expression = "";
    steps.push_back(step2);
    
    double h = (b - a) / n;
    
    NumericalStep step3;
    step3.description = "Step size:";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(6);
    oss3 << "h = (b-a)/n = " << h;
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    NumericalStep step4;
    step4.description = "Formula:";
    step4.expression = "Integral f(x)dx ~ h/2 * [f(x0) + 2f(x1) + 2f(x2) + ... + 2f(x(n-1)) + f(xn)]";
    steps.push_back(step4);
    
    double sum = func->evaluate(a) + func->evaluate(b);
    
    NumericalStep step5;
    step5.description = "Computing sum:";
    step5.expression = "";
    steps.push_back(step5);
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += 2.0 * func->evaluate(x);
        
        if (i <= 5) {
            NumericalStep stepI;
            stepI.description = "  x" + std::to_string(i) + ":";
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(6);
            oss << x << ", f(x" << i << ") = " << func->evaluate(x);
            stepI.expression = oss.str();
            steps.push_back(stepI);
        }
    }
    
    double result = (h / 2.0) * sum;
    
    NumericalStep finalStep;
    finalStep.description = "=== Result ===";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(8);
    oss << "∫f(x)dx ≈ " << result;
    finalStep.expression = oss.str();
    steps.push_back(finalStep);
}

void NumericalMethods::simpsonsRule(const ASTNode* func, double a, double b, int n) {
    steps.clear();
    
    if (n % 2 != 0) {
        n++; // Simpson's rule requires even number of intervals
    }
    
    NumericalStep step1;
    step1.description = "=== Simpson's Rule ===";
    step1.expression = "∫[" + std::to_string(a) + "," + std::to_string(b) + "] " + func->toString() + " dx";
    steps.push_back(step1);
    
    NumericalStep step2;
    step2.description = "Number of subintervals: n = " + std::to_string(n) + " (must be even)";
    step2.expression = "";
    steps.push_back(step2);
    
    double h = (b - a) / n;
    
    NumericalStep step3;
    step3.description = "Step size:";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(6);
    oss3 << "h = (b-a)/n = " << h;
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    NumericalStep step4;
    step4.description = "Formula:";
    step4.expression = "Integral f(x)dx ~ h/3 * [f(x0) + 4f(x1) + 2f(x2) + 4f(x3) + ... + f(xn)]";
    steps.push_back(step4);
    
    double sum = func->evaluate(a) + func->evaluate(b);
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 0) {
            sum += 2.0 * func->evaluate(x);
        } else {
            sum += 4.0 * func->evaluate(x);
        }
    }
    
    double result = (h / 3.0) * sum;
    
    NumericalStep finalStep;
    finalStep.description = "=== Result ===";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(8);
    oss << "∫f(x)dx ≈ " << result;
    finalStep.expression = oss.str();
    steps.push_back(finalStep);
    
    NumericalStep note;
    note.description = "Note:";
    note.expression = "Simpson's rule is more accurate than trapezoidal rule";
    steps.push_back(note);
}

void NumericalMethods::forwardDifference(const ASTNode* func, double x, double h) {
    steps.clear();
    
    NumericalStep step1;
    step1.description = "=== Forward Difference Approximation ===";
    step1.expression = "f'(x) at x = " + std::to_string(x);
    steps.push_back(step1);
    
    NumericalStep step2;
    step2.description = "Formula:";
    step2.expression = "f'(x) ≈ [f(x+h) - f(x)] / h";
    steps.push_back(step2);
    
    NumericalStep step3;
    step3.description = "Step size: h = " + std::to_string(h);
    step3.expression = "";
    steps.push_back(step3);
    
    double fx = func->evaluate(x);
    double fxh = func->evaluate(x + h);
    double derivative = (fxh - fx) / h;
    
    NumericalStep step4;
    step4.description = "Computing:";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(8);
    oss << "f(" << x << ") = " << fx << "\n";
    oss << "f(" << (x+h) << ") = " << fxh;
    step4.expression = oss.str();
    steps.push_back(step4);
    
    NumericalStep finalStep;
    finalStep.description = "=== Result ===";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(8);
    oss2 << "f'(" << x << ") ≈ " << derivative;
    finalStep.expression = oss2.str();
    steps.push_back(finalStep);
}

void NumericalMethods::centralDifference(const ASTNode* func, double x, double h) {
    steps.clear();
    
    NumericalStep step1;
    step1.description = "=== Central Difference Approximation ===";
    step1.expression = "f'(x) at x = " + std::to_string(x);
    steps.push_back(step1);
    
    NumericalStep step2;
    step2.description = "Formula:";
    step2.expression = "f'(x) ≈ [f(x+h) - f(x-h)] / (2h)";
    steps.push_back(step2);
    
    NumericalStep step3;
    step3.description = "Step size: h = " + std::to_string(h);
    step3.expression = "(Central difference is more accurate than forward/backward)";
    steps.push_back(step3);
    
    double fxh = func->evaluate(x + h);
    double fxmh = func->evaluate(x - h);
    double derivative = (fxh - fxmh) / (2.0 * h);
    
    NumericalStep step4;
    step4.description = "Computing:";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(8);
    oss << "f(" << (x+h) << ") = " << fxh << "\n";
    oss << "f(" << (x-h) << ") = " << fxmh;
    step4.expression = oss.str();
    steps.push_back(step4);
    
    NumericalStep finalStep;
    finalStep.description = "=== Result ===";
    std::ostringstream oss2;
    oss2 << std::fixed << std::setprecision(8);
    oss2 << "f'(" << x << ") ≈ " << derivative;
    finalStep.expression = oss2.str();
    steps.push_back(finalStep);
}

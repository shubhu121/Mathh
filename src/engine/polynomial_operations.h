#pragma once
#include <vector>
#include <string>

struct PolynomialStep {
    std::string description;
    std::string expression;
};

class PolynomialOperations {
private:
    std::vector<PolynomialStep> steps;
    
public:
    // Solve quadratic equation ax^2 + bx + c = 0
    void solveQuadratic(double a, double b, double c);
    
    // Evaluate polynomial at a given x value
    void evaluatePolynomial(const std::vector<double>& coeffs, double x);
    
    // Find polynomial roots for quadratic
    void findRoots(double a, double b, double c);
    
    // Polynomial long division
    void polynomialDivision(const std::vector<double>& dividend, const std::vector<double>& divisor);
    
    const std::vector<PolynomialStep>& getSteps() const { return steps; }
};

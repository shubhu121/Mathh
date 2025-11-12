#include "polynomial_operations.h"
#include <cmath>
#include <sstream>
#include <iomanip>

void PolynomialOperations::solveQuadratic(double a, double b, double c) {
    steps.clear();
    
    PolynomialStep step1;
    step1.description = "=== Quadratic Equation Solver ===";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << a << "x^2 ";
    if (b >= 0) oss << "+ ";
    oss << b << "x ";
    if (c >= 0) oss << "+ ";
    oss << c << " = 0";
    step1.expression = oss.str();
    steps.push_back(step1);
    
    PolynomialStep step2;
    step2.description = "Quadratic Formula:";
    step2.expression = "x = [-b ± sqrt(b^2 - 4ac)] / (2a)";
    steps.push_back(step2);
    
    PolynomialStep step3;
    step3.description = "Coefficients:";
    std::ostringstream oss3;
    oss3 << std::fixed << std::setprecision(2);
    oss3 << "a = " << a << ", b = " << b << ", c = " << c;
    step3.expression = oss3.str();
    steps.push_back(step3);
    
    double discriminant = b * b - 4 * a * c;
    
    PolynomialStep step4;
    step4.description = "Calculate Discriminant:";
    std::ostringstream oss4;
    oss4 << std::fixed << std::setprecision(4);
    oss4 << "Δ = b^2 - 4ac = " << discriminant;
    step4.expression = oss4.str();
    steps.push_back(step4);
    
    if (discriminant > 0) {
        PolynomialStep step5;
        step5.description = "Nature of Roots:";
        step5.expression = "Δ > 0: Two distinct real roots";
        steps.push_back(step5);
        
        double x1 = (-b + std::sqrt(discriminant)) / (2 * a);
        double x2 = (-b - std::sqrt(discriminant)) / (2 * a);
        
        PolynomialStep step6;
        step6.description = "Solutions:";
        std::ostringstream oss6;
        oss6 << std::fixed << std::setprecision(6);
        oss6 << "x1 = " << x1 << "\n";
        oss6 << "x2 = " << x2;
        step6.expression = oss6.str();
        steps.push_back(step6);
        
        PolynomialStep step7;
        step7.description = "Verification:";
        double check1 = a * x1 * x1 + b * x1 + c;
        double check2 = a * x2 * x2 + b * x2 + c;
        std::ostringstream oss7;
        oss7 << std::fixed << std::setprecision(8);
        oss7 << "f(x1) = " << check1 << " ≈ 0\n";
        oss7 << "f(x2) = " << check2 << " ≈ 0";
        step7.expression = oss7.str();
        steps.push_back(step7);
    } else if (discriminant == 0) {
        PolynomialStep step5;
        step5.description = "Nature of Roots:";
        step5.expression = "Δ = 0: One repeated real root";
        steps.push_back(step5);
        
        double x = -b / (2 * a);
        
        PolynomialStep step6;
        step6.description = "Solution:";
        std::ostringstream oss6;
        oss6 << std::fixed << std::setprecision(6);
        oss6 << "x = " << x << " (repeated root)";
        step6.expression = oss6.str();
        steps.push_back(step6);
    } else {
        PolynomialStep step5;
        step5.description = "Nature of Roots:";
        step5.expression = "Δ < 0: Two complex conjugate roots";
        steps.push_back(step5);
        
        double realPart = -b / (2 * a);
        double imagPart = std::sqrt(-discriminant) / (2 * a);
        
        PolynomialStep step6;
        step6.description = "Complex Solutions:";
        std::ostringstream oss6;
        oss6 << std::fixed << std::setprecision(6);
        oss6 << "x1 = " << realPart << " + " << imagPart << "i\n";
        oss6 << "x2 = " << realPart << " - " << imagPart << "i";
        step6.expression = oss6.str();
        steps.push_back(step6);
    }
}

void PolynomialOperations::evaluatePolynomial(const std::vector<double>& coeffs, double x) {
    steps.clear();
    
    PolynomialStep step1;
    step1.description = "=== Polynomial Evaluation ===";
    std::ostringstream oss;
    oss << "P(x) = ";
    for (int i = coeffs.size() - 1; i >= 0; i--) {
        if (i == coeffs.size() - 1) {
            oss << coeffs[i];
        } else {
            if (coeffs[i] >= 0) oss << " + ";
            else oss << " ";
            oss << coeffs[i];
        }
        if (i > 0) oss << "x";
        if (i > 1) oss << "^" << i;
    }
    step1.expression = oss.str();
    steps.push_back(step1);
    
    PolynomialStep step2;
    step2.description = "Evaluate at:";
    std::ostringstream oss2;
    oss2 << "x = " << x;
    step2.expression = oss2.str();
    steps.push_back(step2);
    
    PolynomialStep step3;
    step3.description = "Using Horner's Method:";
    step3.expression = "Efficient polynomial evaluation";
    steps.push_back(step3);
    
    double result = coeffs[coeffs.size() - 1];
    std::ostringstream detailOss;
    detailOss << std::fixed << std::setprecision(4);
    detailOss << "Start: " << result << "\n";
    
    for (int i = coeffs.size() - 2; i >= 0; i--) {
        result = result * x + coeffs[i];
        detailOss << "Step " << (coeffs.size() - 1 - i) << ": " << result << "\n";
    }
    
    PolynomialStep step4;
    step4.description = "Calculation Steps:";
    step4.expression = detailOss.str();
    steps.push_back(step4);
    
    PolynomialStep step5;
    step5.description = "=== Result ===";
    std::ostringstream oss5;
    oss5 << std::fixed << std::setprecision(6);
    oss5 << "P(" << x << ") = " << result;
    step5.expression = oss5.str();
    steps.push_back(step5);
}

void PolynomialOperations::findRoots(double a, double b, double c) {
    solveQuadratic(a, b, c);
}

void PolynomialOperations::polynomialDivision(const std::vector<double>& dividend, const std::vector<double>& divisor) {
    steps.clear();
    
    PolynomialStep step1;
    step1.description = "=== Polynomial Long Division ===";
    step1.expression = "Dividing polynomials";
    steps.push_back(step1);
    
    // Display dividend
    std::ostringstream divOss;
    divOss << "Dividend: ";
    for (int i = dividend.size() - 1; i >= 0; i--) {
        if (i < dividend.size() - 1 && dividend[i] >= 0) divOss << "+ ";
        divOss << dividend[i];
        if (i > 0) divOss << "x";
        if (i > 1) divOss << "^" << i;
        divOss << " ";
    }
    
    PolynomialStep step2;
    step2.description = divOss.str();
    step2.expression = "";
    steps.push_back(step2);
    
    // Display divisor
    std::ostringstream divsorOss;
    divsorOss << "Divisor: ";
    for (int i = divisor.size() - 1; i >= 0; i--) {
        if (i < divisor.size() - 1 && divisor[i] >= 0) divsorOss << "+ ";
        divsorOss << divisor[i];
        if (i > 0) divsorOss << "x";
        if (i > 1) divsorOss << "^" << i;
        divsorOss << " ";
    }
    
    PolynomialStep step3;
    step3.description = divsorOss.str();
    step3.expression = "";
    steps.push_back(step3);
    
    if (divisor.size() > dividend.size()) {
        PolynomialStep error;
        error.description = "Error:";
        error.expression = "Divisor degree is greater than dividend degree";
        steps.push_back(error);
        return;
    }
    
    std::vector<double> remainder = dividend;
    std::vector<double> quotient;
    
    while (remainder.size() >= divisor.size() && remainder[remainder.size() - 1] != 0) {
        double coeff = remainder[remainder.size() - 1] / divisor[divisor.size() - 1];
        quotient.push_back(coeff);
        
        for (size_t i = 0; i < divisor.size(); i++) {
            remainder[remainder.size() - 1 - i] -= coeff * divisor[divisor.size() - 1 - i];
        }
        remainder.pop_back();
    }
    
    // Display quotient
    std::ostringstream quotOss;
    quotOss << "Quotient: ";
    if (quotient.empty()) {
        quotOss << "0";
    } else {
        for (int i = quotient.size() - 1; i >= 0; i--) {
            if (i < quotient.size() - 1 && quotient[quotient.size() - 1 - i] >= 0) quotOss << "+ ";
            quotOss << quotient[quotient.size() - 1 - i];
            if (i > 0) quotOss << "x";
            if (i > 1) quotOss << "^" << i;
            quotOss << " ";
        }
    }
    
    PolynomialStep step4;
    step4.description = "=== Result ===";
    step4.expression = quotOss.str();
    steps.push_back(step4);
    
    // Display remainder
    std::ostringstream remOss;
    remOss << "Remainder: ";
    if (remainder.empty() || (remainder.size() == 1 && remainder[0] == 0)) {
        remOss << "0";
    } else {
        for (int i = remainder.size() - 1; i >= 0; i--) {
            if (i < remainder.size() - 1 && remainder[i] >= 0) remOss << "+ ";
            remOss << remainder[i];
            if (i > 0) remOss << "x";
            if (i > 1) remOss << "^" << i;
            remOss << " ";
        }
    }
    
    PolynomialStep step5;
    step5.description = remOss.str();
    step5.expression = "";
    steps.push_back(step5);
}

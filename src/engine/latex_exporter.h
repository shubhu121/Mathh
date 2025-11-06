#ifndef LATEX_EXPORTER_H
#define LATEX_EXPORTER_H

#include <string>
#include <vector>
#include <fstream>
#include "ast.h"
#include "differentiator.h"
#include "integrator.h"
#include "limit_calculator.h"
#include "matrix_operations.h"

class LaTeXExporter {
public:
    LaTeXExporter();
    
    // Convert AST expression to LaTeX
    std::string astToLaTeX(const ASTNode* node) const;
    
    // Export differentiation steps to LaTeX file
    bool exportDifferentiation(
        const std::string& expression,
        const std::vector<DifferentiationStep>& steps,
        const ASTNode* result,
        const std::string& filename
    );
    
    // Export indefinite integration steps to LaTeX file
    bool exportIndefiniteIntegration(
        const std::string& expression,
        const std::vector<IntegrationStep>& steps,
        const ASTNode* result,
        const std::string& filename
    );
    
    // Export definite integration steps to LaTeX file
    bool exportDefiniteIntegration(
        const std::string& expression,
        const std::vector<IntegrationStep>& steps,
        double result,
        double lowerBound,
        double upperBound,
        const std::string& filename
    );
    
    // Export limit calculation steps to LaTeX file
    bool exportLimit(
        const std::string& expression,
        const std::vector<LimitStep>& steps,
        double result,
        double limitPoint,
        LimitType limitType,
        const std::string& filename
    );
    
    // Export matrix multiplication steps to LaTeX file
    bool exportMatrixMultiplication(
        const Matrix& matrixA,
        const Matrix& matrixB,
        const Matrix& result,
        const std::vector<MatrixStep>& steps,
        const std::string& filename
    );
    
    // Generate complete LaTeX document with steps
    std::string generateLaTeXDocument(
        const std::string& title,
        const std::string& content
    ) const;
    
    // Compile LaTeX to PDF (requires pdflatex)
    bool compileToPDF(const std::string& texFilename);
    
private:
    std::string escapeLaTeX(const std::string& text) const;
    std::string formatOperator(const std::string& op) const;
    std::string matrixToLaTeX(const Matrix& matrix) const;
    std::string limitTypeToLaTeX(double point, LimitType type) const;
};

#endif // LATEX_EXPORTER_H

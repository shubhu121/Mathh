#pragma once
#include <vector>
#include <string>

struct MatrixStep {
    std::string description;
    std::string expression;
};

class Matrix {
public:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;
    
    Matrix(int r, int c);
    Matrix(const std::vector<std::vector<double>>& values);
    
    std::string toString() const;
    void set(int row, int col, double value);
    double get(int row, int col) const;
};

class MatrixOperations {
private:
    std::vector<MatrixStep> steps;
    
public:
    Matrix multiply(const Matrix& A, const Matrix& B);
    const std::vector<MatrixStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
    
    // Validation
    bool canMultiply(const Matrix& A, const Matrix& B) const;
    std::string getErrorMessage(const Matrix& A, const Matrix& B) const;
};

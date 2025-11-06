#include "matrix_operations.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    if (r <= 0 || c <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    data.resize(r, std::vector<double>(c, 0.0));
}

Matrix::Matrix(const std::vector<std::vector<double>>& values) {
    if (values.empty() || values[0].empty()) {
        throw std::invalid_argument("Matrix cannot be empty");
    }
    
    rows = values.size();
    cols = values[0].size();
    
    // Validate all rows have same length
    for (const auto& row : values) {
        if (row.size() != cols) {
            throw std::invalid_argument("All rows must have the same number of columns");
        }
    }
    
    data = values;
}

void Matrix::set(int row, int col, double value) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    data[row][col] = value;
}

double Matrix::get(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return data[row][col];
}

std::string Matrix::toString() const {
    std::ostringstream oss;
    
    for (int i = 0; i < rows; i++) {
        oss << "[ ";
        for (int j = 0; j < cols; j++) {
            oss << std::setw(8) << std::fixed << std::setprecision(2) << data[i][j];
            if (j < cols - 1) oss << " ";
        }
        oss << " ]";
        if (i < rows - 1) oss << "\n";
    }
    
    return oss.str();
}

bool MatrixOperations::canMultiply(const Matrix& A, const Matrix& B) const {
    return A.cols == B.rows;
}

std::string MatrixOperations::getErrorMessage(const Matrix& A, const Matrix& B) const {
    if (!canMultiply(A, B)) {
        std::ostringstream oss;
        oss << "Cannot multiply: A(" << A.rows << "×" << A.cols << ") × B(" 
            << B.rows << "×" << B.cols << ")\n";
        oss << "Number of columns in A (" << A.cols << ") must equal number of rows in B (" << B.rows << ")";
        return oss.str();
    }
    return "";
}

Matrix MatrixOperations::multiply(const Matrix& A, const Matrix& B) {
    steps.clear();
    
    // Step 1: Show dimensions
    std::ostringstream dimOss;
    dimOss << "Matrix A: " << A.rows << "×" << A.cols << ", Matrix B: " << B.rows << "×" << B.cols;
    steps.push_back({"Matrix Dimensions", dimOss.str()});
    
    // Step 2: Validate multiplication
    if (!canMultiply(A, B)) {
        std::string error = getErrorMessage(A, B);
        steps.push_back({"Error - Invalid Dimensions", error});
        throw std::invalid_argument(error);
    }
    
    steps.push_back({"Validation", "✓ Multiplication is valid (A.cols = B.rows)"});
    
    // Step 3: Show result dimensions
    std::ostringstream resultDimOss;
    resultDimOss << "Result will be " << A.rows << "×" << B.cols;
    steps.push_back({"Result Dimensions", resultDimOss.str()});
    
    // Step 4: Create result matrix
    Matrix result(A.rows, B.cols);
    
    // Step 5: Show the formula
    steps.push_back({"Formula", "C[i][j] = Σ(k=0 to n-1) A[i][k] × B[k][j]"});
    
    // Step 6: Perform multiplication with detailed steps (show first few calculations)
    int stepCount = 0;
    int maxStepsToShow = 6; // Limit detailed steps for large matrices
    
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {
            double sum = 0.0;
            std::ostringstream calcOss;
            
            if (stepCount < maxStepsToShow) {
                calcOss << "C[" << i << "][" << j << "] = ";
                
                for (int k = 0; k < A.cols; k++) {
                    double aVal = A.get(i, k);
                    double bVal = B.get(k, j);
                    sum += aVal * bVal;
                    
                    if (k > 0) calcOss << " + ";
                    calcOss << "(" << std::fixed << std::setprecision(2) << aVal 
                           << " × " << bVal << ")";
                }
                
                calcOss << " = " << std::fixed << std::setprecision(2) << sum;
                steps.push_back({"Computing element [" + std::to_string(i) + "][" + std::to_string(j) + "]", calcOss.str()});
            } else {
                // Just compute without showing steps for large matrices
                for (int k = 0; k < A.cols; k++) {
                    sum += A.get(i, k) * B.get(k, j);
                }
            }
            
            result.set(i, j, sum);
            stepCount++;
        }
    }
    
    if (stepCount > maxStepsToShow) {
        steps.push_back({"Note", "Remaining " + std::to_string(stepCount - maxStepsToShow) + " calculations omitted for brevity"});
    }
    
    // Step 7: Show final result
    steps.push_back({"Final Result Matrix", result.toString()});
    
    return result;
}

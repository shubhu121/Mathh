#include "latex_exporter.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

LaTeXExporter::LaTeXExporter() {}

std::string LaTeXExporter::escapeLaTeX(const std::string& text) const {
    std::string result;
    for (char c : text) {
        switch (c) {
            case '&': result += "\\&"; break;
            case '%': result += "\\%"; break;
            case '$': result += "\\$"; break;
            case '#': result += "\\#"; break;
            case '_': result += "\\_"; break;
            case '{': result += "\\{"; break;
            case '}': result += "\\}"; break;
            case '~': result += "\\textasciitilde{}"; break;
            case '^': result += "\\textasciicircum{}"; break;
            case '\\': result += "\\textbackslash{}"; break;
            default: result += c;
        }
    }
    return result;
}

std::string LaTeXExporter::formatOperator(const std::string& op) const {
    if (op == "sin") return "\\sin";
    if (op == "cos") return "\\cos";
    if (op == "tan") return "\\tan";
    if (op == "ln") return "\\ln";
    if (op == "log") return "\\log";
    if (op == "exp") return "\\exp";
    if (op == "sqrt") return "\\sqrt";
    return op;
}

std::string LaTeXExporter::astToLaTeX(const ASTNode* node) const {
    if (!node) return "";
    
    std::ostringstream latex;
    
    switch (node->type) {
        case NodeType::NUMBER: {
            auto* numNode = dynamic_cast<const NumberNode*>(node);
            if (numNode) {
                latex << std::fixed << std::setprecision(4) << numNode->value;
            }
            break;
        }
            
        case NodeType::VARIABLE: {
            auto* varNode = dynamic_cast<const VariableNode*>(node);
            if (varNode) {
                latex << varNode->name;
            }
            break;
        }
            
        case NodeType::BINARY_OP: {
            auto* binNode = dynamic_cast<const BinaryOpNode*>(node);
            if (binNode) {
                if (binNode->op == BinaryOp::ADD) {
                    latex << astToLaTeX(binNode->left.get()) << " + " << astToLaTeX(binNode->right.get());
                } else if (binNode->op == BinaryOp::SUB) {
                    latex << astToLaTeX(binNode->left.get()) << " - " << astToLaTeX(binNode->right.get());
                } else if (binNode->op == BinaryOp::MUL) {
                    latex << astToLaTeX(binNode->left.get()) << " \\cdot " << astToLaTeX(binNode->right.get());
                } else if (binNode->op == BinaryOp::DIV) {
                    latex << "\\frac{" << astToLaTeX(binNode->left.get()) << "}{" << astToLaTeX(binNode->right.get()) << "}";
                } else if (binNode->op == BinaryOp::POW) {
                    latex << "{" << astToLaTeX(binNode->left.get()) << "}^{" << astToLaTeX(binNode->right.get()) << "}";
                }
            }
            break;
        }
            
        case NodeType::UNARY_FUNC: {
            auto* funcNode = dynamic_cast<const UnaryFuncNode*>(node);
            if (funcNode) {
                if (funcNode->func == UnaryFunc::SIN) {
                    latex << "\\sin\\left(" << astToLaTeX(funcNode->arg.get()) << "\\right)";
                } else if (funcNode->func == UnaryFunc::COS) {
                    latex << "\\cos\\left(" << astToLaTeX(funcNode->arg.get()) << "\\right)";
                } else if (funcNode->func == UnaryFunc::TAN) {
                    latex << "\\tan\\left(" << astToLaTeX(funcNode->arg.get()) << "\\right)";
                } else if (funcNode->func == UnaryFunc::LN) {
                    latex << "\\ln\\left(" << astToLaTeX(funcNode->arg.get()) << "\\right)";
                } else if (funcNode->func == UnaryFunc::EXP) {
                    latex << "e^{" << astToLaTeX(funcNode->arg.get()) << "}";
                } else if (funcNode->func == UnaryFunc::SQRT) {
                    latex << "\\sqrt{" << astToLaTeX(funcNode->arg.get()) << "}";
                }
            }
            break;
        }
    }
    
    return latex.str();
}

std::string LaTeXExporter::matrixToLaTeX(const Matrix& matrix) const {
    std::ostringstream latex;
    latex << "\\begin{bmatrix}\n";
    
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            latex << std::fixed << std::setprecision(2) << matrix.get(i, j);
            if (j < matrix.cols - 1) {
                latex << " & ";
            }
        }
        if (i < matrix.rows - 1) {
            latex << " \\\\\n";
        } else {
            latex << "\n";
        }
    }
    
    latex << "\\end{bmatrix}";
    return latex.str();
}

std::string LaTeXExporter::limitTypeToLaTeX(double point, LimitType type) const {
    std::ostringstream latex;
    latex << "x \\to ";
    
    switch (type) {
        case LimitType::FINITE:
            latex << std::fixed << std::setprecision(2) << point;
            break;
        case LimitType::POSITIVE_INFINITY:
            latex << "+\\infty";
            break;
        case LimitType::NEGATIVE_INFINITY:
            latex << "-\\infty";
            break;
    }
    
    return latex.str();
}

std::string LaTeXExporter::generateLaTeXDocument(const std::string& title, const std::string& content) const {
    std::ostringstream doc;
    
    doc << "\\documentclass[12pt,a4paper]{article}\n";
    doc << "\\usepackage{amsmath}\n";
    doc << "\\usepackage{amssymb}\n";
    doc << "\\usepackage{geometry}\n";
    doc << "\\geometry{margin=1in}\n";
    doc << "\\usepackage{enumitem}\n";
    doc << "\\usepackage{xcolor}\n";
    doc << "\n";
    doc << "\\title{" << escapeLaTeX(title) << "}\n";
    doc << "\\author{Mathematics Engine}\n";
    doc << "\\date{\\today}\n";
    doc << "\n";
    doc << "\\begin{document}\n";
    doc << "\\maketitle\n";
    doc << "\n";
    doc << content;
    doc << "\n";
    doc << "\\end{document}\n";
    
    return doc.str();
}

bool LaTeXExporter::exportDifferentiation(
    const std::string& expression,
    const std::vector<DifferentiationStep>& steps,
    const ASTNode* result,
    const std::string& filename
) {
    std::ostringstream content;
    
    content << "\\section*{Differentiation}\n\n";
    content << "\\textbf{Problem:} Find the derivative of:\n";
    content << "\\[ f(x) = " << expression << " \\]\n\n";
    content << "\\textbf{Solution:}\n\n";
    content << "\\begin{enumerate}\n";
    
    for (size_t i = 0; i < steps.size(); i++) {
        const auto& step = steps[i];
        content << "\\item " << escapeLaTeX(step.description) << "\n";
        content << "\\[ " << step.expression << " \\]\n\n";
    }
    
    content << "\\end{enumerate}\n\n";
    content << "\\textbf{Final Answer:}\n";
    content << "\\[ \\frac{d}{dx}\\left[" << expression << "\\right] = " << astToLaTeX(result) << " \\]\n";
    
    std::string doc = generateLaTeXDocument("Differentiation Solution", content.str());
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << doc;
    file.close();
    return true;
}

bool LaTeXExporter::exportIndefiniteIntegration(
    const std::string& expression,
    const std::vector<IntegrationStep>& steps,
    const ASTNode* result,
    const std::string& filename
) {
    std::ostringstream content;
    
    content << "\\section*{Indefinite Integration}\n\n";
    content << "\\textbf{Problem:} Find the indefinite integral:\n";
    content << "\\[ \\int " << expression << " \\, dx \\]\n\n";
    content << "\\textbf{Solution:}\n\n";
    content << "\\begin{enumerate}\n";
    
    for (size_t i = 0; i < steps.size(); i++) {
        const auto& step = steps[i];
        content << "\\item " << escapeLaTeX(step.description) << "\n";
        content << "\\[ " << step.expression << " \\]\n\n";
    }
    
    content << "\\end{enumerate}\n\n";
    content << "\\textbf{Final Answer:}\n";
    content << "\\[ \\int " << expression << " \\, dx = " << astToLaTeX(result) << " + C \\]\n";
    
    std::string doc = generateLaTeXDocument("Integration Solution", content.str());
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << doc;
    file.close();
    return true;
}

bool LaTeXExporter::exportDefiniteIntegration(
    const std::string& expression,
    const std::vector<IntegrationStep>& steps,
    double result,
    double lowerBound,
    double upperBound,
    const std::string& filename
) {
    std::ostringstream content;
    
    content << "\\section*{Definite Integration}\n\n";
    content << "\\textbf{Problem:} Evaluate the definite integral:\n";
    content << "\\[ \\int_{" << std::fixed << std::setprecision(2) << lowerBound 
            << "}^{" << upperBound << "} " << expression << " \\, dx \\]\n\n";
    content << "\\textbf{Solution:}\n\n";
    content << "\\begin{enumerate}\n";
    
    for (size_t i = 0; i < steps.size(); i++) {
        const auto& step = steps[i];
        content << "\\item " << escapeLaTeX(step.description) << "\n";
        content << "\\[ " << step.expression << " \\]\n\n";
    }
    
    content << "\\end{enumerate}\n\n";
    content << "\\textbf{Final Answer:}\n";
    content << "\\[ \\int_{" << std::fixed << std::setprecision(2) << lowerBound 
            << "}^{" << upperBound << "} " << expression << " \\, dx = " 
            << std::fixed << std::setprecision(6) << result << " \\]\n";
    
    std::string doc = generateLaTeXDocument("Definite Integration Solution", content.str());
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << doc;
    file.close();
    return true;
}

bool LaTeXExporter::exportLimit(
    const std::string& expression,
    const std::vector<LimitStep>& steps,
    double result,
    double limitPoint,
    LimitType limitType,
    const std::string& filename
) {
    std::ostringstream content;
    
    content << "\\section*{Limit Calculation}\n\n";
    content << "\\textbf{Problem:} Find the limit:\n";
    content << "\\[ \\lim_{" << limitTypeToLaTeX(limitPoint, limitType) << "} " 
            << expression << " \\]\n\n";
    content << "\\textbf{Solution:}\n\n";
    content << "\\begin{enumerate}\n";
    
    for (size_t i = 0; i < steps.size(); i++) {
        const auto& step = steps[i];
        content << "\\item " << escapeLaTeX(step.description) << "\n";
        content << "\\[ " << step.expression << " \\]\n\n";
    }
    
    content << "\\end{enumerate}\n\n";
    content << "\\textbf{Final Answer:}\n";
    
    std::string resultStr;
    if (std::isnan(result)) {
        resultStr = "\\text{undefined}";
    } else if (std::isinf(result)) {
        resultStr = (result > 0) ? "+\\infty" : "-\\infty";
    } else {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << result;
        resultStr = oss.str();
    }
    
    content << "\\[ \\lim_{" << limitTypeToLaTeX(limitPoint, limitType) << "} " 
            << expression << " = " << resultStr << " \\]\n";
    
    std::string doc = generateLaTeXDocument("Limit Calculation Solution", content.str());
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << doc;
    file.close();
    return true;
}

bool LaTeXExporter::exportMatrixMultiplication(
    const Matrix& matrixA,
    const Matrix& matrixB,
    const Matrix& result,
    const std::vector<MatrixStep>& steps,
    const std::string& filename
) {
    std::ostringstream content;
    
    content << "\\section*{Matrix Multiplication}\n\n";
    content << "\\textbf{Problem:} Multiply matrices:\n\n";
    content << "\\[ A = " << matrixToLaTeX(matrixA) << " \\quad B = " << matrixToLaTeX(matrixB) << " \\]\n\n";
    content << "Find $C = A \\times B$\n\n";
    content << "\\textbf{Solution:}\n\n";
    content << "\\begin{enumerate}\n";
    
    for (size_t i = 0; i < steps.size(); i++) {
        const auto& step = steps[i];
        content << "\\item " << escapeLaTeX(step.description) << "\n";
        if (!step.expression.empty()) {
            content << "\\[ " << step.expression << " \\]\n";
        }
        content << "\n";
    }
    
    content << "\\end{enumerate}\n\n";
    content << "\\textbf{Final Answer:}\n";
    content << "\\[ C = " << matrixToLaTeX(result) << " \\]\n";
    
    std::string doc = generateLaTeXDocument("Matrix Multiplication Solution", content.str());
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << doc;
    file.close();
    return true;
}

bool LaTeXExporter::compileToPDF(const std::string& texFilename) {
    // Build the command to compile LaTeX to PDF
    // This requires pdflatex to be installed and in PATH
    std::string command = "pdflatex -interaction=nonstopmode \"" + texFilename + "\"";
    
    int result = system(command.c_str());
    
    // Run twice for proper references
    if (result == 0) {
        system(command.c_str());
    }
    
    return result == 0;
}

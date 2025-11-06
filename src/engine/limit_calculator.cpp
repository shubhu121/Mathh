#include "limit_calculator.h"
#include "differentiator.h"
#include <cmath>
#include <sstream>
#include <iomanip>

std::string LimitCalculator::limitTypeToString(double point, LimitType type) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    switch (type) {
        case LimitType::FINITE:
            oss << "x → " << point;
            break;
        case LimitType::POSITIVE_INFINITY:
            oss << "x → +∞";
            break;
        case LimitType::NEGATIVE_INFINITY:
            oss << "x → -∞";
            break;
    }
    return oss.str();
}

bool LimitCalculator::isIndeterminate(double numerator, double denominator) {
    bool numIsNan = std::isnan(numerator);
    bool denIsNan = std::isnan(denominator);
    bool numIsInf = std::isinf(numerator);
    bool denIsInf = std::isinf(denominator);
    bool numIsZero = std::abs(numerator) < 1e-10;
    bool denIsZero = std::abs(denominator) < 1e-10;
    
    // 0/0 form
    if (numIsZero && denIsZero) return true;
    
    // ∞/∞ form
    if (numIsInf && denIsInf) return true;
    
    // NaN cases
    if (numIsNan || denIsNan) return true;
    
    return false;
}

double LimitCalculator::applyLHopital(const ASTNode* node, double point, LimitType type, int depth) {
    if (depth > 3) {
        steps.push_back({"Maximum L'Hôpital iterations reached", "Limit may not exist or requires advanced techniques"});
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    // Check if it's a fraction (division)
    if (node->type != NodeType::BINARY_OP) {
        return evaluateLimit(node, point, type);
    }
    
    const BinaryOpNode* binOp = static_cast<const BinaryOpNode*>(node);
    if (binOp->op != BinaryOp::DIV) {
        return evaluateLimit(node, point, type);
    }
    
    // Apply L'Hôpital's rule
    std::ostringstream oss;
    oss << "Applying L'Hôpital's rule (iteration " << (depth + 1) << ")";
    steps.push_back({oss.str(), "Differentiate numerator and denominator separately"});
    
    // Differentiate numerator and denominator
    Differentiator diff;
    auto numDerivative = diff.differentiate(binOp->left.get());
    auto denDerivative = diff.differentiate(binOp->right.get());
    
    std::string numDerStr = numDerivative->toString();
    std::string denDerStr = denDerivative->toString();
    
    steps.push_back({"After differentiation", "(" + numDerStr + ") / (" + denDerStr + ")"});
    
    // Evaluate the derivatives
    double numValue, denValue;
    
    if (type == LimitType::FINITE) {
        numValue = numDerivative->evaluate(point);
        denValue = denDerivative->evaluate(point);
    } else if (type == LimitType::POSITIVE_INFINITY) {
        numValue = numDerivative->evaluate(1e6);
        denValue = denDerivative->evaluate(1e6);
    } else { // NEGATIVE_INFINITY
        numValue = numDerivative->evaluate(-1e6);
        denValue = denDerivative->evaluate(-1e6);
    }
    
    // Check if still indeterminate
    if (isIndeterminate(numValue, denValue)) {
        steps.push_back({"Still indeterminate form", "Applying L'Hôpital's rule again"});
        
        // Create new division node and recurse
        auto newNode = std::make_unique<BinaryOpNode>(
            BinaryOp::DIV,
            std::move(numDerivative),
            std::move(denDerivative)
        );
        
        return applyLHopital(newNode.get(), point, type, depth + 1);
    }
    
    std::ostringstream resultOss;
    resultOss << std::fixed << std::setprecision(4);
    resultOss << numValue << " / " << denValue << " = ";
    
    if (std::abs(denValue) < 1e-10) {
        if (numValue > 0) {
            resultOss << "+∞";
            return std::numeric_limits<double>::infinity();
        } else if (numValue < 0) {
            resultOss << "-∞";
            return -std::numeric_limits<double>::infinity();
        } else {
            resultOss << "undefined";
            return std::numeric_limits<double>::quiet_NaN();
        }
    }
    
    double result = numValue / denValue;
    resultOss << result;
    steps.push_back({"L'Hôpital result", resultOss.str()});
    
    return result;
}

double LimitCalculator::evaluateLimit(const ASTNode* node, double point, LimitType type) {
    double testValue;
    
    switch (type) {
        case LimitType::FINITE:
            testValue = point;
            break;
        case LimitType::POSITIVE_INFINITY:
            testValue = 1e6; // Large positive value
            break;
        case LimitType::NEGATIVE_INFINITY:
            testValue = -1e6; // Large negative value
            break;
    }
    
    try {
        return node->evaluate(testValue);
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double LimitCalculator::calculateLimit(const ASTNode* root, double point, LimitType type) {
    steps.clear();
    
    if (!root) {
        steps.push_back({"Error", "Invalid expression"});
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    // Step 1: Show the limit expression
    std::ostringstream oss;
    oss << "lim [" << limitTypeToString(point, type) << "] (" << root->toString() << ")";
    steps.push_back({"Evaluating limit", oss.str()});
    
    // Step 2: Try direct substitution
    double directResult;
    
    if (type == LimitType::FINITE) {
        steps.push_back({"Direct substitution", "Substitute x = " + std::to_string(point)});
        directResult = root->evaluate(point);
    } else if (type == LimitType::POSITIVE_INFINITY) {
        steps.push_back({"Approaching infinity", "Evaluate as x → +∞"});
        directResult = root->evaluate(1e6);
    } else {
        steps.push_back({"Approaching negative infinity", "Evaluate as x → -∞"});
        directResult = root->evaluate(-1e6);
    }
    
    // Step 3: Check if result is valid
    if (std::isnan(directResult)) {
        steps.push_back({"Result", "Indeterminate form (NaN)"});
        
        // Try L'Hôpital's rule if it's a division
        if (root->type == NodeType::BINARY_OP) {
            const BinaryOpNode* binOp = static_cast<const BinaryOpNode*>(root);
            if (binOp->op == BinaryOp::DIV) {
                double numValue, denValue;
                
                if (type == LimitType::FINITE) {
                    numValue = binOp->left->evaluate(point);
                    denValue = binOp->right->evaluate(point);
                } else if (type == LimitType::POSITIVE_INFINITY) {
                    numValue = binOp->left->evaluate(1e6);
                    denValue = binOp->right->evaluate(1e6);
                } else {
                    numValue = binOp->left->evaluate(-1e6);
                    denValue = binOp->right->evaluate(-1e6);
                }
                
                if (isIndeterminate(numValue, denValue)) {
                    steps.push_back({"Indeterminate form detected", "0/0 or ∞/∞ - applying L'Hôpital's rule"});
                    return applyLHopital(root, point, type, 0);
                }
            }
        }
        
        return std::numeric_limits<double>::quiet_NaN();
    } else if (std::isinf(directResult)) {
        steps.push_back({"Result", directResult > 0 ? "+∞" : "-∞"});
        return directResult;
    } else {
        std::ostringstream resultOss;
        resultOss << std::fixed << std::setprecision(6) << directResult;
        steps.push_back({"Result", resultOss.str()});
        return directResult;
    }
}

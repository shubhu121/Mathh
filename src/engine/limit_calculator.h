#pragma once
#include "ast.h"
#include <vector>
#include <string>
#include <limits>

struct LimitStep {
    std::string description;
    std::string expression;
};

enum class LimitType {
    FINITE,           // lim x->c
    POSITIVE_INFINITY, // lim x->+∞
    NEGATIVE_INFINITY  // lim x->-∞
};

class LimitCalculator {
private:
    std::vector<LimitStep> steps;
    
    double evaluateLimit(const ASTNode* node, double point, LimitType type);
    bool isIndeterminate(double numerator, double denominator);
    double applyLHopital(const ASTNode* node, double point, LimitType type, int depth);
    
public:
    double calculateLimit(const ASTNode* root, double point, LimitType type);
    const std::vector<LimitStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
    
    // Helper to format limit type as string
    std::string limitTypeToString(double point, LimitType type) const;
};

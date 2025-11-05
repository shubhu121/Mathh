#pragma once
#include "ast.h"
#include <vector>

class Differentiator {
private:
    std::vector<DifferentiationStep> steps;
    
    std::unique_ptr<ASTNode> differentiateNode(const ASTNode* node);
    std::unique_ptr<ASTNode> applyChainRule(const ASTNode* node);
    
public:
    std::unique_ptr<ASTNode> differentiate(const ASTNode* root);
    const std::vector<DifferentiationStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
};

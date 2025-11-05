#pragma once
#include "ast.h"
#include <vector>

struct IntegrationStep {
    std::string description;
    std::string expression;
};

class Integrator {
private:
    std::vector<IntegrationStep> steps;
    
    std::unique_ptr<ASTNode> integrateNode(const ASTNode* node);
    
public:
    std::unique_ptr<ASTNode> integrate(const ASTNode* root);
    const std::vector<IntegrationStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); }
    
    // Definite integration: evaluate integral from a to b
    double evaluateDefinite(const ASTNode* root, double a, double b);
};

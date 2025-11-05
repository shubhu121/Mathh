#include "integrator.h"
#include <cmath>

std::unique_ptr<ASTNode> Integrator::integrate(const ASTNode* root) {
    steps.clear();
    
    IntegrationStep initialStep;
    initialStep.description = "Initial expression";
    initialStep.expression = "∫ " + root->toString() + " dx";
    steps.push_back(initialStep);
    
    auto result = integrateNode(root);
    
    IntegrationStep finalStep;
    finalStep.description = "Final integral (+ C for indefinite)";
    finalStep.expression = "∫ f(x) dx = " + result->toString() + " + C";
    steps.push_back(finalStep);
    
    return result;
}

std::unique_ptr<ASTNode> Integrator::integrateNode(const ASTNode* node) {
    switch (node->type) {
        case NodeType::NUMBER: {
            auto numNode = static_cast<const NumberNode*>(node);
            
            IntegrationStep step;
            step.description = "Constant Rule: ∫ c dx = c·x";
            step.expression = "∫ " + std::to_string((int)numNode->value) + " dx = " + 
                             std::to_string((int)numNode->value) + "·x";
            steps.push_back(step);
            
            // c * x
            return std::make_unique<BinaryOpNode>(
                BinaryOp::MUL,
                std::make_unique<NumberNode>(numNode->value),
                std::make_unique<VariableNode>("x")
            );
        }
        
        case NodeType::VARIABLE: {
            IntegrationStep step;
            step.description = "Power Rule: ∫ x dx = x²/2";
            step.expression = "∫ x dx = x²/2";
            steps.push_back(step);
            
            // x^2 / 2
            auto x2 = std::make_unique<BinaryOpNode>(
                BinaryOp::POW,
                std::make_unique<VariableNode>("x"),
                std::make_unique<NumberNode>(2)
            );
            
            return std::make_unique<BinaryOpNode>(
                BinaryOp::DIV,
                std::move(x2),
                std::make_unique<NumberNode>(2)
            );
        }
        
        case NodeType::BINARY_OP: {
            auto binOp = static_cast<const BinaryOpNode*>(node);
            
            switch (binOp->op) {
                case BinaryOp::ADD: {
                    IntegrationStep step;
                    step.description = "Sum Rule: ∫ (f + g) dx = ∫ f dx + ∫ g dx";
                    step.expression = "∫ (" + binOp->left->toString() + " + " + binOp->right->toString() + ") dx";
                    steps.push_back(step);
                    
                    auto leftIntegral = integrateNode(binOp->left.get());
                    auto rightIntegral = integrateNode(binOp->right.get());
                    
                    return std::make_unique<BinaryOpNode>(
                        BinaryOp::ADD,
                        std::move(leftIntegral),
                        std::move(rightIntegral)
                    );
                }
                
                case BinaryOp::SUB: {
                    IntegrationStep step;
                    step.description = "Difference Rule: ∫ (f - g) dx = ∫ f dx - ∫ g dx";
                    step.expression = "∫ (" + binOp->left->toString() + " - " + binOp->right->toString() + ") dx";
                    steps.push_back(step);
                    
                    auto leftIntegral = integrateNode(binOp->left.get());
                    auto rightIntegral = integrateNode(binOp->right.get());
                    
                    return std::make_unique<BinaryOpNode>(
                        BinaryOp::SUB,
                        std::move(leftIntegral),
                        std::move(rightIntegral)
                    );
                }
                
                case BinaryOp::MUL: {
                    // Check if one operand is constant
                    if (binOp->left->type == NodeType::NUMBER) {
                        auto numNode = static_cast<const NumberNode*>(binOp->left.get());
                        
                        IntegrationStep step;
                        step.description = "Constant Multiple Rule: ∫ c·f(x) dx = c·∫ f(x) dx";
                        step.expression = "∫ " + std::to_string((int)numNode->value) + "·" + 
                                        binOp->right->toString() + " dx";
                        steps.push_back(step);
                        
                        auto integral = integrateNode(binOp->right.get());
                        return std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::make_unique<NumberNode>(numNode->value),
                            std::move(integral)
                        );
                    }
                    else if (binOp->right->type == NodeType::NUMBER) {
                        auto numNode = static_cast<const NumberNode*>(binOp->right.get());
                        
                        IntegrationStep step;
                        step.description = "Constant Multiple Rule: ∫ f(x)·c dx = c·∫ f(x) dx";
                        step.expression = "∫ " + binOp->left->toString() + "·" + 
                                        std::to_string((int)numNode->value) + " dx";
                        steps.push_back(step);
                        
                        auto integral = integrateNode(binOp->left.get());
                        return std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::make_unique<NumberNode>(numNode->value),
                            std::move(integral)
                        );
                    }
                    
                    // General case - not supported for now
                    IntegrationStep step;
                    step.description = "Product integration (advanced - using numerical approximation)";
                    step.expression = "∫ " + node->toString() + " dx ≈ (complex)";
                    steps.push_back(step);
                    
                    return node->clone();
                }
                
                case BinaryOp::POW: {
                    // Check if it's x^n where n is constant
                    if (binOp->left->type == NodeType::VARIABLE && 
                        binOp->right->type == NodeType::NUMBER) {
                        
                        auto numNode = static_cast<const NumberNode*>(binOp->right.get());
                        double n = numNode->value;
                        
                        if (n == -1) {
                            IntegrationStep step;
                            step.description = "Special case: ∫ x⁻¹ dx = ln|x|";
                            step.expression = "∫ x⁻¹ dx = ln|x|";
                            steps.push_back(step);
                            
                            return std::make_unique<UnaryFuncNode>(
                                UnaryFunc::LN,
                                std::make_unique<VariableNode>("x")
                            );
                        }
                        else {
                            IntegrationStep step;
                            step.description = "Power Rule: ∫ xⁿ dx = xⁿ⁺¹/(n+1)";
                            step.expression = "∫ x^" + std::to_string((int)n) + " dx = x^" + 
                                            std::to_string((int)(n+1)) + "/" + std::to_string((int)(n+1));
                            steps.push_back(step);
                            
                            // x^(n+1) / (n+1)
                            auto newPower = std::make_unique<BinaryOpNode>(
                                BinaryOp::POW,
                                std::make_unique<VariableNode>("x"),
                                std::make_unique<NumberNode>(n + 1)
                            );
                            
                            return std::make_unique<BinaryOpNode>(
                                BinaryOp::DIV,
                                std::move(newPower),
                                std::make_unique<NumberNode>(n + 1)
                            );
                        }
                    }
                    break;
                }
                
                default:
                    break;
            }
            break;
        }
        
        case NodeType::UNARY_FUNC: {
            auto funcNode = static_cast<const UnaryFuncNode*>(node);
            
            // Only handle simple cases where argument is x
            if (funcNode->arg->type == NodeType::VARIABLE) {
                switch (funcNode->func) {
                    case UnaryFunc::SIN: {
                        IntegrationStep step;
                        step.description = "Trig Rule: ∫ sin(x) dx = -cos(x)";
                        step.expression = "∫ sin(x) dx = -cos(x)";
                        steps.push_back(step);
                        
                        auto cosNode = std::make_unique<UnaryFuncNode>(
                            UnaryFunc::COS,
                            std::make_unique<VariableNode>("x")
                        );
                        
                        return std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::make_unique<NumberNode>(-1),
                            std::move(cosNode)
                        );
                    }
                    
                    case UnaryFunc::COS: {
                        IntegrationStep step;
                        step.description = "Trig Rule: ∫ cos(x) dx = sin(x)";
                        step.expression = "∫ cos(x) dx = sin(x)";
                        steps.push_back(step);
                        
                        return std::make_unique<UnaryFuncNode>(
                            UnaryFunc::SIN,
                            std::make_unique<VariableNode>("x")
                        );
                    }
                    
                    case UnaryFunc::EXP: {
                        IntegrationStep step;
                        step.description = "Exponential Rule: ∫ exp(x) dx = exp(x)";
                        step.expression = "∫ exp(x) dx = exp(x)";
                        steps.push_back(step);
                        
                        return std::make_unique<UnaryFuncNode>(
                            UnaryFunc::EXP,
                            std::make_unique<VariableNode>("x")
                        );
                    }
                    
                    default: {
                        IntegrationStep step;
                        step.description = "Advanced integration (not implemented)";
                        step.expression = "∫ " + node->toString() + " dx";
                        steps.push_back(step);
                        break;
                    }
                }
            }
            break;
        }
    }
    
    // Default: return original node (not integrated)
    return node->clone();
}

double Integrator::evaluateDefinite(const ASTNode* root, double a, double b) {
    // Get indefinite integral
    auto indefinite = integrate(root);
    
    // Evaluate at bounds: F(b) - F(a)
    double F_b = indefinite->evaluate(b);
    double F_a = indefinite->evaluate(a);
    
    IntegrationStep boundsStep;
    boundsStep.description = "Fundamental Theorem: ∫[a,b] f(x) dx = F(b) - F(a)";
    boundsStep.expression = "F(" + std::to_string(b) + ") - F(" + std::to_string(a) + ") = " + 
                           std::to_string(F_b) + " - " + std::to_string(F_a) + " = " + 
                           std::to_string(F_b - F_a);
    steps.push_back(boundsStep);
    
    return F_b - F_a;
}

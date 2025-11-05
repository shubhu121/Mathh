#include "differentiator.h"

std::unique_ptr<ASTNode> Differentiator::differentiate(const ASTNode* root) {
    steps.clear();
    
    DifferentiationStep initialStep;
    initialStep.description = "Initial expression";
    initialStep.expression = "∂/∂x(" + root->toString() + ")";
    steps.push_back(initialStep);
    
    auto result = differentiateNode(root);
    
    DifferentiationStep finalStep;
    finalStep.description = "Final derivative";
    finalStep.expression = "f'(x) = " + result->toString();
    steps.push_back(finalStep);
    
    return result;
}

std::unique_ptr<ASTNode> Differentiator::differentiateNode(const ASTNode* node) {
    switch (node->type) {
        case NodeType::NUMBER: {
            DifferentiationStep step;
            step.description = "Constant Rule: ∂/∂x(c) = 0";
            step.expression = "∂/∂x(" + node->toString() + ") = 0";
            steps.push_back(step);
            return std::make_unique<NumberNode>(0);
        }
        
        case NodeType::VARIABLE: {
            DifferentiationStep step;
            step.description = "Power Rule: ∂/∂x(x) = 1";
            step.expression = "∂/∂x(x) = 1";
            steps.push_back(step);
            return std::make_unique<NumberNode>(1);
        }
        
        case NodeType::BINARY_OP: {
            auto binOp = static_cast<const BinaryOpNode*>(node);
            
            switch (binOp->op) {
                case BinaryOp::ADD: {
                    DifferentiationStep step;
                    step.description = "Sum Rule: ∂/∂x(f + g) = f' + g'";
                    step.expression = "∂/∂x(" + binOp->left->toString() + " + " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    return std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(leftDeriv), std::move(rightDeriv));
                }
                
                case BinaryOp::SUB: {
                    DifferentiationStep step;
                    step.description = "Difference Rule: ∂/∂x(f - g) = f' - g'";
                    step.expression = "∂/∂x(" + binOp->left->toString() + " - " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    return std::make_unique<BinaryOpNode>(BinaryOp::SUB, std::move(leftDeriv), std::move(rightDeriv));
                }
                
                case BinaryOp::MUL: {
                    DifferentiationStep step;
                    step.description = "Product Rule: ∂/∂x(f ⋅ g) = f' ⋅ g + f ⋅ g'";
                    step.expression = "∂/∂x(" + binOp->left->toString() + " ⋅ " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    
                    // f' * g
                    auto term1 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::move(leftDeriv),
                        binOp->right->clone()
                    );
                    
                    // f * g'
                    auto term2 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        binOp->left->clone(),
                        std::move(rightDeriv)
                    );
                    
                    return std::make_unique<BinaryOpNode>(BinaryOp::ADD, std::move(term1), std::move(term2));
                }
                
                case BinaryOp::DIV: {
                    DifferentiationStep step;
                    step.description = "Quotient Rule: ∂/∂x(f/g) = (f' ⋅ g - f ⋅ g') / g²";
                    step.expression = "∂/∂x(" + binOp->left->toString() + " / " + binOp->right->toString() + ")";
                    steps.push_back(step);
                    
                    auto leftDeriv = differentiateNode(binOp->left.get());
                    auto rightDeriv = differentiateNode(binOp->right.get());
                    
                    // f' * g
                    auto term1 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::move(leftDeriv),
                        binOp->right->clone()
                    );
                    
                    // f * g'
                    auto term2 = std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        binOp->left->clone(),
                        std::move(rightDeriv)
                    );
                    
                    // f' * g - f * g'
                    auto numerator = std::make_unique<BinaryOpNode>(
                        BinaryOp::SUB,
                        std::move(term1),
                        std::move(term2)
                    );
                    
                    // g^2
                    auto denominator = std::make_unique<BinaryOpNode>(
                        BinaryOp::POW,
                        binOp->right->clone(),
                        std::make_unique<NumberNode>(2)
                    );
                    
                    return std::make_unique<BinaryOpNode>(BinaryOp::DIV, std::move(numerator), std::move(denominator));
                }
                
                case BinaryOp::POW: {
                    // Check if exponent is constant
                    if (binOp->right->type == NodeType::NUMBER) {
                        auto numNode = static_cast<const NumberNode*>(binOp->right.get());
                        
                        DifferentiationStep step;
                        step.description = "Power Rule: ∂/∂x(x^n) = n ⋅ x^(n-1)";
                        step.expression = "∂/∂x(" + binOp->left->toString() + "^" + std::to_string((int)numNode->value) + ")";
                        steps.push_back(step);
                        
                        // n * x^(n-1) * x'
                        auto coeff = std::make_unique<NumberNode>(numNode->value);
                        auto newPower = std::make_unique<NumberNode>(numNode->value - 1);
                        auto baseDeriv = differentiateNode(binOp->left.get());
                        
                        auto power = std::make_unique<BinaryOpNode>(
                            BinaryOp::POW,
                            binOp->left->clone(),
                            std::move(newPower)
                        );
                        
                        auto mult1 = std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::move(coeff),
                            std::move(power)
                        );
                        
                        return std::make_unique<BinaryOpNode>(
                            BinaryOp::MUL,
                            std::move(mult1),
                            std::move(baseDeriv)
                        );
                    }
                    // General case: a^b = exp(b * ln(a))
                    break;
                }
            }
            break;
        }
        
        case NodeType::UNARY_FUNC: {
            auto funcNode = static_cast<const UnaryFuncNode*>(node);
            auto innerDeriv = differentiateNode(funcNode->arg.get());
            
            switch (funcNode->func) {
                case UnaryFunc::SIN: {
                    DifferentiationStep step;
                    step.description = "Chain Rule: ∂/∂x(sin(u)) = cos(u) ⋅ u'";
                    step.expression = "∂/∂x(sin(" + funcNode->arg->toString() + ")) = cos(" + funcNode->arg->toString() + ") ⋅ ∂/∂x(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto cosNode = std::make_unique<UnaryFuncNode>(UnaryFunc::COS, funcNode->arg->clone());
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(cosNode), std::move(innerDeriv));
                }
                
                case UnaryFunc::COS: {
                    DifferentiationStep step;
                    step.description = "Chain Rule: ∂/∂x(cos(u)) = -sin(u) ⋅ u'";
                    step.expression = "∂/∂x(cos(" + funcNode->arg->toString() + ")) = -sin(" + funcNode->arg->toString() + ") ⋅ ∂/∂x(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto sinNode = std::make_unique<UnaryFuncNode>(UnaryFunc::SIN, funcNode->arg->clone());
                    auto negOne = std::make_unique<NumberNode>(-1);
                    auto negSin = std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(negOne), std::move(sinNode));
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(negSin), std::move(innerDeriv));
                }
                
                case UnaryFunc::TAN: {
                    DifferentiationStep step;
                    step.description = "Chain Rule: ∂/∂x(tan(u)) = sec²(u) ⋅ u'";
                    step.expression = "∂/∂x(tan(" + funcNode->arg->toString() + "))";
                    steps.push_back(step);
                    
                    // 1 / cos^2(u)
                    auto cosNode = std::make_unique<UnaryFuncNode>(UnaryFunc::COS, funcNode->arg->clone());
                    auto cos2 = std::make_unique<BinaryOpNode>(BinaryOp::POW, std::move(cosNode), std::make_unique<NumberNode>(2));
                    auto sec2 = std::make_unique<BinaryOpNode>(BinaryOp::DIV, std::make_unique<NumberNode>(1), std::move(cos2));
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(sec2), std::move(innerDeriv));
                }
                
                case UnaryFunc::LN: {
                    DifferentiationStep step;
                    step.description = "Chain Rule: ∂/∂x(ln(u)) = (1/u) ⋅ u'";
                    step.expression = "∂/∂x(ln(" + funcNode->arg->toString() + ")) = (1/" + funcNode->arg->toString() + ") ⋅ ∂/∂x(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto oneOverU = std::make_unique<BinaryOpNode>(
                        BinaryOp::DIV,
                        std::make_unique<NumberNode>(1),
                        funcNode->arg->clone()
                    );
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(oneOverU), std::move(innerDeriv));
                }
                
                case UnaryFunc::EXP: {
                    DifferentiationStep step;
                    step.description = "Chain Rule: ∂/∂x(exp(u)) = exp(u) ⋅ u'";
                    step.expression = "∂/∂x(exp(" + funcNode->arg->toString() + ")) = exp(" + funcNode->arg->toString() + ") ⋅ ∂/∂x(" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto expNode = std::make_unique<UnaryFuncNode>(UnaryFunc::EXP, funcNode->arg->clone());
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(expNode), std::move(innerDeriv));
                }
                
                case UnaryFunc::SQRT: {
                    DifferentiationStep step;
                    step.description = "Chain Rule: ∂/∂x(√u) = (1/(2√u)) ⋅ u'";
                    step.expression = "∂/∂x(√" + funcNode->arg->toString() + ")";
                    steps.push_back(step);
                    
                    auto sqrtNode = std::make_unique<UnaryFuncNode>(UnaryFunc::SQRT, funcNode->arg->clone());
                    auto two = std::make_unique<NumberNode>(2);
                    auto twoSqrt = std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(two), std::move(sqrtNode));
                    auto coeff = std::make_unique<BinaryOpNode>(BinaryOp::DIV, std::make_unique<NumberNode>(1), std::move(twoSqrt));
                    return std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(coeff), std::move(innerDeriv));
                }
            }
            break;
        }
    }
    
    return std::make_unique<NumberNode>(0);
}

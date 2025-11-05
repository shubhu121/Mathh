#include "simplifier.h"

bool Simplifier::isZero(const ASTNode* node) {
    if (node->type == NodeType::NUMBER) {
        return static_cast<const NumberNode*>(node)->value == 0;
    }
    return false;
}

bool Simplifier::isOne(const ASTNode* node) {
    if (node->type == NodeType::NUMBER) {
        return static_cast<const NumberNode*>(node)->value == 1;
    }
    return false;
}

double Simplifier::getNumericValue(const ASTNode* node, bool& isNumeric) {
    if (node->type == NodeType::NUMBER) {
        isNumeric = true;
        return static_cast<const NumberNode*>(node)->value;
    }
    isNumeric = false;
    return 0;
}

std::unique_ptr<ASTNode> Simplifier::simplify(std::unique_ptr<ASTNode> node) {
    if (node->type == NodeType::BINARY_OP) {
        auto binOp = static_cast<BinaryOpNode*>(node.get());
        
        // Recursively simplify children
        binOp->left = simplify(std::move(binOp->left));
        binOp->right = simplify(std::move(binOp->right));
        
        switch (binOp->op) {
            case BinaryOp::ADD:
                // 0 + x = x
                if (isZero(binOp->left.get())) {
                    return std::move(binOp->right);
                }
                // x + 0 = x
                if (isZero(binOp->right.get())) {
                    return std::move(binOp->left);
                }
                // Constant folding
                {
                    bool leftNum, rightNum;
                    double leftVal = getNumericValue(binOp->left.get(), leftNum);
                    double rightVal = getNumericValue(binOp->right.get(), rightNum);
                    if (leftNum && rightNum) {
                        return std::make_unique<NumberNode>(leftVal + rightVal);
                    }
                }
                break;
                
            case BinaryOp::SUB:
                // x - 0 = x
                if (isZero(binOp->right.get())) {
                    return std::move(binOp->left);
                }
                // 0 - x = -x
                if (isZero(binOp->left.get())) {
                    return std::make_unique<BinaryOpNode>(
                        BinaryOp::MUL,
                        std::make_unique<NumberNode>(-1),
                        std::move(binOp->right)
                    );
                }
                // Constant folding
                {
                    bool leftNum, rightNum;
                    double leftVal = getNumericValue(binOp->left.get(), leftNum);
                    double rightVal = getNumericValue(binOp->right.get(), rightNum);
                    if (leftNum && rightNum) {
                        return std::make_unique<NumberNode>(leftVal - rightVal);
                    }
                }
                break;
                
            case BinaryOp::MUL:
                // 0 * x = 0
                if (isZero(binOp->left.get()) || isZero(binOp->right.get())) {
                    return std::make_unique<NumberNode>(0);
                }
                // 1 * x = x
                if (isOne(binOp->left.get())) {
                    return std::move(binOp->right);
                }
                // x * 1 = x
                if (isOne(binOp->right.get())) {
                    return std::move(binOp->left);
                }
                // -1 * -1 = 1
                {
                    bool leftNum, rightNum;
                    double leftVal = getNumericValue(binOp->left.get(), leftNum);
                    double rightVal = getNumericValue(binOp->right.get(), rightNum);
                    if (leftNum && rightNum) {
                        return std::make_unique<NumberNode>(leftVal * rightVal);
                    }
                }
                break;
                
            case BinaryOp::DIV:
                // 0 / x = 0
                if (isZero(binOp->left.get())) {
                    return std::make_unique<NumberNode>(0);
                }
                // x / 1 = x
                if (isOne(binOp->right.get())) {
                    return std::move(binOp->left);
                }
                // Constant folding
                {
                    bool leftNum, rightNum;
                    double leftVal = getNumericValue(binOp->left.get(), leftNum);
                    double rightVal = getNumericValue(binOp->right.get(), rightNum);
                    if (leftNum && rightNum && rightVal != 0) {
                        return std::make_unique<NumberNode>(leftVal / rightVal);
                    }
                }
                break;
                
            case BinaryOp::POW:
                // x^0 = 1
                if (isZero(binOp->right.get())) {
                    return std::make_unique<NumberNode>(1);
                }
                // x^1 = x
                if (isOne(binOp->right.get())) {
                    return std::move(binOp->left);
                }
                // 0^x = 0
                if (isZero(binOp->left.get())) {
                    return std::make_unique<NumberNode>(0);
                }
                // 1^x = 1
                if (isOne(binOp->left.get())) {
                    return std::make_unique<NumberNode>(1);
                }
                // Constant folding
                {
                    bool leftNum, rightNum;
                    double leftVal = getNumericValue(binOp->left.get(), leftNum);
                    double rightVal = getNumericValue(binOp->right.get(), rightNum);
                    if (leftNum && rightNum) {
                        return std::make_unique<NumberNode>(std::pow(leftVal, rightVal));
                    }
                }
                break;
        }
    }
    else if (node->type == NodeType::UNARY_FUNC) {
        auto funcNode = static_cast<UnaryFuncNode*>(node.get());
        funcNode->arg = simplify(std::move(funcNode->arg));
    }
    
    return node;
}

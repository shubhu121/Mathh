#pragma once
#include "ast.h"

class Simplifier {
public:
    static std::unique_ptr<ASTNode> simplify(std::unique_ptr<ASTNode> node);
    
private:
    static bool isZero(const ASTNode* node);
    static bool isOne(const ASTNode* node);
    static double getNumericValue(const ASTNode* node, bool& isNumeric);
};

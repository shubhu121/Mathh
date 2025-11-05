#include "parser.h"

std::unique_ptr<ASTNode> Parser::parse(const std::string& expr) {
    input = expr;
    pos = 0;
    skipWhitespace();
    auto result = parseExpression();
    skipWhitespace();
    if (pos < input.length()) {
        throw std::runtime_error("Unexpected character at position " + std::to_string(pos));
    }
    return result;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();
    
    while (true) {
        skipWhitespace();
        char c = peek();
        
        if (c == '+' || c == '-') {
            consume();
            auto right = parseTerm();
            BinaryOp op = (c == '+') ? BinaryOp::ADD : BinaryOp::SUB;
            left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto left = parsePower();
    
    while (true) {
        skipWhitespace();
        char c = peek();
        
        if (c == '*' || c == '/') {
            consume();
            auto right = parsePower();
            BinaryOp op = (c == '*') ? BinaryOp::MUL : BinaryOp::DIV;
            left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
        } else if (std::isalpha(c) || c == '(') {
            // Implicit multiplication
            auto right = parsePower();
            left = std::make_unique<BinaryOpNode>(BinaryOp::MUL, std::move(left), std::move(right));
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parsePower() {
    auto left = parseFactor();
    
    skipWhitespace();
    if (peek() == '^') {
        consume();
        auto right = parsePower(); // Right associative
        return std::make_unique<BinaryOpNode>(BinaryOp::POW, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    skipWhitespace();
    char c = peek();
    
    if (c == '-') {
        consume();
        auto arg = parseFactor();
        return std::make_unique<BinaryOpNode>(
            BinaryOp::MUL,
            std::make_unique<NumberNode>(-1),
            std::move(arg)
        );
    }
    
    if (c == '+') {
        consume();
        return parseFactor();
    }
    
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    skipWhitespace();
    char c = peek();
    
    // Number
    if (std::isdigit(c) || c == '.') {
        std::string numStr;
        while (std::isdigit(peek()) || peek() == '.') {
            numStr += consume();
        }
        return std::make_unique<NumberNode>(std::stod(numStr));
    }
    
    // Parentheses
    if (c == '(') {
        consume();
        auto expr = parseExpression();
        skipWhitespace();
        if (peek() != ')') {
            throw std::runtime_error("Expected closing parenthesis");
        }
        consume();
        return expr;
    }
    
    // Function or variable
    if (std::isalpha(c)) {
        std::string name;
        while (std::isalpha(peek())) {
            name += consume();
        }
        
        skipWhitespace();
        if (peek() == '(') {
            consume();
            auto arg = parseExpression();
            skipWhitespace();
            if (peek() != ')') {
                throw std::runtime_error("Expected closing parenthesis");
            }
            consume();
            
            // Match function name
            if (name == "sin") return std::make_unique<UnaryFuncNode>(UnaryFunc::SIN, std::move(arg));
            if (name == "cos") return std::make_unique<UnaryFuncNode>(UnaryFunc::COS, std::move(arg));
            if (name == "tan") return std::make_unique<UnaryFuncNode>(UnaryFunc::TAN, std::move(arg));
            if (name == "exp") return std::make_unique<UnaryFuncNode>(UnaryFunc::EXP, std::move(arg));
            if (name == "ln") return std::make_unique<UnaryFuncNode>(UnaryFunc::LN, std::move(arg));
            if (name == "sqrt") return std::make_unique<UnaryFuncNode>(UnaryFunc::SQRT, std::move(arg));
            
            throw std::runtime_error("Unknown function: " + name);
        }
        
        return std::make_unique<VariableNode>(name);
    }
    
    throw std::runtime_error("Unexpected character: " + std::string(1, c));
}

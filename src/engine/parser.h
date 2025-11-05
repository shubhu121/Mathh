#pragma once
#include "ast.h"
#include <cctype>
#include <stdexcept>

class Parser {
private:
    std::string input;
    size_t pos;
    
    char peek() {
        if (pos >= input.length()) return '\0';
        return input[pos];
    }
    
    char consume() {
        return input[pos++];
    }
    
    void skipWhitespace() {
        while (pos < input.length() && std::isspace(input[pos])) {
            pos++;
        }
    }
    
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parsePower();
    std::unique_ptr<ASTNode> parsePrimary();
    
public:
    std::unique_ptr<ASTNode> parse(const std::string& expr);
};

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <cmath>

enum class NodeType {
    NUMBER,
    VARIABLE,
    BINARY_OP,
    UNARY_FUNC
};

enum class BinaryOp {
    ADD, SUB, MUL, DIV, POW
};

enum class UnaryFunc {
    SIN, COS, TAN, EXP, LN, SQRT
};

class ASTNode {
public:
    NodeType type;
    virtual ~ASTNode() = default;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
    virtual std::string toString() const = 0;
    virtual double evaluate(double x) const = 0;
};

class NumberNode : public ASTNode {
public:
    double value;
    
    NumberNode(double val) : value(val) {
        type = NodeType::NUMBER;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<NumberNode>(value);
    }
    
    std::string toString() const override {
        if (value == (int)value) {
            return std::to_string((int)value);
        }
        return std::to_string(value);
    }
    
    double evaluate(double x) const override {
        return value;
    }
};

class VariableNode : public ASTNode {
public:
    std::string name;
    
    VariableNode(const std::string& n) : name(n) {
        type = NodeType::VARIABLE;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<VariableNode>(name);
    }
    
    std::string toString() const override {
        return name;
    }
    
    double evaluate(double x) const override {
        return x;
    }
};

class BinaryOpNode : public ASTNode {
public:
    BinaryOp op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    
    BinaryOpNode(BinaryOp o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {
        type = NodeType::BINARY_OP;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<BinaryOpNode>(op, left->clone(), right->clone());
    }
    
    std::string toString() const override {
        std::string opStr;
        switch (op) {
            case BinaryOp::ADD: opStr = " + "; break;
            case BinaryOp::SUB: opStr = " - "; break;
            case BinaryOp::MUL: opStr = " ⋅ "; break;
            case BinaryOp::DIV: opStr = " / "; break;
            case BinaryOp::POW: opStr = "^"; break;
        }
        
        bool needParensLeft = (left->type == NodeType::BINARY_OP);
        bool needParensRight = (right->type == NodeType::BINARY_OP);
        
        std::string leftStr = needParensLeft ? "(" + left->toString() + ")" : left->toString();
        std::string rightStr = needParensRight ? "(" + right->toString() + ")" : right->toString();
        
        return leftStr + opStr + rightStr;
    }
    
    double evaluate(double x) const override {
        double l = left->evaluate(x);
        double r = right->evaluate(x);
        
        switch (op) {
            case BinaryOp::ADD: return l + r;
            case BinaryOp::SUB: return l - r;
            case BinaryOp::MUL: return l * r;
            case BinaryOp::DIV: return l / r;
            case BinaryOp::POW: return std::pow(l, r);
        }
        return 0;
    }
};

class UnaryFuncNode : public ASTNode {
public:
    UnaryFunc func;
    std::unique_ptr<ASTNode> arg;
    
    UnaryFuncNode(UnaryFunc f, std::unique_ptr<ASTNode> a)
        : func(f), arg(std::move(a)) {
        type = NodeType::UNARY_FUNC;
    }
    
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<UnaryFuncNode>(func, arg->clone());
    }
    
    std::string toString() const override {
        std::string funcStr;
        switch (func) {
            case UnaryFunc::SIN: funcStr = "sin"; break;
            case UnaryFunc::COS: funcStr = "cos"; break;
            case UnaryFunc::TAN: funcStr = "tan"; break;
            case UnaryFunc::EXP: funcStr = "exp"; break;
            case UnaryFunc::LN: funcStr = "ln"; break;
            case UnaryFunc::SQRT: funcStr = "√"; break;
        }
        return funcStr + "(" + arg->toString() + ")";
    }
    
    double evaluate(double x) const override {
        double a = arg->evaluate(x);
        
        switch (func) {
            case UnaryFunc::SIN: return std::sin(a);
            case UnaryFunc::COS: return std::cos(a);
            case UnaryFunc::TAN: return std::tan(a);
            case UnaryFunc::EXP: return std::exp(a);
            case UnaryFunc::LN: return std::log(a);
            case UnaryFunc::SQRT: return std::sqrt(a);
        }
        return 0;
    }
};

struct DifferentiationStep {
    std::string description;
    std::string expression;
};

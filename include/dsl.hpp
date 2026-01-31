#pragma once

#include "node.hpp"
#include <memory>
#include <string>
#include <vector>

namespace AST
{

inline std::unique_ptr<ConstantNode> constant(int value)
{
    return std::make_unique<ConstantNode>(value);
}

inline std::unique_ptr<VariableNode> variable(const std::string &name)
{
    return std::make_unique<VariableNode>(name);
}

inline std::unique_ptr<BinaryOpNode> binary_op(ExprPtr lhs, AST::BinaryOp op, ExprPtr rhs)
{
    return std::make_unique<BinaryOpNode>(std::move(lhs), op, std::move(rhs));
}

inline std::unique_ptr<UnaryOpNode> unary_op(ExprPtr operand, AST::UnaryOp op)
{
    return std::make_unique<UnaryOpNode>(std::move(operand), op);
}

inline std::unique_ptr<AssignNode> assignment(std::unique_ptr<VariableNode> dest, AssignType type, ExprPtr expr)
{
    return std::make_unique<AssignNode>(std::move(dest), type, std::move(expr));
}

inline std::unique_ptr<IfNode> if_stmt(ExprPtr condition, StmtPtr action)
{
    return std::make_unique<IfNode>(std::move(condition), std::move(action));
}

inline std::unique_ptr<WhileNode> while_stmt(ExprPtr condition, StmtPtr scope)
{
    return std::make_unique<WhileNode>(std::move(condition), std::move(scope));
}

inline std::unique_ptr<ForNode> for_stmt(std::unique_ptr<AssignNode> assignment,
                                         ExprPtr                     condition,
                                         std::unique_ptr<AssignNode> iter,
                                         StmtPtr                     scope)
{
    return std::make_unique<ForNode>(std::move(assignment),
                                     std::move(condition),
                                     std::move(iter),
                                     std::move(scope));
}

inline std::unique_ptr<PrintNode> print(ExprPtr expr)
{
    return std::make_unique<PrintNode>(std::move(expr));
}

inline std::unique_ptr<InNode> in()
{
    return std::make_unique<InNode>();
}

inline std::unique_ptr<ScopeNode> scope(std::vector<StmtPtr> statements)
{
    return std::make_unique<ScopeNode>(std::move(statements));
}

} // namespace AST

#define CONST(val) AST::constant(val)
#define VAR(name) AST::variable(name);
#define BINARY(lhs, op, rhs) AST::binary_op(std::move(lhs), op, std::move(rhs))
#define UNARY(operand, op) AST::unary_op(std::move(operand), op)
#define ASSIGN(var, expr) AST::assignment(std::move(var), std::move(expr))
#define PRINT(expr) AST::print(std::move(expr))
#define WHILE(cond, body) AST::while_stmt(std::move(cond), std::move(body))
#define FOR(init, cond, iter, body) AST::for_stmt(std::move(init), std::move(cond), std::move(iter), std::move(body))
#define IF(cond, action) AST::if_stmt(std::move(cond), std::move(action))
#define IFELSE(cond, action, else_like) AST::if_stmt(std::move(cond), std::move(action), std::move(else_like))
#define ELSEIF(cond, action) AST::else_if_stmt(std::move(cond), std::move(action))
#define ELSE_IFELSE(cond, action, else_like) AST::else_if_stmt(std::move(cond), std::move(action), std::move(else_like))
#define ELSE(action) AST::else_stmt(std::move(action))
#define IN() AST::in()
#define BLOCK(stmts) AST::scope(std::move(stmts))

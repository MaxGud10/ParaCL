#pragma once

#include <memory>
#include <string>
#include <vector>

#include "node.hpp"
#include "detail/builder.hpp" 

namespace AST 
{

inline NodeBuilder& dsl_builder()
{
    static NodeBuilder bld;
    return bld;
}

inline void dsl_reset()
{
    dsl_builder().clear();
}

inline ConstantNode* constant(int value) 
{
    return dsl_builder().create<ConstantNode>(value);
}

inline VariableNode* variable(const std::string& name)
{
    return dsl_builder().create<VariableNode>(name);
}

inline BinaryOpNode* binary_op(ExprPtr lhs, AST::BinaryOp op, ExprPtr rhs)
{
    return dsl_builder().create<BinaryOpNode>(lhs, op, rhs);
}

inline UnaryOpNode* unary_op(ExprPtr operand, AST::UnaryOp op)
{
    return dsl_builder().create<UnaryOpNode>(operand, op);
}

inline AssignNode* assignment(VariableNode* dest, ExprPtr expr)
{
    return dsl_builder().create<AssignNode>(dest, expr);
}

inline IfNode* if_stmt(ExprPtr condition, StmtPtr action, StmtPtr else_action = nullptr)
{
    return dsl_builder().create<IfNode>(condition, action, else_action);
}

inline WhileNode* while_stmt(ExprPtr condition, StmtPtr scope)
{
    return dsl_builder().create<WhileNode>(condition, scope);
}

inline ForNode* for_stmt(AssignNode* init, ExprPtr condition, AssignNode* iter, StmtPtr scope)
{
    return dsl_builder().create<ForNode>(init, condition, iter, scope);
}

inline PrintNode* print(ExprPtr expr)
{
    return dsl_builder().create<PrintNode>(expr);
}

inline InNode* in()
{
    return dsl_builder().create<InNode>();
}

inline ScopeNode* scope(std::vector<StmtPtr> statements)
{
    return dsl_builder().create<ScopeNode>(std::move(statements));
}

} // namespace AST

#define CONST(val)                      AST::constant(val)
#define VAR(name)                       AST::variable(name)
#define BINARY(lhs, op, rhs)            AST::binary_op((lhs), (op), (rhs))
#define UNARY(operand, op)              AST::unary_op((operand), (op))
#define ASSIGN(var, expr)               AST::assignment((var), (expr))
#define PRINT(expr)                     AST::print((expr))
#define WHILE(cond, body)               AST::while_stmt((cond), (body))
#define FOR(init, cond, iter, body)     AST::for_stmt((init), (cond), (iter), (body))
#define IF(cond, action)                AST::if_stmt((cond), (action))
#define IFELSE(cond, action, else_like) AST::if_stmt((cond), (action), (else_like))
#define IN()                            AST::in()
#define BLOCK(stmts)                    AST::scope((stmts))

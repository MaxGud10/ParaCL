#pragma once

#include <iostream>
#include "nodeDump.hpp"

namespace AST {

class ForNode;
class ConstantNode;
class AssignNode;
class WhileNode;
class IfNode;
class InNode;
class VariableNode;
class BinaryOpNode;
class UnaryOpNode;
class PrintNode;
class ScopeNode;
class FunctionNode;
class CallNode;
class ReturnNode;
class ConditionalStatementNode;
class ExpressionNode;
class StatementNode;

}


class Visitor {
    public:
        virtual void Visit (AST::ConditionalStatementNode& node)    = 0;
        virtual void Visit (AST::StatementNode& node)               = 0;
        virtual void Visit (AST::ExpressionNode& node)              = 0;
        virtual void Visit (AST::ForNode& node)                     = 0;
        virtual void Visit (AST::ConstantNode& node)                = 0;
        virtual void Visit (AST::AssignNode& node)                  = 0;
        virtual void Visit (AST::WhileNode& node)                   = 0;
        virtual void Visit (AST::IfNode& node)                      = 0;
        virtual void Visit (AST::InNode& node)                      = 0;
        virtual void Visit (AST::VariableNode& node)                = 0;
        virtual void Visit (AST::BinaryOpNode& node)                = 0;
        virtual void Visit (AST::UnaryOpNode& node)                 = 0;
        virtual void Visit (AST::PrintNode& node)                   = 0;
        virtual void Visit (AST::ScopeNode& node)                   = 0;
        virtual void Visit (AST::FunctionNode& node)                = 0;
        virtual void Visit (AST::CallNode& node)                    = 0;
        virtual void Visit (AST::ReturnNode& node)                  = 0;


        virtual void Visit (const AST::ConditionalStatementNode& node)  const = 0;
        virtual void Visit (const AST::StatementNode& node)             const = 0;
        virtual void Visit (const AST::ExpressionNode& node)            const = 0;
        virtual void Visit (const AST::ForNode& node)                   const = 0;
        virtual void Visit (const AST::ConstantNode& node)              const = 0;
        virtual void Visit (const AST::AssignNode& node)                const = 0;
        virtual void Visit (const AST::WhileNode& node)                 const = 0;
        virtual void Visit (const AST::IfNode& node)                    const = 0;
        virtual void Visit (const AST::InNode& node)                    const = 0;
        virtual void Visit (const AST::VariableNode& node)              const = 0;
        virtual void Visit (const AST::BinaryOpNode& node)              const = 0;
        virtual void Visit (const AST::UnaryOpNode& node)               const = 0;
        virtual void Visit (const AST::PrintNode& node)                 const = 0;
        virtual void Visit (const AST::ScopeNode& node)                 const = 0;
        virtual void Visit (const AST::FunctionNode& node)              const = 0;
        virtual void Visit (const AST::CallNode& node)                  const = 0;
        virtual void Visit (const AST::ReturnNode& node)                const = 0;

        virtual ~Visitor() = default;

};

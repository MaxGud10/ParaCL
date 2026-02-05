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

}


class Visitor {
    public:
        virtual void VisitForNode       (const AST::ForNode& node)           const = 0;
        virtual void VisitConstantNode  (const AST::ConstantNode& node)      const = 0;
        virtual void VisitAssignNode    (const AST::AssignNode& node)        const = 0;
        virtual void VisitWhileNode     (const AST::WhileNode& node)         const = 0;
        virtual void VisitIfNode        (const AST::IfNode& node)            const = 0;
        virtual void VisitInNode        (const AST::InNode& node)            const = 0;
        virtual void VisitVariableNode  (const AST::VariableNode& node)      const = 0;
        virtual void VisitBinaryOpNode  (const AST::BinaryOpNode& node)      const = 0;
        virtual void VisitUnaryOpNode   (const AST::UnaryOpNode& node)       const = 0;
        virtual void VisitPrintNode     (const AST::PrintNode& node)         const = 0;
        virtual void VisitScopeNode     (const AST::ScopeNode& node)         const = 0;
        virtual ~Visitor() = default;

};

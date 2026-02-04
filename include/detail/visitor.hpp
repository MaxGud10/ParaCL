#include <iostream>

class Visitor {
    public:
        virtual void VisitForNode       (const ForNode& node)           const = 0;
        virtual void VisitConstantNode  (const Constant& node)          const = 0;
        virtual void VisitAssignNode    (const AssignNode& node)        const = 0;
        virtual void VisitWhileNode     (const WhileNode& node)         const = 0;
        virtual void VisitIfNode        (const IfNode& node)            const = 0;
        virtual void VisitInNode        (const InNode& node)            const = 0;
        virtual void VisitVariableNode  (const VariableNode& node)      const = 0;
        virtual void VisitBinaryOpNode  (const BinaryOpNode& node)      const = 0;
        virtual void VisitUnaryOpNode   (const UnaryOpNode& node)       const = 0;
        virtual void VisitPrintNode     (const PrintNode& node)         const = 0;
        virtual void VisitScopeNode     (const ScopeNode& node)         const = 0;

};

class DotPrinter : public Visitor {
    private:
        std::ostream& os_;

    DotPrinter(std::ostream& os = std::cout) : os_(os) {
        os_ << "digraph{" << std::endl;
    }

    ~DotPrinter() {
        os_ << "}" << std::endl;
    }

    void VistScopeNode(const ScopeNode& n) const override {
        const auto& children = n.get_children();
        for (const auto &child : n) {
            child.accept(*this);;
        }
    }

    void VisitConstantNode(const ConstantNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << n.get_val() << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::CONSTANT_NODE_COLOR << std::dec
        << END_NODE;
    }

    void VisitAssignNode(const AssignNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "ASSIGN '='" << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR    << std::hex << AST::dump_style::ASSIGN_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_expr() << std::endl;

        n.get_expr()->accept(*this);
    }

    void VisitWhileNode(const WhileNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "WHILE"   << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::WHILE_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.cond_  << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.scope_ << std::endl;

        n.get_cond()->accept(*this);
        n.get_scope()->accept(*this);

    }

    void VisitIfNode(const IfNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "IF"      << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::IF_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_cond()        << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_action()      << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_else_action() << std::endl;

        n.get_cond()->accept(*this);
        n.action_->accept(*this);
        if (n.get_else_action()) n.get_else_action().accept(*this);

    }

    void VisitPrintNode(const PrintNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "PRINT"   << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::PRINT_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_expr() << std::endl;

        n.get_expr()->accept(*this);
    }

    void VisitInNode(const InNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "IN"      << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::PRINT_NODE_COLOR << std::dec
        << END_NODE;

    }

    void VisitVariableNode(const VariableNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << n.get_name()   << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::VARIABLE_NODE_COLOR << std::dec
        << END_NODE;

    }

    void VisitBinaryOpNode(const BinaryOpNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "binary: " << BinaryOpNames[static_cast<std::size_t>(n.get_op())]    << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR  << std::hex << AST::dump_style::BINARYOP_NODE_COLOR << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_left()  << std::endl;
        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_right() << std::endl;

        n.get_left()->accept(*this);
        n.get_right()->accept(*this);

    }

    void VisitUnaryOpNode(const UnaryOpNode& n) const override {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "unary: " << UnaryOpNames[static_cast<std::size_t>(n.get_op())]    << SET_ADR << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::UNARYOP_NODE_COLOR  << std::dec
        << END_NODE;

        os << SET_NODE << &n << SET_LINK << SET_NODE << n.get_operand() << std::endl;

        n.get_operand()->accept(*this);

    }

    void VisitForNode(const ForNode& n) const override {
        os_ << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "FOR"     << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::WHILE_NODE_COLOR << std::dec
        << END_NODE;

        os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_init() << std::endl;
        os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_cond() << std::endl;
        os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_iter() << std::endl;
        os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_body() << std::endl;

        n.get_init()->accept(*this);
        n.get_cond()->accept(*this);
        n.get_iter()->accept(*this);
        n.get_body()->accept(*this);
    }
}


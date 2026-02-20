    #pragma once

    #include <iostream>
    #include "detail/ivisitor.hpp"
    #include "node.hpp"

    class DotPrinter : public Visitor {
        private:
            std::ostream& os_;

        public:
        DotPrinter(std::ostream& os = std::cout) : os_(os) {
            os_ << "digraph{" << std::endl;
        }

        ~DotPrinter() {
            os_ << "}" << std::endl;
        }

        void Visit(const AST::ScopeNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "SCOPE"   << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::SCOPE_NODE_COLOR << std::dec
            << END_NODE;

            const auto& children = n.get_children();
            for (const auto &child : children) {
                os_ << SET_NODE << &n << SET_LINK << SET_NODE << child << std::endl;
                child->accept(*this);
            }
        }

        void Visit(const AST::ConstantNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << n.get_val() << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR   << std::hex << AST::dump_style::CONSTANT_NODE_COLOR << std::dec
            << END_NODE;
        }

        void Visit(const AST::AssignNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "ASSIGN '='" << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR    << std::hex << AST::dump_style::ASSIGN_NODE_COLOR << std::dec
            << END_NODE;

            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_dest() << std::endl;
            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_expr() << std::endl;

            n.get_dest()->accept(*this);
            n.get_expr()->accept(*this);
        }

        void Visit(const AST::WhileNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "WHILE"   << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::WHILE_NODE_COLOR << std::dec
            << END_NODE;

            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_cond()  << std::endl;
            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_scope() << std::endl;

            n.get_cond()->accept(*this);
            n.get_scope()->accept(*this);
        }

        void Visit(const AST::IfNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "IF"      << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::IF_NODE_COLOR << std::dec
            << END_NODE;

            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_cond()        << std::endl;
            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_action()      << std::endl;
            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_else_action() << std::endl;

            n.get_cond()->accept(*this);
            n.get_action()->accept(*this);
            if (n.get_else_action()) n.get_else_action()->accept(*this);
        }

        void Visit(const AST::PrintNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "PRINT"   << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::PRINT_NODE_COLOR << std::dec
            << END_NODE;

            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_expr() << std::endl;

            n.get_expr()->accept(*this);
        }

        void Visit(const AST::InNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "IN"      << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::PRINT_NODE_COLOR << std::dec
            << END_NODE;
        }

        void Visit(const AST::VariableNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << n.get_name()   << SET_ADR  << &n << END_LABEL
            << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::VARIABLE_NODE_COLOR << std::dec
            << END_NODE;
        }

        void Visit(const AST::BinaryOpNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "binary: " << AST::BinaryOpNames[static_cast<std::size_t>(n.get_op())]    << SET_ADR << &n << END_LABEL
            << SET_FILLED << SET_COLOR  << std::hex << AST::dump_style::BINARYOP_NODE_COLOR << std::dec
            << END_NODE;

            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_left()  << std::endl;
            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_right() << std::endl;

            n.get_left()->accept(*this);
            n.get_right()->accept(*this);
        }

        void Visit(const AST::UnaryOpNode& n) const override {
            os_ << SET_NODE << &n
            << SET_MRECORD_SHAPE
            << SET_LABEL  << "unary: " << AST::UnaryOpNames[static_cast<std::size_t>(n.get_op())]    << SET_ADR << &n << END_LABEL
            << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::UNARYOP_NODE_COLOR  << std::dec
            << END_NODE;

            os_ << SET_NODE << &n << SET_LINK << SET_NODE << n.get_operand() << std::endl;

            n.get_operand()->accept(*this);
        }

        void Visit(const AST::ForNode& n) const override {
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

        void Visit(AST::ForNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::ConstantNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::AssignNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::WhileNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::IfNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::InNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::VariableNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::BinaryOpNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::UnaryOpNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::PrintNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
        void Visit(AST::ScopeNode& node) override { throw std::runtime_error("DotPrinter::Visit non-const not implemented"); }
    };



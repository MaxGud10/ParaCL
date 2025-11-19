#include <gtest/gtest.h>  // for Test, TestInfo (ptr only), TEST
#include <string>         // for basic_string


#include "dsl.hpp"
#include "node.hpp"
#include "ast.hpp"
#include "test_utils.hpp"   // for run_test

TEST(common, basic_1)              { test_utils::run_test("/common/basic_1"); }

TEST(common, local_variables)      { test_utils::run_test("/common/local_variables"); }

TEST(common, if_1)                 { test_utils::run_test("/common/if_1"); }

TEST(common, if_2)                 { test_utils::run_test("/common/if_2"); }

TEST(common, while_1)              { test_utils::run_test("/common/while_1"); }

TEST(common, while_uminus_logical) { test_utils::run_test("/common/while_uminus_logical"); }

TEST(common, while_if_logical_mod) { test_utils::run_test("/common/while_if_logical_mod"); }

TEST(common, expression_1)         { test_utils::run_test("/common/expression_1"); }

TEST(common, scopes)               { test_utils::run_test("/common/scopes"); }

TEST(common, for_1)                { test_utils::run_test("/common/for_1"); }

TEST(common, for_sum)              { test_utils::run_test("/common/for_sum"); }

TEST(common, if_else)              { test_utils::run_test("/common/if_else"); }

TEST(common, else_if_1)            { test_utils::run_test("/common/else_if_1"); }

TEST(common, else_if_2)            { test_utils::run_test("/common/else_if_2"); }

TEST(common, for_if_else_complex)  { test_utils::run_test("/common/for_if_else_complex"); }

TEST(common, print)                { test_utils::run_test("/common/printf"); }

TEST(common, and_or)               { test_utils::run_test("/common/if_and_or"); }

TEST(common, and_or_truth_table)   { test_utils::run_test("/common/and_or_truth_table"); }

TEST(common, and_or_precedence)    { test_utils::run_test("/common/and_or_precedence"); }

TEST(common, and_or_relational)    { test_utils::run_test("/common/and_or_relational"); }

TEST(common, bitwise_and_or)       { test_utils::run_test("/common/bitwise_and_or"); }

TEST(common, bitwise_basic)        { test_utils::run_test("/common/bitwise_basic"); }

TEST(common, bitwise_precedence)   { test_utils::run_test("/common/bitwise_precedence"); }

TEST(common, bitwise_flags)        { test_utils::run_test("/common/bitwise_flags"); }

TEST(common, bitwise_in_if)        { test_utils::run_test("/common/bitwise_in_if"); }

TEST(common, bitwise_in_while)     { test_utils::run_test("/common/bitwise_in_while");}

TEST(common, bitwise_and_logical)  { test_utils::run_test("/common/bitwise_and_logical");}



TEST(ASTTest, CreateConstant)
{
    auto constNode = CONST(42);
    ASSERT_NE(constNode, nullptr);
    EXPECT_EQ(constNode->get_val(), 42);
}

TEST(ASTTest, CreateVariable)
{
    auto varNode = VAR("x");
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->get_name(), "x");
}

TEST(ASTTest, CreateBinaryOpADD)
{
    auto lhs = CONST(1000);
    auto rhs = CONST(7);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::ADD, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);

    EXPECT_EQ(result, 1007);
}

TEST(ASTTest, CreateBinaryOpSUB)
{
    auto lhs = CONST(1000);
    auto rhs = CONST(7);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::SUB, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 993);
}

TEST(ASTTest, CreateBinaryOpMUL)
{
    auto lhs = CONST(15);
    auto rhs = CONST(3);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::MUL, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 45);
}

TEST(ASTTest, CreateBinaryOpDIV)
{
    auto lhs = CONST(42);
    auto rhs = CONST(6);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::DIV, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 7);
}

TEST(ASTTest, CreateBinaryOpDIVByZero)
{
    auto lhs = CONST(42);
    auto rhs = CONST(0);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::DIV, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    EXPECT_THROW(binOpNode->eval(ctx), std::runtime_error);
}

TEST(ASTTest, CreateBinaryOpMOD)
{
    auto lhs = CONST(10);
    auto rhs = CONST(3);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::MOD, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}


TEST(ASTTest, CreateBinaryOpAND)
{
    auto lhs = CONST(1);
    auto rhs = CONST(0);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::AND, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);

    EXPECT_EQ(result, 0);
}

TEST(ASTTest, CreateBinaryOpOR)
{
    auto lhs = CONST(1);
    auto rhs = CONST(0);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::OR, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}

TEST(ASTTest, CreateBinaryOpLESS)
{
    auto lhs = CONST(3);
    auto rhs = CONST(7);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::LS, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}

TEST(ASTTest, CreateBinaryOpLESSorEQUAL)
{
    auto lhs = CONST(3);
    auto rhs = CONST(7);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::LS_EQ, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}

TEST(ASTTest, CreateBinaryOpGREATER)
{
    auto lhs = CONST(10);
    auto rhs = CONST(5);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::GR, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}

TEST(ASTTest, CreateBinaryOpGREATERorEQUAL)
{
    auto lhs = CONST(5);
    auto rhs = CONST(5);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::GR_EQ, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}

TEST(ASTTest, CreateBinaryOpEQ)
{
    auto lhs = CONST(42);
    auto rhs = CONST(42);
    auto binOpNode = binary_op(std::move(lhs), AST::BinaryOp::EQ, std::move(rhs));

    ASSERT_NE(binOpNode, nullptr);

    AST::detail::Context ctx;
    int result = binOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}

TEST(ASTTest, CreateUnaryOpNEG)
{
    auto operand = CONST(-42);
    auto unaryOpNode = unary_op(std::move(operand), AST::UnaryOp::NEG);

    ASSERT_NE(unaryOpNode, nullptr);

    AST::detail::Context ctx;
    int result = unaryOpNode->eval(ctx);
    EXPECT_EQ(result, 42);
}

TEST(ASTTest, CreateUnaryOpNOT)
{
    auto operand = CONST(0);
    auto unaryOpNode = unary_op(std::move(operand), AST::UnaryOp::NOT);

    ASSERT_NE(unaryOpNode, nullptr);

    AST::detail::Context ctx;
    int result = unaryOpNode->eval(ctx);
    EXPECT_EQ(result, 1);
}

TEST(ASTTest, CreateAssignmentNode)
{
    auto var = VAR("x");
    auto expr = CONST(42);
    auto assignmentNode = assignment(std::move(var), std::move(expr));

    ASSERT_NE(assignmentNode, nullptr);

    AST::detail::Context ctx;
    ctx.varTables_.emplace_back();
    ctx.curScope_ = 0;

    int result = assignmentNode->eval(ctx);

    EXPECT_EQ(result, 42);
}

TEST(ASTTest, WhileNode_ConditionTrue)
{
    // Create a simple while loop: while (x < 10) { x = x + 1 }
    auto condition = AST::binary_op(AST::variable("x"), AST::BinaryOp::LS, AST::constant(10));
    auto action = AST::assignment(AST::variable("x"), AST::binary_op(AST::variable("x"), AST::BinaryOp::ADD, AST::constant(1)));
    auto whileNode = AST::while_stmt(std::move(condition), std::move(action));

    ASSERT_NE(whileNode, nullptr);

    // Initialize context with x = 0
    AST::detail::Context ctx;
    ctx.varTables_.emplace_back();
    ctx.varTables_[0]["x"] = 0;  // Set initial value of x
    ctx.curScope_ = 0;

    // Evaluate the while loop
    whileNode->eval(ctx);

    // After the loop, x should be 10
    EXPECT_EQ(ctx.varTables_[0]["x"], 10);
}

TEST(ASTTest, IfNode_TrueCondition)
{
    // Create an if statement: if (x == 10) { y = 20 }
    auto condition = AST::binary_op(AST::variable("x"), AST::BinaryOp::EQ, AST::constant(10));
    auto action = AST::assignment(AST::variable("y"), AST::constant(20));

    auto ifNode = AST::if_stmt(std::move(condition), std::move(action));

    ASSERT_NE(ifNode, nullptr);

    // Initialize context with x = 10
    AST::detail::Context ctx;
    ctx.varTables_.emplace_back();
    ctx.varTables_[0]["x"] = 10;
    ctx.curScope_ = 0;

    // Evaluate the if statement
    ifNode->eval(ctx);

    // After the if statement, y should be 20
    EXPECT_EQ(ctx.varTables_[0]["y"], 20);
}


TEST(ASTTest, IfNode_FalseCondition)
{
    // Create an if statement: if (x == 10) { y = 20 }
    auto condition = AST::binary_op(AST::variable("x"), AST::BinaryOp::EQ, AST::constant(10));
    auto action = AST::assignment(AST::variable("y"), AST::constant(20));
    auto ifNode = AST::if_stmt(std::move(condition), std::move(action));

    ASSERT_NE(ifNode, nullptr);

    // Initialize context with x = 5 (condition will be false)
    AST::detail::Context ctx;
    ctx.varTables_.emplace_back();
    ctx.varTables_[0]["x"] = 5;
    ctx.curScope_ = 0;

    // Evaluate the if statement
    ifNode->eval(ctx);

    // After the if statement, y should not be assigned, it should not exist in the context
    EXPECT_EQ(ctx.varTables_[0].count("y"), 0);
}

TEST(ASTTest, PrintNode) {
    // Create a print statement: print(x)
    auto printNode = AST::print(AST::variable("x"));

    ASSERT_NE(printNode, nullptr);

    // Redirect std::cout to a stringstream to capture output
    std::stringstream ss;
    AST::detail::Context ctx(ss);

    // Initialize context with x = 42
    ctx.varTables_.emplace_back();
    ctx.varTables_[0]["x"] = 42;
    ctx.curScope_ = 0;

    // Evaluate the print statement
    printNode->eval(ctx);

    // Check that the output was correct
    EXPECT_EQ(ss.str(), "42\n");
}

TEST(ASTTest, InNode) // TODO
{
    std::string simulated_input = "42";
    std::istringstream input_stream(simulated_input);

    // Redirect std::cin to the input stream
    std::cin.rdbuf(input_stream.rdbuf());

    auto var = VAR("x");
    auto expr = IN();
    auto assignmentNode = assignment(std::move(var), std::move(expr));

    ASSERT_NE(assignmentNode, nullptr);

    AST::detail::Context ctx;
    ctx.varTables_.emplace_back();
    ctx.curScope_ = 0;

    int result = assignmentNode->eval(ctx);

    EXPECT_EQ(result, 42);
}

TEST(ASTTest, ForNode_SimpleCount)
{
    // for (x = 0; x < 3; x = x + 1) { y = y + 1; }

    auto init = AST::assignment(AST::variable("x"), AST::constant(0));
    auto cond = AST::binary_op(AST::variable("x"), AST::BinaryOp::LS, AST::constant(3));
    auto iter = AST::assignment(AST::variable("x"),
                AST::binary_op(AST::variable("x"), AST::BinaryOp::ADD, AST::constant(1)));

    auto body = AST::assignment(AST::variable("y"),
                AST::binary_op(AST::variable("y"), AST::BinaryOp::ADD, AST::constant(1)));

    auto forNode = AST::for_stmt(std::move(init), std::move(cond), std::move(iter), std::move(body));

    AST::detail::Context ctx;
    ctx.varTables_.emplace_back();
    ctx.varTables_[0]["y"] = 0;
    ctx.curScope_ = 0;

    forNode->eval(ctx);

    EXPECT_EQ(ctx.varTables_[0]["y"], 3);
}

TEST(ASTTest, BinaryOpANDTruthTable)
{
    {
        auto lhs = CONST(0);
        auto rhs = CONST(0);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::AND, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 0);
    }
    {
        auto lhs = CONST(0);
        auto rhs = CONST(1);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::AND, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 0);
    }
    {
        auto lhs = CONST(1);
        auto rhs = CONST(0);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::AND, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 0);
    }
    {
        auto lhs = CONST(1);
        auto rhs = CONST(1);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::AND, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 1);
    }
}

TEST(ASTTest, BinaryOpORTruthTable)
{
    {
        auto lhs = CONST(0);
        auto rhs = CONST(0);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::OR, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 0);
    }
    {
        auto lhs = CONST(0);
        auto rhs = CONST(1);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::OR, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 1);
    }
    {
        auto lhs = CONST(1);
        auto rhs = CONST(0);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::OR, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 1);
    }
    {
        auto lhs = CONST(1);
        auto rhs = CONST(1);
        auto node = binary_op(std::move(lhs), AST::BinaryOp::OR, std::move(rhs));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 1);
    }
}

TEST(ASTTest, AndOrPrecedence)
{
    // 1 || (0 && 0)  ->  1
    {
        auto inner = binary_op(CONST(0), AST::BinaryOp::AND, CONST(0));
        auto expr  = binary_op(CONST(1), AST::BinaryOp::OR, std::move(inner));
        AST::detail::Context ctx;
        EXPECT_EQ(expr->eval(ctx), 1);
    }

    // (1 || 0) && 0  ->  0
    {
        auto left  = binary_op(CONST(1), AST::BinaryOp::OR, CONST(0));
        auto expr  = binary_op(std::move(left), AST::BinaryOp::AND, CONST(0));
        AST::detail::Context ctx;
        EXPECT_EQ(expr->eval(ctx), 0);
    }
}

TEST(ASTTest, AndOrWithRelational)
{
    // (1 < 2) && (2 < 3)  -> 1 && 1 -> 1
    {
        auto leftCmp  = binary_op(CONST(1), AST::BinaryOp::LS, CONST(2));
        auto rightCmp = binary_op(CONST(2), AST::BinaryOp::LS, CONST(3));
        auto expr     = binary_op(std::move(leftCmp), AST::BinaryOp::AND, std::move(rightCmp));
        AST::detail::Context ctx;
        EXPECT_EQ(expr->eval(ctx), 1);
    }

    // (1 > 2) || (2 < 3)  -> 0 || 1 -> 1
    {
        auto leftCmp  = binary_op(CONST(1), AST::BinaryOp::GR, CONST(2));
        auto rightCmp = binary_op(CONST(2), AST::BinaryOp::LS, CONST(3));
        auto expr     = binary_op(std::move(leftCmp), AST::BinaryOp::OR, std::move(rightCmp));
        AST::detail::Context ctx;
        EXPECT_EQ(expr->eval(ctx), 1);
    }

    // (1 == 1) && (3 == 4)  -> 1 && 0 -> 0
    {
        auto leftCmp  = binary_op(CONST(1), AST::BinaryOp::EQ, CONST(1));
        auto rightCmp = binary_op(CONST(3), AST::BinaryOp::EQ, CONST(4));
        auto expr     = binary_op(std::move(leftCmp), AST::BinaryOp::AND, std::move(rightCmp));
        AST::detail::Context ctx;
        EXPECT_EQ(expr->eval(ctx), 0);
    }
}

TEST(ASTTest, BinaryOpBIT_AND)
{
    auto lhs  = CONST(10); // 1010
    auto rhs  = CONST(12); // 1100
    auto node = binary_op(std::move(lhs), AST::BinaryOp::BIT_AND, std::move(rhs));

    AST::detail::Context ctx;
    EXPECT_EQ(node->eval(ctx), 8); // 1000
}

TEST(ASTTest, BinaryOpBIT_OR)
{
    auto lhs  = CONST(10); // 1010
    auto rhs  = CONST(12); // 1100
    auto node = binary_op(std::move(lhs), AST::BinaryOp::BIT_OR, std::move(rhs));

    AST::detail::Context ctx;
    EXPECT_EQ(node->eval(ctx), 14); // 1110
}

TEST(ASTTest, BitAndOrWithZero)
{
    {
        auto node = binary_op(CONST(0), AST::BinaryOp::BIT_AND, CONST(123));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 0);
    }
    {
        auto node = binary_op(CONST(0), AST::BinaryOp::BIT_OR, CONST(123));
        AST::detail::Context ctx;
        EXPECT_EQ(node->eval(ctx), 123);
    }
}

TEST(ASTTest, BitAndOrCombined)
{
    // (10 & 12) | 3  =>  (8) | 3 = 11
    auto andNode  = binary_op(CONST(10), AST::BinaryOp::BIT_AND, CONST(12));
    auto fullExpr = binary_op(std::move(andNode), AST::BinaryOp::BIT_OR, CONST(3));

    AST::detail::Context ctx;
    EXPECT_EQ(fullExpr->eval(ctx), 11);
}

TEST(ASTTest, BitwiseWithRelational)
{
    // (10 & 12) == 8  -> 1
    {
        auto bit  = binary_op(CONST(10), AST::BinaryOp::BIT_AND, CONST(12));
        auto expr = binary_op(std::move(bit), AST::BinaryOp::EQ, CONST(8));

        AST::detail::Context ctx;
        EXPECT_EQ(expr->eval(ctx), 1);
    }

    // (10 | 12) < 20 -> 1 (14 < 20)
    {
        auto bit  = binary_op(CONST(10), AST::BinaryOp::BIT_OR, CONST(12));
        auto expr = binary_op(std::move(bit), AST::BinaryOp::LS, CONST(20));

        AST::detail::Context ctx;
        EXPECT_EQ(expr->eval(ctx), 1);
    }
}

TEST(ASTTest, BitwiseWithLogical)
{
    // (10 & 12) && (10 | 12)  -> 8 && 14 -> 1
    {
        auto leftBit   = binary_op(CONST(10), AST::BinaryOp::BIT_AND, CONST(12));
        auto rightBit  = binary_op(CONST(10), AST::BinaryOp::BIT_OR,  CONST(12));
        auto fullExpr  = binary_op(std::move(leftBit), AST::BinaryOp::AND, std::move(rightBit));

        AST::detail::Context ctx;
        EXPECT_EQ(fullExpr->eval(ctx), 1);
    }

    // (10 & 0) || (0 | 0) -> 0 || 0 -> 0
    {
        auto leftBit   = binary_op(CONST(10), AST::BinaryOp::BIT_AND, CONST(0));
        auto rightBit  = binary_op(CONST(0),  AST::BinaryOp::BIT_OR,  CONST(0));
        auto fullExpr  = binary_op(std::move(leftBit), AST::BinaryOp::OR, std::move(rightBit));

        AST::detail::Context ctx;
        EXPECT_EQ(fullExpr->eval(ctx), 0);
    }
}



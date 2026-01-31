int eval(detail::Context& ctx) const override
{
    MSG("Evaluating Binary Operation\n");

    int result = 0;

    switch (op_)
    {
        case BinaryOp::AND:
        {
            const int leftVal = left_->eval(ctx);
            if (!leftVal)
                return 0;               // short-circuit: false && ... == false

            const int rightVal = right_->eval(ctx);
            return rightVal ? 1 : 0;    // приводим к 0/1 как у тебя
        }

        case BinaryOp::OR:
        {
            const int leftVal = left_->eval(ctx);
            if (leftVal)
                return 1;               // short-circuit: true || ... == true

            const int rightVal = right_->eval(ctx);
            return rightVal ? 1 : 0;
        }

        case BinaryOp::DIV:
        {
            const int leftVal = left_->eval(ctx);
            const int rightVal = right_->eval(ctx);
            if (rightVal == 0) throw std::runtime_error("Divide by zero");
            result = leftVal / rightVal;
            break;
        }

        default:
        {
            const int leftVal = left_->eval(ctx);
            const int rightVal = right_->eval(ctx);

            switch (op_)
            {
                case BinaryOp::ADD:    result = leftVal + rightVal; break;
                case BinaryOp::SUB:    result = leftVal - rightVal; break;
                case BinaryOp::MUL:    result = leftVal * rightVal; break;
                case BinaryOp::MOD:    result = leftVal % rightVal; break;

                case BinaryOp::LS:     result = leftVal <  rightVal; break;
                case BinaryOp::GR:     result = leftVal >  rightVal; break;
                case BinaryOp::LS_EQ:  result = leftVal <= rightVal; break;
                case BinaryOp::GR_EQ:  result = leftVal >= rightVal; break;
                case BinaryOp::EQ:     result = leftVal == rightVal; break;
                case BinaryOp::NOT_EQ: result = leftVal != rightVal; break;

                case BinaryOp::BIT_AND: result = leftVal & rightVal; break;
                case BinaryOp::BIT_OR:  result = leftVal | rightVal; break;

                // AND/OR обработаны выше
                default:
                    throw std::runtime_error("Unknown binary operation");
            }
        }
    }

    LOG("It's {}\n", result);
    return result;
}

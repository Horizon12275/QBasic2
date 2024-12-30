#include "exp.h"

/*
 * --------------------------------------------------------------
 * Implementation notes: Expression
 * --------------------------------------------------------------
 */

Expression::Expression()
{
}

Expression::~Expression()
{
}

/*
 * --------------------------------------------------------------
 * Implementation notes: ConstantExp
 * --------------------------------------------------------------
 */

ConstantExp::ConstantExp(int val)
{
    value = val;
}

std::string ConstantExp::toString()
{
    return std::to_string(value);
}

ExpressionType ConstantExp::type()
{
    return CONSTANT;
}

int ConstantExp::getConstantValue()
{
    return value;
}

int ConstantExp::eval(EvaluationContext &context)
{
    return value;
}

/*
 * --------------------------------------------------------------
 * Implementation notes: IdentifierExp
 * --------------------------------------------------------------
 */

IdentifierExp::IdentifierExp(string name)
{
    this->name = name;
}

std::string IdentifierExp::toString()
{
    return name;
}

ExpressionType IdentifierExp::type()
{
    return IDENTIFIER;
}

string IdentifierExp::getIdentifierName()
{
    return name;
}

int IdentifierExp::eval(EvaluationContext &context)
{
    if (!context.isDefined(name))
    {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return context.getValue(name);
}

/*
 * --------------------------------------------------------------
 * Implementation notes: CompoundExp
 * --------------------------------------------------------------
 */

CompoundExp::CompoundExp(std::string op, Expression *lhs, Expression *rhs)
{
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

CompoundExp::~CompoundExp()
{
    delete lhs;
    delete rhs;
}

std::string CompoundExp::toString()
{
    return "(" + lhs->toString() + " " + op + " " + rhs->toString() + ")";
}

ExpressionType CompoundExp::type()
{
    return COMPOUND;
}

std::string CompoundExp::getOperator()
{
    return op;
}

Expression *CompoundExp::getLHS()
{
    return lhs;
}

Expression *CompoundExp::getRHS()
{
    return rhs;
}

int CompoundExp::eval(EvaluationContext &context)
{
    int right = rhs->eval(context);
    if (op == "=")
    {
        context.setValue(lhs->getIdentifierName(), right);
        return right;
    }
    int left = lhs->eval(context);
    if (op == "+")
        return left + right;
    if (op == "-")
        return left - right;
    if (op == "*")
        return left * right;
    if (op == "/")
    {
        if (right == 0)
        {
            throw std::runtime_error("Division by 0");
        }
        return left / right;
    }
    if (op == "%")
    {
        if (right == 0)
        {
            throw std::runtime_error("MOD by 0");
        }
        return left % right;
    }
    if (op == "^")
    {
        if (left == 0 && right == 0)
        {
            throw std::runtime_error("0^0 is undefined");
        }
        return pow(left, right);
    }
    throw std::runtime_error("Illegal operator in expression");
    return 0;
}

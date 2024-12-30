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

int Expression::getConstantValue()
{
    throw std::runtime_error("getConstantValue: Illegal expression type");
}

std::string Expression::getIdentifierName()
{
    throw std::runtime_error("getIdentifierName: Illegal expression type");
}

std::string Expression::getOperator()
{
    throw std::runtime_error("getOperator: Illegal expression type");
}

Expression *Expression::getLHS()
{
    throw std::runtime_error("getLHS: Illegal expression type");
}

Expression *Expression::getRHS()
{
    throw std::runtime_error("getRHS: Illegal expression type");
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

// 递归析构左右子树
CompoundExp::~CompoundExp()
{
    while (lhs != NULL)
    {
        Expression *temp = lhs;
        lhs = lhs->getLHS();
        delete temp;
    }
    while (rhs != NULL)
    {
        Expression *temp = rhs;
        rhs = rhs->getRHS();
        delete temp;
    }
}

// use for print syntax tree, use BFS, also note the current level
std::string CompoundExp::toString()
{
    int level = 0;
    std::string res = this->op + "\n";
    std::queue<Expression *> current_level;
    std::queue<Expression *> next_level;
    current_level.push(lhs);
    current_level.push(rhs);
    while (!current_level.empty())
    {
        Expression *temp = current_level.front();
        current_level.pop();
        if (temp == NULL)
        {
            throw std::runtime_error("Syntax tree is not complete");
        }
        if (temp->type() == COMPOUND)
        {
            next_level.push(temp->getLHS());
            next_level.push(temp->getRHS());
            res += addTabBefore(level + 1, temp->getOperator()) + "\n";
        }
        else if (temp->type() == CONSTANT || temp->type() == IDENTIFIER)
        {
            res += addTabBefore(level + 1, temp->toString()) + "\n";
        }
        if (current_level.empty())
        {
            level++;
            while (!next_level.empty())
            {
                current_level.push(next_level.front());
                next_level.pop();
            }
        }
    }
    return res;
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

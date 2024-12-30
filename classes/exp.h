// Declare/implement expressions

#pragma once

#include <string>
#include <stdexcept>
#include <cmath>
#include "tokenizer.h"
#include "evalstate.h"
using namespace std;

/* Forward reference */

class EvaluationContext;

/*
 * Type: ExpressionType
 * --------------------
 * This enumerated type is used to differentiate the three different
 * expression types: CONSTANT, IDENTIFIER, and COMPOUND.
 */

enum ExpressionType
{
    CONSTANT,
    IDENTIFIER,
    COMPOUND
};

/*
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Expression itself is an abstract class.  Every Expression object
 * is therefore created using one of the three concrete subclasses:
 * ConstantExp, IdentifierExp, or CompoundExp.
 */

class Expression
{

public:
    Expression();
    virtual ~Expression();
    virtual int eval(EvaluationContext &context) = 0;
    virtual std::string toString() = 0;
    virtual ExpressionType type() = 0;

    /* Getter methods for convenience */

    virtual int getConstantValue() = 0;
    virtual std::string getIdentifierName() = 0;
    virtual std::string getOperator() = 0;
    virtual Expression *getLHS() = 0;
    virtual Expression *getRHS() = 0;
};

/*
 * Class: ConstantExp
 * ------------------
 * This subclass represents a constant integer expression.
 */

class ConstantExp : public Expression
{

public:
    ConstantExp(int val);

    virtual int eval(EvaluationContext &context);
    virtual std::string toString();
    virtual ExpressionType type();

    virtual int getConstantValue();

private:
    int value;
};

/*
 * Class: IdentifierExp
 * --------------------
 * This subclass represents a expression corresponding to a variable.
 */

class IdentifierExp : public Expression
{

public:
    IdentifierExp(string name);

    virtual int eval(EvaluationContext &context);
    virtual std::string toString();
    virtual ExpressionType type();

    virtual string getIdentifierName();

private:
    std::string name;
};

/*
 * Class: CompoundExp
 * ------------------
 * This subclass represents a compound expression.
 */

class CompoundExp : public Expression
{

public:
    CompoundExp(string op, Expression *lhs, Expression *rhs);
    virtual ~CompoundExp();

    virtual int eval(EvaluationContext &context);
    virtual std::string toString();
    virtual ExpressionType type();

    virtual std::string getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();

private:
    std::string op;
    Expression *lhs, *rhs;
};
#include "evalstate.h"

void EvaluationContext::setValue(std::string var, int value)
{
    symbolTable[var] = value;
}

int EvaluationContext::getValue(std::string var)
{
    return symbolTable[var];
}

bool EvaluationContext::isDefined(std::string var)
{
    if (symbolTable.find(var) == symbolTable.end())
    {
        throw std::runtime_error("Undefined variable: " + var);
    }
    return true;
}

void EvaluationContext::clear()
{
    symbolTable.clear();
}
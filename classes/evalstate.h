/*
 * Class: EvaluationContext
 * ------------------------
 * This class encapsulates the information that the evaluator needs to
 * know in order to evaluate an expression.
 */

#pragma once

#include <string>
#include <map>
#include <stdexcept>

class EvaluationContext
{

public:
   void setValue(std::string var, int value);
   int getValue(std::string var);
   bool isDefined(std::string var);
   void clear();

private:
   std::map<std::string, int> symbolTable;
};

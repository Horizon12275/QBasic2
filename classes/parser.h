// Parse a given expression

#pragma once

#include "settings.h"
#include "statement.h"

class Parser
{

public:
    Parser(/* args */);
    ~Parser();
    Statement *parseStatement(string cmdInput);
};
// Parse a given expression

#pragma once

#include "settings.h"
#include "statement.h"
#include <stack>

class Parser
{

public:
    Statement *parseStatement(const vector<string> &tokens);
    RemStmt *parseRemStmt(const vector<string> &tokens);
    LetStmt *parseLetStmt(const vector<string> &tokens);
    PrintStmt *parsePrintStmt(const vector<string> &tokens);
    InputStmt *parseInputStmt(const vector<string> &tokens);
    GotoStmt *parseGotoStmt(const vector<string> &tokens);
    IfStmt *parseIfStmt(const vector<string> &tokens);
    EndStmt *parseEndStmt(const vector<string> &tokens);
    Expression *parseExpression(const vector<string> &tokens);

    bool varNameValid(const string &varName);
    bool isValidLineNum(const string &lineNum);
    bool isConstant(const string &str);
    bool isOperator(const string &str);
    bool isValidExpression(const vector<string> &tokens);
};
// Declare/implement statements

#pragma once

#include "settings.h"
#include "exp.h"

enum StatementType
{
    REM,
    LET,
    PRINT,
    INPUT,
    GOTO,
    IF,
    END
};

class Statement
{

public:
    int lineNum;
    Expression *rootExp;
    vector<string> tokens;

    Statement(int lineNum, vector<string> tokens);
    ~Statement();
    virtual std::string getSyntaxTree() = 0;
    virtual StatementType type() = 0;
    virtual std::string getStatementTypeString() = 0;
    std::string getContent();
};

class RemStmt : public Statement
{
public:
    std::string remContent;

    RemStmt(int lineNum, std::string remContent, vector<string> tokens);
    std::string getSyntaxTree();
    StatementType type();
    std::string getStatementTypeString();
};

class LetStmt : public Statement
{
public:
    LetStmt(int lineNum, vector<string> tokens);
    ~LetStmt();
    std::string getSyntaxTree();
    StatementType type();
    std::string getStatementTypeString();
};

class PrintStmt : public Statement
{
public:
    PrintStmt(int lineNum, vector<string> tokens);
    ~PrintStmt();
    std::string getSyntaxTree();
    StatementType type();
    std::string getStatementTypeString();
};

class InputStmt : public Statement
{
public:
    std::string varName;

    InputStmt(int lineNum, vector<string> tokens);
    std::string getSyntaxTree();
    StatementType type();
    std::string getStatementTypeString();
};

class GotoStmt : public Statement
{
public:
    int targetLineNum;

    GotoStmt(int lineNum, vector<string> tokens);
    std::string getSyntaxTree();
    StatementType type();
    std::string getStatementTypeString();
};

class IfStmt : public Statement
{
public:
    int targetLineNum;
    Expression *lhs;
    Expression *rhs;
    std::string cmpOp;

    IfStmt(int lineNum, vector<string> tokens);
    ~IfStmt();
    std::string getSyntaxTree();
    StatementType type();
    std::string getStatementTypeString();
};

class EndStmt : public Statement
{
public:
    EndStmt(int lineNum, vector<string> tokens);
    std::string getSyntaxTree();
    StatementType type();
    std::string getStatementTypeString();
};
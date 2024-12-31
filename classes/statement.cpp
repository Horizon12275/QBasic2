#include "statement.h"

/*
 * --------------------------------------------------------------
 * Implementation notes: Statement
 * --------------------------------------------------------------
 */

Statement::Statement(int lineNum, vector<string> tokens)
{
    this->lineNum = lineNum;
    this->tokens = tokens;
    this->rootExp = NULL;
}

// 遍历左右子树，析构所有节点
Statement::~Statement()
{
    delete rootExp;
}

std::string Statement::getContent()
{
    std::string content = "";
    for (size_t i = 1; i < tokens.size(); i++)
    {
        content += tokens[i] + " ";
    }
    return content;
}

/*
 * --------------------------------------------------------------
 * Implementation notes: RemStmt
 * --------------------------------------------------------------
 */

RemStmt::RemStmt(int lineNum, std::string remContent, vector<string> tokens) : Statement(lineNum, tokens)
{
    this->remContent = remContent;
    this->rootExp = NULL;
}

std::string RemStmt::getSyntaxTree()
{
    return std::to_string(lineNum) + " REM\n" + MYTAB + remContent + "\n";
}

StatementType RemStmt::type()
{
    return REM;
}

std::string RemStmt::getStatementTypeString()
{
    return "REM";
}

/*
 * --------------------------------------------------------------
 * Implementation notes: LetStmt
 * --------------------------------------------------------------
 */

LetStmt::LetStmt(int lineNum, vector<string> tokens) : Statement(lineNum, tokens)
{
    this->rootExp = NULL;
}

LetStmt::~LetStmt()
{
    delete rootExp;
}

std::string LetStmt::getSyntaxTree()
{
    return std::to_string(lineNum) + " LET " + rootExp->toString() + "\n";
}

StatementType LetStmt::type()
{
    return LET;
}

std::string LetStmt::getStatementTypeString()
{
    return "LET";
}

/*
 * --------------------------------------------------------------
 * Implementation notes: PrintStmt
 * --------------------------------------------------------------
 */

PrintStmt::PrintStmt(int lineNum, vector<string> tokens) : Statement(lineNum, tokens)
{
    this->rootExp = NULL;
}

PrintStmt::~PrintStmt()
{
    delete rootExp;
}

std::string PrintStmt::getSyntaxTree()
{
    return std::to_string(lineNum) + " PRINT\n" + addMYTabBeforeEachLine(rootExp->toString()) + "\n";
}

StatementType PrintStmt::type()
{
    return PRINT;
}

std::string PrintStmt::getStatementTypeString()
{
    return "PRINT";
}

/*
 * --------------------------------------------------------------
 * Implementation notes: InputStmt
 * --------------------------------------------------------------
 */

InputStmt::InputStmt(int lineNum, vector<string> tokens) : Statement(lineNum, tokens)
{
    this->rootExp = NULL;
    this->varName = "";
}

std::string InputStmt::getSyntaxTree()
{
    return std::to_string(lineNum) + " INPUT\n" + MYTAB + varName + "\n";
}

StatementType InputStmt::type()
{
    return INPUT;
}

std::string InputStmt::getStatementTypeString()
{
    return "INPUT";
}

/*
 * --------------------------------------------------------------
 * Implementation notes: GotoStmt
 * --------------------------------------------------------------
 */

GotoStmt::GotoStmt(int lineNum, vector<string> tokens) : Statement(lineNum, tokens)
{
    this->targetLineNum = -1;
}

std::string GotoStmt::getSyntaxTree()
{
    return std::to_string(lineNum) + " GOTO\n" + MYTAB + std::to_string(targetLineNum) + "\n";
}

StatementType GotoStmt::type()
{
    return GOTO;
}

std::string GotoStmt::getStatementTypeString()
{
    return "GOTO";
}

/*
 * --------------------------------------------------------------
 * Implementation notes: IfStmt
 * --------------------------------------------------------------
 */

IfStmt::IfStmt(int lineNum, vector<string> tokens) : Statement(lineNum, tokens)
{
    this->rootExp = NULL;
    this->targetLineNum = -1;
    this->lhs = NULL;
    this->rhs = NULL;
}

IfStmt::~IfStmt()
{
    delete lhs;
    delete rhs;
}

// TODO
std::string IfStmt::getSyntaxTree()
{
    std::string res = std::to_string(lineNum) + " IF THEN\n";
    res += addMYTabBeforeEachLine(lhs->toString()) + "\n";
    res += MYTAB + cmpOp + "\n";
    res += addMYTabBeforeEachLine(rhs->toString()) + "\n";
    res += MYTAB + std::to_string(targetLineNum) + "\n";
    return res;
}

StatementType IfStmt::type()
{
    return IF;
}

std::string IfStmt::getStatementTypeString()
{
    return "IF";
}

/*
 * --------------------------------------------------------------
 * Implementation notes: EndStmt
 * --------------------------------------------------------------
 */

EndStmt::EndStmt(int lineNum, vector<string> tokens) : Statement(lineNum, tokens)
{
    this->rootExp = NULL;
}

std::string EndStmt::getSyntaxTree()
{
    return std::to_string(lineNum) + " END" + "\n";
}

StatementType EndStmt::type()
{
    return END;
}

std::string EndStmt::getStatementTypeString()
{
    return "END";
}
#include "parser.h"

Statement *Parser::parseStatement(const vector<string> &tokens)
{
    if (tokens.empty())
    {
        throw std::runtime_error("Empty statement");
    }
    if (!isValidLineNum(tokens[0]))
    {
        throw std::runtime_error("Invalid line number: " + tokens[0]);
    }
    return NULL;
}

// 行号需要为不超过1000000的正整数，否则提示用户行号错误。
bool Parser::isValidLineNum(const string &lineNum)
{
    if (lineNum.empty())
    {
        return false;
    }
    for (char c : lineNum)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    int num = stoi(lineNum);
    if (num <= 0 || num >= 1000000)
    {
        return false;
    }
    return true;
}

// 判断变量名是否合法，变量名需要遵守C/C++的要求，即不能是关键字（比如IF），也不能以数字开头。例如1a和IF这样的变量名是非法的。
bool Parser::varNameValid(const string &varName)
{
    if (varName.empty())
    {
        return false;
    }
    if (find(KEYWORDS.begin(), KEYWORDS.end(), varName) != KEYWORDS.end())
    {
        return false;
    }
    if (!isalpha(varName[0]))
    {
        return false;
    }
    for (char c : varName)
    {
        if (!isalnum(c) && c != '_')
        {
            return false;
        }
    }
    return true;
}

bool Parser::isConstant(const string &str)
{
    if (str.empty())
    {
        return false;
    }
    for (char c : str)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

bool Parser::isOperator(const string &str)
{
    if (str.empty())
    {
        return false;
    }
    return str == "+" || str == "-" || str == "*" || str == "/" || str == "=" || str == "(" || str == ")" || str == "%" || str == "^";
}

// 一个expression里的token必须是一个合法的变量名、一个合法的常数、或者是一个合法的操作符，否则提示用户表达式错误。
bool Parser::isValidExpression(const vector<string> &tokens)
{
    if (tokens.empty())
    {
        return false;
    }
    if (tokens.size() == 1)
    {
        return isConstant(tokens[0]) || varNameValid(tokens[0]);
    }
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (!isConstant(tokens[i]) && !varNameValid(tokens[i]) && !isOperator(tokens[i]))
        {
            return false;
        }
    }
    return true;
}
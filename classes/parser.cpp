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
    if (tokens[1] == "REM")
    {
        return parseRemStmt(tokens);
    }
    if (tokens[1] == "LET")
    {
        return parseLetStmt(tokens);
    }
    if (tokens[1] == "PRINT")
    {
        return parsePrintStmt(tokens);
    }
    if (tokens[1] == "INPUT")
    {
        return parseInputStmt(tokens);
    }
    if (tokens[1] == "GOTO")
    {
        return parseGotoStmt(tokens);
    }
    if (tokens[1] == "IF")
    {
        return parseIfStmt(tokens);
    }
    if (tokens[1] == "END")
    {
        return parseEndStmt(tokens);
    }
    throw std::runtime_error("Invalid statement: " + tokens[1]);
    return NULL;
}

RemStmt *Parser::parseRemStmt(const vector<string> &tokens)
{
    if (tokens.size() < 3)
    {
        throw std::runtime_error("REM statement too short");
    }
    string remContent = "";
    for (size_t i = 2; i < tokens.size(); i++)
    {
        remContent += tokens[i] + " ";
    }
    return new RemStmt(stoi(tokens[0]), remContent, tokens);
}

LetStmt *Parser::parseLetStmt(const vector<string> &tokens)
{
    if (tokens.size() < 5)
    {
        throw std::runtime_error("LET statement too short");
    }
    if (!varNameValid(tokens[2]))
    {
        throw std::runtime_error("Invalid variable name: " + tokens[2]);
    }
    if (tokens[3] != "=")
    {
        throw std::runtime_error("Invalid LET statement");
    }
    vector<string> expTokens(tokens.begin() + 2, tokens.end());
    LetStmt *stmt = new LetStmt(stoi(tokens[0]), tokens);
    stmt->rootExp = parseExpression(expTokens);
    return stmt;
}

PrintStmt *Parser::parsePrintStmt(const vector<string> &tokens)
{
    if (tokens.size() < 3)
    {
        throw std::runtime_error("PRINT statement too short");
    }
    vector<string> expTokens(tokens.begin() + 2, tokens.end());
    PrintStmt *stmt = new PrintStmt(stoi(tokens[0]), tokens);
    stmt->rootExp = parseExpression(expTokens);
    return stmt;
}

InputStmt *Parser::parseInputStmt(const vector<string> &tokens)
{
    if (tokens.size() != 3)
    {
        throw std::runtime_error("INPUT statement length error");
    }
    if (!varNameValid(tokens[2]))
    {
        throw std::runtime_error("Invalid variable name: " + tokens[2]);
    }
    InputStmt *stmt = new InputStmt(stoi(tokens[0]), tokens);
    stmt->varName = tokens[2];
    return stmt;
}

// GOTO 后面是常量
GotoStmt *Parser::parseGotoStmt(const vector<string> &tokens)
{
    if (tokens.size() != 3)
    {
        throw std::runtime_error("GOTO statement length error");
    }
    if (!isValidLineNum(tokens[2]))
    {
        throw std::runtime_error("Invalid line number: " + tokens[2]);
    }
    GotoStmt *stmt = new GotoStmt(stoi(tokens[0]), tokens);
    stmt->targetLineNum = stoi(tokens[2]);
    return stmt;
}

IfStmt *Parser::parseIfStmt(const vector<string> &tokens)
{
    int n = tokens.size();
    if (tokens[n - 2] != "THEN" || !isValidLineNum(tokens[n - 1]))
    {
        throw std::runtime_error("Invalid IF statement");
    }
    // find the cmp operator
    int cmpIndex = -1;
    for (size_t i = 2; i < tokens.size() - 2; i++)
    {
        if (tokens[i] == "<" || tokens[i] == ">" || tokens[i] == "=")
        {
            cmpIndex = i;
            break;
        }
    }
    if (cmpIndex == -1)
    {
        throw std::runtime_error("Invalid IF statement");
    }
    vector<string> lhsTokens(tokens.begin() + 2, tokens.begin() + cmpIndex);
    vector<string> rhsTokens(tokens.begin() + cmpIndex + 1, tokens.end() - 2);
    IfStmt *stmt = new IfStmt(stoi(tokens[0]), tokens);
    stmt->lhs = parseExpression(lhsTokens);
    stmt->rhs = parseExpression(rhsTokens);
    stmt->cmpOp = tokens[cmpIndex];
    stmt->targetLineNum = stoi(tokens[n - 1]);
    return stmt;
}

EndStmt *Parser::parseEndStmt(const vector<string> &tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("END statement length error");
    }
    return new EndStmt(stoi(tokens[0]), tokens);
}

Expression *Parser::parseExpression(const vector<string> &tokens)
{
    if (!isValidExpression(tokens))
    {
        throw std::runtime_error("Invalid expression");
    }
    Expression *root = NULL;
    stack<Expression *> expStack;
    stack<string> opStack;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const string &token = tokens[i];
        if (isConstant(token))
        {
            expStack.push(new ConstantExp(stoi(token)));
        }
        else if (varNameValid(token))
        {
            expStack.push(new IdentifierExp(token));
        }
        else if (isOperator(token))
        {
            if (token == "+" || token == "-")
            {
                if (i == 0 || tokens[i - 1] == "(")
                {
                    expStack.push(new ConstantExp(0));
                }
            }
            while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(token))
            {
                string op = opStack.top();
                opStack.pop();
                if (expStack.size() < 2)
                {
                    throw std::runtime_error("Invalid expression");
                }
                Expression *rhs = expStack.top();
                expStack.pop();
                Expression *lhs = expStack.top();
                expStack.pop();
                expStack.push(new CompoundExp(op, lhs, rhs));
            }
            opStack.push(token);
        }
        else if (token == "(")
        {
            opStack.push(token);
        }
        else if (token == ")")
        {
            while (!opStack.empty() && opStack.top() != "(")
            {
                string op = opStack.top();
                opStack.pop();
                if (expStack.size() < 2)
                {
                    throw std::runtime_error("Invalid expression");
                }
                Expression *rhs = expStack.top();
                expStack.pop();
                Expression *lhs = expStack.top();
                expStack.pop();
                expStack.push(new CompoundExp(op, lhs, rhs));
            }
            if (opStack.empty())
            {
                throw std::runtime_error("Mismatched parentheses");
            }
            opStack.pop();
        }
        else
        {
            throw std::runtime_error("Invalid token: " + token);
        }
    }

    while (!opStack.empty())
    {
        string op = opStack.top();
        opStack.pop();
        if (expStack.size() < 2)
        {
            throw std::runtime_error("Invalid expression");
        }
        Expression *rhs = expStack.top();
        expStack.pop();
        Expression *lhs = expStack.top();
        expStack.pop();
        expStack.push(new CompoundExp(op, lhs, rhs));
    }

    if (expStack.size() != 1)
    {
        throw std::runtime_error("Invalid expression");
    }

    root = expStack.top();
    expStack.pop();

    return root;
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
    vector<string> KEYWORDS = {"REM", "LET", "PRINT", "INPUT", "GOTO", "IF", "THEN", "END", "RUN", "LOAD", "LIST", "CLEAR", "HELP", "QUIT"};
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
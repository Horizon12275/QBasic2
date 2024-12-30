#include "statement.h"

Statement::Statement(/* args */)
{
    this->lineNum = 0;
    this->content = "";
}

Statement::Statement(int lineNum, string content)
{
    this->lineNum = lineNum;
    this->content = content;
}

Statement::~Statement()
{
}

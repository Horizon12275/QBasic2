#include "editor.h"

Editor::Editor()
{
    this->code = map<int, Statement *>();
}

Editor::~Editor()
{
    for (auto stmt : this->code)
    {
        delete stmt.second;
    }
    this->code.clear();
}

void Editor::addStatement(int lineNum, Statement *stmt)
{
    this->code[lineNum] = stmt;
}

void Editor::removeStatement(int lineNum)
{
    this->code.erase(lineNum);
}

string Editor::getAllStatements()
{
    string allStmts = "";
    for (auto stmt : this->code)
    {
        allStmts += stmt.second->content + "\n";
    }
    return allStmts;
}
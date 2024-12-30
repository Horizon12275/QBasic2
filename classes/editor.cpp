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
    // If the line number already exists, remove the old statement and release the memory
    if (this->code.find(lineNum) != this->code.end())
    {
        delete this->code[lineNum];
        this->removeStatement(lineNum);
    }
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
        allStmts += std::to_string(stmt.second->lineNum) + " " + stmt.second->getContent() + "\n";
    }
    return allStmts;
}

void Editor::clear()
{
    for (auto stmt : this->code)
    {
        delete stmt.second;
    }
    this->code.clear();
}

std::string Editor::getAllSyntaxTrees()
{
    string allTrees = "";
    for (auto stmt : this->code)
    {
        allTrees += stmt.second->getSyntaxTree();
    }
    return allTrees;
}
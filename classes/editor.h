#pragma once

#include "settings.h"
#include "statement.h"

class Editor
{
public:
    map<int, Statement *> code;

    Editor();
    ~Editor();
    void addStatement(int lineNum, Statement *stmt);
    void removeStatement(int lineNum);
    string getAllStatements();
    string getAllSyntaxTrees();
    void clear();
};
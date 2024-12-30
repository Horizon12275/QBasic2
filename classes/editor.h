#pragma once

#include "settings.h"
#include "statement.h"

class Editor
{

private:
    /* data */
    map<int, Statement *> code;

public:
    Editor();
    ~Editor();

    void addStatement(int lineNum, Statement *stmt);
    void removeStatement(int lineNum);
    string getAllStatements();
    string getAllSyntaxTrees();
    void clear();
};
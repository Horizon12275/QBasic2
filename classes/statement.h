// Declare/implement statements

#pragma once

#include "settings.h"

class Statement
{
    friend class Editor;
    friend class MainWindow;

private:
    /* data */
    int lineNum;
    string content;

public:
    Statement(/* args */);
    Statement(int lineNum, string content);
    ~Statement();
    virtual void printSyntaxTree() = 0;
};
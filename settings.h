#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <QMessageBox>
#include <sstream>

using namespace std;

// vector<string> KEYWORDS = {"REM", "LET", "PRINT", "INPUT", "GOTO", "IF", "THEN", "END", "RUN", "LOAD", "LIST", "CLEAR", "HELP", "QUIT"};
// vector<string> COMMANDS = {"RUN", "LOAD", "LIST", "CLEAR", "HELP", "QUIT"};

#define HELPINFO "Commands:\n"                                                \
                 "\tRUN: \trun the code in the editor\n"                      \
                 "\tLOAD: \tload the code from a file\n"                      \
                 "\tLIST: \tlist all the statements in the editor\n"          \
                 "\tCLEAR: \tclear all the statements in the editor\n"        \
                 "\tHELP: \tshow the help information\n"                      \
                 "\tQUIT: \tquit the program\n"                               \
                 "\n"                                                         \
                 "Direct Keywords Usage:\n"                                   \
                 "\tLET: \tLET <variable> = <expression>\n"                   \
                 "\tPRINT: \tPRINT <expression>\n"                            \
                 "\tINPUT: \tINPUT <variable>\n"                              \
                 "\n"                                                         \
                 "Code Keywords Usage:\n"                                     \
                 "\tREM: \t<line number> REM <comment>\n"                     \
                 "\tLET: \t<line number> LET <variable> = <expression>\n"     \
                 "\tPRINT: \t<line number> PRINT <expression>\n"              \
                 "\tINPUT: \t<line number> INPUT <variable>\n"                \
                 "\tGOTO: \t<line number> GOTO <line number>\n"               \
                 "\tIF: \t<line number> IF <expression> THEN <line number>\n" \
                 "\tEND: \t<line number> END\n"
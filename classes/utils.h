#pragma once

#include <string>
#include <stdexcept>
#include <cmath>
#include <settings.h>

// The MOD operator has the same precedence as * and /. In the expression LET r = a MOD b,
// the absolute value of r should be less than the absolute value of b, and the sign of r is the
// same as that of b. For example, 5 MOD 3 is 2 and 5 MOD (-3) is -1.
int myMod(int a, int b);

std::string addTabBefore(int tabCnt, std::string str);

// 在每一行的开头加上一个MYTAB（每一行用\n分隔）
std::string addMYTabBeforeEachLine(std::string str);

int getPrecedence(std::string op);

string trim(const string &s);
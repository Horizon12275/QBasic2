#include "utils.h"

// The MOD operator has the same precedence as * and /. In the expression LET r = a MOD b,
// the absolute value of r should be less than the absolute value of b, and the sign of r is the
// same as that of b. For example, 5 MOD 3 is 2 and 5 MOD (-3) is -1.
int myMod(int a, int b)
{
    if (b == 0)
    {
        throw std::invalid_argument("Division by zero in myMod");
    }
    int result = a % b;
    if ((result > 0 && b < 0) || (result < 0 && b > 0))
    {
        result += b;
    }
    return result;
}

std::string addTabBefore(int tabCnt, std::string str)
{
    std::string tabStr = "";
    for (int i = 0; i < tabCnt; i++)
    {
        tabStr += MYTAB;
    }
    return tabStr + str;
}

int precedence(std::string op)
{
    if (op == "+" || op == "-")
    {
        return 1;
    }
    if (op == "*" || op == "/" || op == "%")
    {
        return 2;
    }
    if (op == "^")
    {
        return 3;
    }
    return 0;
}
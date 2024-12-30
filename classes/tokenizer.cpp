// Convert strings to a list of tokens

#include "tokenizer.h"
#include <regex>
#include <cctype>

Tokenizer::Tokenizer() {}

std::vector<std::string> Tokenizer::tokenize(const std::string &str)
{
    // Replace 'MOD' with '%'
    std::string modStr = std::regex_replace(str, std::regex("MOD"), "%");

    // Replace '**' with '^'
    std::string powStr = std::regex_replace(modStr, std::regex("\\*\\*"), "^");

    std::string finalStr = powStr;

    // Split the string into tokens
    std::vector<std::string> tokens;
    std::string token;
    for (size_t i = 0; i < finalStr.size(); ++i)
    {
        char c = finalStr[i];
        if (std::isspace(c))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
            continue; // Skip spaces
        }
        if (isOperator(c))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, c));
        }
        else
        {
            token += c;
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}

bool Tokenizer::isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || c == '%' || c == '^';
}

// Convert strings to a list of tokens
#pragma once

#include <string>
#include <vector>

class Tokenizer
{
public:
    Tokenizer();
    std::vector<std::string> tokenize(const std::string &str);

private:
    bool isOperator(char c);
    bool isDelimiter(char c);
};
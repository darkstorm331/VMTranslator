#pragma once

#include <string>
#include <fstream>

class Parser
{
private:
    std::ifstream fileStream;
    std::string currentLine;

public:
    Parser(std::string filename);
    ~Parser();

    bool hasMoreLines;
    std::string command;
    std::string arg1;
    int arg2;

    void Advance();
    void Close();
};
#pragma once
#include <iostream>
#include <fstream>
#include <string>

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

    Parser* MakeParser(std::string filename);
};
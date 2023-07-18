#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

class CodeWriter
{
private:
    std::string filepath;
    std::string filename;
    std::ofstream fileStream;
    int labelCounter;

    void AddSubAndOr(std::string command, std::string op);
    void NegNot(std::string command, std::string op);
    void EqGtLt(std::string command, std::string op);
    void PushConstant(int index);
    void PushLclArgThisThat(std::string segment, int index);
    void PushStatic(int index);
    void PushTemp(int index);
    void PushPointer(int index);
    void PopLclArgThisThat(std::string segment, int index);
    void PopStatic(int index);
    void PopTemp(int index);
    void PopPointer(int index);
    void Label(std::string label);
    void GoTo(std::string label);
    void IfGoTo(std::string label);

public:
    CodeWriter(std::string filepath, std::string outfile);
    ~CodeWriter();

    void WriteInit();
    void WriteOut(std::string command, std::string arg1, int arg2);
    void Close();
};

#pragma once

#include <iostream>
#include <string>
#include <fstream>

class CodeWriter
{
private:
    std::string filepath;
    std::string filename;
    std::ofstream fileStream;
    int labelCounter;
    int functionReturnCounter;
    std::string currentFunction;
    bool IsSingleFile;
    int currentArgs;

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
    void Function(std::string functionName, int args);
    void Call(std::string functionName, int args);
    void FuncReturn();

public:
    CodeWriter(std::string filepath, std::string outfile, bool isSingleFile);
    ~CodeWriter();

    void WriteInit();
    void ChangeInFile(std::string file);
    void WriteOut(std::string command, std::string arg1, int arg2);
    void Close();
};

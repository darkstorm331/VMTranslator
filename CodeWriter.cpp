#include "CodeWriter.h"
#include <string>
#include <fstream>
#include <regex>

CodeWriter::CodeWriter(std::string filepath)
{
    filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    filename = filename.substr(0, filename.find("."));

    std::string outFile = std::regex_replace(filepath, std::regex(".vm"), ".asm");

    fileStream.open(outFile, std::fstream::in);

    labelCounter = 0;
}

CodeWriter::~CodeWriter()
{
}

void CodeWriter::WriteOut(std::string command, std::string arg1, int arg2) {
    if(command == "push") {
        if(arg1 == "local" || arg1 == "argument" || arg1 == "this" || arg1 == "that") {
            PushLclArgThisThat(arg1, arg2);
        } else if(arg1 == "constant") {
            PushConstant(arg2);
        } else if(arg1 == "static") {
            PushStatic(arg2);
        } else if(arg1 == "temp") {
            PushTemp(arg2);
        } else if(arg1 == "pointer") {
            PushPointer(arg2);
        }
    } else if(command == "pop") {
        if(arg1 == "local" || arg1 == "argument" || arg1 == "this" || arg1 == "that") {
            PopLclArgThisThat(arg1, arg2);
        } else if(arg1 == "static") {
            PopStatic(arg2);
        } else if(arg1 == "temp") {
            PopTemp(arg2);
        } else if(arg1 == "pointer") {
            PopPointer(arg2);
        }
    } else if(command == "add") {
        AddSubAndOr(arg1, "+");
    } else if(command == "sub") {
        AddSubAndOr(arg1, "-");
    } else if(command == "and") {
        AddSubAndOr(arg1, "&");
    } else if(command == "or") {
        AddSubAndOr(arg1, "|");
    } else if(command == "neg") {
        NegNot(arg1, "-");
    } else if(command == "not") {
        NegNot(arg1, "!");
    } else if(command == "eq") {
        EqGtLt(arg1, "JEQ");
    } else if(command == "gt") {
        EqGtLt(arg1, "JGT");
    } else if(command == "lt") {
        EqGtLt(arg1, "JLT");
    }
}

void CodeWriter::Close() {
    fileStream.close();
}

void CodeWriter::PushLclArgThisThat(std::string segment, int index) {
    std::string regis = "";

    if(segment == "local") {
        regis = "LCL";
    } else if(segment == "argument") {
        regis = "ARG";
    } else if (segment == "this") {
        regis = "THIS";
    } else if(segment == "that") {
        regis = "THAT";
    } else {
        regis = "";
    }

    fileStream << "// push " << segment << " " << index << std::endl;
    fileStream << "@" << index << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@" << regis << std::endl;
    fileStream << "A=M+D" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
}

void CodeWriter::PushConstant(int index) {  
   fileStream << "// push constant " << index << std::endl;
   fileStream << "@" << index << std::endl;
   fileStream << "D=A" << std::endl;
   fileStream << "@SP" << std::endl;
   fileStream << "A=M" << std::endl;
   fileStream << "M=D" << std::endl;
   fileStream << "@SP" << std::endl;
   fileStream << "M=M+1" << std::endl;
}

void CodeWriter::PushStatic(int index) {
   fileStream << "// push static " << index << std::endl;
   fileStream << "@" << filename << "." << index << std::endl;
   fileStream << "D=M" << std::endl;
   fileStream << "@SP" << std::endl;
   fileStream << "A=M" << std::endl;
   fileStream << "M=D" << std::endl;
   fileStream << "@SP" << std::endl;
   fileStream << "M=M+1" << std::endl;
}

void CodeWriter::PushTemp(int index) {
    fileStream << "// push temp " << index << std::endl;
    fileStream << "@5" << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@" << index << std::endl;
    fileStream << "D=D+A" << std::endl;
    fileStream << "A=D" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
}

void CodeWriter::PushPointer(int index) {
    std::string access = index == 0 ? "THIS" : "THAT";

    fileStream << "// push pointer " << index << std::endl;
    fileStream << "@" << access << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;   
}

void CodeWriter::PopLclArgThisThat(std::string segment, int index) {
    std::string regis = "";

    if(segment == "local") {
        regis = "LCL";
    } else if(segment == "argument") {
        regis = "ARG";
    } else if (segment == "this") {
        regis = "THIS";
    } else if(segment == "that") {
        regis = "THAT";
    } else {
        regis = "";
    }

    fileStream << "// pop " << segment << " " << index << std::endl;
    fileStream << "@" << regis << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@" << index << std::endl;
    fileStream << "D=D+A" << std::endl;
    fileStream << "@R13" << std::endl;    
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl; 
    fileStream << "A=M" << std::endl; 
    fileStream << "D=M" << std::endl; 
    fileStream << "@R13" << std::endl; 
    fileStream << "A=M" << std::endl; 
    fileStream << "M=D" << std::endl; 
}

void CodeWriter::PopStatic(int index) {
    fileStream << "// pop static " << index << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@" << filename << "." << index << std::endl;    
    fileStream << "M=D" << std::endl;
}

void CodeWriter::PopTemp(int index) {
    fileStream << "// pop temp " << index << std::endl;
    fileStream << "@5" << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@" << index << std::endl;
    fileStream << "D=D+A" << std::endl;
    fileStream << "@R13" << std::endl;  
    fileStream << "M=D" << std::endl;    
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@R13" << std::endl;    
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;    
}

void CodeWriter::PopPointer(int index) {
    std::string access = index == 0 ? "THIS" : "THAT";

    fileStream << "// pop pointer " << index << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@" << access << std::endl;    
    fileStream << "M=D" << std::endl;    
}

void CodeWriter::AddSubAndOr(std::string command, std::string op) {
    fileStream << "// " << command << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@R13" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@R13" << std::endl;
    fileStream << "D=D" << op << "M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
}

void CodeWriter::NegNot(std::string command, std::string op) {    
    fileStream << "// " << command << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=D" << op << "M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
}

void CodeWriter::EqGtLt(std::string command, std::string op) {
    std::string trueLabel = "_TRUE_" + std::to_string(labelCounter);
    std::string falseLabel = "_FALSE_" + std::to_string(labelCounter);
    std::string endLabel = "_END_" + std::to_string(labelCounter);

    fileStream << "// " << command << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@R13" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@R13" << std::endl;
    fileStream << "D=D-M" << std::endl;
    fileStream << "@" << trueLabel << std::endl;
    fileStream << "D ; " << op << std::endl;
    fileStream << "@" << falseLabel << std::endl;
    fileStream << "0 ; JMP" << std::endl;
    fileStream << "(" << trueLabel << ")" << std::endl;
    fileStream << "D=-1" << std::endl;
    fileStream << "@" << endLabel << std::endl;
    fileStream << "0 ; JMP" << std::endl;
    fileStream << "(" << falseLabel << ")" << std::endl;
    fileStream << "D=0" << std::endl;
    fileStream << "@" << endLabel << std::endl;
    fileStream << "0 ; JMP" << std::endl;
    fileStream << "(" << endLabel << ")" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;

    labelCounter++;
}
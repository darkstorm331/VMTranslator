#include "CodeWriter.h"
#include <fstream>

CodeWriter::CodeWriter(std::string filepath, std::string outfile)
{
    filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    filename = filename.substr(0, filename.find("."));
    
    fileStream.open(outfile);

    labelCounter = 0;
    functionReturnCounter = 0;

    currentFunction = "";

    WriteInit();
}

CodeWriter::~CodeWriter()
{
}

void CodeWriter::WriteInit() {
    fileStream << "// init " << std::endl;
    fileStream << "@256" << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@0" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@300" << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@1" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@400" << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@2" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@3000" << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@3" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@3010" << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@4" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;

    currentFunction = "Bootstrap";
    Call("Sys.init", 0);

    //fileStream << "@Sys.init" << std::endl;
    //fileStream << "0 ; JMP" << std::endl;
}

void CodeWriter::ChangeInFile(std::string filepath) {
    filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    filename = filename.substr(0, filename.find("."));
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
        AddSubAndOr(command, "+");
    } else if(command == "sub") {
        AddSubAndOr(command, "-");
    } else if(command == "and") {
        AddSubAndOr(command, "&");
    } else if(command == "or") {
        AddSubAndOr(command, "|");
    } else if(command == "neg") {
        NegNot(command, "-");
    } else if(command == "not") {
        NegNot(command, "!");
    } else if(command == "eq") {
        EqGtLt(command, "JEQ");
    } else if(command == "gt") {
        EqGtLt(command, "JGT");
    } else if(command == "lt") {
        EqGtLt(command, "JLT");
    } else if(command == "label") {
        Label(arg1);
    } else if(command == "goto") {
        GoTo(arg1);
    } else if(command == "if-goto") {
        IfGoTo(arg1);
    } else if(command == "function") {
        Function(arg1, arg2);
    } else if(command == "call") {
        Call(arg1, arg2);
    } else if(command == "return") {
        FuncReturn();
    } else {
        return;        
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
    fileStream << "D=" << op << "M" << std::endl;
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

void CodeWriter::Label(std::string labelText) {
    std::string labelFull = currentFunction + "$" + labelText;

    fileStream << "(" << labelFull << ")" << std::endl;
}

void CodeWriter::Function(std::string functionName, int args) {
    currentFunction = functionName;

    fileStream << "(" << functionName << ")" << std::endl;
    
    for(int i = 0; i < args; i++) {
        PushConstant(0);
    }       
}

void CodeWriter::Call(std::string functionName, int args) {    
    std::string goToFunction = functionName;
    std::string returnLabel = currentFunction + "$ret." + std::to_string(functionReturnCounter);

    fileStream << "// call " << functionName << args << std::endl;
    fileStream << "@" << returnLabel << std::endl;
    fileStream << "D=A" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
    fileStream << "@LCL" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
    fileStream << "@ARG" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
    fileStream << "@THIS" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
    fileStream << "@THAT" << std::endl; 
    fileStream << "D=M" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M+1" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@5" << std::endl;
    fileStream << "D=D-A" << std::endl;
    fileStream << "@" << args << std::endl;
    fileStream << "D=D-A" << std::endl;
    fileStream << "@ARG" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@LCL" << std::endl;
    fileStream << "M=D" << std::endl;
    fileStream << "@" << goToFunction << std::endl;
    fileStream << "0 ; JMP" << std::endl;
    fileStream << "(" << returnLabel << ")" << std::endl;

    functionReturnCounter++;
}

void CodeWriter::FuncReturn() {
    fileStream << "// return" << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;

    fileStream << "@ARG" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "M=D" << std::endl; 

    fileStream << "@ARG" << std::endl;
    fileStream << "D=M" << std::endl;
  
    fileStream << "@SP" << std::endl;   
    fileStream << "M=D+1" << std::endl;

    fileStream << "@LCL" << std::endl;   
    fileStream << "D=M" << std::endl;
    fileStream << "@R14" << std::endl;
    fileStream << "M=D-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;   
    fileStream << "@THAT" << std::endl;
    fileStream << "M=D" << std::endl;

    fileStream << "@R14" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@THIS" << std::endl;
    fileStream << "M=D" << std::endl;

    fileStream << "@R14" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@ARG" << std::endl;
    fileStream << "M=D" << std::endl;

    fileStream << "@R14" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@LCL" << std::endl;
    fileStream << "M=D" << std::endl;


    fileStream << "@R14" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "A=D" << std::endl;
    fileStream << "0 ; JMP" << std::endl;
}

void CodeWriter::GoTo(std::string label) {
    std::string labelFull = currentFunction + "$" + label;

    fileStream << "// goto " << label << std::endl;
    fileStream << "@" << labelFull << std::endl;
    fileStream << "0 ; JMP" << std::endl;
}

void CodeWriter::IfGoTo(std::string label) {
    std::string labelFull = currentFunction + "$" + label;

    fileStream << "// if-goto " << label << std::endl;
    fileStream << "@SP" << std::endl;
    fileStream << "M=M-1" << std::endl;
    fileStream << "A=M" << std::endl;
    fileStream << "D=M" << std::endl;
    fileStream << "@" << labelFull << std::endl;
    fileStream << "D ; JNE" << std::endl;
}
#include "Parser.h"
#include <iostream>
#include <fstream>
#include <string>

Parser::Parser(std::string filename)
{    
    fileStream.open(filename);
    hasMoreLines = true;
}

Parser::~Parser()
{
}

void Parser::Advance() {
    std::string delimiter = " ";

    if(fileStream.is_open()) {
        if(fileStream.peek()!=EOF) {
            std::getline (fileStream, currentLine);
            
            if(currentLine.find("//") == std::string::npos) {
                command = currentLine.substr(0, currentLine.find(delimiter));
            } else {
                command = "";
            }
        } else {
            hasMoreLines = false;
        }
    }
}

void Parser::Close() {
    fileStream.close();
}

Parser* Parser::MakeParser(std::string filename) {
    return new Parser(filename);
}

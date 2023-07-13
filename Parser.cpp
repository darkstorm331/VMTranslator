#include "Parser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

Parser::Parser(std::string filename)
{    
    fileStream.open(filename);
    hasMoreLines = true;
}

Parser::~Parser()
{
}

void Parser::Advance() {
    char delimiter = ' ';

    if(fileStream.is_open() && hasMoreLines) {
        if(fileStream.peek() != EOF) {
            std::vector<std::string> commandSegments;
            std::string currentLine;            
            std::getline(fileStream, currentLine);

            if(currentLine.find("//") == std::string::npos) {
                std::stringstream ss(currentLine);
                std::string token;
                while (getline(ss, token, ' ')) {                   
                    // store token string in the vector
                    commandSegments.push_back(token);
                }

                switch (commandSegments.size())
                {
                case 0:
                    break;
                
                case 3:
                    arg2 = std::stoi(commandSegments.at(2));

                case 2:
                    arg1 = commandSegments.at(1);

                case 1:
                    command = commandSegments.at(0);

                default:
                    break;
                }
            } else {
                command.clear();
                arg1.clear();
                arg2 = -999;
                return;
            }
        } else {
            hasMoreLines = false;
            command.clear();
            arg1.clear();
            arg2 = -999;
            return;
        }
    }
}

void Parser::Close() {
    fileStream.close();
}

Parser* Parser::MakeParser(std::string filename) {
    return new Parser(filename);
}

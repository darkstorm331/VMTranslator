#include "Parser.h"
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

            if(currentLine.find("//") > 0) {
                std::stringstream ss(currentLine);
                std::string token;
                while (getline(ss, token, ' ')) {                   
                    // store token string in the vector
                    commandSegments.push_back(token);
                }

                int commandSize = commandSegments.size();

                if(commandSize > 3) {
                    if(commandSegments.at(2) == "" || commandSegments.at(2) == "//") {
                        if(commandSegments.at(1) == "") {
                            if(commandSegments.at(0) == "") {
                                commandSize = 0;
                            } else {
                                commandSize = 1;
                            }
                        } else {
                            commandSize = 2;
                        }
                    } else {
                        commandSize = 3;
                    }
                }

                switch (commandSize)
                {
                case 0:
                    command.clear();
                    arg1.clear();
                    arg2 = -999;
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
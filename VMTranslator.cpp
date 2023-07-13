#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include "Parser.h"

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    std::copy(argv + 1, argv + argc, std::back_inserter(args));    

    //Check that number of arguments is 2 (First argument is the directory of exe)
    if(argc != 2) {
        printf("Incorrect number of arguments provided. Expected: 1, Actual %d", argc-1);
        return -1;
    } 

    //Check that the file is a .vm file
    std::filesystem::path fpath = args[0];

    if(fpath.extension() != ".vm") {
        printf("Provided file is not a .vm file");
        return -1;
    }

    //Get cracking
    Parser p(args[0]);

    while(p.hasMoreLines) {
        p.Advance();

        if(p.command.length() > 0) {
            
        } 
    }

    p.Close();

    return 0;
}

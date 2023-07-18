#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include "Parser.h"
#include "CodeWriter.h"
#include <regex>
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    std::copy(argv + 1, argv + argc, std::back_inserter(args));    

    //Check that number of arguments is 2 (First argument is the directory of exe)
    if(argc != 2) {
        printf("Incorrect number of arguments provided. Expected: 1, Actual %d", argc-1);
        return -1;
    } 

    std::error_code ec; // For using the non-throwing overloads of functions below.
    if (fs::is_directory(args[0], ec))
    { 
        
        // Process a directory.
    }
    if (ec) // Optional handling of possible errors.
    {
        std::cerr << "Error in is_directory: " << ec.message();
    }
    if (fs::is_regular_file(args[0], ec))
    {
        std::filesystem::path fpath = args[0];

        if(fpath.extension() == ".vm") {
            Parser p(args[0]);
            CodeWriter cw(args[0], std::regex_replace(args[0], std::regex(".vm"), ".asm"));

            while(p.hasMoreLines) {
                p.Advance();

                if(p.command.length() > 0) {
                    cw.WriteOut(p.command, p.arg1, p.arg2);           
                } 
            }

            p.Close();
            cw.Close();
        } 
    }
    if (ec) // Optional handling of possible errors. Usage of the same ec object works since fs functions are calling ec.clear() if no errors occur.
    {
        std::cerr << "Error in is_regular_file: " << ec.message();
    }

    return 0;
}

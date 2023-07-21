#include <filesystem>
#include "Parser.h"
#include "CodeWriter.h"
#include <regex>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    std::copy(argv + 1, argv + argc, std::back_inserter(args));    

    //Check that number of arguments is 2 (First argument is the directory of exe)
    if(argc != 2) 
    {
        printf("Incorrect number of arguments provided. Expected: 1, Actual %d", argc-1);
        return -1;
    } 

    std::error_code ec; // For using the non-throwing overloads of functions below.
    if (fs::is_directory(args[0], ec))
    {     
        //std::string outFileName = args[0].substr(args[0].find_last_of("/\\") + 1); 
        std::string outFileName = args[0].substr(args[0].find_last_of("/") + 1);
        //CodeWriter cw("", args[0] + "\\" + outFileName + ".asm", false);
        CodeWriter cw("", args[0] + "/" + outFileName + ".asm", false);
   
        for (auto &p : fs::recursive_directory_iterator(args[0]))
        {
            if (p.path().extension() == ".vm") 
            {             
                std::string path = p.path().string();

                Parser parser(path);
                cw.ChangeInFile(path);

                while(parser.hasMoreLines) {
                    parser.Advance();

                    if(parser.command.length() > 0) 
                    {
                        cw.WriteOut(parser.command, parser.arg1, parser.arg2);           
                    } 
                } 

                parser.Close();               
            }
        }

        cw.Close();
    }
    if (ec) // Optional handling of possible errors.
    {
        std::cerr << "Error in is_directory: " << ec.message();
    }
    if (fs::is_regular_file(args[0], ec))
    {
        std::filesystem::path fpath = args[0];

        if(fpath.extension() == ".vm") 
        {
            Parser p(args[0]);
            CodeWriter cw(args[0], std::regex_replace(args[0], std::regex(".vm"), ".asm"), true);

            while(p.hasMoreLines) 
            {
                p.Advance();

                if(p.command.length() > 0) 
                {
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

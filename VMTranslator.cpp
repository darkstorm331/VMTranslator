#include <stdio.h>

int main(int argc, char* argv[])
{
    //Check that number of arguments is 2 (First argument is the directory of exe)
    printf("args: %d\n", argc);

    if(argc != 2) {
        printf("Incorrect number of arguments provided. Expected: 1, Actual %d", argc-1);
        return -1;
    } else {
        printf("hello");
    }



    return 0;
}

#include "main.h"


char buf[MAX_BUF_SIZE];


int main(int argc, char *argv[])
{
    int iter = 0;
    char ch;

    if(argc < 2)
    {
        printf("%s", USAGE);
        return -1;
    }

    FILE *fptr = fopen(argv[1], "r");  
    if (!fptr)
    {
        printf("Failed to open File\n");
        return 0;
    }



    printf("~~~~~~~LEXICAL ANALYSER~~~~~~~");
    printf("\n");

    while ((ch = fgetc(fptr)) != EOF)
    {
        if(iter < sizeof(buf)-1)
            buf[iter++] = ch;
    }
    buf[iter]='\0';


    fclose(fptr);
    lexical_analyzer();
    
    printf("\nToken Generated\n");

    return 0;
}
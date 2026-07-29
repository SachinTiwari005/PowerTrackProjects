#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "mp3.h"

int main(int argc, char *argv[])
{

    FILE *fptr;

    if (argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        char *ext = strrchr(argv[2], '.');

        if (ext != NULL && strcmp(ext, ".mp3") == 0) // MODE: View
        {
            fptr = mp3_check(argv[2], 0);

            if (fptr == NULL)
            {
                return -1;
            }
            else
            {
                // to view
                mp3view(fptr);
                return 0;
            }
        }
        else
        {
            printf("%s\n", ERROR_USAGE);
            return -1;
        }
    }
    else if (argc == 5 && strcmp(argv[1], "-e") == 0)
    {
        char *ext = strrchr(argv[4], '.');

        if (ext != NULL && strcmp(ext, ".mp3") == 0) // MODE: EDIT
        {
            fptr = mp3_check(argv[4], 1);

            if (fptr == NULL)
            {
                return -1;
            }
            else
            {
                unsigned int text_len = strlen(argv[3]) + 1;
                char change_text[text_len];

                strcpy(change_text, argv[3]);
                change_text[text_len - 1] = '\0';

                if(mp3edit(argv[2], change_text, fptr) == -1)
                {
                    return -1;
                }
                return 0;
            }
        }
        else
        {
            printf("%s\n", ERROR_USAGE);
            return -1;
        }
    }
    else if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        printf("%s\n", HELP_MENU);
    }
    else
    {
        printf("%s\n", ERROR_USAGE);
    }

    return 0;
}



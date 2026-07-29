#ifndef MP3_H
#define MP3_H

#define ERROR_USAGE "--------------------------------------------------------------------------------------\n" \
                    "ERROR: ./a.out : Invalid Arguments.\n"                                                    \
                    "USAGE:\n"                                                                                 \
                    "To view, please pass like:  ./a.out -v <filename.mp3>\n"                                  \
                    "To Edit, please pass like:  ./a.out -e -t/-a/-A/-m/-y/-c changing_text <filename.mp3>\n"  \
                    "To get help pass like: ./a.out --help\n"                                                  \
                    "---------------------------------------------------------------------------------------\n"

#define HELP_MENU "------------------------------------HELP MENU---------------------------------------------\n" \
                  "1. -v -> to view mp3 file contents\n"                                                         \
                  "1. -e -> to edit mp3 file contents\n"                                                         \
                  "         2.1. -t -> to edit song title\n"                                                     \
                  "         2.2. -a -> to edit artist name\n"                                                    \
                  "         2.3. -A -> to edit album name\n"                                                     \
                  "         2.4. -y -> to edit year\n"                                                           \
                  "         2.5. -m -> to edit content\n"                                                        \
                  "         2.6. -c -> to edit comment\n"                                                        \
                  "-----------------------------------------------------------------------------------------\n"

#define VIEW_OUTPUT "------------------------------------SELECTED VIEW DETAILS---------------------------------------------\n\n\n" \
                    "------------------------------------------------------------------------------------------------------\n"     \
                    "                             MP3 TAG READER AND EDITOR FOR ID3v2\n"                                           \
                    "------------------------------------------------------------------------------------------------------\n"

#define EDIT_DETAIL "------------------------------------SELECTED EDIT DETAILS---------------------------------------------\n\n\n" \
                    "---------------------SELECTED EDIT OPTION----------------------\n\n"

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

FILE *mp3_check(char fname[], int mode);
int mp3view(FILE *fptr);
int mp3edit(char *option, char text[], FILE *fptr);



#endif
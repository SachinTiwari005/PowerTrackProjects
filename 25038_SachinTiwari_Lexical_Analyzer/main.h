#ifndef MAIN_H
#define MAIN_H

#define MAX_BUF_SIZE        10000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define USAGE       "\nGive .c file along with executable file.\n    \
                    Usage:-  ./lexi.out <filename.c>\n      \
                    Note: keep .c file in the same directory.\n"


 void lexical_analyzer(void);


#endif
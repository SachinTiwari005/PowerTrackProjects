#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

const char *res_keywords[] = {"const", "volatile", "extern", "auto", "register", "static",  \
                                "signed", "unsigned", "short", "long", "double", "char",    \
                                "float", "int", "struct", "union", "enum", "void", "typedef"};

const char *res_keywords_nondata[] = {"break", "case", "continue",                      \
                                        "default", "do", "else", "for", "goto", "if",   \
                                        "return", "sizeof", "switch", "while"};

const char operators[] = {'/', '+', '*', '-', '%', '=', '<', '>', '~', '&', ',', '!', '^', '|'};
const char symbols[] = {'(', ')', '{', '}', '[', ']', ':', ';'};


void lexical_analyzer();
int isKeyword(char *word_str);
int isOperator(char ch);
int isSymbol(char ch);
int braces_err_check(int line, int col, char buf[], int iter);
int bracket_err_check(int line, int col, char buf[], int iter);
int parenthesis_err_check(int line, int col, char buf[], int iter);
int float_err_check(int line, int col, char *word_str);
int integer_err_check(int line, int col, char *word_str);

#endif
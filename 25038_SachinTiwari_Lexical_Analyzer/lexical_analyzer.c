#include "main.h"
#include "lexical_analyzer.h"

extern char buf[MAX_BUF_SIZE];

int isKeyword(char *word_str)
{
    int n = sizeof(res_keywords) / sizeof(res_keywords[0]);

    for (int iter = 0; iter < n; iter++)
    {
        if (strcmp(word_str, res_keywords[iter]) == 0)
            return 1;
    }

    n = sizeof(res_keywords_nondata) / sizeof(res_keywords_nondata[0]);

    for (int iter = 0; iter < n; iter++)
    {
        if (strcmp(word_str, res_keywords_nondata[iter]) == 0)
            return 2;
    }

    return 0;
}

int isOperator(char ch)
{
    int n = sizeof(operators) / sizeof(operators[0]);

    for (int iter = 0; iter < n; iter++)
    {
        if (operators[iter] == ch)
            return 1;
    }

    return 0;
}

int isSymbol(char ch)
{
    int n = sizeof(symbols) / sizeof(symbols[0]);

    for (int iter = 0; iter < n; iter++)
    {
        if (symbols[iter] == ch)
            return 1;
    }

    return 0;
}



void lexical_analyzer()
{
    char word_str[1000];
    int line = 1, col = 0;
    int j = 0;
    char ch;
    int iter=0;

    while(buf[iter] != '\0')
    {
        ch = buf[iter];
        col++;

        if(isspace(ch))  //whitespace  (space/tab/newline)
        {
            if(ch == '\n')
            {
                line++;
                col = 0;
            }

            iter++;
            col++;
            continue;
        }


        //Preprocessor 
        if (ch == '#')
        {
            while (buf[iter] != '\n' && buf[iter] != '\0')  //skip entire line
            {
                iter++;
                col++;
            }

            iter++;
            col++;
            continue;
        }
        
        //Comments
        if (ch == '/' && buf[iter + 1] == '/')  //single line comment
        {
            iter = iter + 2;
            col = col + 2;

            while (buf[iter] != '\n' &&  buf[iter] != '\0')  //check termination and also skip till new line
            {
                iter++;
                col++;
            }

            iter++;
            col++;
            continue;
        }
        else if (ch == '/' && buf[iter + 1] == '*')  //multiline comment
        {
            iter = iter + 2;
            col = col + 2;

            while (!(buf[iter] == '*' && buf[iter+1] == '/'))  //skip whole comment 
            {
                if (buf[iter] == '\n')
                {
                    line++;
                    col = 0;
                }
                else
                    col++;


                if (buf[iter] == '\0')  //check for termination
                {
                    printf("Line:%d\tColumn:%d: error: unterminated comment\n", line, col);

                    return;
                }


                iter++;
            }

            //skip end of the comment char ---> '/'
            iter = iter + 2;
            col = col + 2;
            continue;
        }

        //String Literals
        if (ch == '"')
        {
            j = 0;
            iter++;
            col++;

            word_str[j++] = ch;


            while (buf[iter] != '"' && buf[iter] != '\n' && buf[iter] != '\0')
            {
                if (j < sizeof(word_str) - 1)
                {
                    word_str[j++] = buf[iter];
                }

                iter++;
                col++;
            }

            if (buf[iter] == '"')
            {
                if (j < sizeof(word_str) - 1)
                {
                    word_str[j++] = buf[iter];
                }

                word_str[j] = '\0';

                printf("%-22s: %s\n","String Literal", word_str);
                
                iter++;
                col++;
                continue;
            }
            else
            {
                printf("Line:%d\tColumn:%d: error: missing terminating '\"' character\n", line, col);

                return;
            }
        }

        //Character Literals
        if (ch == '\'')
        {
            j = 0;

            word_str[j++] = ch;

            iter++;
            col++;

            
            if (buf[iter] == '\\')       //escape sequence
            {
                if (j < sizeof(word_str) - 1)
                {
                    word_str[j++] = buf[iter];
                }

                iter++;
                col++;
            }

            
            if (buf[iter] != '\0' && buf[iter] != '\n')   // Actual character
            {
                if (j < sizeof(word_str) - 1)
                {
                    word_str[j++] = buf[iter];
                }

                iter++;
                col++;
            }

            
            if (buf[iter] == '\'')       // Closing --> '
            {
                if (j < sizeof(word_str) - 1)
                {
                    word_str[j++] = buf[iter];
                }

                word_str[j] = '\0';

                printf("%-22s: %s\n","Character Literal", word_str);

                iter++;
                col++;
                continue;
            }
            else
            {
                printf("Line:%d\tColumn:%d: error: missing terminating '\'' character\n", line, col);

                return;
            }
        }


        // Letter --> Identifier, Keyword
        if (isalpha(ch) || ch == '_')
        {
            j = 0;

            while (isalnum(buf[iter]) || buf[iter] == '_')
            {
                if (j < sizeof(word_str) - 1)
                {
                    word_str[j++] = buf[iter];
                }

                iter++;
                col++;
            }

            word_str[j] = '\0';

            iter--;
            col--;

            //Checking the Keyword using array
            if (isKeyword(word_str))
                printf("%-22s: %s\n","Keyword", word_str);
            else
                printf("%-22s: %s\n","Identifier", word_str);

            iter++;
            col++;
            continue;
        }


        //Digit
        if (isdigit(ch))
        {
            int dot = 0;
            j = 0;

            while (isdigit(buf[iter]) || buf[iter] == '.')
            {
                if (buf[iter] == '.')
                {
                    dot++;
                }

                if (j < sizeof(word_str) - 1)
                {
                    word_str[j++] = buf[iter];
                }

                iter++;
                col++;
            }

            /* For invalid identifier such as 1234abcd */
            if (isalpha(buf[iter]) || buf[iter] == '_')
            {
                while (isalnum(buf[iter]) || buf[iter] == '_')
                {
                    if (j < sizeof(word_str) - 1)
                    {
                        word_str[j++] = buf[iter];
                    }

                    iter++;
                    col++;
                }

                word_str[j] = '\0';

                printf("Line:%d\tColumn:%d: error: invalid identifier '%s'\n", line, col, word_str);
                return;
            }

            word_str[j] = '\0';
            iter--;
            col--;

            
            if (dot)        //Floating constant
            {
                if(float_err_check(line, col, word_str) == 0)
                    return;

                printf("%-22s: %s\n","Constant", word_str);
            }
            else
            {
                if(integer_err_check(line, col, word_str) == 0)
                    return;

                printf("%-22s: %s\n","Constant", word_str);
            }

            iter++;
            col++;
            continue;
        }

        //Operators
        if(isOperator(ch))
        {
            char ch_1 = buf[iter+1];
            
            if ((ch == '+' && ch_1 == '+') ||
                (ch == '-' && ch_1 == '-') ||
                (ch == '+' && ch_1 == '=') ||
                (ch == '-' && ch_1 == '=') ||
                (ch == '*' && ch_1 == '=') ||
                (ch == '/' && ch_1 == '=') ||
                (ch == '=' && ch_1 == '=') ||
                (ch == '!' && ch_1 == '=') ||
                (ch == '<' && ch_1 == '=') ||
                (ch == '>' && ch_1 == '=') ||
                (ch == '&' && ch_1 == '&') ||
                (ch == '|' && ch_1 == '|') ||
                (ch == '|' && ch_1 == '=') ||
                (ch == '%' && ch_1 == '=') ||
                (ch == '&' && ch_1 == '=') ||
                (ch == '^' && ch_1 == '=') ||
                (ch == '>' && ch_1 == '>') ||
                (ch == '<' && ch_1 == '<'))   
            {
                printf("%-22s: %c%c\n","Relational Operator", ch, ch_1);

                iter++;
                col++;
            }
            else
            {
                if(ch == '=')
                {
                    printf("%-22s: %c\n","Assignment Operator", ch);
                }

                printf("%-22s: %c\n","Operator", ch);
            }

            iter++;
            col++;
            continue;
        }

        if(isSymbol(ch))
        {
            if(ch == '(')
            {
                if(parenthesis_err_check(line, col, buf, iter) == 0)
                    return;
            }
            else if(ch == '{')
            {
                if(braces_err_check(line, col, buf, iter) == 0)
                    return;
            }
            else if(ch == '[')
            {
                if(bracket_err_check(line, col, buf, iter) == 0)
                    return;
            }

            printf("%-22s: %c\n","Special Character", ch);

            iter++;
            col++;
            continue;
        }


        //Unknown tokens
        printf("Line:%d\tColumn:%d: error: unknown token '%c'\n", line, col, ch);

        return;      
    }
}





/******************************************
**             Error Checks              **
*******************************************/

int braces_err_check(int line, int col, char buf[], int iter)
{
    int count = 1;

    iter++;

    while (buf[iter] != '\0')
    {
        if (buf[iter] == '{')
            count++;
        else if (buf[iter] == '}')
            count--;

        if (count == 0)
            return 1;

        iter++;
    }

    printf("Line:%d\tColumn:%d: Error: expected '}' before end of input\n", line, col);
    return 0;
}


int bracket_err_check(int line, int col, char buf[], int iter)
{
    int count = 1;

    iter++;

    while (buf[iter] != '\0')
    {
        if (buf[iter] == '[')
            count++;
        else if (buf[iter] == ']')
            count--;

        if (count == 0)
            return 1;

        iter++;
    }

    printf("Line:%d\tColumn:%d: Error: Expected ']' before end of input\n", line, col);
    return 0;
}



int parenthesis_err_check(int line, int col, char buf[], int iter)
{
    int count = 1;

    iter++;

    while (buf[iter] != '\0')
    {
        if (buf[iter] == '(')
            count++;
        else if (buf[iter] == ')')
            count--;

        if (count == 0)
            return 1;

        iter++;
    }

    printf("Line:%d\tColumn:%d: Error: Expected ')' before end of input\n", line, col);

    return 0;
}




int float_err_check(int line, int col, char *word_str)
{
    int dot = 0;

    for (int iter = 0; word_str[iter] != '\0'; iter++)
    {
        if (word_str[iter] == '.')
        {
            dot++;

            if (dot > 1)
            {
                printf("Line:%d\tColumn:%d: Error: Invalid float constant '%s'\n", line, col, word_str);

                return 0;
            }
        }
        else if (!isdigit(word_str[iter]))
        {
            printf("Line:%d\tColumn:%d: Error: Invalid suffix '%c' on floating constant\n", line, col + iter, word_str[iter]);

            return 0;
        }
    }

    return 1;
}


int integer_err_check(int line, int col, char *word_str)
{
    int iter;

    for (iter = 0; word_str[iter] != '\0'; iter++)
    {
        if (!isdigit(word_str[iter]))
        {
            printf("Line:%d\tColumn:%d: Error: Invalid integer constant '%s'\n", line, col, word_str);

            return 0;
        }
    }

    return 1;
}
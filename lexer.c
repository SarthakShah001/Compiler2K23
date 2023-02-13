#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define buff_size 500

char buffer1[buff_size];
char buffer2[buff_size];

bool is_buffer1_filled = false, is_buffer2_filled = false;

char *begin_ptr, *forward_ptr;

FILE *fptr;

typedef struct token
{
    char token_type[20];
    char lexeme[30];
    int line_no;
} Token;

void start_lexer(FILE *fp)
{
    begin_ptr = buffer1;
    forward_ptr = begin_ptr;

    is_buffer1_filled = true;

    fread(buffer1, 1, buff_size, fp);

    printf("%s\n", buffer1);
}

void fill_buffer()
{
    if (is_buffer1_filled)
    {
        fread(buffer2, 1, buff_size, fptr);
        is_buffer1_filled = false;
        is_buffer2_filled = true;
    }
}

char get_next_char(char *forward_ptr)
{
    if (*forward_ptr == '\0')
    {
        fill_buffer();
    }
    else if (*forward_ptr == EOF)
    {
        // close_lexor();
    }
    return *(forward_ptr++);
}

Token get_next_token()
{
    Token currtoken;
    int current_state = 0;

    while (1)
    {
        switch (current_state)
        {
        case 0:
        {
            char incoming = get_next_char(forward_ptr);
            break;
        }

        default:
            break;
        }
    }

    // return NULL;
}

int main()
{
    fptr = fopen("./Testcases/Testcase1", "r");
    if (fptr == NULL)
    {
        printf("File not opened\n");
        return 0;
    }
    start_lexer(fptr);
    while (1)
    {
        Token current_token = get_next_token();
    }
    return 0;
}
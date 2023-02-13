#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define buff_size 500

char buffer1[buff_size];
char buffer2[buff_size]; 

bool is_buffer1_filled = false, is_buffer2_filled = false;
// bool end_file=false;
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
    int x= fread(buffer1,1, buff_size, fp);
    if(x<buff_size){
        is_buffer1_filled = true;
    }
    
}

bool fill_buffer()
{
    if (is_buffer1_filled)
    {
        fread(buffer2, 1, buff_size, fptr);
        is_buffer1_filled = false;
        is_buffer2_filled = true;
        forward_ptr=buffer2;
    }
    else if(is_buffer2_filled){
        fread(buffer1, 1, buff_size, fptr);
        is_buffer2_filled = false;
        is_buffer1_filled = true;
        forward_ptr=buffer2;
    }
    else{
        return true;
    }
    return false;
}

char get_next_char(char *forward_ptr)
{
    bool end_file=false;
    if (*forward_ptr == '\0')
    {
        end_file =fill_buffer();
    }
    if(end_file){
        return '\0';
    }
    return *(forward_ptr++);
}
Token tokenise(char type[],int retract_length,bool is_final_state){

}
Token get_next_token()
{
    Token currtoken;
    int current_state = 0;
    bool is_tokenised=false;
    while (!is_tokenised)
    {   
        char c;
        switch (current_state)
        {
        case 0:
        {
             c = get_next_char(forward_ptr);
            if( (c <=90 && c >=65) ||(c <=122 && c >=97) || (c=='_') ){
                current_state = 1;
            }
            else if(c=='\0'){
                // tokenise();
            }
            break;
        }
        case 1:
        {
            c = get_next_char(forward_ptr);
            if((c <=90 && c >=65) ||(c <=122 && c >=97) || (c=='_')||(c >=48 && c<=57)){
                continue;
            }
            else{
                // retract here by 1
                // tokenise();
            }
        }
        default:
            break;
        }
    }

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
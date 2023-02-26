#include "lexer.h"
void lexer_init(){
    is_buffer1_filled=false;
    is_buffer2_filled=false;
    begin_ptr=NULL;
    forward_ptr=NULL;
    retract_case = false;
    curr_line_no = 1;
    where_begin=false;
    where_forward=false;
    char_count = 0;
}
void start_lexer()
{
    begin_ptr = buffer1;
    where_begin = false;
    forward_ptr = begin_ptr;
    where_forward = false;
    int x = fread(buffer1, sizeof(char), buff_size, fptr);
    // might have to handle error in case of not read
    is_buffer1_filled = true;
    buffer1[x] = EOF;
}

void fill_buffer()
{
    if (is_buffer1_filled)
    {
        int x = fread(buffer2, sizeof(char), buff_size, fptr);
        is_buffer1_filled = false;
        is_buffer2_filled = true;
        where_forward = true;
        buffer2[x] = EOF;
        forward_ptr = buffer2;
    }
    else if (is_buffer2_filled)
    {
        int x = fread(buffer1, sizeof(char), buff_size, fptr);
        is_buffer2_filled = false;
        is_buffer1_filled = true;
        buffer1[x] = EOF;
        forward_ptr = buffer1;
        where_forward = false;
    }
}

char get_next_char()
{
    if (!where_forward)
    {
        if (forward_ptr == buffer1 + buff_size)
        {
            // forward at the end of buffer 1
            if (retract_case)
            {
                retract_case = false;
                forward_ptr = buffer2;
                where_forward = true;
            }
            else
            {
                fill_buffer();
            }
        }
    }
    else
    {
        if (forward_ptr == buffer2 + buff_size)
        {
            // forward at the end of buffer 2
            if (retract_case)
            {
                retract_case = false;
                forward_ptr = buffer1;
                where_forward = false;
            }
            else
            {
                fill_buffer();
            }
        }
    }
    char_count++;

    char return_char = *forward_ptr;
    forward_ptr++;

    return return_char;
}
void retract(int n)
{
    if ((!where_begin && !where_forward) || (where_begin && where_forward))
    {
        forward_ptr = forward_ptr - n;
    }
    else if (!where_begin && where_forward)
    {
        if (forward_ptr - buffer2 < n)
        {
            retract_case = true;
            n = n - (forward_ptr - buffer2) - 1;
            forward_ptr = buffer1 + buff_size - 1;
            forward_ptr -= n;
            where_forward = false;
        }
        else
        {
            forward_ptr -= n;
        }
    }
    else
    {
        if (forward_ptr - buffer1 < n)
        {
            retract_case = true;
            n = n - (forward_ptr - buffer1) - 1;
            forward_ptr = buffer2 + buff_size - 1;
            forward_ptr -= n;
            where_forward = true;
        }
        else
        {
            forward_ptr -= n;
        }
    }
}
char *get_lexeme()
{

    char *lexeme = (char *)malloc((char_count + 1) * sizeof(char));
    int i = 0;
    if (!where_begin && !where_forward)
    {
        char *temp = begin_ptr;
        while (temp != forward_ptr)
        {
            lexeme[i] = *temp;
            temp++;
            i++;
        }
    }
    else if (!where_begin && where_forward)
    {
        while (begin_ptr != buffer1 + buff_size)
        {
            lexeme[i] = *begin_ptr;
            begin_ptr++;
            i++;
        }
        char *temp = buffer2;
        while (temp != forward_ptr)
        {
            lexeme[i] = *temp;
            temp++;
            i++;
        }
    }
    else if (where_begin && !where_forward)
    {
        while (begin_ptr != buffer2 + buff_size)
        {
            lexeme[i] = *begin_ptr;
            begin_ptr++;
            i++;
        }
        char *temp = buffer1;
        while (temp != forward_ptr)
        {
            lexeme[i] = *temp;
            temp++;
            i++;
        }
    }
    else
    {
        char *temp = begin_ptr;
        while (temp != forward_ptr)
        {
            lexeme[i] = *temp;
            temp++;
            i++;
        }
    }

    begin_ptr = forward_ptr;
    where_begin = where_forward;
    return lexeme;
}

void report_error(int state, char *lexeme)
{
    switch (state)
    {
    case -1:
    {
        printf("\nError at line = %d : CHARACTERS GREATER THAN 20 AT => %s\n\n", curr_line_no, lexeme);
        break;
    }

    case 0:
    {
        printf("\nError at line = %d : UNKNOWN IDENTIFIER AT => %s\n\n", curr_line_no, lexeme);
        break;
    }
    case 5:
    {
        printf("\nError at line = %d : NOT A VALID DECIMAL AT => %s\n\n", curr_line_no, lexeme);
        break;
    }
    case 8:
    {
        printf("\nError at line = %d : NOT A VALID DECIMAL AT => %s\n\n", curr_line_no, lexeme);
        break;
    }
    case 9:
    {
        printf("\nError at line = %d : NOT A VALID DECIMALAT => %s\n\n", curr_line_no, lexeme);
        break;
    }
    case 13:
    {
        printf("\nError at line = %d : NOT A VALID SYMBOLAT => %s\n\n", curr_line_no, lexeme);
        break;
    }
    case 43:
    {
        printf("\nError at line = %d : NOT A VALID RELATIONAL OPERATOR AT => %s\n\n", curr_line_no, lexeme);
        break;
    }
    case 45:
    {
        printf("\nError at line = %d : NOT A RELATIONAL OPERATOR AT => %s\n\n", curr_line_no, lexeme);
        break;
    }

    default:
        break;
    }
}

Token tokenise(tkType tokenincoming, int retract_length, bool is_final_state, int state, bool *istok)
{
    tkType tokentype=tokenincoming;
    Token token;
    retract(retract_length);
    lexeme l;
    char_count -= retract_length;
    char *str = get_lexeme();
    
    if (char_count > 20)
    {
        // throw error
        report_error(-1, str);
        token.token_type=TK_ERROR;
        begin_ptr = forward_ptr;
        where_begin = where_forward;
        *istok = false;
        char_count = 0;
        return token;
    }

    if (tokentype== TK_ERROR)
    {
        report_error(state, str);
        begin_ptr = forward_ptr;
        where_begin = where_forward;
        token.token_type=TK_ERROR;
        *istok = false;
        char_count = 0;
        return token;
    }
    if (tokentype==TK_ID)
    {
        for (int i = 0; i < 30; i++)
        {
            if (strcmp(reserved[i].first, str) == 0)
            {
                tokentype=reserved[i].second;
            }
        }
    }
    strcpy(l.value,str);
    if (tokentype==TK_NUM)
    {
        int x = atoi(l.value);
        l.integer = x;
    }
    else if (tokentype==TK_RNUM)
    {
        double x = atof(l.value);
        l.decimal = x;
    }
    
    token.token_type=tokentype;
    // printf("%s\n",tokentype);
    token.lex = l;
    token.line_no = curr_line_no;
    *istok = true;
    char_count = 0;
    return token;
}
Token get_next_token()
{
    Token currtoken;
    int current_state = 0;
    char *lexeme;
    bool is_tokenised = false;
    while (!is_tokenised)
    {
        char c;
        switch (current_state)
        {
        case 0:
        {
            c = get_next_char();
            if ((c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a') || (c == '_'))
            {
                current_state = 1;
            }
            else if (c >= '0' && c <= '9')
            {
                current_state = 3;
            }
            else if (c == '.')
            {
                current_state = 13;
            }
            else if (c == '+')
            {
                current_state = 16;
            }
            else if (c == '-')
            {
                current_state = 17;
            }
            else if (c == '*')
            {
                current_state = 18;
            }
            else if (c == '/')
            {
                current_state = 24;
            }
            else if (c == '<')
            {
                current_state = 25;
            }
            else if (c == ' ' || c == '\t')
            {
                current_state = 31;
            }
            else if (c == '[')
            {
                current_state = 33;
            }
            else if (c == ']')
            {
                current_state = 34;
            }
            else if (c == '(')
            {
                current_state = 35;
            }
            else if (c == ')')
            {
                current_state = 36;
            }
            else if (c == '>')
            {
                current_state = 37;
            }
            else if (c == '=')
            {
                current_state = 43;
            }
            else if (c == '!')
            {
                current_state = 45;
            }
            else if (c == ':')
            {
                current_state = 47;
            }
            else if (c == ';')
            {
                current_state = 50;
            }
            else if (c == ',')
            {
                current_state = 51;
            }
            else if (c == '\n')
            {
                current_state = 15;
            }
            else if (c == EOF)
            {
                // tokenid=sentinel
                // we have assumed 0 to be final state

                currtoken = tokenise(TK_EOF, 0, true, 0, &is_tokenised);
                current_state = 0;
            }
            else
            {
                // error
                tokenise(TK_ERROR, 0, true, 0, &is_tokenised);
                current_state = 0;
            }
            break;
        }
        case 1:
        {
            c = get_next_char();
            if ((c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a') || (c == '_') || (c >= '0' && c <= '9'))
            {
                current_state = 1;
            }
            else
            {
                current_state = 2;
            }

            break;
        }
        case 2:
        {
            // tokenise for id and retract 1 position forward ptr
            currtoken = tokenise(TK_ID, 1, true, 2, &is_tokenised);
            current_state = 0;
            break;
        }
        case 3:
        {
            c = get_next_char();
            if (c >= '0' && c <= '9')
            {
                current_state = 3;
            }
            else if (c == '.')
            {
                current_state = 5;
            }
            else
            {

                current_state = 4;
            }

            break;
        }
        case 4:
        {
            // tokenise for number and retract
            currtoken = tokenise(TK_NUM, 1, true, 4, &is_tokenised);
            current_state = 0;
            break;
        }

        case 5:
        {
            c = get_next_char();
            if (c >= '0' && c <= '9')
            {
                current_state = 7;
            }
            else if (c == '.')
            {
                current_state = 6;
            }
            else
            {
                // error
                tokenise(TK_ERROR, 1, false, 5, &is_tokenised);
                current_state = 0;
            }
            break;
        }
        case 6:
        {
            // retract 2 positions and tokenise the previous read number
            currtoken = tokenise(TK_NUM, 2, true, 6, &is_tokenised);
            current_state = 0;
            break;
        }
        case 7:
        {
            c = get_next_char();
            if (c >= '0' && c <= '9')
            {
                current_state = 7;
            }
            else if (c == 'E' || c == 'e')
            {
                current_state = 8;
            }
            else
            {
                current_state = 12;
            }
            break;
        }
        case 8:
        {
            c = get_next_char();
            if (c >= '0' && c <= '9')
            {
                current_state = 10;
            }
            else if (c == '+' || c == '-')
            {
                current_state = 9;
            }
            else
            {
                // error
                tokenise(TK_ERROR, 1, false, 8, &is_tokenised);
                current_state = 0;
            }
            break;
        }
        case 9:
        {
            c = get_next_char();
            if (c >= '0' && c <= '9')
            {
                current_state = 10;
            }
            else
            {
                // error
                tokenise(TK_ERROR, 1, false, 9, &is_tokenised);
                current_state = 0;
            }
            break;
        }

        case 10:
        {
            c = get_next_char();
            if (c >= '0' && c <= '9')
            {
                current_state = 10;
            }
            else
            {
                current_state = 11;
            }
            break;
        }
        case 11:
        {
            // retract and tokenise for real number here E notation is used
            currtoken = tokenise(TK_RNUM, 1, true, 11, &is_tokenised);
            current_state = 0;
            break;
        }

        case 12:
        {
            // retract and tokenise for real number here decimal notation is used
            currtoken = tokenise(TK_RNUM, 1, true, 12, &is_tokenised);
            current_state = 0;
            break;
        }
        case 13:
        {
            c = get_next_char();
            if (c == '.')
            {
                current_state = 14;
            }
            else
            {
                // error
                tokenise(TK_ERROR, 1, false, 13, &is_tokenised);
                current_state = 0;
            }
        }
        case 14:
        {
            // tokenise: TK_RANGEP
            currtoken = tokenise(TK_RANGEOP, 0, true, 14, &is_tokenised);
            current_state = 0;
            break;
        }
        case 15:
        {
            curr_line_no++;
            begin_ptr = forward_ptr;
            where_begin = where_forward;
            char_count = 0;
            current_state = 0;
            break;
        }
        case 16:
        {
            // tokenise: TK_PLUS
            currtoken = tokenise(TK_PLUS, 0, true, 16, &is_tokenised);
            current_state = 0;
            break;
        }
        case 17:
        {
            // tokenise: TK_MINUS
            currtoken = tokenise(TK_MINUS, 0, true, 17, &is_tokenised);
            current_state = 0;
            break;
        }
        case 18:
        {
            c = get_next_char();
            if (c == '*')
            {
                current_state = 20;
            }
            else
            {
                current_state = 19;
            }
            break;
        }
        case 19:
        {
            // tokenise: TK_MUL
            // retract
            currtoken = tokenise(TK_MUL, 1, true, 19, &is_tokenised);
            current_state = 0;
            break;
        }
        case 20:
        {
            c = get_next_char();
            if (c == '*')
            {
                current_state = 21;
            }
            else if (c == '\n')
            {
                current_state = 22;
                curr_line_no++;
            }
            else
            {
                current_state = 20;
            }
            break;
        }
        case 21:
        {
            c = get_next_char();
            if (c == '*')
            {
                current_state = 23;
            }
            else if (c == '\n')
            {
                current_state = 22;
                curr_line_no++;
            }
            else
            {
                current_state = 20;
            }
            break;
        }
        case 22:
        {
            c = get_next_char();
            if (c == '*')
            {
                current_state = 21;
            }
            else if (c == '\n')
            {
                current_state = 22;
                curr_line_no++;
            }
            else
            {
                current_state = 20;
            }
            break;
        }
        case 23:
        {
            begin_ptr = forward_ptr;
            where_begin = where_forward;
            char_count = 0;
            current_state = 0;
            break;
        }
        case 24:
        {
            currtoken = tokenise(TK_DIVIDE, 0, true, 24, &is_tokenised);
            current_state = 0;
            break;
        }
        case 25:
        {
            c = get_next_char();
            if (c == '=')
            {
                current_state = 26;
            }
            else if (c == '<')
            {
                current_state = 27;
            }
            else
            {
                current_state = 30;
            }
            break;
        }
        case 26:
        {
            // tokenise: TK_LE
            currtoken = tokenise(TK_LE, 0, true, 26, &is_tokenised);
            current_state = 0;
            break;
        }
        case 27:
        {
            c = get_next_char();
            if (c == '<')
            {
                current_state = 28;
            }
            else
            {
                current_state = 29;
            }
            break;
        }
        case 28:
        {
            // tokenise: TK_DRIVERDEFF
            currtoken = tokenise(TK_DRIVERDEF, 0, true, 28, &is_tokenised);
            current_state = 0;
            break;
        }
        case 29:
        {
            // tokenise: TK_DEF
            // retract
            currtoken = tokenise(TK_DEF, 1, true, 29, &is_tokenised);
            current_state = 0;
            break;
        }
        case 30:
        {
            // tokenise: TK_LT
            // retract
            currtoken = tokenise(TK_LT, 1, true, 30, &is_tokenised);
            current_state = 0;
            break;
        }
        case 31:
        {
            c = get_next_char();
            if (c == ' ' || c == '\t')
            {
                current_state = 31;
            }
            else
            {
                current_state = 32;
            }
            break;
        }
        case 32:
        {
            // delim final retraction state
            retract(1);
            begin_ptr = forward_ptr;
            where_begin = where_forward;
            char_count = 0;
            current_state = 0;
            break;
        }
        case 33:
        {
            // Tokenise [
            currtoken = tokenise(TK_SQBO, 0, true, 33, &is_tokenised);
            current_state = 0;
            break;
        }
        case 34:
        {
            // Tokenise ]
            currtoken = tokenise(TK_SQBC, 0, true, 34, &is_tokenised);
            current_state = 0;
            break;
        }
        case 35:
        {
            // Tokenise (
            currtoken = tokenise(TK_BO, 0, true, 35, &is_tokenised);
            current_state = 0;
            break;
        }
        case 36:
        {
            // Tokenise )
            currtoken = tokenise(TK_BC, 0, true, 36, &is_tokenised);
            current_state = 0;
            break;
        }
        case 37:
        {
            c = get_next_char();
            if (c == '=')
            {
                current_state = 38;
            }
            else if (c == '>')
            {
                current_state = 40;
            }
            else
            {
                current_state = 39;
            }
            break;
        }
        case 38:
        {
            // Tokenise >=
            currtoken = tokenise(TK_GE, 0, true, 38, &is_tokenised);
            current_state = 0;
            break;
        }
        case 39:
        {
            // Retract and Tokenise >
            currtoken = tokenise(TK_GT, 1, true, 39, &is_tokenised);
            current_state = 0;
            break;
        }
        case 40:
        {
            c = get_next_char();
            if (c == '>')
            {
                current_state = 41;
            }
            else
            {
                current_state = 42;
            }
            break;
        }
        case 41:
        {
            // Tokenise >>>
            currtoken = tokenise(TK_DRIVERENDDEF, 0, true, 41, &is_tokenised);
            current_state = 0;

            break;
        }
        case 42:
        {
            // Retract and Tokenise >>
            currtoken = tokenise(TK_ENDDEF, 1, true, 42, &is_tokenised);
            current_state = 0;
            break;
        }
        case 43:
        {
            c = get_next_char();
            if (c == '=')
            {
                current_state = 44;
            }
            else
            {
                // Trap state
                tokenise(TK_ERROR, 1, false, 43, &is_tokenised);
                current_state = 0;
            }
            break;
        }
        case 44:
        {
            // Tokenise ==
            currtoken = tokenise(TK_EQ, 0, true, 44, &is_tokenised);
            current_state = 0;
            break;
        }
        case 45:
        {
            c = get_next_char();
            if (c == '=')
            {
                current_state = 46;
            }
            else
            {
                // Trap state
                tokenise(TK_ERROR, 1, false, 45, &is_tokenised);
                current_state = 0;
            }
            break;
        }
        case 46:
        {
            // Tokenise !=
            currtoken = tokenise(TK_NE, 0, true, 46, &is_tokenised);
            current_state = 0;
            break;
        }
        case 47:
        {
            c = get_next_char();
            if (c == '=')
            {
                current_state = 48;
            }
            else
            {
                current_state = 49;
            }
            break;
        }
        case 48:
        {
            // Tokenise :=
            currtoken = tokenise(TK_ASSIGNOP, 0, true, 48, &is_tokenised);
            current_state = 0;
            break;
        }
        case 49:
        {
            // Retract and Tokenise :
            currtoken = tokenise(TK_COLON, 1, true, 49, &is_tokenised);
            current_state = 0;
            break;
        }
        case 50:
        {
            // Tokenise ;
            currtoken = tokenise(TK_SEMICOLON, 0, true, 50, &is_tokenised);
            current_state = 0;
            break;
        }
        case 51:
        {
            // Tokenise ,
            currtoken = tokenise(TK_COMMA, 0, true, 51, &is_tokenised);
            current_state = 0;
            break;
        }

        default:
        {
            break;
        }
        }
    }
    // printf("IN SWITCH %d\n", currtoken.token_type);
    return currtoken;
}

int main()
{
    char testcase;
    printf("Enter no. of testcase you wanna check upon:");
    scanf("%c",&testcase);
    char filename[]="./Testcases/Testcase0";
    filename[20]=testcase;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("File not opened\n");
        return 0;
    }
    printf("Tokenization Started\n");
    lexer_init();
    start_lexer();
    while (true)
    {
        Token current_token = get_next_token();
        // printf("%d\n",current_token.token_type);
        if (current_token.token_type==TK_NUM)
        {
            printf("TOKEN TYPE => <%s>  LEXEME => %d   LINE => %d\n", terminal_str[current_token.token_type], current_token.lex.integer, current_token.line_no);
        }
        else if (current_token.token_type==TK_RNUM)
        {
            printf("TOKEN TYPE => <%s>  LEXEME => %f   LINE=>%d\n", terminal_str[current_token.token_type], current_token.lex.decimal, current_token.line_no);
        }
        else
        {
            //  printf("%s\n", current_token.token_type);
            printf("TOKEN TYPE => <%s>  LEXEME => %s   LINE => %d\n",terminal_str[current_token.token_type], current_token.lex.value, current_token.line_no);
        }

        if (current_token.token_type==TK_EOF)
        {
            printf("Tokenization Process Finished\n");
            break;
        }
    }
    return 0;
}
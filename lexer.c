#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define buff_size 500

char buffer1[buff_size+1];
char buffer2[buff_size+1]; 

bool is_buffer1_filled = false, is_buffer2_filled = false;
// bool end_file=false;
char *begin_ptr, *forward_ptr;
FILE *fptr;
int curr_line_no;
int char_count;
bool where_begin,where_forward;//0 indicates buff1, 1 indicates buff2
bool retract_case;
typedef struct token
{
    char token_type;
    char *lexeme;
    int line_no;
} Token;

void start_lexer(FILE *fp)
{
    retract_case=false;
    curr_line_no=1;
    char_count=0;
    begin_ptr = buffer1;
    where_begin=false;
    forward_ptr = begin_ptr;
    where_forward=false;
    int x= fread(buffer1,sizeof(char), buff_size, fp);
    is_buffer1_filled=true;
    buffer1[x]=EOF;
}

void fill_buffer()
{
    if (is_buffer1_filled)
    {
        int x=fread(buffer2, 1, buff_size, fptr);
        is_buffer1_filled = false;
        is_buffer2_filled = true;
        where_forward=true;
        buffer2[x]=EOF;
        forward_ptr=buffer2;
    }
    else if(is_buffer2_filled){
        int x= fread(buffer1, 1, buff_size, fptr);
        is_buffer2_filled = false;
        is_buffer1_filled = true;
        buffer1[x]=EOF;
        forward_ptr=buffer1;
        where_forward=false;
    }
}

char get_next_char(char *forward_ptr)
{
    if(!where_forward){
        if(forward_ptr==buffer1+buff_size){
            //forward at the end of buffer 1
            if(retract_case){
                retract_case=false;
                forward_ptr=buffer2;
                where_forward=true;
            }
            else{
            fill_buffer();
            }
        }
    }
    else{
        if(forward_ptr==buffer2+buff_size){
            //forward at the end of buffer 2
            if(retract_case){
                retract_case=false;
                forward_ptr=buffer1;
                where_forward=false;
            }
            else{
            fill_buffer();
            }
        }
    }
    char_count++;
    return *(forward_ptr++);

}
void retract(int n){
    if((!where_begin&&!where_forward)||(where_begin&&where_forward)){
        forward_ptr=forward_ptr-n;
    }
    else if(!where_begin&&where_forward){
        if(forward_ptr-buffer2<n){
            retract_case=true;
            n=n-(forward_ptr-buffer2)-1;
            forward_ptr=buffer1+buff_size-1;
            forward_ptr-=n;
            where_forward=false;
        }
        else{
            forward_ptr-=n;
        }
    }
    else{
        if(forward_ptr-buffer1<n){
            retract_case=true;
            n=n-(forward_ptr-buffer1)-1;
            forward_ptr=buffer2+buff_size-1;
            forward_ptr-=n;
            where_forward=true;
        }
        else{
            forward_ptr-=n;
        }
    }
}
Token tokenise(char type[],int retract_length,bool is_final_state){



}
char* get_lexeme(char*begin_ptr,char*forward_ptr){
    char *lexeme=(char*)malloc((char_count+1)*sizeof(char));
    if(char_count>20){
        //throw error
    }
    int i=0;
    if(!where_begin&&!where_forward){
        char *temp=begin_ptr;
        while(temp!=forward_ptr){
            lexeme[i]=*temp;
            temp++;
            i++;
        }
    }
    else if(!where_begin&&where_forward){
        while(begin_ptr!=buffer1+buff_size){
            lexeme[i]=*begin_ptr;
            begin_ptr++;
            i++;
        }
        char* temp=buffer2;
        while(temp!=forward_ptr){
            lexeme[i]=*temp;
            temp++;
            i++;
        }
    }
    else if(where_begin&&!where_forward){
        while(begin_ptr!=buffer2+buff_size){
            lexeme[i]=*begin_ptr;
            begin_ptr++;
            i++;
        }
        char* temp=buffer1;
        while(temp!=forward_ptr){
            lexeme[i]=*temp;
            temp++;
            i++;
        }
    }
    else{
        char *temp=begin_ptr;
        while(temp!=forward_ptr){
            lexeme[i]=*temp;
            temp++;
            i++;
        }
    }
    char_count=0;
    begin_ptr=forward_ptr;
    where_begin=where_forward;
    return lexeme;

}
Token get_next_token()
{
    
    Token currtoken;
    int current_state = 0;
    char *lexeme;
    bool is_tokenised=false;
    while (!is_tokenised)
    {   //siddharth start
        char c;
        switch (current_state)
        {
        case 0:
        {
             c = get_next_char(forward_ptr);
            if( (c <=90 && c >=65) ||(c <=122 && c >=97) || (c=='_') ){
                current_state = 1;
            }
             else if(c >=48 && c<=57){
                current_state = 3;
            }
            else if(c =='.'){
                current_state = 13;
            }
            else if(c == '+'){
                current_state = 16;
            }
            else if(c == '-'){
                current_state = 17;
            }
            else if(c == '*'){
                current_state = 18;
            }
            else if(c == '/'){
                current_state = 24;
            }
            else if(c == '<'){
                current_state = 25;
            }
            else if(c == ' ' || c == '\t'){
                current_state = 31;
            }
            else if(c == '['){
                current_state = 33;
            }
            else if(c == ']'){
                current_state = 34;
            }
            else if(c == '('){
                current_state = 35;
            }
            else if(c == ')'){
                current_state = 36;
            }
            else if(c == '>'){
                current_state = 37;
            }
            else if(c == '='){
                current_state = 43;
            }
            else if(c == '!'){
                current_state = 45;
            }
            else if(c == ':'){
                current_state = 47;
            }
            else if(c == ';'){
                current_state = 50;
            }
            else if(c == ','){
                current_state = 51;
            }
            else if(c == '\n'){
                current_state = 15;
            }
            else if(c==EOF){
                // tokenid=sentinel
            }
            else{
                // error
            }
            break;
        }
        case 1:
        {
            c = get_next_char(forward_ptr);
            if((c <=90 && c >=65) ||(c <=122 && c >=97) || (c=='_')||(c >=48 && c<=57)){
               
            }
            else{
                current_state = 2;
            }
           
            break;
        }
        case 2:
        {
            //tokenise for id and retract 1 position forward ptr
            current_state = 0;
            break;
        }
        case 3:
        {   
            c = get_next_char(forward_ptr);
            if(c >=48 && c<=57){
                
            }
            else if(c =='.'){
                current_state=5;
            }
            else{
                current_state=4;
            }

            break;  
            
        }
        case 4:
        {
            //tokenise for number and retract 
            current_state = 0;
            break;
        }

        case 5:{
            c = get_next_char(forward_ptr);
            if(c >=48 && c<=57){
                current_state = 7;
            }
            else if(c == '.'){
                current_state = 6;
            }
            else{
                //error
            }
            break;
        }
        case 6:{
            // retract 2 positions and tokenise the previous read number
            current_state = 0;
            break;
        }
        case 7:{
            c = get_next_char(forward_ptr);
            if(c >=48 && c<=57){
                current_state = 7;
            }
            else if(c == 'E'|| c == 'e'){
                current_state = 8;
            }
            else{
                current_state = 12;
            }
            break;
        }
        case 8:{
            c = get_next_char(forward_ptr);
            if(c >=48 && c<=57){
                current_state = 10;
            }
            else if(c == '+'|| c =='-'){
                current_state = 9;
            }
            else{
                //error
            }
            break;
        }
        case 9:{
            c = get_next_char(forward_ptr);
            if(c >=48 && c<=57){
                current_state = 10;
            }
            else{
                //error
            }
            break;
        }

        case 10:{
            c = get_next_char(forward_ptr);
            if(c >=48 && c<=57){
                current_state = 10;
            }
            else{
                current_state = 11;
            }
            break;
        }
        case 11:{
           // retract and tokenise for real number here E notation is used
           current_state = 0;
           break;
        }

        case 12:{
            // retract and tokenise for real number here decimal notation is used
           current_state = 0;
           break;
        }
        //siddharth end
        //rishi start
        case 13:
        {
            c = get_next_char(forward_ptr);
            if(c == '.'){
                current_state = 14;
            }
            else{
                //error
            }
        }
        case 14:
        {
            //tokenise: TK_RANGEP
            current_state = 0;
            break;
        }
        case 15:
        {
            curr_line_no++;
            current_state = 0;
            break;
        }
        case 16:
        {
            //tokenise: TK_PLUS
            current_state = 0;
            break;
        }
        case 17:
        {
            //tokenise: TK_MINUS
            current_state = 0;
            break;
        }
        case 18:
        {
            c = get_next_char(forward_ptr);
            if(c == '*'){
                current_state = 20;
            }
            else{
                current_state = 19;
            }
            break;
        }
        case 19:
        {
            //tokenise: TK_MUL
            //retract 
            current_state = 0;
            break;
        }
        case 20:
        {
            c = get_next_char(forward_ptr);
            if(c == '*'){
                current_state = 21;
            }
            if(c == '\n'){
                current_state = 22;
            }else{
                current_state = 20;
            }
            break;
        }
        case 21:
        {
            c = get_next_char(forward_ptr);
            if(c == '*'){
                current_state = 23;
            }
            if(c == '\n'){
                current_state = 22;
            }else{
                current_state = 20;
            }
            break;
        }
        case 22:
        {
            curr_line_no++;
            c = get_next_char(forward_ptr);
            if(c == '*'){
                current_state = 21;
            }
            if(c == '\n'){
                current_state = 22;
            }else{
                current_state = 20;
            }
            break;
        }
        case 23:
        {
            current_state = 0;
            break;
        }
        case 24:
        {
            //tokenise: TK_DIVIDE
            current_state = 0;
            break;
        }
        case 25:
        {
            c = get_next_char(forward_ptr);
            if(c == '='){
                current_state = 26;
            }
            if(c == '<'){
                current_state = 27;
            }else{
                current_state = 30;
            }
            break;
        }
        case 26:
        {
            //tokenise: TK_LE
            current_state = 0;
            break;
        }
        case 27:
        {
            c = get_next_char(forward_ptr);
            if(c == '<'){
                current_state = 28;
            }else{
                current_state = 30;
            }
            break;
        }
        case 28:
        {
            //tokenise: TK_DRIVERDEFF
            current_state = 0;
            break;
        }
        case 29:
        {
            //tokenise: TK_DEF
            //retract
            current_state = 0;
            break;
        }
        case 30:
        {
            //tokenise: TK_LT
            //retract 
            current_state = 0;
            break;
        }
        //rishi end
        //archaj start
        case 31:
        {
            c = get_next_char(forward_ptr);
            if(c == ' ' || c == '\t'){
                current_state = 31;
            }
            else{
                current_state = 32;
            }
            break;
        }
        case 32:
        {
            // delim final retraction state
            current_state = 0;
            break;
        }
        case 33:
        {
            // Tokenise [
            current_state = 0;
            break;
        }
        case 34:
        {
            // Tokenise ]
            current_state = 0;
            break;
        }
        case 35:
        {
            // Tokenise (
            current_state = 0;
            break;
        }
        case 36:
        {
            // Tokenise )
            current_state = 0;
            break;
        }
        case 37:
        {
            c = get_next_char(forward_ptr);
            if(c == '='){
                current_state = 38;
            }
            else if(c == '>'){
                current_state = 40;
            }
            else{
                current_state = 39;
            }
            break;
        }
        case 38:
        {
            // Tokenise >=
            current_state = 0;
            break;
        }
        case 39:
        {
            // Retract and Tokenise >
            current_state = 0;
            break;
        }
        case 40:
        {
            c = get_next_char(forward_ptr);
            if(c == '>'){
                current_state = 41;
            }
            else{
                current_state = 42;
            }
            break;
        }
        case 41:
        {
            // Tokenise >>>
            current_state = 0;
            break;
        }
        case 42:
        {
            // Retract and Tokenise >>
            current_state = 0;
            break;
        }
        case 43:
        {
            c = get_next_char(forward_ptr);
            if(c == '='){
                current_state = 44;
            }
            else{
                // Trap state
            }
            break;
        }
        case 44:
        {
            // Tokenise ==
            current_state = 0;
            break;
        }
        case 45:
        {
            c = get_next_char(forward_ptr);
            if(c == '='){
                current_state = 46;
            }
            else{
                // Trap state
            }
            break;
        }
        case 46:
        {
            // Tokenise !=
            current_state = 0;
            break;
        }
        case 47:
        {
            c = get_next_char(forward_ptr);
            if(c == '='){
                current_state = 48;
            }
            else{
                current_state = 49;
            }
            break;
        }
        case 48:
        {
            // Tokenise :=
            current_state = 0;
            break;
        }
        case 49:
        {
            // Retract and Tokenise :
            current_state = 0;
            break;
        }
        case 50:
        {
            // Tokenise ;
            current_state = 0;
            break;
        }
        case 51:
        {
            // Tokenise ,
            current_state = 0;
            break;
        }
        
        //archaj end
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
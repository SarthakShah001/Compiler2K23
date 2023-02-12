#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define buff_size 4096
char buff1[buff_size];
char buff2[buff_size];
bool fill_1=false,fill_2=false;
typedef struct token
{
   char* token_type[20];
   char* lexeme[30];
   int line_no; 
}Token;

Token getNextToken(){


}


int main(){
FILE *fptr;
fptr = fopen("./Testcases/Testcase1","r");
if(fptr==NULL){
    printf("File not opened\n");
    return 0;
}
fread(buff1,1,buff_size,fptr);
char *begin,*forward;
forward=buff1;
int lineno=1;
int i=0;
printf("%s\n",buff1);
for(i=0;i<buff_size;i++){
    if(buff1[i]=='\n'){
        lineno++;
    }
}
printf("%d\n",lineno);
return 0;
}
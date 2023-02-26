#include<stdio.h>
int main(){
    char *s="abcd";
    while(*s!='\0'){
        printf("%c ",*s++);
    }
}
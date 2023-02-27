#include <stdio.h>
#include <string.h>
void removeComments(char *testcaseFile, char *cleanFile){
    int i =0;
    while(i<strlen(testcaseFile)){
        int count = 0;
        if(testcaseFile[i] == '*' && testcaseFile[i+1] == '*'){
            int j = i+2;
            while(!(testcaseFile[j] == '*' && testcaseFile[j+1] == '*')){
                if(testcaseFile[j] == '\n'){
                    count ++;
                }
                j++;
            }
            i = j+2;
        }
        while(count--){
            *cleanFile = '\n';
            cleanFile++;
        }
        *cleanFile = testcaseFile[i];
        cleanFile++;
        i++;
    }
}
int main()
{
    char *s = "<<module mod1>> \ntakes input [index: integer, val_: integer]; \n** this function does not return any value \n rbgu \n rubgu** \nstart \n     declare i_1: integer; \n     i_1:= val_+ index - 4; \n     i_1:= val_+ index - 4;\n     print(i_1);\nend";
    char p[500];
    removeComments(s,p);
    puts(p);
    return 0;
}

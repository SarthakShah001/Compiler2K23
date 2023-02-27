#ifndef _removeComments_
#define _removeComments_
#include <string.h>

// char* testCaseFile => path to testcase file
void removeComments(char* testCaseFile ,char *cleanFile){
    int i =0;
    while(i<strlen(testCaseFile)){
        int count = 0;
        if(testCaseFile[i] == '*' && testCaseFile[i+1] == '*'){
            int j = i+2;
            while(!(testCaseFile[j] == '*' && testCaseFile[j+1] == '*')){
                if(testCaseFile[j] == '\n'){
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
        *cleanFile = testCaseFile[i];
        cleanFile++;
        i++;
    }
}

#endif
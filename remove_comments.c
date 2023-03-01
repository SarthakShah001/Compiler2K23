#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include "removeComments.h"
void removeComments(char* testCaseFile ,char *cleanFile){
    int j=0;
    int n=strlen(testCaseFile);
    for(int i=0;i<n;){
        if(testCaseFile[i]=='*'){
            if(i<n-1&&testCaseFile[i+1]=='*'){
                i+=2;
                while(i<n-1&&!(testCaseFile[i]=='*'&&testCaseFile[i+1]=='*')){
                    if(testCaseFile[i]=='\n'){
                        cleanFile[j]='\n';
                        j++;
                    }
                    i++;
                }
                i+=2;
            }
            else{
                cleanFile[j]=testCaseFile[i];
                j++;
                i++;
            }
        }
        else{
            cleanFile[j]=testCaseFile[i];
            i++;
            j++;
        }
    }
}
/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _hashtabledef_
#define _hashtabledef_

#define HASHSIZE 1000
struct ELEMENT{
    char lexeme[40];
    bool is_present;
    int value;
};
typedef struct ELEMENT hashelement;
typedef hashelement hashtable[HASHSIZE];





#endif
#ifndef _hashtabledef_
#define _hashtabledef_

#define HASHSIZE 100
typedef struct ELEMENT{
    char lexeme[20];
    bool is_present;
    int value;
}hashelement;
typedef hashelement hashtable[HASHSIZE];

#endif
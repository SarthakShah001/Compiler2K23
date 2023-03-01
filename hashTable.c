/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "hashTable.h"
#include "parserDef.h"
int hash(char *str)
{
    long long int char_sum = 0;
    int len = 0;
    while (*str != '\0')
    {
        char_sum += (*str) * (1 << ((*str) % 8));
        str++;
        len++;
    }
    char_sum += len % 2;
    return char_sum % HASHSIZE;
}
void hash_init(hashtable t)
{
    for (int i = 0; i < HASHSIZE; i++)
    {
        t[i].is_present = false;
        strcpy(t[i].lexeme, "");
        t[i].value = -1;
    }
}
void hash_insert(hashtable t, char *str, int value)
{
    int prob = 0;
    int hash_val = hash(str);
    while (t[hash_val].is_present)
    {
        prob++;
        hash_val = (hash_val + prob) % HASHSIZE;
    }
    t[hash_val].is_present = true;
    t[hash_val].value = value;
    strcpy(t[hash_val].lexeme, str);
}
int find_value(hashtable t, char *str)
{
    int hash_val = hash(str);
    int prob = 0;
    while (t[hash_val].is_present == true)
    {
        if (strcmp(t[hash_val].lexeme, str) == 0)
        {
            return t[hash_val].value;
        }
        prob++;
        hash_val = (hash_val + prob) % HASHSIZE;
    }
    return -1;
}
bool is_str_present(hashtable t, char *str)
{
    int hash_val = hash(str);
    int prob = 0;
    while (t[hash_val].is_present == true)
    {
        if (strcmp(t[hash_val].lexeme, str) == 0)
        {
            return true;
        }
        prob++;
        hash_val = (hash_val + prob) % HASHSIZE;
    }
    return false;
}

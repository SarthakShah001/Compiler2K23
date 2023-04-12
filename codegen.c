/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include "codegen.h"

FILE *asmFile;

int spaceUsed(symbol_table st)
{
    int ans = 0;
    if (st == NULL)
    {
        return 0;
    }
    else
    {
        int maxoffset = -1;
        int maxwidth = -1;
        for (int i = 0; i < 1000; i++)
        {
            if (st->sym[i] == NULL)
            {
                continue;
            }
            else
            {
                if (st->sym[i]->offset > maxoffset)
                {
                    maxoffset = st->sym[i]->offset;
                    maxwidth = st->sym[i]->width;
                }
            }
        }
        ans += maxoffset + maxwidth;
        for (int i = 0; i < 1000; i++)
        {
            if (st->child[i] == NULL)
            {
                break;
            }
            else
            {
                ans += spaceUsed(st->child[i]);
            }
        }
    }
    return ans;
}

void createCode(qNode head)
{
    asmFile = fopen("./code.asm", "w");

    // macro definition and code skeleton

    fprintf(asmFile, "section .data\n\n");

    fprintf(asmFile, "\t\t int_val : db \"%%d\" , 0\n");
    fprintf(asmFile, "\t\t real_val : db \"%%lf\" , 0\n");
    fprintf(asmFile, "\t\t bool_val : db \"%%d\" , 0\n");

    fprintf(asmFile, "section .text\n\n");

    fprintf(asmFile, "%%macro pushAll 0\n");
    fprintf(asmFile, "\t\t push RAX \n");
    fprintf(asmFile, "\t\t push RBX \n");
    fprintf(asmFile, "\t\t push RCX \n");
    fprintf(asmFile, "\t\t push RDX \n");
    fprintf(asmFile, "\t\t push RSP \n");
    fprintf(asmFile, "\t\t push RBP \n");
    fprintf(asmFile, "\t\t push RSI \n");
    fprintf(asmFile, "\t\t push RDI \n");
    fprintf(asmFile, "%%endmacro \n\n");

    fprintf(asmFile, "%%macro popAll 0\n");
    fprintf(asmFile, "\t\t pop RDI \n");
    fprintf(asmFile, "\t\t pop RSI \n");
    fprintf(asmFile, "\t\t pop RBP \n");
    fprintf(asmFile, "\t\t pop RSP \n");
    fprintf(asmFile, "\t\t pop RDX \n");
    fprintf(asmFile, "\t\t pop RCX \n");
    fprintf(asmFile, "\t\t pop RBX \n");
    fprintf(asmFile, "\t\t pop RAX \n");
    fprintf(asmFile, "%%endmacro \n\n");

    while (head)
    {

        switch (head->op)
        {
        case ASSIGN_OP:
        {
            // x = 10 or x = t1;
            fprintf(asmFile, "\t\t pushAll\n");
            if (strcmp(head->arg2, "") == 0)
            {

                if (head->arg1symbol == NULL)
                {
                    if (head->arg1Node->tok->token_type == TK_RNUM)
                    {
                    }
                    else if (head->arg1Node->tok->token_type == TK_TRUE)
                    {
                    }
                    else if (head->arg1Node->tok->token_type == TK_FALSE)
                    {
                    }
                    else if (head->arg1Node->tok->token_type == TK_NUM)
                    {
                        int offset = head->resultsymbol->offset;

                        fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg1);
                        fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                    }
                }
                else
                {
                    int width = head->arg1symbol->width;
                    if (width == 1)
                    {
                        // fprintf(asmFile);
                    }
                    else if (width == 2)
                    {
                        fprintf(asmFile, "\t\t ");
                    }
                    else if (width == 4)
                    {
                    }
                }
            }

            else
            {
            }
            fprintf(asmFile, "\t\t popAll\n");
            break;
        }

        case MUL_OP:
        {
            fprintf(asmFile, "\t\t pushAll\n");

            if (head->arg1symbol == NULL && head->arg2symbol == NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg1);
                    fprintf(asmFile, "\t\t mov RBX , %s \n", head->arg2);
                    fprintf(asmFile, "\t\t mul RBX \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else if (head->arg1symbol == NULL && head->arg2symbol != NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;
                    int offset2 = head->arg2symbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg1);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset2);
                    fprintf(asmFile, "\t\t mul RBX \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else if (head->arg1symbol != NULL && head->arg2symbol == NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;
                    int offset1 = head->arg1symbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg2);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset1);
                    fprintf(asmFile, "\t\t mul RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {

                    int offset = head->resultsymbol->offset;
                    int offset1 = head->arg1symbol->offset;
                    int offset2 = head->arg2symbol->offset;
                    fprintf(asmFile, "\t\t mov RAX , [RBP-%d] \n", offset1);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset2);
                    fprintf(asmFile, "\t\t mul RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }

            fprintf(asmFile, "\t\t popAll\n");

            break;
        }

        case PLUS_OP:
        {

            fprintf(asmFile, "\t\t pushAll\n");

            if (head->arg1symbol == NULL && head->arg2symbol == NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg1);
                    fprintf(asmFile, "\t\t mov RBX , %s \n", head->arg2);
                    fprintf(asmFile, "\t\t add RAX , RBX \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else if (head->arg1symbol == NULL && head->arg2symbol != NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;
                    int offset2 = head->arg2symbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg1);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset2);
                    fprintf(asmFile, "\t\t add RAX ,RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else if (head->arg1symbol != NULL && head->arg2symbol == NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;
                    int offset1 = head->arg1symbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg2);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset1);
                    fprintf(asmFile, "\t\t add RAX ,RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {

                    int offset = head->resultsymbol->offset;
                    int offset1 = head->arg1symbol->offset;
                    int offset2 = head->arg2symbol->offset;
                    fprintf(asmFile, "\t\t mov RAX , [RBP-%d] \n", offset1);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset2);
                    fprintf(asmFile, "\t\t add RAX ,RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }

            fprintf(asmFile, "\t\t popAll\n");

            break;
        }

        case MINUS_OP:
        {
            fprintf(asmFile, "\t\t pushAll\n");

            if (head->arg1symbol == NULL && head->arg2symbol == NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg1);
                    fprintf(asmFile, "\t\t mov RBX , %s \n", head->arg2);
                    fprintf(asmFile, "\t\t sub RAX , RBX \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else if (head->arg1symbol == NULL && head->arg2symbol != NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;
                    int offset2 = head->arg2symbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg1);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset2);
                    fprintf(asmFile, "\t\t sub RAX ,RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else if (head->arg1symbol != NULL && head->arg2symbol == NULL)
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {
                    int offset = head->resultsymbol->offset;
                    int offset1 = head->arg1symbol->offset;

                    fprintf(asmFile, "\t\t mov RAX , %s \n", head->arg2);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset1);
                    fprintf(asmFile, "\t\t sub RAX ,RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }
            else
            {
                if (head->arg1Node->tok->token_type == TK_RNUM)
                {
                }
                else if (head->arg1Node->tok->token_type == TK_NUM)
                {

                    int offset = head->resultsymbol->offset;
                    int offset1 = head->arg1symbol->offset;
                    int offset2 = head->arg2symbol->offset;
                    fprintf(asmFile, "\t\t mov RAX , [RBP-%d] \n", offset1);
                    fprintf(asmFile, "\t\t mov RBX , [RBP-%d] \n", offset2);
                    fprintf(asmFile, "\t\t sub RAX ,RBX  \n");
                    fprintf(asmFile, "\t\t mov [RBP-%d], RAX\n", offset);
                }
            }

            fprintf(asmFile, "\t\t popAll\n");

            break;
        }

        case DIV_OP:
        {

            break;
        }

        case LT_OP:
        {

            break;
        }

        case EQ_OP:
        {

            break;
        }

        case NE_OP:
        {

            break;
        }

        case GE_OP:
        {

            break;
        }

        case LE_OP:
        {

            break;
        }

        case OR_OP:
        {

            break;
        }

        case AND_OP:
        {

            break;
        }

        case GETVAL_OP:
        {

            fprintf(asmFile, "\t\t pushAll\n");

            int offset = head->arg1symbol->offset ; 

            switch (head->arg1Node->tok->token_type)
            {
            case TK_NUM:
            {
                fprintf(asmFile, "\t\t mov RDI , int_val \n");

                break;
            }
            case TK_RNUM:
            {
                fprintf(asmFile, "\t\t mov RDI , real_val \n");

                break;
            }
            case TK_BOOLEAN:
            {
                fprintf(asmFile, "\t\t mov RDI , bool_val \n");
                break;
            }

                // array case need to be handled

            default:
                break;
            }

            fprintf(asmFile, "\t\t popAll\n");

            break;
        }

        case PRINTVAL_OP:
        {
            fprintf(asmFile, "\t\t pushAll\n");

           
            fprintf(asmFile, "\t\t popAll\n");

            break;
        }

        case LABEL_OP:
        {

            break;
        }
        case DRIVERDEFN_OP:
        {
            int x = find_mod_no("driver");
            symbol_table st = global_symbol_table[x]->table;
            int space = spaceUsed(st);
            fprintf(asmFile, "global main");
            fprintf(asmFile, "main:\n");
            fprintf(asmFile, "\t\t ENTER %d,0\n", space);
            fprintf(asmFile, "\t\tSUB RSP , 0\n\n");
            break;
        }

        case NULL_OP:
        {

            break;
        }
        }

        head = head->next;
    }

    fprintf(asmFile, "\t\t ret");

    fclose(asmFile);
}
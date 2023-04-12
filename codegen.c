/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include "codegen.h"

FILE* asmFile ;

void codegen(qNode head){
    asmFile = fopen("./code.asm" , "w") ; 


    // macro definition and code skeleton 

    fprintf(asmFile , "section .data\n") ;

    fprintf(asmFile , "section .text\n") ;

    fprintf(asmFile , "%%macro pushAll \n");
    fprintf(asmFile , "\t\t pop RAX \n");
    fprintf(asmFile , "\t\t pop RBX \n");
    fprintf(asmFile , "\t\t pop RCX \n");
    fprintf(asmFile , "\t\t pop RDX \n");
    fprintf(asmFile , "\t\t pop RSP \n");
    fprintf(asmFile , "\t\t pop RBP \n");
    fprintf(asmFile , "\t\t pop RSI \n");
    fprintf(asmFile , "\t\t pop RDI \n");
    fprintf(asmFile , "%%endmacro \n");


    fprintf(asmFile , "%%macro popAll \n");
    fprintf(asmFile , "\t\t push RAX \n");
    fprintf(asmFile , "\t\t push RBX \n");
    fprintf(asmFile , "\t\t push RCX \n");
    fprintf(asmFile , "\t\t push RDX \n");
    fprintf(asmFile , "\t\t push RSP \n");
    fprintf(asmFile , "\t\t push RBP \n");
    fprintf(asmFile , "\t\t push RSI \n");
    fprintf(asmFile , "\t\t push RDI \n");
    fprintf(asmFile , "%%endmacro \n");



    while(head){

        switch (head->op)
        {
            case ASSIGN_OP:{
                // x = 10 or x = t1; 
                if(find_symbol(head->))
                fprintf(asmFile , "") ; 
                break ; 
            }

            case MUL_OP:{

                break ; 
            }

            case PLUS_OP:{

                break ; 
            }

            case MINUS_OP:{

                break ; 
            }

            case DIV_OP:{

                break ;
            }

            case LT_OP:{
                
                break ; 
            }

            case EQ_OP:{

                break ; 
            }

            case NE_OP:{

                break ; 
            }

            case GE_OP:{

                break ; 
            }

            case LE_OP:{

                break ; 
            }

            case OR_OP:{

                break ; 
            }

            case AND_OP:{

                break ; 
            }

            case GETVAL_OP:{

                break ; 
            }

            case PRINTVAL_OP:{

                break ; 
            }

            case LABEL_OP:{

                break ; 
            }

            case NULL_OP:{

                break ; 
            }

            
        }


        head = head->next ; 
    }


    fclose(asmFile) ; 
}
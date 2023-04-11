#include "codegen.h"

FILE* asmFile ;

void codegen(qNode head){
    asmFile = fopen("./code.asm" , "w") ; 


    // macro definition and code skeleton 

    fprintf(asmFile , "section .data\n") ;

    fprintf(asmFile , "section .text\n") ;

    fprintf(asmFile , "%%macro pushAll \n
                        push RAX\n
                        push RBX\n
                        push RCX\n
                        push RDX\n
                        push RSP\n
                        push RBP\n
                        push RSI\n
                        push RDI\n
                       %%endmacro \n\n
                    ") ;

    fprintf(asmFile , "%%macro popAll \n
                        pop RAX\n
                        pop RBX\n
                        pop RCX\n
                        pop RDX\n
                        pop RSP\n
                        pop RBP\n
                        pop RSI\n
                        pop RDI\n
                       %%endmacro \n\n
                    ") ;


    while(head){

        switch (head->op)
        {
            case ASSIGN_OP:{
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
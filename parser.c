#include "parser.h"
#include "setADT.h"
int arr1[num_nonterminals];
int arr2[num_nonterminals];
char *nonterminal_str[]={
"<program>",
"<moduleDeclarations>",
"<moduleDeclaration>",
"<otherModules>",
"<driverModule>",
"<module>",
"<ret>",
"<input_plist>",
"<N1>",
"<output_plist>",
"<N2>",
"<dataType>",
"<range_arrays>",
"<type>",
"<moduleDef>",
"<statements>",
"<statement>",
"<ioStmt>",
"<boolConstt>",
"<var_print>",
"<P1>",
"<simpleStmt>",
"<assignmentStmt>",
"<whichStmt>",
"<lvalueIDStmt>",
"<lvalueARRStmt>",
"<index_arr>",
"<new_index>",
"<sign>",
"<moduleReuseStmt>",
"<actual_para_list>",
"<optional>",
"<idList>",
"<N3>",
"<expression>",
"<U>",
"<new_NT>",
"<var_id_num>",
"<unary_op>",
"<arithmeticOrBooleanExpr>",
"<N7>",
"<AnyTerm>",
"<N8>",
"<arithmeticExpr>",
"<N4>",
"<term>",
"<N5>",
"<factor>",
"<N11>",
"<element_index_with_expressions>",
"<arrExpr>",
"<arr_N4>",
"<arrTerm>",
"<arr_N5>",
"<arrFactor>",
"<N10>",
"<op1>",
"<op2>",
"<logicalOp>",
"<relationalOp>",
"<declareStmt>",
"<condionalStmt>",
"<caseStmts>",
"<N9>",
"<value>",
"<default>",
"<iterativeStmt>",
"<range_for_loop>",
"<index_for_loop>",
"<new_index_for_loop>",
"<sign_for_loop>",
};

hashtable terminals,nonterminals;
rule grep[num_rules];
setNode first_set[num_nonterminals];
setNode follow_set[num_nonterminals];
void fill_terminals(hashtable terminals){
    hash_init(terminals);
    for(int i=0;i<num_terminals;i++){
        hash_insert(terminals,terminal_str[i],i);
    }
}
void fill_nonterminals(hashtable nonterminals){
    hash_init(nonterminals);
    for(int i=0;i<num_nonterminals;i++){
        hash_insert(nonterminals,nonterminal_str[i],i);
    }
}
void fill_grammer(){
    FILE *fp=fopen("./grammar.txt","r");
    if(fp==NULL){
        "Grammar file not opened";
        return;
    }
    int rule_no=0;
    char buffer[max_rule_length];
    while(fgets(buffer,max_rule_length,fp)!=NULL){
        char *rule_str=strtok(buffer," \n");
        // printf("%s\n",rule_str);
        grep[rule_no].LHS= find_value(nonterminals,rule_str);
        // printf("%d\n",grep[rule_no].LHS);
        rule_str=strtok(NULL," \n");
        rule_str=strtok(NULL," \n");
        // printf("%s\n",rule_str);
        dlinkedlist rhs=createDLinkedList();
        while(rule_str!=NULL){
        symbol s=(symbol)malloc(sizeof(struct SYMBOL));
        if(*rule_str=='<'){
            s->is_terminal=false;
            s->nt=find_value(nonterminals,rule_str);
        }
        else{
            s->is_terminal=true;
            s->t=find_value(terminals,rule_str);
        }
        
        // printf("%d %d",s->t,find_value(terminals,rule_str));
        
        insertEnd(rhs,s);
        rule_str=strtok(NULL," \n");
        }
        grep[rule_no].RHS=rhs;
        // printf("%d\n",grep[rule_no].RHS->head->val->t);
        // print(rhs);
        rule_no++;
        
        // printf("%d\n",rule_no);
    }
}
// bool* fill_first_set(nonterminal nt){
    
// }
void fill_hash_tables(){
    hash_init(terminals);
    hash_init(nonterminals);
    for(int i=0;i<num_nonterminals;i++){
        hash_insert(nonterminals,nonterminal_str[i],i);
    }
    for(int i=0;i<num_terminals;i++){
        hash_insert(terminals,terminal_str[i],i);
    }
    // for(int i=0;i<num_terminals;i++){
    //    printf("%s %d %d %d\n",terminal_str[i], is_str_present(terminals,terminal_str[i]),hash(terminal_str[i]),find_value(terminals,terminal_str[i]));
    // }
    // for(int i=0;i<HASHSIZE;i++){
    //     printf("%d\n",terminals[i]);
    // }
}
void populateFirstSet(nonterminal nt){
    //firstly initialise the set
    if(first_set[nt]->is_filled){
        return;
    }
    first_set[nt]->is_filled=true;
    for(int i=0;i<num_rules;i++){
        if(grep[i].LHS==nt){
            printf("rule_no=%d,%s\n",i,nonterminal_str[nt]);
            dlinkedlist dl=grep[i].RHS;
            dllnode dn=dl->head;
            bool will_be_epsilon=true;
            bool f=true;
            while(f&&dn!=NULL){
                // f=false;
                symbol s=dn->val;
                if(s->is_terminal&&s->t==epsilon){
                    printf("1 ");
                    first_set[nt]->arr[epsilon]=true;
                    break;
                }
                else if(s->is_terminal){
                    printf("2 ");
                    first_set[nt]->arr[s->t]=true;
                    will_be_epsilon=false;
                    break;
                }
                else{
                    do{
                        printf("3 ");
                        s=dn->val;
                        populateFirstSet(s->nt);
                        setUnion(first_set[nt],first_set[s->nt],first_set[nt]);
                        if(first_set[s->nt]->arr[epsilon]){
                            printf("4 ");
                            first_set[nt]->arr[epsilon]=false;
                            dn=dn->next;
                        }
                        else{
                            printf("5 ");
                            // printf("5 ");
                            will_be_epsilon=false;
                            f=false;
                            break;
                        }
                    }
                    while(dn!=NULL);
                    break;
            }
            if(will_be_epsilon){
                first_set[nt]->arr[epsilon]=true;
            }
            }
            // printf("\n");
         }
     }         
}

// dn = dn->next;
//                 while(dn!=NULL && !(dn->val)->is_terminal ){
//                     
//                     setUnion(follow_set[nt1],first_set[(dn->val)->nt],follow_set[nt1]);
//                     if (findInSet(first_set[(dn->val)->nt], epsilon)){
//                         deleteElement(follow_set[nt1], epsilon);
                            // dn = dn->next;
//                         continue;
//                     }
//                     else break ;  
//                 }
//                 if(dn==NULL){
//                     populateFollowSet(grep[i].LHS);
//                     setUnion(follow_set[nt1],follow_set[grep[i].LHS],follow_set[nt1]);
                    

//                 }
                //    else if((dn->val)->is_terminal){
                //     insertElement(follow_set[nt1],(dn->val)->t);
                //    }
// }

void populateFollowSet(nonterminal nt1){
    if(follow_set[nt1]->is_filled){
        return;
    } 
    follow_set[nt1]->is_filled = true;
    for(int i=0;i<num_rules;i++){
        printf("rule_no=%d %s\n",i,nonterminal_str[nt1]);
        dlinkedlist dl = grep[i].RHS;
        dllnode dn = dl->head;
        while(dn!=NULL){
            if((dn->val)->is_terminal || (dn->val)->nt!= nt1){
                printf("1 ");
                dn = dn->next;
                continue;
            }
            else{
                dn=dn->next;
                while(dn!=NULL){
                    if(dn->val->is_terminal){
                        printf("2 ");
                        insertElement(follow_set[nt1],dn->val->t);
                        break;
                    }
                    else{
                        printf("3 ");
                        // populateFirstSet((dn->val)->nt);
                        setUnion(follow_set[nt1],first_set[(dn->val)->nt],follow_set[nt1]);
                        if (findInSet(first_set[(dn->val)->nt], epsilon)){
                            printf("4 ");
                            deleteElement(follow_set[nt1], epsilon);
                            dn=dn->next;
                            continue;
                        }
                        else break ;
                    }
                }
                if(dn==NULL){
                    printf("5 ");
                    populateFollowSet(grep[i].LHS);
                    setUnion(follow_set[nt1],follow_set[grep[i].LHS],follow_set[nt1]);
                }
            }
            printf("\n");
        }
    }
}

int main(){
    fill_hash_tables();
    fill_grammer();
    // for(int i=0;i<num_rules;i++){
    //     printf("%d %d\n",grep[i].LHS,grep[i].RHS->head->val->is_terminal);
    // }
    for(int i=0;i<num_nonterminals;i++){
        first_set[i]=initSet();
    }
    for(int i=0;i<num_nonterminals;i++){
        follow_set[i]=initSet();
    }
    follow_set[0]->arr[$]=true;
    for(int i=0;i<num_nonterminals;i++){
        populateFirstSet(i);
    }
    for(int i=0;i<num_nonterminals;i++){
        populateFollowSet(i);
    }
    // for(int i=0;i<num_nonterminals;i++){
    //     printf("%s\n",nonterminal_str[i]);
    //     for(int j=0;j<num_terminals;j++){
    //         // printf("%d ",first_set[0]->arr[j]);
    //         if(follow_set[i]->arr[j]){
    //             printf("%s ",terminal_str[j]);
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("\n%d\n",find_value(terminals,"TK_REAL"));
}
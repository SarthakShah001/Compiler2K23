#include "parser.h"
#include "setADT.h"
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
dllnode parse_table[num_nonterminals][num_terminals];
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
        grep[rule_no].LHS= find_value(nonterminals,rule_str);
        rule_str=strtok(NULL," \n");
        rule_str=strtok(NULL," \n");
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
        insertEnd(rhs,s);
        rule_str=strtok(NULL," \n");
        }
        grep[rule_no].RHS=rhs;
        rule_no++;
    }
}
void fill_hash_tables(){
    hash_init(terminals);
    hash_init(nonterminals);
    for(int i=0;i<num_nonterminals;i++){
        hash_insert(nonterminals,nonterminal_str[i],i);
    }
    for(int i=0;i<num_terminals;i++){
        hash_insert(terminals,terminal_str[i],i);
    }
}
void populateFirstSet(nonterminal nt){
    //firstly initialise the set
    if(first_set[nt]->is_filled){
        return;
    }
    first_set[nt]->is_filled=true;
    for(int i=0;i<num_rules;i++){
        if(grep[i].LHS==nt){
            dlinkedlist dl=grep[i].RHS;
            dllnode dn=dl->head;
            bool will_be_epsilon=true;
            bool f=true;
            while(f&&dn!=NULL){
                // f=false;
                symbol s=dn->val;
                if(s->is_terminal&&s->t==epsilon){
                    first_set[nt]->arr[epsilon]=true;
                    break;
                }
                else if(s->is_terminal){
                    first_set[nt]->arr[s->t]=true;
                    will_be_epsilon=false;
                    break;
                }
                else{
                    do{
                        s=dn->val;
                        populateFirstSet(s->nt);
                        setUnion(first_set[nt],first_set[s->nt],first_set[nt]);
                        if(first_set[s->nt]->arr[epsilon]){
                            first_set[nt]->arr[epsilon]=false;
                            dn=dn->next;
                        }
                        else{
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
         }
     }         
}

void populateFollowSet(nonterminal nt1){
    if(follow_set[nt1]->is_filled){
        return;
    } 
    follow_set[nt1]->is_filled = true;
    for(int i=0;i<num_rules;i++){
        dlinkedlist dl = grep[i].RHS;
        dllnode dn = dl->head;
        while(dn!=NULL){
            if((dn->val)->is_terminal || (dn->val)->nt!= nt1){
                dn = dn->next;
                continue;
            }
            else{
                dn=dn->next;
                while(dn!=NULL){
                    if(dn->val->is_terminal){
                        insertElement(follow_set[nt1],dn->val->t);
                        break;
                    }
                    else{
                        setUnion(follow_set[nt1],first_set[(dn->val)->nt],follow_set[nt1]);
                        if (findInSet(first_set[(dn->val)->nt], epsilon)){
                            deleteElement(follow_set[nt1], epsilon);
                            dn=dn->next;
                            continue;
                        }
                        else break ;
                    }
                }
                if(dn==NULL){
                    populateFollowSet(grep[i].LHS);
                    setUnion(follow_set[nt1],follow_set[grep[i].LHS],follow_set[nt1]);
                }
            }
        }
    }
}
void init_parse_table(){
    for(int i=0;i<num_nonterminals;i++){
        for(int j=0;j<num_terminals;j++){
            parse_table[i][j]=NULL;
        }
    }
}

void fill_parse_table(){
    init_parse_table();
    for(int i=0;i<num_rules;i++){
        setNode temp=(setNode)malloc(sizeof(struct SETNODE));
        dlinkedlist dl=grep[i].RHS;
            dllnode dn=dl->head;
            bool will_be_epsilon=true;
            bool f=true;
            while(f&&dn!=NULL){
                // f=false;
                symbol s=dn->val;
                if(s->is_terminal&&s->t==epsilon){
                    temp->arr[epsilon]=true;
                    break;
                }
                else if(s->is_terminal){
                    temp->arr[s->t]=true;
                    will_be_epsilon=false;
                    break;
                }
                else{
                    do{
                        s=dn->val;
                        // populateFirstSet(s->nt);
                        setUnion(temp,first_set[s->nt],temp);
                        if(first_set[s->nt]->arr[epsilon]){
                            temp->arr[epsilon]=false;
                            dn=dn->next;
                        }
                        else{
                            will_be_epsilon=false;
                            f=false;
                            break;
                        }
                    }
                    while(dn!=NULL);
                    break;
            }
            if(will_be_epsilon){
                temp->arr[epsilon]=true;
            }
            }
        for(int j=0;j<num_terminals;j++){
            if(temp->arr[j]){
                parse_table[grep[i].LHS][j]=grep[i].RHS->tail;
            }
        }
    }
}
int main(){
    fill_hash_tables();
    fill_grammer();
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
    fill_parse_table();
    for(int i=0;i<num_nonterminals;i++){
        printf("%s -> ",nonterminal_str[i]);
        for(int j=0;j<num_terminals;j++){
            if(parse_table[i][j]!=NULL){
                printf("%d ",parse_table[i][j]->val->is_terminal);
            }
        }
        printf("\n");
    }
    // for(int i=0;i<num_nonterminals;i++){
    //     printf("%s\n",nonterminal_str[i]);
    //     for(int j=0;j<num_terminals;j++){
    //         // printf("%d ",first_set[0]->arr[j]);
    //         if(first_set[i]->arr[j]){
    //             printf("%s ",terminal_str[j]);
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("\n%d\n",find_value(terminals,"TK_REAL"));
}
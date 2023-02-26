#include "parser.h"
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
"<id_num_rnum>",
"<array_element_for_print>",
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
rule grep[num_rules];
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
    char buffer[max_rule_length];
    while(fgets(buffer,max_rule_length,fp)!=NULL){
        char *rule_str=strtok(buffer," \n");

        while(rule_str!=NULL){
        printf("%s\n",rule_str);
        rule_str=strtok(NULL," \n");
        }
        // printf("line read\n");
    }

}
int main(){
    fill_grammer();
}
typedef struct stacknode* StackNode;
typedef union ele element;


struct stacknode {

    // data type for union of terminal, non terminal and $
    union ele {
        // datatype for terminals(token type)
        // datatype for non terminals
        int terminal;
        float nonterminal;
    };

    bool is_terminal;
    int val; //for dummy purpose

    StackNode next;

};


typedef struct {
    int size;
    StackNode top;

} stack;

stack createStack();

void push(stack S, int x);

void pop(stack S);
 
void top(stack S);





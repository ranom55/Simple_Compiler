#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TableEntry
{
    int symbol;
    char type;
    int location;
};

typedef struct TableEntry TableEntry;
struct stack {
    int data2;
    char data;
    struct stack *nextPtr;
};

typedef struct stack stack;
typedef stack *stackPtr;

void push2(stackPtr *ptr, int value) {

    stackPtr no = malloc(sizeof(stackPtr));
    if (no != NULL) {
        no->data2 = value;
        no->nextPtr = NULL;
        if (*ptr == NULL) {
            *ptr = no;
        }
        else {
            no->nextPtr = *ptr;
            *ptr = no;
        }
    }
}
void push(stackPtr *ptr, char value) {

    stackPtr no = malloc(sizeof(stackPtr));
    if (no != NULL) {
        no->data = value;
        no->nextPtr = NULL;
        if (*ptr == NULL) {
            *ptr = no;
        }
        else {
            no->nextPtr = *ptr;
            *ptr = no;
        }
    }
}

void pop(stackPtr *pilha) {
    
    stackPtr tempPtr;

    if (*pilha != NULL) {
        tempPtr = *pilha;
        *pilha = (*pilha)->nextPtr;
        free(tempPtr);
    }
  
}
int IsEmpty(stackPtr stack) {

    return stack == NULL;
}
int isOperator(char c) {

    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {

        return 1;
    }

    return 0;

}
char stackTop(stackPtr stack) {
    return stack->data;
}
int precedence(char operator1, char operator2) {

    int grupo;
    int grupo2;

    if (operator1 == '/' || operator1 == '*' || operator1 == '%') {

        grupo = 2;
    }
    if (operator2 == '/' || operator2 == '*' || operator2 == '%') {

        grupo2 = 2;
    }
    if (operator1 == '+' || operator1 == '-') {

        grupo = 1;
    }
    if (operator2 == '+' || operator2 == '-') {

        grupo2 = 1;
    }
    if (operator1 == '^') {

        grupo = 3;
    }
    if (operator2 == '^') {

        grupo2 = 3;
    }
    if (grupo == grupo2) {

        return 0;
    }
    if (grupo > grupo2) {

        return 1;
    }
    if (grupo < grupo2) {

        return -1;
    }

}
void convertToPostfix(char infix[], char postfix[])
{
    stackPtr stack = NULL;
    int cont = 0, cont2 = 0, cont3;
    char top;
    push(&stack, '(');
    strcat(infix, ")");
    while (!IsEmpty(stack))
    {
        if (isalnum(infix[cont]))
        {
            for (int x = cont; isalnum(infix[x]) != 0  || infix[x] == '.' || infix[x] == ','; x++)
            {
                postfix[cont2] = infix[x];
                ++cont2;
                cont3 = x;
            }
            cont = cont3;
            postfix[cont2] = ' ';
            ++cont2;
            
        }
        if (infix[cont] == '(')
        {
            push(&stack, '(');
        }
        if (isOperator(infix[cont]) == 1)
        {
            top = stackTop(stack);
            
            
            while (isOperator(top) == 1 && (precedence(top, infix[cont]) == 1 || precedence(top, infix[cont]) == 0))
            {
                postfix[cont2] = top;
                ++cont2;
                postfix[cont2] = ' ';
                ++cont2;
                pop(&stack);
                top = stackTop(stack);
            } 
            
            push(&stack, infix[cont]);
        } 
        if (infix[cont] == ')')
        {
            top = stackTop(stack);
            while (top != '(')
            {
                postfix[cont2] = top;
                ++cont2;
                postfix[cont2] = ' ';
                ++cont2;
                pop(&stack);
                top = stackTop(stack);
            }
            pop(&stack);
        }
        ++cont;
    }
    postfix[cont2] = '\0';
}
void evaluatePostfixExpression(char *expr , int memory[], int *Ip, TableEntry *symbolTable, int *location, int *variable_Counter, int *register_Counter) 
{
    stackPtr stack = NULL;
    int x , y, find, cont = 0;
    char *token = strtok(expr, " ");

    while (token != NULL) 
    {   
        if (isdigit(token[0]) != 0) 
        {
            find = -1;

            for (int x = 0; x < 1000; x++)
            {
                if (symbolTable[x].symbol == atoi(token) && symbolTable[x].type == 'C')
                {
                    find = symbolTable[x].location;
                    break;
                }
            }
            if (find != -1)
            {
                push2(&stack, find);
            }
            else
            {
                symbolTable[*register_Counter].symbol = atoi(token);
                symbolTable[*register_Counter].type = 'C';
                symbolTable[*register_Counter].location = *variable_Counter;
                (*register_Counter)++;
                push2(&stack, *variable_Counter);
                (*variable_Counter)--;
            }      
        }
        if (isalpha(token[0]) != 0)
        {
            find = -1;
            for (int x = 0; x < 1000; x++)
            {
                if (symbolTable[x].symbol == token[0] && symbolTable[x].type == 'V')
                {
                    find = symbolTable[x].location;
                    break;
                }
            }
            if (find != -1)
            {
                push2(&stack, find);
            }
            else
            {
                symbolTable[*register_Counter].symbol = token[0];
                symbolTable[*register_Counter].type = 'V';
                symbolTable[*register_Counter].location = *variable_Counter;
                (*register_Counter)++;
                push2(&stack, *variable_Counter);
                (*variable_Counter)--;
            }      
        }
        else if (isOperator(token[0]) == 1)
        {
            x = stack->data2;
            pop(&stack);
            y = stack->data2;
            pop(&stack);
            if (token[0] == '+')
            {
                memory[*Ip] = 2000 + x;
                (*Ip)++;
                memory[*Ip] = 3000 + y;
                (*Ip)++;
                memory[*Ip] = 2100 + *location;
                (*Ip)++;
                push2(&stack, *location);
            }
            if (token[0] == '-')       
            {
                memory[*Ip] = 2000 + y;
                (*Ip)++;
                memory[*Ip] = 3100 + x;
                (*Ip)++;
                memory[*Ip] = 2100 + *location;
                (*Ip)++;
                push2(&stack, *location);
            }
            if (token[0] == '*')       
            {
                if (x == *location || y == *location)
                {
                    memory[*Ip] = 2000 + x;
                    (*Ip)++;
                    memory[*Ip] = 3300 + y;
                    (*Ip)++;
                    memory[*Ip] = 2100 + *location;
                    (*Ip)++;
                    push2(&stack, *location);
                }
                else 
                {
                    memory[*Ip] = 2000 + x;
                    (*Ip)++;
                    memory[*Ip] = 3300 + y;
                    (*Ip)++;
                    memory[*Ip] = 2100 + *variable_Counter - cont;
                    (*Ip)++;
                    push2(&stack, *variable_Counter - cont);
                    ++cont;
                }    
            }
            if (token[0] == '/')       
            {
                if (x == *location || y == *location)
                {
                    memory[*Ip] = 2000 + y;
                    (*Ip)++;
                    memory[*Ip] = 3200 + x;
                    (*Ip)++;
                    memory[*Ip] = 2100 + *location;
                    (*Ip)++;
                    push2(&stack, *location);
                }
                else
                {
                    memory[*Ip] = 2000 + y;
                    (*Ip)++;
                    memory[*Ip] = 3200 + x;
                    (*Ip)++;
                    memory[*Ip] = 2100 + *variable_Counter - cont;
                    (*Ip)++;
                    push2(&stack, *variable_Counter - cont);
                    ++cont;
                }
            }        
        }    
        
        token = strtok(NULL, " ");
    }
    pop(&stack);   
    
}
int main(void) {

    int pos = -1, left_pos = -1, right_pos = -1, goto_line, find = -1, equal_pos;
    int Flags[100];
    char nula[] = "";
    char postfix[100];
    char infix[100];
    TableEntry symbolTable[1000];
    int memory[100] = {0};
    int variable_Counter = 99;
    int instruction_Pointer = 0;
    int register_Count = 0;
    FILE *file;
    FILE *out = fopen("output.txt", "w");
    char arquivo[300];
    char *token;
    char line[100];
    char left_val, right_val;
    char operator[4];
    for (int x = 0; x < 100; x++)
    {
        Flags[x] = -1;
    }

    printf("Ola\n");
    file = fopen("t.txt", "r");

    if (file != NULL)
    {
        while (!feof(file))
        {
            fgets(line, 100, file);
            token = strtok(line, " ");
            symbolTable[register_Count].symbol = atoi(token);
            symbolTable[register_Count].type = 'L';
            symbolTable[register_Count].location = instruction_Pointer;
            ++register_Count;
            token = strtok(NULL, " ");

            while (token != NULL)
            {        
                find = -1;
                if (!strcmp(token, "let"))
                {
                    strcpy(infix, nula);
                    strcpy(postfix, nula);
                    token = strtok(NULL, " ");
                    for (int x = 0; x < 1000; x++)
                    {
                        if (symbolTable[x].symbol == token[0] && symbolTable[x].type == 'V')
                        {
                            find = symbolTable[x].location;
                            break;
                        }
                    }
                    if (find == -1)
                    {
                        symbolTable[register_Count].symbol = token[0];
                        symbolTable[register_Count].type = 'V';
                        symbolTable[register_Count].location = variable_Counter;
                        ++register_Count;
                        find = variable_Counter;
                        --variable_Counter;
                    }
                    token = strtok(NULL, " ");
                    token = strtok(NULL, " ");
                    while (token != NULL)
                    {
                        strcat(infix, token);
                        token = strtok(NULL, " ");
                    }
                    printf("%s", infix);
                    convertToPostfix(infix, postfix);
                    evaluatePostfixExpression(postfix, memory, &instruction_Pointer, symbolTable, &find, &variable_Counter, &register_Count);
                    break;
                }
                if(!strcmp(token, "goto"))
                {
                    token = strtok(NULL, " ");
                    if (token != NULL)
                    {
                        goto_line = atoi(token);
                    }
                    for (int x = 0; x < 100; x++)
                    {
                        if (symbolTable[x].symbol == goto_line)
                        {
                            find = symbolTable[x].location;
                            break;
                        }
                    }
                    if (find != -1)
                    {
                        memory[instruction_Pointer] = 4000 + find;
                        ++instruction_Pointer;
                    }
                    else 
                    {
                        memory[instruction_Pointer] = 4000;
                        Flags[instruction_Pointer] = goto_line;
                        ++instruction_Pointer;
                    }
                    break;
                }
                if (!strcmp(token, "if"))
                {
                    find = -1;
                    left_pos = -1;
                    right_pos = -1;
                    token = strtok(NULL, " ");
                    if (token != NULL)
                    {
                        left_val = token[0];
                        for (int x = 0; x < 100; x++)
                        {
                            if (symbolTable[x].symbol == left_val && symbolTable[x].type == 'V')
                            {
                                left_pos = x;
                                break;
                            }
                        }
                        if (left_pos == -1)
                        {
                            symbolTable[register_Count].symbol = token[0];
                            symbolTable[register_Count].type = 'V';
                            symbolTable[register_Count].location = variable_Counter;
                            left_pos = register_Count;
                            ++register_Count;  
                            --variable_Counter; 
                        }
                        token = strtok(NULL, " ");
                    }
                    if (token != NULL)
                    {
                        strcpy(operator, token);
                        token = strtok(NULL, " ");
                    }
                    if (token != NULL)
                    {
                        right_val = token[0];
                        for (int x = 0; x < 100; x++)
                        {
                            if (symbolTable[x].symbol == right_val && symbolTable[x].type == 'V')
                            {
                                right_pos = x;
                                break;
                            }
                        }
                        if (right_pos == -1)
                        {
                            symbolTable[register_Count].symbol = token[0];
                            symbolTable[register_Count].type = 'V';
                            symbolTable[register_Count].location = variable_Counter;
                            right_pos = register_Count;
                            ++register_Count;  
                            --variable_Counter; 
                        }
                        token = strtok(NULL, " ");
                    }
                    if(token != NULL && !strcmp(token, "goto"))
                    {
                        token = strtok(NULL, " ");
                        if (token != NULL)
                        {
                            goto_line = atoi(token);
                        }
                    }
                    for (int x = 0; x < 100; x++)
                    {
                        if (symbolTable[x].symbol == goto_line)
                        {
                            find = symbolTable[x].location;
                            break;
                        }
                    }
                    if (!strcmp(operator, "=="))
                    {
                        memory[instruction_Pointer] = 2000 + symbolTable[left_pos].location;   
                        ++instruction_Pointer;
                        memory[instruction_Pointer] = 3100 + symbolTable[right_pos].location;
                        ++instruction_Pointer;
                        if (find != -1)
                        {
                        memory[instruction_Pointer] = 4200 + find;
                        ++instruction_Pointer;
                        }
                        else 
                        {
                        memory[instruction_Pointer] = 4200;
                        Flags[instruction_Pointer] = goto_line;
                        ++instruction_Pointer;
                        }
                    }
                    if (!strcmp(operator, "!="))
                    {
                        memory[instruction_Pointer] = 2000 + symbolTable[left_pos].location;
                        ++instruction_Pointer;
                        memory[instruction_Pointer] = 3100 + symbolTable[right_pos].location;
                        ++instruction_Pointer;
                        if (find != -1)
                        {
                            memory[instruction_Pointer] = 4100 + find;
                            ++instruction_Pointer;
                        }
                        else 
                        {
                            memory[instruction_Pointer] = 4100;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                        }
                        memory[instruction_Pointer] = 2000 + symbolTable[right_pos].location;
                        ++instruction_Pointer;
                        memory[instruction_Pointer] = 3100 + symbolTable[left_pos].location;
                        ++instruction_Pointer;
                        if (find != -1)
                        {
                            memory[instruction_Pointer] = 4100 + find;
                            ++instruction_Pointer;
                        }
                        else 
                        {
                            memory[instruction_Pointer] = 4100;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                        }
                    }
                    if (!strcmp(operator, ">="))
                    {
                        memory[instruction_Pointer] = 2000 + symbolTable[right_pos].location;
                        ++instruction_Pointer;
                        memory[instruction_Pointer] = 3100 + symbolTable[left_pos].location;
                        ++instruction_Pointer;
                        if (find != -1)
                        {
                            memory[instruction_Pointer] = 4100 + find;
                            ++instruction_Pointer;
                            memory[instruction_Pointer] = 4200 + find;
                            ++instruction_Pointer;
                        }
                        else 
                        {
                            memory[instruction_Pointer] = 4100;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                            memory[instruction_Pointer] = 4200;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                        }
                    }
                    if (!strcmp(operator, "<="))
                    {
                        memory[instruction_Pointer] = 2000 + symbolTable[left_pos].location;
                        ++instruction_Pointer;
                        memory[instruction_Pointer] = 3100 + symbolTable[right_pos].location;
                        ++instruction_Pointer;
                        if (find != -1)
                        {
                            memory[instruction_Pointer] = 4100 + find;
                            ++instruction_Pointer;
                            memory[instruction_Pointer] = 4200 + find;
                            ++instruction_Pointer;
                        }
                        else 
                        {
                            memory[instruction_Pointer] = 4100;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                            memory[instruction_Pointer] = 4200;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                        }
                    }
                    if (!strcmp(operator, ">"))
                    {
                        memory[instruction_Pointer] = 2000 + symbolTable[right_pos].location;
                        ++instruction_Pointer;
                        memory[instruction_Pointer] = 3100 + symbolTable[left_pos].location;
                        ++instruction_Pointer;
                        if (find != -1)
                        {
                            memory[instruction_Pointer] = 4100 + find;
                            ++instruction_Pointer;
                        }
                        else 
                        {
                            memory[instruction_Pointer] = 4100;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                            }
                    }
                    if (!strcmp(operator, "<"))
                    {
                        memory[instruction_Pointer] = 2000 + symbolTable[left_pos].location;
                        ++instruction_Pointer;
                        memory[instruction_Pointer] = 3100 + symbolTable[right_pos].location;
                        ++instruction_Pointer;
                        if (find != -1)
                        {
                            memory[instruction_Pointer] = 4100 + find;
                            ++instruction_Pointer;
                        }
                        else 
                        {
                            memory[instruction_Pointer] = 4100;
                            Flags[instruction_Pointer] = goto_line;
                            ++instruction_Pointer;
                            }
                    }
                    break;
                }
                if (!strcmp(token , "end\n"))
                {
                    memory[instruction_Pointer] = 4300;
                    ++instruction_Pointer;
                    break;
                }
                if (!strcmp(token, "rem"))
                {
                    break;
                }
                if (!strcmp(token, "input"))
                {
                    token = strtok(NULL, " ");
                    if (token != NULL)
                    {
                        symbolTable[register_Count].symbol = token[0];
                        symbolTable[register_Count].type = 'V';
                        symbolTable[register_Count].location = variable_Counter;
                        memory[instruction_Pointer] = 1000 + symbolTable[register_Count].location;
                        ++register_Count;
                        ++instruction_Pointer; 
                        --variable_Counter;     
                    }
                    break;          
                }
                if (!strcmp(token, "print"))
                {
                    token = strtok(NULL, " ");

                    if (token != NULL) 
                    {
                        for (int x = 0; x < 100; x++)
                        {
                            if (symbolTable[x].symbol == token[0] && symbolTable[x].type == 'V')
                            {
                                pos = x;
                                break;
                            }
                        }
                        if (pos != -1)
                        {
                            memory[instruction_Pointer] = 1100 + symbolTable[pos].location;
                            ++instruction_Pointer;
                            pos = -1;
                        }
                        else 
                        {   
                            symbolTable[register_Count].symbol = token[0];
                            symbolTable[register_Count].type = 'V';
                            symbolTable[register_Count].location = variable_Counter;
                            memory[instruction_Pointer] = 1100 + variable_Counter;
                            ++register_Count;
                            ++instruction_Pointer;     
                            --variable_Counter;   
                        }
                    }    
                    break;
                }
            }   
        }
    } 
    fclose(file);
    for (int x = 0; x < 100; x++)
    {
        if (Flags[x] != -1)
        {
            for (int y = 0; y < 1000; y++)
            {
                if (symbolTable[y].symbol == Flags[x] && symbolTable[y].type == 'L')
                {
                    memory[x] += symbolTable[y].location;
                    break;
                }
            }
        }
    }

    for (int x = 0; x < instruction_Pointer; x++)
    {   
       
        fprintf(out, "%i %i \n", x, memory[x]);
    }
    for (int x = 0, y = 0; x < 1000; x++)
    {
        if (symbolTable[x].type == 'C' && symbolTable[x].symbol != 0)
        {   
            if (y == 0)
            {
                fprintf(out, "%i %i \n", -1, -1);
                ++y;
            }
            fprintf(out, "%i %i \n", symbolTable[x].location, symbolTable[x].symbol);
        }
    }

    return 0;
}
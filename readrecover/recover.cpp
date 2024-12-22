#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "recover.h"
#include "../tree/tree.h"

struct Token
{
    Type type;

    double value;

    int oper;

    char var;

    unsigned int line;
    unsigned int symbol;
};

char* LowString(char* str);
Token* LexicalAnalis(char* text);
unsigned int WordsCounter(char* str);
unsigned int SymbolsCounter(char* str);
void LexicErr(unsigned int line, unsigned int symbol);
Node* GetAddSub (struct Token* tokens);
Node* GetMulDiv (struct Token* tokens);
Node* GetLog    (struct Token* tokens);
Node* GetDeg    (struct Token* tokens);
Node* GetTrig   (struct Token* tokens);
Node* GetBracket(struct Token* tokens);
Node* GetNumVar (struct Token* tokens);
void SyntaxErr(struct Token* tokens);

unsigned int POS = 0;

Node* Parser(char* text)
{
    text = LowString(text);

    Token* tokens = LexicalAnalis(text);
$$$ printf("BIG BLACK COMMENT4\n");

    Node* node = GetAddSub(tokens);
$$$ printf("BIG BLACK COMMENT5\n");

    if(tokens[POS].oper != END)
        SyntaxErr(tokens);

    free(tokens);
$$$ printf("BIG BLACK COMMENT6\n");

    return node;
}

char* LowString(char* str)
{
    unsigned int str_len = sizeof(str)/sizeof(char);

    for(int i = 0; i < str_len; i++)
    {
        *(str + i) = tolower(*(str + i));
    }

    return str;
}

Node* GetAddSub(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetMulDiv(tokens);

    while(tokens[POS].oper == ADD || tokens[POS].oper == SUB)
    {
        int op = tokens[POS].oper;
        POS += 1;

        Node* nodeleft = new_node;
        Node* noderight = GetMulDiv(tokens);

        if(op == ADD)
            new_node = CreateOper(nodeleft, noderight, ADD);

        else
            new_node = CreateOper(nodeleft, noderight, SUB);
    }

    return new_node;
}

Node* GetMulDiv(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetLog(tokens);

    while(tokens[POS].oper == MULT || tokens[POS].oper == DIV)
    {
        int op = tokens[POS].oper;
        POS += 1;

        Node* nodeleft = new_node;
        Node* noderight = GetLog(tokens);
        
        if(op == MULT)
            new_node = CreateOper(nodeleft, noderight, MULT);

        else
            new_node = CreateOper(nodeleft, noderight, DIV);
    }
    
    return new_node;
}

Node* GetLog(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetDeg(tokens);

    while(tokens[POS].oper == LOG)
    {
        POS += 1;

        Node* node_left = new_node;
        Node* noderight = GetDeg(tokens);

        return CreateOper(node_left, noderight, LOG);
    }

    return new_node;
}

Node* GetDeg(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetTrig(tokens);

    while(tokens[POS].oper == POW)
    {
        POS += 1;

        Node* node_left = new_node;
        Node* noderight = GetTrig(tokens);

        return CreateOper(node_left, noderight, POW);
    }

    return new_node;
}

Node* GetTrig(struct Token* tokens)
{
$$$ assert(tokens);

    Node* new_node = GetBracket(tokens);

    if(tokens[POS].oper == SIN || tokens[POS].oper == COS || tokens[POS].oper == TAN)
    {
    //$$$ printf("Trigo\n");
        while(tokens[POS].oper == SIN || tokens[POS].oper == COS || tokens[POS].oper == TAN)
        {
            int op = tokens[POS].oper;
            POS += 1;

            Node* node = GetBracket(tokens);

            if(op == SIN)
                new_node = CreateOper(NULL, node, SIN);

            else if(op == COS)
                new_node = CreateOper(NULL, node, COS);

            else
                new_node = CreateOper(NULL, node, TAN);
        }
    }

    return new_node;
}

Node* GetBracket(struct Token* tokens)
{
$$$ assert(tokens);
//$$$ printf("BIG BLACK COMMENTs6 pos = %u\n", POS);
    if(tokens[POS].oper == OP_BR)
    {
        POS += 1;

        Node* node = GetAddSub(tokens);

        if(tokens[POS].oper != CL_BR)
            SyntaxErr(tokens);
    
        POS += 1;

        return node;
    }

    else
        return GetNumVar(tokens);
}

Node* GetNumVar(struct Token* tokens)
{
$$$ assert(tokens);
//$$$ printf("BIG BLACK COMMENTs7 pos = %u\n", POS);

    if(tokens[POS].type == VAR)
    {
        POS += 1;
//$$$ printf("VAR pos = %u var = %c\n", POS, tokens[POS - 1].var);

        Node* node = CreateVar(NULL, NULL, tokens[POS - 1].var);
//$$$ printf("VAR2 pos = %u node = %p\n", POS, node);
        return node;
    }

    if(tokens[POS].type == NUM)
    {
        POS += 1;
//$$$ printf("%d NUM pos = %u\n", __LINE__, POS);
        Node* node = CreateNum(NULL, NULL, tokens[POS - 1].value);
//$$$ printf("%d NUM pos = %u\n", __LINE__, POS);
        return node;
    }

    else
        return NULL;


}

Token* LexicalAnalis(char* text)
{
    unsigned int count_words = WordsCounter(text) + 10;
$$$ printf("Count of words = %u\n", count_words);
    Token* tokens = (Token*) calloc(count_words, sizeof(Token));

    unsigned int count_tokens = 0;

    unsigned int text_size = SymbolsCounter(text);
$$$ printf("Count of symbols = %u\n", text_size);

    unsigned int line = 1;
    unsigned int symbol = 1;

    for(unsigned int i = 0; i < text_size; i++)
    {
$$$ printf("BIG BLACK COMMENT%d\n", i);
        symbol += 1;

        if(text[i] == '\r')
        {
            line += 1;
            symbol = 0;
            i += 2;
        }

        else if(text[i] == ' ')
            i += 1;

        else
        {
            tokens[count_tokens].line = line;
            tokens[count_tokens].symbol = symbol;

            if(text[i] == '(')
            {
            $$$ printf("Open Br i = %d\n", i);
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = OP_BR;
            }

            else if(text[i] == ')')
            {
            $$$ printf("Close Br i = %d\n", i);
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = CL_BR;
            }

            else if(text[i] == '+')
            {
            $$$ printf("sum i = %d\n", i);
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = ADD;
            }

            else if(text[i] == '-')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = SUB;
            }

            else if(text[i] == '*')
            {
                $$$ printf("mult i = %d\n", i);
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = MULT;
            }

            else if(text[i] == '/')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = DIV;
            }

            else if(text[i] == '^')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = POW;
            }

            else if(text[i] == ',')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = COMMA;
            }

            else if(text[i] == '\0')
            {
                $$$ printf("end\n");
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = END;

                break;
            }

            else if(isdigit(text[i]) != 0)
            {
            $$$ printf("num i = %d\n", i);
                tokens[count_tokens].type = NUM;

                char* endp = NULL;

                tokens[count_tokens].value = strtod(text + i, &endp);
            $$$ printf("value = %g text+i = %p endp = %p tokens[count_tokens].value = %g\n", tokens[count_tokens].value, text+i, endp, tokens[count_tokens].value);

            $$$ printf("i = %d shift = %d\n", i, (endp - text));
                i += (unsigned int) (endp - text) - i - 1;
            $$$ printf("i = %d\n", i);
            }
            
            else if(isalpha(text[i]) != 0)
            {
                $$$ printf("isalpha i = %d %s\n", i, text+i);
                if(strncmp(text + i, "sin", sizeof("sin")/sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = SIN;
                    i += 2;
                }

                else if(strncmp(text + i, "log", sizeof("log")/sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = LOG;
                    i += 2;
                }

                else if(strncmp(text + i, "cos", sizeof("cos")/sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = COS;
                    i += 2;
                }

                else if(strncmp(text, "tan", sizeof("tan")/sizeof(char) - 1) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = TAN;
                    i += 2;
                }
                
                else if('a' < text[i] < 'z')
                {
                    tokens[count_tokens].type = VAR;
                    tokens[count_tokens].var = text[i];
                    $$$ printf("var i = %d\n", i);
                }
            }
            
            count_tokens += 1;
        }
    }

$$$ printf("count_tokens = %u\n", count_tokens);
    return tokens;
}

unsigned int WordsCounter(char* str)
{
    unsigned int count = 1;
    unsigned int pos   = 1;

    while(str[pos] != '\0')
    {

        if((isdigit(str[pos - 1]) != isdigit(str[pos]) || isalpha(str[pos - 1]) != isalpha(str[pos])) && str[pos] != ' ' && str[pos] != '.')
            count += 1;

        pos += 1;
    }

    return count;
}

unsigned int SymbolsCounter(char* str)
{
    unsigned int count = 1;
    unsigned int pos   = 0;

    while(str[pos] != '\0')
    {
        count += 1;

        pos   += 1;
    }

    return count;
}

void LexicErr(unsigned int line, unsigned int symbol)
{
    printf("error on: line = %u, symbol = %u\n", line, symbol);

    exit(1);
}

void SyntaxErr(struct Token* tokens)
{
$$$ assert(tokens);
    printf("error on: line = %u symbol = %u\n", tokens[POS].line, tokens[POS].symbol);

    exit(1);
}

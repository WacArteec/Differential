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
Node* GetAddSub (struct Token* tokens, struct Tree* tree);
Node* GetMulDiv (struct Token* tokens, struct Tree* tree);
Node* GetTrig   (struct Token* tokens, struct Tree* tree);
Node* GetLog    (struct Token* tokens, struct Tree* tree);
Node* GetDeg    (struct Token* tokens, struct Tree* tree);
Node* GetBracket(struct Token* tokens, struct Tree* tree);
Node* GetNumVar (struct Token* tokens, struct Tree* tree);
void SyntaxErr(struct Token* tokens);

unsigned int pos = 0;

Node* Parser(char* text, struct Tree* tree)
{
    text = LowString(text);

    Token* tokens = LexicalAnalis(text);
$$$ printf("BIG BLACK COMMENT4\n");

    Node* node = GetAddSub(tokens, tree);
$$$ printf("BIG BLACK COMMENT5\n");

    if(tokens[pos].oper != END)
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

Token* LexicalAnalis(char* text)
{
    unsigned int count_words = WordsCounter(text) + 1;
$$$ printf("Count of words = %u\n", count_words);
    Token* tokens = (Token*) calloc(count_words, sizeof(Token));

    unsigned int count_tokens = 0;

    unsigned int text_size = SymbolsCounter(text);
$$$ printf("Count ofsymbols = %u\n", text_size);

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
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = OP_BR;
            }

            else if(text[i] == ')')
            {
                tokens[count_tokens].type = OPER;
                tokens[count_tokens].oper = CL_BR;
            }

            else if(text[i] == '+')
            {
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
                tokens[count_tokens].type = NUM;

                char* endp = NULL;

                tokens[count_tokens].value = strtod(text, &endp);

                i += (unsigned int) (endp - text);
            }
            
            else if(isalpha(text[i]) != 0)
            {
                if(strncmp(text, "sin", sizeof("sin")/sizeof(char)) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = SIN;
                    i += 2;
                }

                else if(strncmp(text, "log", sizeof("log")/sizeof(char)) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = LOG;
                    i += 2;
                }

                else if(strncmp(text, "cos", sizeof("cos")/sizeof(char)) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = LOG;
                    i += 2;
                }

                else if(strncmp(text, "tan", sizeof("tan")/sizeof(char)) == 0)
                {
                    tokens[count_tokens].type = OPER;
                    tokens[count_tokens].oper = LOG;
                    i += 2;
                }
                
                else if('a' < text[i] < 'z')
                {
                    tokens[count_tokens].type = VAR;
                    tokens[count_tokens].var = text[i];
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

        if((isdigit(str[pos - 1]) != isdigit(str[pos]) || isalpha(str[pos - 1]) != isalpha(str[pos])) && str[pos] != ' ' && str[pos] != '.' && str[pos - 1] != '.')
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

Node* GetAddSub(struct Token* tokens, struct Tree* tree)
{
$$$ assert(tokens);
$$$ printf("BIG BLACK COMMENTs1 pos = %u\n", pos);
    Node* node = GetMulDiv(tokens, tree);
$$$ printf("BIG BLACK COMMENTs1.1 pos = %u\n", pos);

    while(tokens[pos].oper == ADD || tokens[pos].oper == SUB)
    {
$$$ printf("ADD/SUB s1 pos = %u\n", pos);
        int op = tokens[pos].oper;
        pos += 1;

        Node* node2 = GetMulDiv(tokens, tree);

        if(op == ADD)
        {
$$$ printf("ADD/SUB s1.1 pos = %u\n", pos);
            return CreateNode(NULL, node, node2, OPER, 0, ADD);
        }

        else
            return CreateNode(NULL, node, node2, OPER, 0, SUB);
    }

    return node;
}

Node* GetMulDiv(struct Token* tokens, struct Tree* tree)
{
$$$ assert(tokens);
$$$ printf("BIG BLACK COMMENTs2 pos = %u\n", pos);
    Node* node = GetTrig(tokens, tree);
$$$ printf("BIG BLACK COMMENTs2.1 pos = %u\n", pos);

    while(tokens[pos].oper == MULT || tokens[pos].oper == DIV)
    {
        int op = tokens[pos].oper;
        pos++;

        Node* node2 = GetTrig(tokens, tree);
        

        if(op == MULT)
            return CreateNode(NULL, node, node2, OPER, 0, MULT);

        else
        {
            if(node2->value != 0)
                return CreateNode(NULL, node, node, OPER, 0, DIV);
            
            else
                pos--;
        }
    }
    
    return node;
}

Node* GetTrig(struct Token* tokens, struct Tree* tree)
{
$$$ assert(tokens);
$$$ printf("BIG BLACK COMMENTs3 pos = %u\n", pos);
    if(tokens[pos].oper == SIN || tokens[pos].oper == COS || tokens[pos].oper == TAN)
    {
        while(tokens[pos].oper == SIN || tokens[pos].oper == COS || tokens[pos].oper == TAN)
        {
            int op = tokens[pos].oper;
            pos += 1;

            Node* node = GetLog(tokens, tree);

            if(op == SIN)
                return CreateNode(NULL, NULL, node, OPER, 0, SIN);

            else if(op == COS)
                return CreateNode(NULL, NULL, node, OPER, 0, COS);

            else
                return CreateNode(NULL, NULL, node, OPER, 0, TAN);
        }
    }

    Node* node = GetLog(tokens, tree);
$$$ printf("BIG BLACK COMMENTs3.1 pos = %u\n", pos);
    return node;
}

Node* GetLog(struct Token* tokens, struct Tree* tree)
{
$$$ assert(tokens);
$$$ printf("BIG BLACK COMMENTs4 pos = %u\n", pos);
    Node* node = GetDeg(tokens, tree);
$$$ printf("BIG BLACK COMMENTs4.1 pos = %u\n", pos);

    while(tokens[pos].oper == LOG)
    {
        pos += 1;

        Node* node2 = GetDeg(tokens, tree);

        if(node->value > 0 && node2->value > 0 && node2->value != 0)
        {
                return CreateNode(NULL, node, node2, OPER, 0, LOG);
        }
            
        else
            pos -= 1;
    }
    return node;
}

Node* GetDeg(struct Token* tokens, struct Tree* tree)
{
$$$ assert(tokens);
$$$ printf("BIG BLACK COMMENTs5 pos = %u\n", pos);
    Node* node = GetBracket(tokens, tree);
$$$ printf("BIG BLACK COMMENTs5.1 pos = %u\n", pos);

    while(tokens[pos].oper == POW)
    {
        pos += 1;

        Node* node2 = GetBracket(tokens, tree);

        if(node->value != 0 || node2->value > 0)
            return CreateNode(NULL, node, node2, OPER, 0, POW);
                
        else
            pos -= 1;

    }
    return node;
}

Node* GetBracket(struct Token* tokens, struct Tree* tree)
{
$$$ assert(tokens);
$$$ printf("BIG BLACK COMMENTs6 pos = %u\n", pos);
    if(tokens[pos].oper == OP_BR)
    {
        pos += 1;

        Node* node = GetAddSub(tokens, tree);

        if(tokens[pos].oper != CL_BR)
            SyntaxErr(tokens);
    
        pos += 1;
        return node;
    }

    else
        return GetNumVar(tokens, tree);
}

Node* GetNumVar(struct Token* tokens, struct Tree* tree)
{
$$$ assert(tokens);
$$$ printf("BIG BLACK COMMENTs7 pos = %u\n", pos);

    if(tokens[pos].type == VAR)
    {
        pos += 1;
$$$ printf("VAR pos = %u\n", pos);

        Node* node = CreateNode(NULL, NULL, NULL, VAR, (double) tokens[pos - 1].var, DEF_OPER);
$$$ printf("VAR2 pos = %u\n", pos);
        return node;
    }

    if(tokens[pos].type == NUM)
    {
        pos += 1;
$$$ printf("NUM pos = %u\n", pos);
        Node* node = CreateNode(NULL, NULL, NULL, NUM, tokens[pos - 1].value, DEF_OPER);
        return node;
    }

    else
        return NULL;

}

void SyntaxErr(struct Token* tokens)
{
$$$ assert(tokens);
    printf("error on: line = %u symbol = %u\n", tokens[pos].line, tokens[pos].symbol);

    exit(1);
}
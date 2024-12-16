#ifndef TREE_H
#define TREE_H
#define $$$

#include <stdio.h>
#include <stdlib.h>

enum Side
{
    ZERO,
    LEFT,
    RIGHT
};

enum Type
{
    DEFAULT,
    NUM,
    OPER,
    VAR
};

enum Operations
{
    DEF_OPER,
    OP_BR,
    CL_BR,
    ADD,
    SUB,
    MULT,
    DIV,
    POW,
    LOG,
    SIN,
    COS,
    TAN,
    COMMA,
    EQUAL,
    INIT,
    IF,
    TILL,
    END
};

struct Tree;

union Data
{
    double value;
    Operations oper;
    char var;
};

struct Node
{
    Type type = DEFAULT;

    Data data = {};

    Node* left = NULL;
    Node* right = NULL;
};

struct Tree
{
    Node* root = NULL;
    unsigned int size = 0;      // add file_name for dump(dot and default), FILE*?
};

Node* CreateNum(struct Node* left, struct Node* right, float value);
Node* CreateVar(struct Node* left, struct Node* right, char var);
Node* CreateOper(struct Node* left, struct Node* right, Operations oper);
Node* DeleteNode(struct Node* node);
void SaveTree(struct Tree* tree);
void PrintNode(FILE* stream, struct Node* leaf);
void Ctor(struct Tree* tree);
void Dtor(struct Tree* tree);
void GraphDump(struct Tree* tree);

#endif //TREE_H
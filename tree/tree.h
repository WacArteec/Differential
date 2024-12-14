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
    END
};

struct Tree;

union Value
{
    double value;
    Operations oper_code;
};

struct Node
{
    Type type = DEFAULT;

    float value = 0;
    Operations oper = DEF_OPER;

    Node* left = NULL;
    Node* right = NULL;

    Node* parent = NULL;

    Tree* root_tree = NULL;
};

struct Tree
{
    Node* root = NULL;
    unsigned int size = 0;      // add file_name for dump(dot and default), FILE*?
};

void InsertNode(struct Node* leaf, Side side, Type type, int value, Operations oper);
Node* CreateNode(struct Node* parent, struct Node* left, struct Node* right, Type type, float value, Operations oper);
void DeleteNode(struct Node* leaf);
void SaveTree(struct Tree* tree);
void PrintNode( FILE* stream, struct Node* leaf);
void Ctor(struct Tree* tree);
void Dtor(struct Tree* tree);
void GraphDump(struct Tree* tree);

#endif //TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"

const char* blue = "#87cefa";
const char* yellow = "#FFFFE0";
const char* green = "#C0FFC0";

// add verificator and dump

void PrintNode( FILE* stream, struct Node* node);
void DotNode(FILE* dot_file, struct Node* node, Side side);

// add asserts

void Ctor(struct Tree* tree)
{
    assert(tree);

    tree->root = (Node*) calloc(1, sizeof(Node));

    tree->root->type = DEFAULT;
    tree->root->data = {};

    tree->root->right = NULL;
    tree->root->left  = NULL;
}

Node* CreateNum(struct Node* left, struct Node* right, float value)
{
    struct Node* node = (Node*) calloc(1, sizeof(Node));
$$$ printf("%d NUM\n", __LINE__);
    assert(node);

    node->type = NUM;
    node->data.value = value;

    node->left  = left;
    node->right = right;

    return node;
}

Node* CreateVar(struct Node* left, struct Node* right, char var)
{
    struct Node* node = (Node*) calloc(1, sizeof(Node));
    $$$ printf("cereate var\n");
    assert(node);

    node->type = VAR;
    node->data.var = var;

    node->left  = left;
    node->right = right;

    return node;
}

Node* CreateOper(struct Node* left, struct Node* right, Operations oper)
{
    struct Node* node = (Node*) calloc(1, sizeof(Node));
    assert(node);

    node->type = OPER;
    node->data.oper = oper;

    node->left  = left;
    node->right = right;

    return node;
}

void SaveTree(struct Tree* tree)
{
    assert(tree);

    FILE* database = fopen("DataBase.txt", "wb");
    assert(database);

    PrintNode(database, tree->root);

    fclose(database);
}

// (
// print_node(node->left)
// printf(node->value)
// print_node(node->right)
// )

void PrintNode(FILE* stream, struct Node* node)
{
    assert(stream);

    if(node != NULL)
    {
        fprintf(stream, " ( %d ", node->type);

        if(node->type == OPER)
            fprintf(stream, " %d ", node->data.oper);

        else if(node->type == VAR)
            fprintf(stream, " %c ", node->data.var);
        
        else if(node->type == NUM)
            fprintf(stream, " %g ", node->data.value);

        PrintNode(stream, node->left);
        PrintNode(stream, node->right);

        fprintf(stream, " ) ");
    }

    else
    {
        fprintf(stream, "nil");
    }

}

Node* DeleteNode(struct Node* node)
{
    if(node == NULL)
        return NULL;

$$$ printf("DeleteNode\n");
    node->right = DeleteNode(node->right);
    node->right = DeleteNode(node->left);

$$$ printf("freeNode\n");
    free(node);

    return NULL;
}

void Dtor(struct Tree* tree)
{
    assert(tree);

    tree->size = 0;

    DeleteNode(tree->root);
}

void GraphDump(struct Tree* tree)
{
    assert(tree);

$$$ printf("\n\n%d\n\n", __LINE__);
    FILE* dot_file = fopen("Tree.dot", "wb");
    assert(dot_file);

    fprintf(dot_file, "digraph G{\n\trankdir=HR;\n");

    fprintf(dot_file, "\tbgcolor = \"#FFF0DB\";\n"); //\tnode[shape=plaintext];\n\tedge[color=white];\n instead of

	fprintf(dot_file, "\tnode0 [shape = Mrecord, style = filled, fillcolor = \"#FFB02E\", label = \"Tree_addres: %p | root: %p | size: %u\"];\n", 
                                                                                                                 tree,      tree->root,tree->size);

    DotNode(dot_file, tree->root, ZERO);

    fprintf(dot_file, "\tnode0 -> node_%p [color = \"#000000\"]\n}", tree->root);

    fclose(dot_file);

$$$ printf("before system\n");
    system("dot -Tpng -O Tree.dot");
$$$ printf("inside system\n");
    system("start Tree.dot.png");
$$$ printf("after system\n");
}

void DotNode(FILE* dot_file, struct Node* node, Side side)
{
    const char* color = "#000000";

    if(node == NULL)
        return;

    switch (side)
    {
        case LEFT:
        {
            color = blue;
            break;
        }
        
        case RIGHT:
        {
            color = yellow;
            break;
        }

        case ZERO:
        {
            color = green;
            break;
        }
        
        default:
            break;
    }
    
    if(node->type == OPER)
	    fprintf(dot_file, "\tnode_%p [shape = Mrecord, style = filled, fillcolor = \"%s\", label = \"{ addres: %p | type: OPER | oper: %d | left(yes): %p | right(no): %p }\"];\n", 
                                  node,                                              color,                  node,                     node->data.oper,node->left,     node->right);

    else if(node->type == VAR)
	    fprintf(dot_file, "\tnode_%p [shape = Mrecord, style = filled, fillcolor = \"%s\", label = \"{ addres: %p | type: VAR | variable: %c | left(yes): %p | right(no): %p }\"];\n", 
                                  node,                                              color,                  node,                      node->data.var, node->left,     node->right);
    
    else if(node->type == NUM)
	    fprintf(dot_file, "\tnode_%p [shape = Mrecord, style = filled, fillcolor = \"%s\", label = \"{ addres: %p | type: NUM | value: %g | left(yes):  %p | right(no): %p }\"];\n", 
                                  node,                                              color,                  node,                     node->data.value,node->left,     node->right);
    
    DotNode(dot_file, node->left, LEFT);

    if(node->left != NULL)
        fprintf(dot_file, "\tnode_%p -> node_%p [color = \"#000000\"]\n\n", node, node->left);

    DotNode(dot_file, node->right, RIGHT);
    
    if(node->right != NULL)
        fprintf(dot_file, "\tnode_%p -> node_%p [color = \"#000000\"]\n\n", node, node->right);
}
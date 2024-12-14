#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"

const char* blue = "#87cefa";
const char* yellow = "#FFFFE0";
const char* green = "#177245";

// add verificator and dump

void PrintNode( FILE* stream, struct Node* leaf);
int DotNode(FILE* dot_file, struct Node* leaf, Side agree, int parent_num);

// add asserts

void Ctor(struct Tree* tree)
{
    assert(tree);

    tree->root = (Node*) calloc(1, sizeof(Node));

    tree->root->type = DEFAULT;
    tree->root->value = {};

    tree->root->parent = NULL;
    tree->root->root_tree = tree;

    tree->root->right = NULL;
    tree->root->left  = NULL;
}

// char *data, left, right, parent

// separate funcs for add node left or right

void InsertNode(struct Node* leaf, Side side, Type type, int value, Operations oper)
{
    switch (side)
    {
        case LEFT:
        {
            leaf->left = CreateNode(leaf, NULL, NULL, type, value, oper);
            break;
        }

        case RIGHT:
        {
            leaf->right = CreateNode(leaf, NULL, NULL, type, value, oper);
            break;
        }

        default:
            break;
    }

    leaf->root_tree->size++;
}

Node* CreateNode(struct Node* parent, struct Node* left, struct Node* right, Type type, float value, Operations oper)
{
$$$ printf("BIG BLACK COMMENT C1 sizeof(Node) = %u \n", sizeof(Node));
    struct Node* node = (Node*) calloc(1, sizeof(struct Node));
$$$ assert(node);
$$$ printf("BIG BLACK COMMENT C2 node = %p\n", node);

    node->type = type;

    node->value = value;
    node->oper = oper;

    node->left  = left;
    node->right = right;

    if(parent != NULL)
    {
        node->parent = parent;
        node->root_tree = parent->root_tree;
    }
    
    else
    {
        node->parent = NULL;
        node->root_tree = NULL;
    }

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

void PrintNode(FILE* stream, struct Node* leaf)
{
    assert(stream);

    if(leaf != NULL)
    {
        fprintf(stream, "(");

        PrintNode(stream, leaf->left);

        fprintf(stream, "%d %d", leaf->type, leaf->value);

        PrintNode(stream, leaf->right);

        fprintf(stream, ")");
    }

    else
    {
        fprintf(stream, "nil");
    }

}

void DeleteNode(struct Node* leaf)
{
    if(leaf == NULL)
        return;

    if(leaf->parent != NULL)
    {
        if(leaf->parent->right == leaf)
            leaf->parent->right = NULL;

        else if(leaf->parent->left == leaf)
            leaf->parent->left = NULL;
    }
$$$ printf("BIG BLACK COMMENT Del\n");

    DeleteNode(leaf->right);
$$$ printf("BIG BLACK COMMENT DelR\n");
    DeleteNode(leaf->left);
$$$ printf("BIG BLACK COMMENT DelL leaf = %p\n", leaf);

    free(leaf);
$$$ printf("BIG BLACK COMMENT DelFree\n");
    leaf = NULL;
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

    DotNode(dot_file, tree->root, ZERO, 0);

    fprintf(dot_file, "\tnode0 -> node1 [color = \"#000000\"]\n}");

    fclose(dot_file);

    system("dot -Tpng -O Tree.dot");
    system("Tree.dot.png");
}

int DotNode(FILE* dot_file, struct Node* leaf, Side side, int parent_num)
{
    const char* color = "#000000";
    int current_num = parent_num + 1;

    if(leaf == NULL)
        return 0;

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
    
	fprintf(dot_file, "\tnode%d [shape = Mrecord, style = filled, fillcolor = \"%s\", label = \"addres: %p | type: %d | value: %d | left(yes): %p | right(no): %p | parent: %p \"];\n", 
                       parent_num + 1,                                          color,                  leaf,      leaf->type, leaf->value,    leaf->left,     leaf->right, leaf->parent);

    int left_child_num = DotNode(dot_file, leaf->left, LEFT, current_num);

    if(left_child_num != 0)
        fprintf(dot_file, "\tnode%d -> node%d [color = \"#000000\"\"]\n\n", current_num, left_child_num);

    int right_child_num = DotNode(dot_file, leaf->right, RIGHT, left_child_num);
    
    if(right_child_num != 0)
        fprintf(dot_file, "\tnode%d -> node%d [color = \"#000000\"\"]\n\n", current_num, right_child_num);

    return current_num;
}
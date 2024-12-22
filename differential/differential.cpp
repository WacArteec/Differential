#include <stdio.h>
#include <math.h>

#include "differential.h"
#include "printdiff.h"
#include "../tree/tree.h"

double e = 2.71828182845905;

Node *Diff(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffAdd(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffSub(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffMult(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffDiv(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffPow(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffLog(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffSin(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffCos(struct Node *node, struct Tree *tree, FILE *stream);
Node *DiffTan(struct Node *node, struct Tree *tree, FILE *stream);

Node *DiffTree(struct Node *node, struct Tree *tree, FILE *stream)
{
    Node *new_node = Diff(node, tree, stream);

    node = DeleteNode(node);
    $$$ printf("new_node = %p\n", new_node);

    return new_node;
}

Node *Diff(struct Node *node, struct Tree *tree, FILE *stream)
{
    Node *new_node = NULL;

    if (node->type == NUM)
    {
        $$$ printf("diff num\n");
        tree->size += 0;

        new_node = CreateNum(NULL, NULL, 0);
    }

    else if (node->type == VAR)
    {
        $$$ printf("diff var\n");
        tree->size += 0;

        new_node = CreateNum(NULL, NULL, 1);
    }

    else if (node->type == OPER)
    {
        if (node->data.oper == ADD)
            new_node = DiffAdd(node, tree, stream);

        else if (node->data.oper == SUB)
            new_node = DiffSub(node, tree, stream);

        else if (node->data.oper == MULT)
            new_node = DiffMult(node, tree, stream);

        else if (node->data.oper == DIV)
            new_node = DiffDiv(node, tree, stream);

        else if (node->data.oper == POW)
            new_node = DiffPow(node, tree, stream);

        else if (node->data.oper == LOG)
            new_node = DiffLog(node, tree, stream);

        else if (node->data.oper == SIN)
            new_node = DiffSin(node, tree, stream);

        else if (node->data.oper == COS)
            new_node = DiffCos(node, tree, stream);

        else if (node->data.oper == TAN)
            new_node = DiffTan(node, tree, stream);
    }

    else
    {
        printf("Problems with tree making\n");
        exit(1);
    }

    return new_node;
}

Node *DiffAdd(struct Node *node, struct Tree *tree, FILE *stream)
{
    $$$ printf("diff add\n");

    assert(node->left);
    Node *diff_left = Diff(node->left, tree, stream);
    Node *diff_right = Diff(node->right, tree, stream);

    $$$ printf("diff left = %p value = %g diff right = %p value = %g\n", diff_left, diff_left->data.value, diff_right, diff_right->data.value);

    Node* new_node = CreateOper(diff_left, diff_right, ADD);
    //Node* simpled_new = TreeSimpler(new_node, tree, stream);

    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(new_node, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffSub(struct Node *node, struct Tree *tree, FILE *stream)
{
    assert(node->left);

    Node *diff_left = Diff(node->left, tree, stream);
    Node *diff_right = Diff(node->right, tree, stream);

    Node* new_node = CreateOper(diff_left, diff_right, SUB);
    //Node* simpled_new = TreeSimpler(new_node, tree, stream);

    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(new_node, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffMult(struct Node *node, struct Tree *tree, FILE *stream)
{
    assert(node->left);
    $$$ printf("diff mult\n");
    Node *diff_diff_left = Diff(node->left, tree, stream);
    Node *diff_left = CreateOper(diff_diff_left, Copy(node->right, tree), MULT);

    Node *diff_diff_right = Diff(node->right, tree, stream);
    Node *diff_right = CreateOper(Copy(node->left, tree), diff_diff_right, MULT);

    tree->size += 2; // from 3( A * B ) to 7( A' * B + A * B' ) - 2 copies

    Node* new_node = CreateOper(diff_left, diff_right, ADD);
    //Node* simpled_new = TreeSimpler(new_node, tree, stream);

    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(new_node, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffDiv(struct Node *node, struct Tree *tree, FILE *stream)
{
    assert(node->left);

    Node *diff_nom_left = Diff(node->left, tree, stream);
    Node *nom_left = CreateOper(diff_nom_left, Copy(node->right, tree), MULT);

    Node *diff_nom_right = Diff(node->right, tree, stream);
    Node *nom_right = CreateOper(Copy(node->left, tree), diff_nom_right, MULT);

    Node *diff_left = CreateOper(nom_left, nom_right, SUB);

    Node *denom_left = Copy(node->right, tree);
    Node *denom_right = Copy(node->right, tree);

    Node *diff_right = CreateOper(denom_left, denom_right, ADD);

    tree->size += 4; // from 3 (A / B) to 11 ( (A' * B - A * B') / (B * B) ) - 4 copies

    Node* new_node = CreateOper(diff_left, diff_right, DIV);
    //Node* simpled_new = TreeSimpler(new_node, tree, stream);

    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(new_node, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffPow(struct Node *node, struct Tree *tree, FILE *stream) // некрасиво написана, но не знаю как переписать
{
    assert(node);
    assert(node->left);
    assert(node->right);

    Node *number_e = CreateNum(NULL, NULL, e);
    Node *ln = CreateOper(number_e, Copy(node->left, tree), LOG);

    Node *diff_add1 = Diff(node->right, tree, stream);
    Node *add1 = CreateOper(ln, diff_add1, MULT);

    Node *diff_mult_in_add2 = Diff(node->left, tree, stream);
    Node *mult_in_add2 = CreateOper(Copy(node->right, tree), diff_mult_in_add2, MULT);

    Node *add2 = CreateOper(mult_in_add2, Copy(node->left, tree), DIV);

    Node *diff_left = CreateOper(add1, add2, ADD);

    Node *diff_right = Copy(node, tree);

    tree->size += 6; // from 3 ( A ^ B ) to 15 ( ( LOG(e, A) * B' + A' * B / A ) * (A ^ B) ) - 6 copies

    Node* new_node = CreateOper(diff_left, diff_right, MULT);
    Node* simpled_new = TreeSimpler(new_node, tree, stream);
    
    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(simpled_new, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffLog(struct Node *node, struct Tree *tree, FILE *stream)
{
    assert(node->left);

    Node *diff_nom_left = Diff(node->right, tree, stream);
    Node *nom_left = CreateOper(diff_nom_left, Copy(node->right, tree), MULT);

    Node *diff_nom_right = Diff(node->left, tree, stream);

    Node *nom_right_mult = CreateOper(diff_nom_right, Copy(node, tree), MULT);
    Node *nom_right = CreateOper(nom_right_mult, Copy(node->left, tree), DIV);

    Node *diff_left = CreateOper(nom_left, nom_right, SUB);

    Node *number_e = CreateNum(NULL, NULL, e);
    Node *denom_right = Copy(node->right, tree);

    Node *diff_right = CreateOper(number_e, denom_right, ADD);

    tree->size += 6; // from 3 log(A, B) to 15 ( (B' / B - A' * log(A, B) / A) / log(e, B) ) - 6 copies

    Node* new_node = CreateOper(diff_left, diff_right, DIV);
    //Node* simpled_new = TreeSimpler(new_node, tree, stream);

    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(new_node, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffSin(struct Node *node, struct Tree *tree, FILE *stream)
{
    //assert(node->left);

    $$$ printf("VAR2 pos = \n");
    Node *diff_right = CreateOper(NULL, Copy(node->right, tree), COS);

    Node *diff_left = Diff(node->right, tree, stream);
    $$$ printf("VAR2 sin = \n");

    $$$ printf("VAR2 sin = \n");

    tree->size += 3; // from 2( sin(A) ) to 6( 1 * A' * cos(A) ) - 1 copy

    $$$ printf("sin left = %p right = %p\n", diff_left, diff_right);

    Node* new_node = CreateOper(diff_left, diff_right, MULT);
    Node* simpled_new = TreeSimpler(new_node, tree, stream);
    
    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(simpled_new, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffCos(struct Node *node, struct Tree *tree, FILE *stream)
{
    assert(node->left);

    Node *num_minus1 = CreateNum(NULL, NULL, -1);
    Node *diff_in_diff_left = Diff(node->left, tree, stream);
    Node *diff_left = CreateOper(diff_in_diff_left, num_minus1, MULT);

    Node *diff_right = CreateOper(NULL, Copy(node->right, tree), SIN);

    tree->size += 3; // from 2( cos(A) ) to 6( (-1) * A' * sin(A) ) - 1 copy

    Node* new_node = CreateOper(diff_left, diff_right, MULT);
    //Node* simpled_new = TreeSimpler(new_node, tree, stream);
    
    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(new_node, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

Node *DiffTan(struct Node *node, struct Tree *tree, FILE *stream)
{
    assert(node->left);

    Node *diff_left = Diff(node->left, tree, stream);

    Node *cos1 = CreateOper(NULL, Copy(node->right, tree), COS);
    Node *cos2 = CreateOper(NULL, Copy(node->right, tree), COS);

    Node *diff_right = CreateOper(cos1, cos2, MULT);

    tree->size += 3; // from 2( tan(A) ) to 7( A' / (cos(A) * cos(A)) ) - 2 copies

    Node* new_node = CreateOper(diff_left, diff_right, DIV);
    //Node* simpled_new = TreeSimpler(new_node, tree, stream);
    
    fprintf(stream, "Clearly:\n $$d(");
    PrintDiff(node, stream);
    fprintf(stream, ") = ");
    PrintDiff(new_node, stream);
    fprintf(stream, " = ");
    //PrintDiff(simpled_new, stream);
    fprintf(stream, "$$\n");

    return new_node;
}

/*void Simpler(struct Tree* tree)
{
    unsigned int old_size = 0;

    while(old_size != tree->size)
    {
        old_size = tree->size;

        NumSimpler(tree->root, tree);
        VarSimpler(tree->root, tree);
    }
}*/



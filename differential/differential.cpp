#include <stdio.h>
#include <math.h>

#include "differential.h"
#include "../tree/tree.h"

double AIM = 1.0e-10;
double e = 2.71828182845905;

enum Error
{
    DIV_0,
    POW_0,
    LOG_0
};

Node *Copy(struct Node *node, struct Tree *tree);
Node *DiffAdd(struct Node *node, struct Tree *tree);
Node *DiffSub(struct Node *node, struct Tree *tree);
Node *DiffMult(struct Node *node, struct Tree *tree);
Node *DiffDiv(struct Node *node, struct Tree *tree);
Node *DiffPow(struct Node *node, struct Tree *tree);
Node *DiffLog(struct Node *node, struct Tree *tree);
Node *DiffSin(struct Node *node, struct Tree *tree);
Node *DiffCos(struct Node *node, struct Tree *tree);
Node *DiffTan(struct Node *node, struct Tree *tree);
int CmpDoubles(double num1, double num2, double accuracy);
Node *NumSimpler(struct Node *node, struct Tree *tree);
void VarSimpler(struct Node *node, struct Tree *tree);
Node *AddSimpler(struct Node *node, struct Tree *tree);
Node *SubSimpler(struct Node *node, struct Tree *tree);
Node *MultSimpler(struct Node *node, struct Tree *tree);
Node *DivSimpler(struct Node *node, struct Tree *tree);
Node *PowSimpler(struct Node *node, struct Tree *tree);
Node *LogSimpler(struct Node *node, struct Tree *tree);
Node *TrigSimpler(struct Node *node, struct Tree *tree);
void MathErr(struct Node *node, Error error);

Node *Diff(struct Node *node, struct Tree *tree)
{
    if (node->type == NUM)
    {
        tree->size += 0;

        DeleteNode(node);

        return CreateNode(NULL, NULL, NULL, NUM, 0, DEF_OPER);
    }

    else if (node->type == VAR)
    {
        tree->size += 0;

        DeleteNode(node);

        return CreateNode(NULL, NULL, NULL, NUM, 1, DEF_OPER);
    }

    else if (node->type == OPER)
    {
        if (node->oper == ADD)
            return DiffAdd(node, tree);

        else if (node->oper == SUB)
            return DiffSub(node, tree);

        else if (node->oper == MULT)
            return DiffMult(node, tree);

        else if (node->oper == DIV)
            return DiffDiv(node, tree);

        else if (node->oper == POW)
            return DiffPow(node, tree);

        else if (node->oper == LOG)
            return DiffLog(node, tree);

        else if (node->oper == SIN)
            return DiffSin(node, tree);

        else if (node->oper == COS)
            return DiffCos(node, tree);

        else if (node->oper == TAN)
            return DiffTan(node, tree);
    }

    else
    {
        printf("Problems with tree making\n");
        exit(1);
    }

    return NULL;
}

Node *Copy(struct Node *node, struct Tree *tree)
{
    if (node != NULL)
    {
        tree->size += 1;
$$$ printf("BIG BLACK COMMENT Copy left = %p right = %p\n", node->left, node->right);

        Node* node = CreateNode(NULL, Copy(node->left, tree), Copy(node->right, tree), node->type, node->value, node->oper);
$$$ printf("BIG BLACK COMMENT Copy Good\n");
        return node;
    }

    else
        return NULL;
}

Node *DiffAdd(struct Node *node, struct Tree *tree)
{
    Node *diff_left  = Diff(node->left, tree);
    Node *diff_right = Diff(node->right, tree);

    tree->size += 0;
$$$ printf("BIG BLACK COMMENT D\n");
    DeleteNode(node);
$$$ printf("BIG BLACK COMMENT D\n");

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, ADD);
}

Node *DiffSub(struct Node *node, struct Tree *tree)
{
    Node *diff_left = Diff(node->left, tree);
    Node *diff_right = Diff(node->right, tree);

    tree->size += 0;

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, SUB);
}

Node *DiffMult(struct Node *node, struct Tree *tree)
{
    Node *diff_diff_left = Diff(node->left, tree);
    Node *diff_left = CreateNode(NULL, diff_diff_left, Copy(node->right, tree), OPER, 0, MULT);

    Node *diff_diff_right = Diff(node->right, tree);
    Node *diff_right = CreateNode(NULL, Copy(node->left, tree), diff_diff_right, OPER, 0, MULT);

    tree->size += 2; // from 3( A * B ) to 7( A' * B + A * B' ) - 2 copies

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, ADD);
}

Node *DiffDiv(struct Node *node, struct Tree *tree)
{
    Node *diff_nom_left = Diff(node->left, tree);
    Node *nom_left = CreateNode(NULL, diff_nom_left, Copy(node->right, tree), OPER, 0, MULT);

    Node *diff_nom_right = Diff(node->right, tree);
    Node *nom_right = CreateNode(NULL, Copy(node->left, tree), diff_nom_right, OPER, 0, MULT);

    Node *diff_left = CreateNode(NULL, nom_left, nom_right, OPER, 0, SUB);

    Node *denom_left = Copy(node->right, tree);
    Node *denom_right = Copy(node->right, tree);

    Node *diff_right = CreateNode(NULL, denom_left, denom_right, OPER, 0, ADD);

    tree->size += 4; // from 3 (A / B) to 11 ( (A' * B - A * B') / (B * B) ) - 4 copies

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, DIV);
}

Node *DiffPow(struct Node *node, struct Tree *tree) // некрасиво написана, но не знаю как переписать
{
    Node *number_e = CreateNode(NULL, NULL, NULL, NUM, e, DEF_OPER);
    Node *ln = CreateNode(NULL, number_e, Copy(node->left, tree), OPER, 0, LOG);

    Node *diff_add1 = Diff(node->right, tree);
    Node *add1 = CreateNode(NULL, ln, diff_add1, OPER, 0, MULT);

    Node *diff_mult_in_add2 = Diff(node->left, tree);
    Node *mult_in_add2 = CreateNode(NULL, Copy(node->right, tree), diff_mult_in_add2, OPER, 0, MULT);

    Node *add2 = CreateNode(NULL, mult_in_add2, Copy(node->left, tree), OPER, 0, DIV);

    Node *diff_left = CreateNode(NULL, add1, add2, OPER, 0, ADD);

    Node *diff_right = Copy(node, tree);

    tree->size += 6; // from 3 ( A ^ B ) to 15 ( ( LOG(e, A) * B' + A' * B / A ) * (A ^ B) ) - 6 copies

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, MULT);
}

Node *DiffLog(struct Node *node, struct Tree *tree)
{
    Node *diff_nom_left = Diff(node->right, tree);
    Node *nom_left = CreateNode(NULL, diff_nom_left, Copy(node->right, tree), OPER, 0, MULT);

    Node *diff_nom_right = Diff(node->left, tree);

    Node *nom_right_mult = CreateNode(NULL, diff_nom_right, Copy(node, tree), OPER, 0, MULT);
    Node *nom_right = CreateNode(NULL, nom_right_mult, Copy(node->left, tree), OPER, 0, DIV);

    Node *diff_left = CreateNode(NULL, nom_left, nom_right, OPER, 0, SUB);

    Node *number_e = CreateNode(NULL, NULL, NULL, NUM, e, DEF_OPER);
    Node *denom_right = Copy(node->right, tree);

    Node *diff_right = CreateNode(NULL, number_e, denom_right, OPER, 0, ADD);

    tree->size += 6; // from 3 log(A, B) to 15 ( (B' / B - A' * log(A, B) / A) / log(e, B) ) - 6 copies

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, DIV);
}

Node *DiffSin(struct Node *node, struct Tree *tree)
{
    Node *num_1 = CreateNode(NULL, NULL, NULL, NUM, 1, DEF_OPER);

    Node *diff_diff_left = Diff(node->left, tree);
    Node *diff_left = CreateNode(NULL, diff_diff_left, num_1, OPER, 0, MULT);

    Node *diff_right = CreateNode(NULL, NULL, Copy(node->right, tree), OPER, 0, COS);

    tree->size += 3; // from 2( sin(A) ) to 6( 1 * A' * cos(A) ) - 1 copy

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, MULT);
}

Node *DiffCos(struct Node *node, struct Tree *tree)
{
    Node *num_minus1 = CreateNode(NULL, NULL, NULL, NUM, -1, DEF_OPER);
    Node *diff_in_diff_left = Diff(node->left, tree);
    Node *diff_left = CreateNode(NULL, diff_in_diff_left, num_minus1, OPER, 0, MULT);

    Node *diff_right = CreateNode(NULL, NULL, Copy(node->right, tree), OPER, 0, SIN);

    tree->size += 3; // from 2( cos(A) ) to 6( (-1) * A' * sin(A) ) - 1 copy

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, MULT);
}

Node *DiffTan(struct Node *node, struct Tree *tree)
{
    Node *diff_left = Diff(node->left, tree);

    Node *cos1 = CreateNode(NULL, NULL, Copy(node->right, tree), OPER, 0, COS);
    Node *cos2 = CreateNode(NULL, NULL, Copy(node->right, tree), OPER, 0, COS);

    Node *diff_right = CreateNode(NULL, cos1, cos2, OPER, 0, MULT);

    tree->size += 3; // from 2( tan(A) ) to 7( A' / (cos(A) * cos(A)) ) - 2 copies

    DeleteNode(node);

    return CreateNode(NULL, diff_left, diff_right, OPER, 0, DIV);
}

int CmpDoubles(double num1, double num2, double accuracy)
{
    if (num1 - num2 < accuracy)
        return 0;

    else if (num1 - num2 > 0)
        return 1;

    else
        return -1;
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

Node *TreeSimpler(struct Node *node, struct Tree *tree)
{
    if (node == NULL || node->type != OPER)
        return node;

    else
    {
        if (node->left->left == NULL && node->left->right == NULL && node->right->left == NULL && node->right->right == NULL)
        {
            if (node->oper == ADD)
                AddSimpler(node, tree);

            else if (node->oper == SUB)
                SubSimpler(node, tree);

            else if (node->oper == MULT)
                MultSimpler(node, tree);

            else if (node->oper == DIV)
                DivSimpler(node, tree);

            else if (node->oper == POW)
                PowSimpler(node, tree);

            else if (node->oper == LOG)
                LogSimpler(node, tree);

            else
                TrigSimpler(node, tree);
        }

        else
        {
            node->left = TreeSimpler(node->left, tree);
            node->right = TreeSimpler(node->right, tree);

            return TreeSimpler(node, tree);
        }
    }

    return NULL;
}

Node *AddSimpler(struct Node *node, struct Tree *tree)
{
$$$ printf("BIG BLACK COMMENT AS\n");
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->value, node->right->value, AIM) == 0)
    {
$$$ printf("BIG BLACK COMMENT AS1\n");
        Node *num = CreateNode(NULL, NULL, NULL, NUM, 2, DEF_OPER);
        Node *var = Copy(node->right, tree);

        DeleteNode(node);

        return CreateNode(NULL, num, var, OPER, 0, MULT);
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->value, 0, AIM) == 0 || CmpDoubles(node->left->value, 0, AIM) == 0))
    {
$$$ printf("BIG BLACK COMMENT AS2\n");
        Node* newnode = NULL;
        if (node->left->type == VAR && CmpDoubles(node->right->value, 0, AIM) == 0)
            newnode = Copy(node->left, tree);

$$$ printf("BIG BLACK COMMENT AS2.1\n");

        if (node->right->type == VAR && CmpDoubles(node->left->value, 0, AIM) == 0)
            newnode = Copy(node->right, tree);

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        node = newnode;
        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
$$$ printf("BIG BLACK COMMENT AS3\n");
        node->value = node->left->value + node->right->value;

        node->type = NUM;
        node->oper = DEF_OPER;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }
$$$ printf("BIG BLACK COMMENT AS4\n");

    return node;
}

Node *SubSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->value, node->right->value, AIM) == 0)
    {
        node->type = NUM;
        node->oper = DEF_OPER;

        node->value = 0;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->value, 0, AIM) == 0 || CmpDoubles(node->left->value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->value, 0, AIM) == 0)
        {
            node = Copy(node->left, tree);

            DeleteNode(node->left);
            node->left = NULL;

            DeleteNode(node->right);
            node->right = NULL;
        }

        else if (node->right->type == VAR && CmpDoubles(node->left->value, 0, AIM) == 0)
        {
            node->oper = MULT;

            node->left->value = -1;
        }

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        node->value = node->left->value - node->right->value;

        node->type = NUM;
        node->oper = DEF_OPER;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    return node;
}

Node *MultSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->value, node->right->value, AIM) == 0)
    {
        node->oper = POW;

        node->right->type = NUM;
        node->right->value = 2;

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->value, 0, AIM) == 0 || CmpDoubles(node->left->value, 0, AIM) == 0))
    {
        node->type = VAR;
        node->value = node->right->value + node->left->value;
        node->oper = DEF_OPER;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->value, 1, AIM) == 0 || CmpDoubles(node->left->value, 1, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->value, 1, AIM) == 0)
            node = Copy(node->left, tree);

        if (node->right->type == VAR && CmpDoubles(node->left->value, 1, AIM) == 0)
            node = Copy(node->right, tree);

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        node->value = node->left->value * node->right->value;

        node->type = NUM;
        node->oper = DEF_OPER;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    return node;
}

Node *DivSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->value, node->right->value, AIM) == 0)
    {
        node->type = NUM;
        node->oper = DEF_OPER;
        node->value = 1;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->value, 0, AIM) == 0 || CmpDoubles(node->left->value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->value, 0, AIM) == 0)
            MathErr(node, DIV_0);

        else if (node->right->type == VAR && CmpDoubles(node->left->value, 0, AIM) == 0)
        {
            node->type = NUM;
            node->oper = DEF_OPER;
            node->value = 0;

            DeleteNode(node->left);
            node->left = NULL;

            DeleteNode(node->right);
            node->right = NULL;

            return node;
        }
    }

    else if (node->left->type == VAR && CmpDoubles(node->right->value, 1, AIM) == 0)
    {
        node = Copy(node->left, tree);

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        if (CmpDoubles(node->right->value, 0, AIM) == 0)
            MathErr(node, DIV_0);

        node->value = node->left->value * node->right->value;

        node->type = NUM;
        node->oper = DEF_OPER;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    return node;
}

Node *PowSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->value, node->right->value, AIM) == 0)
    {
        node->type = NUM;
        node->oper = DEF_OPER;
        node->value = 1;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->value, 0, AIM) == 0 || CmpDoubles(node->left->value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->value, 0, AIM) == 0)
            MathErr(node, DIV_0);

        else if (node->right->type == VAR && CmpDoubles(node->left->value, 0, AIM) == 0)
        {
            node->type = NUM;

            node->oper = DEF_OPER;
            node->value = 0;

            DeleteNode(node->left);
            node->left = NULL;

            DeleteNode(node->right);
            node->right = NULL;

            return node;
        }
    }

    else if (node->left->type == VAR && CmpDoubles(node->right->value, 1, AIM) == 0)
    {
        node = Copy(node->left, tree);

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        if (node->left->value <= 0 && node->right->value <= 0)
            MathErr(node, POW_0);

        node->value = pow(node->left->value, node->right->value);

        node->type = NUM;
        node->oper = DEF_OPER;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    return node;
}

Node *LogSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->value, node->right->value, AIM) == 0)
    {
        node->type = NUM;
        node->oper = DEF_OPER;
        node->value = 1;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (node->right->value <= 0 || node->left->value <= 0 || CmpDoubles(node->left->value, 1, AIM) == 0))
    {
        MathErr(node, LOG_0);
    }

    else if (node->left->type == VAR && CmpDoubles(node->right->value, 1, AIM) == 0)
    {
        node->type = NUM;

        node->oper = DEF_OPER;
        node->value = 0;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        if (node->left->value <= 0 || node->right->value <= 0 || CmpDoubles(node->left->value, 1, AIM) == 0 || CmpDoubles(node->right->value, 1, AIM) == 0)
            MathErr(node, LOG_0);

        node->value = log(node->right->value) / log(node->left->value);

        node->type = NUM;
        node->oper = DEF_OPER;

        DeleteNode(node->left);
        node->left = NULL;

        DeleteNode(node->right);
        node->right = NULL;

        return node;
    }

    return node;
}

Node *TrigSimpler(struct Node *node, struct Tree *tree)
{
    if (node->oper == SIN)
    {
        if (node->right->type == VAR)
        {
            return node;
        }

        else if (node->right->type == NUM)
        {
            node->value = sin(node->right->value);

            node->type = NUM;
            node->oper = DEF_OPER;

            DeleteNode(node->left);
            node->left = NULL;

            DeleteNode(node->right);
            node->right = NULL;

            return node;
        }
    }

    else if (node->oper == COS)
    {
        if (node->right->type == VAR)
        {
            return node;
        }

        else if (node->right->type == NUM)
        {
            node->value = cos(node->right->value);

            node->type = NUM;
            node->oper = DEF_OPER;

            DeleteNode(node->left);
            node->left = NULL;

            DeleteNode(node->right);
            node->right = NULL;

            return node;
        }
    }

    else if (node->oper == TAN)
    {
        if (node->right->type == VAR)
        {
            return node;
        }

        else if (node->right->type == NUM)
        {
            node->value = tan(node->right->value);

            node->type = NUM;
            node->oper = DEF_OPER;

            DeleteNode(node->left);
            node->left = NULL;

            DeleteNode(node->right);
            node->right = NULL;

            return node;
        }
    }

    return node;
}

void MathErr(struct Node *node, Error error)
{
    switch (error)
    {
    case DIV_0:

        printf("YOU TRY TO DIVIDE BY ZERO\n");
        break;

    case POW_0:

        printf("YOU TRY TO POW SMTH LESS ZERO BY DEGREE LESS ZERO\n");
        break;

    case LOG_0:

        printf("YOUR LOG BASIS LESS THAN ZERO OR EQUAL ONE\n");
        break;

    default:
        break;
    }

    exit(1);
}
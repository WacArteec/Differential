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

        node = DeleteNode(node);

        return CreateNum(NULL, NULL, 0);
    }

    else if (node->type == VAR)
    {
        tree->size += 0;

        node = DeleteNode(node);

        return CreateNum(NULL, NULL, 1);
    }

    else if (node->type == OPER)
    {
        if (node->data.oper == ADD)
            return DiffAdd(node, tree);

        else if (node->data.oper == SUB)
            return DiffSub(node, tree);

        else if (node->data.oper == MULT)
            return DiffMult(node, tree);

        else if (node->data.oper == DIV)
            return DiffDiv(node, tree);

        else if (node->data.oper == POW)
            return DiffPow(node, tree);

        else if (node->data.oper == LOG)
            return DiffLog(node, tree);

        else if (node->data.oper == SIN)
            return DiffSin(node, tree);

        else if (node->data.oper == COS)
            return DiffCos(node, tree);

        else if (node->data.oper == TAN)
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
    Node* new_node = NULL;

    if (node != NULL)
    {
        tree->size += 1;
$$$ printf("BIG BLACK COMMENT Copy left = %p right = %p\n", node->left, node->right);

        Node* left_node  = Copy(node->left, tree);
$$$ printf("BIG BLACK COMMENT CopyLeft = %p Good\n", left_node);
        Node* right_node = Copy(node->right, tree);
$$$ printf("BIG BLACK COMMENT CopyRight = %p Good\n", right_node);
    

    if(node->type == NUM)
        new_node = CreateNum(left_node, right_node, node->data.value);

    else if(node->type == VAR)
        new_node = CreateVar(left_node, right_node, node->data.var);

    else if(node->type == OPER)
        new_node = CreateOper(left_node, right_node, node->data.oper);

$$$ printf("BIG BLACK COMMENT Copy Good\n");
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
    node = DeleteNode(node);
$$$ printf("BIG BLACK COMMENT D\n");

    return CreateOper(diff_left, diff_right, ADD);
}

Node *DiffSub(struct Node *node, struct Tree *tree)
{
    Node *diff_left = Diff(node->left, tree);
    Node *diff_right = Diff(node->right, tree);

    tree->size += 0;

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, SUB);
}

Node *DiffMult(struct Node *node, struct Tree *tree)
{
    Node *diff_diff_left = Diff(node->left, tree);
    Node *diff_left = CreateOper(diff_diff_left, Copy(node->right, tree), MULT);

    Node *diff_diff_right = Diff(node->right, tree);
    Node *diff_right = CreateOper(Copy(node->left, tree), diff_diff_right, MULT);

    tree->size += 2; // from 3( A * B ) to 7( A' * B + A * B' ) - 2 copies

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, ADD);
}

Node *DiffDiv(struct Node *node, struct Tree *tree)
{
    Node *diff_nom_left = Diff(node->left, tree);
    Node *nom_left = CreateOper(diff_nom_left, Copy(node->right, tree), MULT);

    Node *diff_nom_right = Diff(node->right, tree);
    Node *nom_right = CreateOper(Copy(node->left, tree), diff_nom_right, MULT);

    Node *diff_left = CreateOper(nom_left, nom_right, SUB);

    Node *denom_left = Copy(node->right, tree);
    Node *denom_right = Copy(node->right, tree);

    Node *diff_right = CreateOper(denom_left, denom_right, ADD);

    tree->size += 4; // from 3 (A / B) to 11 ( (A' * B - A * B') / (B * B) ) - 4 copies

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, DIV);
}

Node *DiffPow(struct Node *node, struct Tree *tree) // некрасиво написана, но не знаю как переписать
{
    Node *number_e = CreateNum(NULL, NULL, e);
    Node *ln = CreateOper(number_e, Copy(node->left, tree), LOG);

    Node *diff_add1 = Diff(node->right, tree);
    Node *add1 = CreateOper(ln, diff_add1, MULT);

    Node *diff_mult_in_add2 = Diff(node->left, tree);
    Node *mult_in_add2 = CreateOper(Copy(node->right, tree), diff_mult_in_add2, MULT);

    Node *add2 = CreateOper(mult_in_add2, Copy(node->left, tree), DIV);

    Node *diff_left = CreateOper(add1, add2, ADD);

    Node *diff_right = Copy(node, tree);

    tree->size += 6; // from 3 ( A ^ B ) to 15 ( ( LOG(e, A) * B' + A' * B / A ) * (A ^ B) ) - 6 copies

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, MULT);
}

Node *DiffLog(struct Node *node, struct Tree *tree)
{
    Node *diff_nom_left = Diff(node->right, tree);
    Node *nom_left = CreateOper(diff_nom_left, Copy(node->right, tree), MULT);

    Node *diff_nom_right = Diff(node->left, tree);

    Node *nom_right_mult = CreateOper(diff_nom_right, Copy(node, tree), MULT);
    Node *nom_right = CreateOper(nom_right_mult, Copy(node->left, tree), DIV);

    Node *diff_left = CreateOper(nom_left, nom_right, SUB);

    Node *number_e = CreateNum(NULL, NULL, e);
    Node *denom_right = Copy(node->right, tree);

    Node *diff_right = CreateOper(number_e, denom_right, ADD);

    tree->size += 6; // from 3 log(A, B) to 15 ( (B' / B - A' * log(A, B) / A) / log(e, B) ) - 6 copies

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, DIV);
}

Node *DiffSin(struct Node *node, struct Tree *tree)
{
    Node *num_1 = CreateNum(NULL, NULL, 1);

    Node *diff_diff_left = Diff(node->left, tree);
    Node *diff_left = CreateOper(diff_diff_left, num_1, MULT);

    Node *diff_right = CreateOper(NULL, Copy(node->right, tree), COS);

    tree->size += 3; // from 2( sin(A) ) to 6( 1 * A' * cos(A) ) - 1 copy

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, MULT);
}

Node *DiffCos(struct Node *node, struct Tree *tree)
{
    Node *num_minus1 = CreateNum(NULL, NULL, -1);
    Node *diff_in_diff_left = Diff(node->left, tree);
    Node *diff_left = CreateOper(diff_in_diff_left, num_minus1, MULT);

    Node *diff_right = CreateOper(NULL, Copy(node->right, tree), SIN);

    tree->size += 3; // from 2( cos(A) ) to 6( (-1) * A' * sin(A) ) - 1 copy

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, MULT);
}

Node *DiffTan(struct Node *node, struct Tree *tree)
{
    Node *diff_left = Diff(node->left, tree);

    Node *cos1 = CreateOper(NULL, Copy(node->right, tree), COS);
    Node *cos2 = CreateOper(NULL, Copy(node->right, tree), COS);

    Node *diff_right = CreateOper(cos1, cos2, MULT);

    tree->size += 3; // from 2( tan(A) ) to 7( A' / (cos(A) * cos(A)) ) - 2 copies

    node = DeleteNode(node);

    return CreateOper(diff_left, diff_right, DIV);
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
            if (node->data.oper == ADD)
                AddSimpler(node, tree);

            else if (node->data.oper == SUB)
                SubSimpler(node, tree);

            else if (node->data.oper == MULT)
                MultSimpler(node, tree);

            else if (node->data.oper == DIV)
                DivSimpler(node, tree);

            else if (node->data.oper == POW)
                PowSimpler(node, tree);

            else if (node->data.oper == LOG)
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
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->data.value, node->right->data.value, AIM) == 0)
    {
$$$ printf("BIG BLACK COMMENT AS1\n");
        Node *num = CreateNum(NULL, NULL, 2);
        Node *var = Copy(node->right, tree);

        node = DeleteNode(node);

        return CreateOper(num, var, MULT);
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->data.value, 0, AIM) == 0 || CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
$$$ printf("BIG BLACK COMMENT AS2\n");
        Node* newnode = NULL;
        if (node->left->type == VAR && CmpDoubles(node->right->data.value, 0, AIM) == 0)
            newnode = Copy(node->left, tree);

$$$ printf("BIG BLACK COMMENT AS2.1\n");

        if (node->right->type == VAR && CmpDoubles(node->left->data.value, 0, AIM) == 0)
            newnode = Copy(node->right, tree);

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        node = newnode;
        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
$$$ printf("BIG BLACK COMMENT AS3\n");
        node->data.value = node->left->data.value + node->right->data.value;

        node->type = NUM;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }
$$$ printf("BIG BLACK COMMENT AS4\n");

    return node;
}

Node *SubSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->data.value, node->right->data.value, AIM) == 0)
    {
        node->type = NUM;

        node->data.value = 0;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->data.value, 0, AIM) == 0 || CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->data.value, 0, AIM) == 0)
        {
            node = Copy(node->left, tree);

            node->left  = DeleteNode(node->left);
            node->right = DeleteNode(node->right);
        }

        else if (node->right->type == VAR && CmpDoubles(node->left->data.value, 0, AIM) == 0)
        {
            node->data.oper = MULT;

            node->left->data.value = -1;
        }

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        node->data.value = node->left->data.value - node->right->data.value;

        node->type = NUM;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *MultSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->data.value, node->right->data.value, AIM) == 0)
    {
        node->data.oper = POW;

        node->right->type = NUM;
        node->right->data.value = 2;

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->data.value, 0, AIM) == 0 || CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        node->type = VAR;
        node->data.value = node->right->data.value + node->left->data.value;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->data.value, 1, AIM) == 0 || CmpDoubles(node->left->data.value, 1, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->data.value, 1, AIM) == 0)
            node = Copy(node->left, tree);

        if (node->right->type == VAR && CmpDoubles(node->left->data.value, 1, AIM) == 0)
            node = Copy(node->right, tree);

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        node->data.value = node->left->data.value * node->right->data.value;

        node->type = NUM;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *DivSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->data.value, node->right->data.value, AIM) == 0)
    {
        node->type = NUM;
        node->data.value = 1;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->data.value, 0, AIM) == 0 || CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->data.value, 0, AIM) == 0)
            MathErr(node, DIV_0);

        else if (node->right->type == VAR && CmpDoubles(node->left->data.value, 0, AIM) == 0)
        {
            node->type = NUM;
            node->data.value = 0;

            node->left  = DeleteNode(node->left);
            node->right = DeleteNode(node->right);

            return node;
        }
    }

    else if (node->left->type == VAR && CmpDoubles(node->right->data.value, 1, AIM) == 0)
    {
        node = Copy(node->left, tree);

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        if (CmpDoubles(node->right->data.value, 0, AIM) == 0)
            MathErr(node, DIV_0);

        node->data.value = node->left->data.value * node->right->data.value;

        node->type = NUM;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *PowSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->data.value, node->right->data.value, AIM) == 0)
    {
        node->type = NUM;
        node->data.value = 1;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (CmpDoubles(node->right->data.value, 0, AIM) == 0 || CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->data.value, 0, AIM) == 0)
            MathErr(node, DIV_0);

        else if (node->right->type == VAR && CmpDoubles(node->left->data.value, 0, AIM) == 0)
        {
            node->type = NUM;

            node->data.value = 0;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

            return node;
        }
    }

    else if (node->left->type == VAR && CmpDoubles(node->right->data.value, 1, AIM) == 0)
    {
        node = Copy(node->left, tree);

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        if (node->left->data.value <= 0 && node->right->data.value <= 0)
            MathErr(node, POW_0);

        node->data.value = pow(node->left->data.value, node->right->data.value);

        node->type = NUM;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *LogSimpler(struct Node *node, struct Tree *tree)
{
    if (node->left->type == VAR && node->right->type == VAR && CmpDoubles(node->left->data.value, node->right->data.value, AIM) == 0)
    {
        node->type = NUM;
        node->data.value = 1;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->left->type == VAR || node->right->type == VAR) && (node->right->data.value <= 0 || node->left->data.value <= 0 || CmpDoubles(node->left->data.value, 1, AIM) == 0))
    {
        MathErr(node, LOG_0);
    }

    else if (node->left->type == VAR && CmpDoubles(node->right->data.value, 1, AIM) == 0)
    {
        node->type = NUM;

        node->data.value = 0;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == NUM || node->right->type == NUM)
    {
        if (node->left->data.value <= 0 || node->right->data.value <= 0 || CmpDoubles(node->left->data.value, 1, AIM) == 0 || CmpDoubles(node->right->data.value, 1, AIM) == 0)
            MathErr(node, LOG_0);

        node->data.value = log(node->right->data.value) / log(node->left->data.value);

        node->type = NUM;

        node->left  = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *TrigSimpler(struct Node *node, struct Tree *tree)
{
    if (node->data.oper == SIN)
    {
        if (node->right->type == VAR)
        {
            return node;
        }

        else if (node->right->type == NUM)
        {
            node->data.value = sin(node->right->data.value);

            node->type = NUM;

            node->left  = DeleteNode(node->left);
            node->right = DeleteNode(node->right);

            return node;
        }
    }

    else if (node->data.oper == COS)
    {
        if (node->right->type == VAR)
        {
            return node;
        }

        else if (node->right->type == NUM)
        {
            node->data.value = cos(node->right->data.value);

            node->type = NUM;

            node->left  = DeleteNode(node->left);
            node->right = DeleteNode(node->right);

            return node;
        }
    }

    else if (node->data.oper == TAN)
    {
        if (node->right->type == VAR)
        {
            return node;
        }

        else if (node->right->type == NUM)
        {
            node->data.value = tan(node->right->data.value);

            node->type = NUM;

            node->left  = DeleteNode(node->left);
            node->right = DeleteNode(node->right);

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
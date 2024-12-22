#include "simpler.h"

Node *AddSimpler(struct Node *node, struct Tree *tree, FILE *stream);
Node *SubSimpler(struct Node *node, struct Tree *tree, FILE *stream);
Node *MultSimpler(struct Node *node, struct Tree *tree, FILE *stream);
Node *DivSimpler(struct Node *node, struct Tree *tree, FILE *stream);
Node *PowSimpler(struct Node *node, struct Tree *tree, FILE *stream);
Node *LogSimpler(struct Node *node, struct Tree *tree, FILE *stream);
Node *TrigSimpler(struct Node *node, struct Tree *tree, FILE *stream);

Node *TreeSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
$$
    if (node == NULL || node->type != OPER)
        return node;

    printf("node = %p type = %d oper = %d\n", node, node->type, node->data.oper);

    node->left = TreeSimpler(node->left, tree, stream);
    printf("node.left = %p \n", node->left);

    node->right = TreeSimpler(node->right, tree, stream);
    printf("node.right = %p \n", node->right);

    Node *new_node = node;

    if (node->left != NULL)
    {
        if ((node->left->left == NULL && node->left->right == NULL) || (node->right->left == NULL && node->right->right == NULL))
        {
            if (node->data.oper == ADD)
                new_node = AddSimpler(node, tree, stream);

            else if (node->data.oper == SUB)
                new_node = SubSimpler(node, tree, stream);

            else if (node->data.oper == MULT)
                new_node = MultSimpler(node, tree, stream);

            else if (node->data.oper == DIV)
                new_node = DivSimpler(node, tree, stream);

            else if (node->data.oper == POW)
                new_node = PowSimpler(node, tree, stream);

            else if (node->data.oper == LOG)
                new_node = LogSimpler(node, tree, stream);
        }
    }

    else
    {
        if (node->right->left == NULL && node->right->right == NULL)
            new_node = TrigSimpler(node, tree, stream);
    }

    return new_node;
}

Node *AddSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
    $$$ printf("BIG BLACK COMMENT AS\n");

    if (node->left->type == VAR && node->right->type == VAR && node->left->data.var == node->right->data.var)
    {
        $$$ printf("BIG BLACK COMMENT AS1\n");
        node->left->type = NUM;
        node->left->data = {};
        node->left->data.value = 2;

        node->data.oper = MULT;
    }

    else if (node->left->type == NUM && node->right->type == NUM)
    {
        $$$ printf("BIG BLACK COMMENT AS3\n");
        node->data.value = node->left->data.value + node->right->data.value;

        node->type = NUM;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->right->type == NUM && CmpDoubles(node->right->data.value, 0, AIM) == 0) || (node->left->type == NUM && CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        $$$ printf("BIG BLACK COMMENT AS2\n");

        if (node->right->type == NUM && CmpDoubles(node->right->data.value, 0, AIM) == 0)
        {
            node->right = DeleteNode(node->right);
            node = node->left;
        }

        $$$ printf("BIG BLACK COMMENT AS2.1\n");
        if (node->left->type == NUM && CmpDoubles(node->left->data.value, 0, AIM) == 0)
        {
            node->left = DeleteNode(node->left);
            $$$ printf("BIG BLACK COMMENT AS2.1left\n");
            node = node->right;
            $$$ printf("BIG BLACK COMMENT AS2.1 node = %p node.type = %d node.oper = %d\n", node, node->type, node->data.oper);
        }
    }
    $$$ printf("BIG BLACK COMMENT AS4\n");

    return node;
}

Node *SubSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
    $$$ printf("simple sub\n");

    if (node->left->type == VAR && node->right->type == VAR && node->left->data.var == node->right->data.var)
    {
        node->type = NUM;

        node->data.value = 0;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);
    }

    else if (node->left->type == NUM && node->right->type == NUM)
    {
        node->data.value = node->left->data.value - node->right->data.value;

        node->type = NUM;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);
    }

    else if ((node->right->type == NUM && CmpDoubles(node->right->data.value, 0, AIM) == 0) || (node->left->type == NUM && CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
    $$$ printf("simple sub 0\n");
        if (node->right->type == NUM && CmpDoubles(node->right->data.value, 0, AIM) == 0)
        {
    $$$ printf("simple sub r0\n");
            node->right = DeleteNode(node->right);

            node = node->left;
        }

        else if ((node->left->type == NUM && CmpDoubles(node->left->data.value, 0, AIM) == 0))
        {
    $$$ printf("simple sub l0\n");
            node->data.oper = MULT;
            
            node->left->data.value = -1;
        }
    }

    $$$ printf("simple sub end\n");
    return node;
}

Node *MultSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
    $$$ printf("simple mult\n");
    if (node->left->type == VAR && node->right->type == VAR && node->left->data.var == node->right->data.var)
    {
        $$$ printf("simple mult var\n");
        node->data.oper = POW;

        node->right->type = NUM;
        node->right->data.value = 2;
    }

    else if ((node->right->type == NUM && CmpDoubles(node->right->data.value, 0, AIM) == 0) || (node->left->type == NUM && CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        $$$ printf("simple mult NV0\n");
        node->type = NUM;
        node->data.value = 0;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);
    }

    else if (node->left->type == NUM && node->right->type == NUM)
    {
        $$$ printf("simple mult num\n");
        node->data.oper = DEF_OPER;
        node->data.value = node->left->data.value * node->right->data.value;

        node->type = NUM;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);
    }

    else if ((node->right->type == NUM && CmpDoubles(node->right->data.value, 1, AIM) == 0) || (node->left->type == NUM && CmpDoubles(node->left->data.value, 1, AIM) == 0))
    {
        $$$ printf("simple mult NV1\n");
        // GraphDump(tree);
        if (node->right->type == NUM && CmpDoubles(node->right->data.value, 1, AIM) == 0)
        {
            node->right = DeleteNode(node->right);
            node = node->left;
        }

        if (node->left->type == NUM && CmpDoubles(node->left->data.value, 1, AIM) == 0)
        {
            node->left = DeleteNode(node->left);
            node = node->right;
        }
    }

    $$$ printf("simple mult end\n");
    // GraphDump(tree);

    return node;
}

Node *DivSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
    if (node->left->type == VAR && node->right->type == VAR && node->left->data.var == node->right->data.var)
    {
        node->type = NUM;
        node->data.value = 1;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->left->type == VAR && CmpDoubles(node->right->data.value, 0, AIM) == 0) || (node->right->type == VAR && CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->data.value, 0, AIM) == 0)
            MathErr(node, DIV_0, __LINE__);

        else if (node->right->type == VAR && CmpDoubles(node->left->data.value, 0, AIM) == 0)
        {
            node->type = NUM;
            node->data.value = 0;

            node->left = DeleteNode(node->left);
            node->right = DeleteNode(node->right);

            return node;
        }
    }

    else if (node->left->type == VAR && node->right->type == NUM && CmpDoubles(node->right->data.value, 1, AIM) == 0)
    {
        node = Copy(node->left, tree);

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == NUM && node->right->type == NUM)
    {
        if (CmpDoubles(node->right->data.value, 0, AIM) == 0)
            MathErr(node, DIV_0, __LINE__);

        node->data.value = node->left->data.value * node->right->data.value;

        node->type = NUM;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *PowSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
    if (node->left->type == VAR && node->right->type == VAR && node->left->data.var == node->right->data.var)
    {
        node->type = NUM;
        node->data.value = 1;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if ((node->left->type == VAR && node->right->type == NUM && CmpDoubles(node->right->data.value, 0, AIM) == 0) || (node->right->type == VAR && node->right->type == NUM && CmpDoubles(node->left->data.value, 0, AIM) == 0))
    {
        if (node->left->type == VAR && CmpDoubles(node->right->data.value, 0, AIM) == 0)
            MathErr(node, DIV_0, __LINE__);

        else if (node->right->type == VAR && CmpDoubles(node->left->data.value, 0, AIM) == 0)
        {
            node->type = NUM;

            node->data.value = 0;

            node->left = DeleteNode(node->left);
            node->right = DeleteNode(node->right);

            return node;
        }
    }

    else if (node->left->type == VAR && node->right->type == NUM && CmpDoubles(node->right->data.value, 1, AIM) == 0)
    {
        node = Copy(node->left, tree);

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == NUM && node->right->type == NUM)
    {
        if (node->left->data.value <= 0 && node->right->data.value <= 0)
            MathErr(node, POW_0, __LINE__);

        node->data.value = pow(node->left->data.value, node->right->data.value);

        node->type = NUM;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *LogSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
    if (node->left->type == VAR && node->right->type == VAR && node->left->data.var == node->right->data.var)
    {
        node->type = NUM;
        node->data.value = 1;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == VAR && node->right->type == NUM && CmpDoubles(node->right->data.value, 1, AIM) == 0)
    {
        node->type = NUM;

        node->data.value = 0;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    else if (node->left->type == NUM && node->right->type == NUM)
    {

        if (node->right->data.value <= 0)
            MathErr(node, LOG_0, __LINE__);

        else if (node->left->data.value <= 0 || node->left->data.value == 1)
            MathErr(node, LOG_0, __LINE__);

        node->data.value = log(node->right->data.value) / log(node->left->data.value);

        node->type = NUM;

        node->left = DeleteNode(node->left);
        node->right = DeleteNode(node->right);

        return node;
    }

    return node;
}

Node *TrigSimpler(struct Node *node, struct Tree *tree, FILE *stream)
{
    $$$ printf("simple trig\n");
    if (node->data.oper == SIN)
    {
        $$$ printf("simple sin\n");
        if (node->right->type == VAR)
        {
            return node;
        }

        else if (node->right->type == NUM)
        {
            node->data.value = sin(node->right->data.value);

            node->type = NUM;

            node->left = DeleteNode(node->left);
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

            node->left = DeleteNode(node->left);
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

            node->left = DeleteNode(node->left);
            node->right = DeleteNode(node->right);

            return node;
        }
    }

    return node;
}

Node *Copy(struct Node *node, struct Tree *tree)
{
    Node *new_node = NULL;

    if (node != NULL)
    {
        tree->size += 1;
        printf("BIG BLACK COMMENT Copy node = %p node.type = %d node.oper = %d left = %p right = %p\n", node, node->type, node->data.oper, node->left, node->right);

        Node *left_node = Copy(node->left, tree);
        //assert(left_node);
        //$$$ printf("BIG BLACK COMMENT CopyLeft = %p Good\n", left_node);
        Node *right_node = Copy(node->right, tree);
        //$$$ printf("BIG BLACK COMMENT CopyRight = %p Good\n", right_node);

        if (node->type == NUM)
            new_node = CreateNum(left_node, right_node, node->data.value);

        else if (node->type == VAR)
            new_node = CreateVar(left_node, right_node, node->data.var);

        else if (node->type == OPER)
            new_node = CreateOper(left_node, right_node, node->data.oper);

        //$$$ printf("BIG BLACK COMMENT Copy Good\n");
    }

    return new_node;
}

void MathErr(struct Node *node, Error error, int line)
{
    switch (error)
    {
    case DIV_0:

        printf("YOU TRY TO DIVIDE BY ZERO");
        break;

    case POW_0:

        printf("YOU TRY TO POW SMTH LESS ZERO BY DEGREE LESS ZERO");
        break;

    case LOG_0:

        printf("YOUR LOG BASIS LESS THAN ZERO OR EQUAL ONE");
        break;

    default:
        break;
    }

    printf(" %d\n", line);

    exit(1);
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
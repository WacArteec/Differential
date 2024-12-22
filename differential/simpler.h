#include <TXLib.h>

#ifndef SIMPLER_H
#define SIMPLER_H

#include "../tree/tree.h"
#include <math.h>

#define AIM 1.0e-10

enum Error
{
    DIV_0,
    POW_0,
    LOG_0
};


Node *TreeSimpler(struct Node *node, struct Tree *tree, FILE *stream);
void MathErr(struct Node *node, Error error, int line);
int CmpDoubles(double num1, double num2, double accuracy);
Node *Copy(struct Node *node, struct Tree *tree);

#endif //SIMPLER_H
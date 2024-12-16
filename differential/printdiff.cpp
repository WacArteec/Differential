#include "../tree/tree.h"
#include "printdiff.h"

void PrintDiff(struct Node* node, FILE* stream)
{
    if(node == NULL)
        return;
        
    fprintf(stream, "(");

    PrintDiff(node->left, stream);

    if(node->type == NUM)
        fprintf(stream, "%g", node->data.value);

    else if(node->type == VAR)
        fprintf(stream, "%c", node->data.var);
    
    else if(node->type == OPER)
    {
        char action = 'D';

        switch (node->data.oper)
        {
            case ADD:
            {
                action = '+';
                break;
            }

            case SUB:
            {
                action = '-';
                break;
            }
            
            case MULT:
            {
                action = '*';
                break;
            }
        
            case DIV:
            {
                action = '/';
                break;
            }
            
            case POW:
            {
                action = '^';
                break;
            }
            
            case LOG:
            {
                action = 'l';
                break;
            }
        
            case SIN:
            {
                action = 's';
                break;
            }
            
            case COS:
            {
                action = 'c';
                break;
            }
            
            case TAN:
            {
                action = 't';
                break;
            }
        
            default:
                break;
        }

        fprintf(stream, "%c", action);
    }

    else
        fprintf(stream, "%p", node);

    PrintDiff(node->right, stream);

    fprintf(stream, ")");
}
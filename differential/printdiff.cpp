#include "../tree/tree.h"
#include "printdiff.h"

void PrintDiff(struct Node* node, FILE* stream)
{
    if(node == NULL)
        return;
        
    fprintf(stream, "{");

    PrintDiff(node->left, stream);

    if(node->type == NUM)
        fprintf(stream, "%g", node->data.value);

    else if(node->type == VAR)
        fprintf(stream, "%c", node->data.var);
    
    else if(node->type == OPER)
    {
        switch (node->data.oper)
        {
            case ADD:
            {
                fprintf(stream, "+");
                PrintDiff(node->right, stream);
                break;
            }

            case SUB:
            {
                fprintf(stream, "-");
                PrintDiff(node->right, stream);
                break;
            }
            
            case MULT:
            {
                fprintf(stream, "\\cdot");
                PrintDiff(node->right, stream);
                break;
            }
        
            case DIV:
            {
                fprintf(stream, "\\over");
                PrintDiff(node->right, stream);
                break;
            }
            
            case POW:
            {
                fprintf(stream, "^");
                PrintDiff(node->right, stream);
                break;
            }
            
            case LOG:
            {
                fprintf(stream, "\\log(");
                PrintDiff(node->right, stream);
                fprintf(stream, ")");
                break;
            }
        
            case SIN:
            {
                fprintf(stream, "\\sin(");
                PrintDiff(node->right, stream);
                fprintf(stream, ")");
                break;
            }
            
            case COS:
            {
                fprintf(stream, "\\cos(");
                PrintDiff(node->right, stream);
                fprintf(stream, ")");
                break;
            }
            
            case TAN:
            {
                fprintf(stream, "\\tan(");
                PrintDiff(node->right, stream);
                fprintf(stream, ")");
                break;
            }
        
            default:
                break;
        }
    }

    else
        fprintf(stream, "%p", node);

    fprintf(stream, "}");
}
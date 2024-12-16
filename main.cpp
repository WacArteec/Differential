#include <stdio.h>

#include "differential/differential.h"
#include "readrecover/reader.h"
#include "readrecover/recover.h"
#include "differential/printdiff.h"

int main()
{
    const char* filename = "expression.txt";
    char* text = Input(filename);
$$$ printf("BIG BLACK COMMENT a\n");

    struct Tree tree = {};
$$$ printf("BIG BLACK COMMENT b\n");

    tree.root = Parser(text);
$$$ printf("BIG BLACK COMMENT c\n");
    PrintNode(stdout, tree.root);
$$$ printf("BIG BLACK COMMENT c1\n");

    tree.root = DiffTree(tree.root, &tree);
$$$ printf("\nBIG BLACK COMMENT e\n");

    PrintNode(stdout, tree.root);
$$$ printf("\nBIG BLACK COMMENT f\n");

    PrintDiff(tree.root, stdout);

    printf("%s", text);

    return 0;
}

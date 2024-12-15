#include <stdio.h>

#include "differential/differential.h"
#include "readrecover/reader.h"
#include "readrecover/recover.h"

int main()
{
    const char* filename = "expression.txt";
    char* text = Input(filename);
$$$ printf("BIG BLACK COMMENT a\n");

    struct Tree tree = {};
$$$ printf("BIG BLACK COMMENT b\n");

    tree.root = Parser(text, &tree);
$$$ printf("BIG BLACK COMMENT c\n");
    PrintNode(stdout, tree.root);
$$$ printf("BIG BLACK COMMENT c1\n");

    tree.root = TreeSimpler(tree.root, &tree);
$$$ printf("BIG BLACK COMMENT d\n");
    PrintNode(stdout, tree.root);

    tree.root = Diff(tree.root, &tree);
$$$ printf("\nBIG BLACK COMMENT e\n");

    tree.root = TreeSimpler(tree.root, &tree);
$$$ printf("BIG BLACK COMMENT g\n");

    PrintNode(stdout, tree.root);
$$$ printf("\nBIG BLACK COMMENT f\n");

    
    printf("%s", text);

    return 0;
}

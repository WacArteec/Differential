#include <TXLib.h>

#include <stdio.h>
#include <assert.h>

#include "differential/differential.h"
#include "readrecover/reader.h"
#include "readrecover/recover.h"
#include "differential/printdiff.h"

int main()
{
    const char* filename = "expression.txt";

    FILE* file_out = fopen("out_expression.txt", "w");
$$$ assert(file_out);

    char* text = Input(filename);
$$$ printf("BIG BLACK COMMENT a\n");

    struct Tree tree = {};
$$$ printf("BIG BLACK COMMENT b\n");

    tree.root = Parser(text);
$$$ printf("BIG BLACK COMMENT c\n");
    PrintNode(stdout, tree.root);
$$$ printf("BIG BLACK COMMENT c1\n");

    //GraphDump(&tree);
    $(offsetof(Node, right));
    tree.root = DiffTree(tree.root, &tree, file_out);
$$$ printf("\nBIG BLACK COMMENT e\n");

    PrintNode(stdout, tree.root);
$$$ printf("\nBIG BLACK COMMENT f\n");

    tree.root = TreeSimpler(tree.root, &tree, file_out);
$$$ printf("\nBIG BLACK COMMENT g\n");
    GraphDump(&tree);

$$$ printf("\nBIG BLACK COMMENT h\n");

    fprintf(file_out, "At the end: \n $$d(%s) = ", text);
    PrintDiff(tree.root, file_out);
    fprintf(file_out, "$$", text);
$$$ printf("\nBIG BLACK COMMENT i\n");

    printf("\n%s\n", text);

    Dtor(&tree);

    return 0;
}

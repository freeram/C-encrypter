#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Argument count: %d\n", argc);

    printf("Arguments:\n");
    for (int i = 0; i < argc; i++) {
        puts(argv[i]);
    }

    return 0;
}
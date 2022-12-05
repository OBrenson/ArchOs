#include <stdio.h>
int main(int argc, char *argv[] ) {
    printf("\t%d\n", (int)strtol(argv[1], NULL, 16));
}
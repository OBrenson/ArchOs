#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

int main(int argc, char *argv[]) {
    float a = atof(argv[1]);
    float b = atof(argv[2]);
    float res = sqrtf(a * a + b * b);
    printf("Result %f\n", res);
}
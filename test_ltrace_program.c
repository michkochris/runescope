#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello from test_ltrace_program!\n");
    int a = 10, b = 20;
    int sum = a + b;
    fprintf(stderr, "Sum is: %d\n", sum);
    return 0;
}

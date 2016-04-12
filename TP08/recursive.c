#include <stdio.h>
#include <math.h>

static unsigned int Counter;
unsigned int p_recursive(unsigned int n);
unsigned int p_dynamic(unsigned int n);
unsigned int p_efficient(unsigned int n);

int main(void) {
    unsigned int n;    

    for (n = 0; n <= 12; n++) {
        printf("N = %d\n", n);
        Counter = 0;
        printf("Recursive = %2u, div = %u\n", p_recursive(n), Counter);
        Counter = 0;
        printf("Dynamic   = %2d, div = %u\n", p(n), Counter);
        Counter = 0;
        printf("Efficient = %2d, div = %u\n", p(n), Counter);
    }

    return 0;
}

unsigned int p_recursive(unsigned int n) {
    if (!n)
        return 1;
    double div = n / 2;
    unsigned int f = (unsigned int)floor(div), c = (unsigned int)ceil(div);
    Counter++;
    return f + c + p_recursive(f) + p_recursive(c);
}


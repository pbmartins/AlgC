#include <stdio.h>
#include <stdlib.h>

static unsigned int Counter;
unsigned int fibo_recursive(unsigned int n);
unsigned int fibo_dynamic(unsigned int n);
unsigned int fibo_repetitive(unsigned int n);


int main(void) {
    unsigned int n;    

    for (n = 1; n < 17; n++) {
        printf("N = %u\n", n);
        Counter = 0;
        printf("recursive = %u, adds = %u\n", fibo_recursive(n), Counter);
        Counter = 0;
        printf("dynamic = %u, adds = %u\n", fibo_dynamic(n), Counter);
        Counter = 0;
        printf("repetitive = %u, adds = %u\n\n", fibo_dynamic(n), Counter);
    }

    return 0;
}

unsigned int fibo_recursive(unsigned int n) {
    if (n < 2)
        return n;
    Counter++;
    return fibo_recursive(n - 1) + fibo_recursive(n - 2);
}

unsigned int fibo_dynamic(unsigned int n) {
    if (n < 2)
        return n;
    unsigned int i, *fibonacci = malloc(sizeof(unsigned int) * (n + 1));
    fibonacci[0] = 0;
    fibonacci[1] = 1;

    for (i = 2; i <= n; i++, Counter++)
        fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];

    return fibonacci[n];
}

unsigned int fibo_repetitive(unsigned int n) {
    if (n < 2)
        return n;
    unsigned int i, preview = 0, actual = 1, next;

    for (i = 2; i <= n; i++, Counter++) {
        next = actual + preview;
        preview = actual;
        actual = next;
    }
    return next;
}

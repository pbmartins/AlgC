#include <stdio.h>

static unsigned int Counter;
double pow_m1(double x, unsigned int n);
double pow_m2(double x, unsigned int n);


int main(void) {
    double x = 0.5;
    unsigned int n;    

    for (n = 8180; n < 8193; n++) {
        printf("N = %u\n", n);
        Counter = 0;
        printf("result1 = %f, mult = %u\n", pow_m1(x, n), Counter);
        Counter = 0;
        printf("result2 = %f, mult = %u\n\n", pow_m2(x, n), Counter);
    }

    return 0;
}

double pow_m1(double x, unsigned int n) {
    if (!n)
        return 1;
    Counter++;
    return x * pow_m1(x, n - 1);
}

double pow_m2 (double x, unsigned int n) {
    if (!n)
        return 1;

    double value = pow_m2(x, n>>1);
    Counter++;
    if (!(n & 1))                // even
        return value * value;
    else {                       // odd
        Counter++;
        return x * value * value;
    }
}

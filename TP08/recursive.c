#include <stdio.h>
#include <stdlib.h>

static unsigned int Counter;
int p_recursive(int n);
int p_recursive_super_efficient(int n);
int p_dynamic(int n);
int p_efficient(int n);

int main(void) {
    int n, result; 

    for (n = 0; n <= 12; n++) {
        printf("N = %d\n", n);
        Counter = 0;
        result = p_recursive(n);
        printf("Recursive = %2d, div = %u\n", result, Counter);
        Counter = 0;
        result = p_dynamic(n);
        printf("Dynamic   = %2d, div = %u\n", result, Counter);
        Counter = 0;
        result = p_efficient(n);
        printf("Efficient = %2d, div = %u\n", result, Counter);
    }

    return 0;
}

int p_recursive(int n) {
    if (n <= 1)
        return 0;
    int floor = n / 2, ceil = floor + (n % 2);
    Counter += 2;
    return floor + ceil + p_recursive(floor) + p_recursive(ceil);
}

int p_dynamic(int n) {
    if (n <= 1)
        return 0;
    int to_return, i, floor, ceil, *array = (int*)calloc(n + 1, sizeof(int));
    array[0] = array[1] = 0;

    for (i = 2; i <= n; i++, Counter += 2) {
        floor = i / 2;
        ceil = floor + (i % 2);
        array[i] = floor + ceil + array[floor] + array[ceil];
    }

    to_return = array[n];
    free(array);
    return to_return;
}

int p_efficient(int n) {
    if (n <= 1)
        return 0;
    int i, floor, ceil, final_f = n / 2, final_c = final_f + (n % 2), to_return;
    int *array = (int*)calloc(final_c + 1, sizeof(int));
    array[0] = array[1] = 0;

    for (i = 2; i <= final_c; i++, Counter += 2) {
        floor = i / 2;
        ceil = floor + (i % 2);
        array[i] = floor + ceil + array[floor] + array[ceil];
    }

    Counter += 2;
    to_return = final_f + final_c + array[final_f] + array[final_c];
    free(array);
    return to_return;
}


int p_recursive_super_efficient(int n) {
    if (n <= 1)
        return 0;
    int f = n / 2, c = f, p_f = p_recursive_super_efficient(f), p_c = p_f;
    Counter++;
    if (n % 2) {
        c = f + 1;
        p_c = p_recursive_super_efficient(c);
    }
    return f + c + p_f + p_c;
}

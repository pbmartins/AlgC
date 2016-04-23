#include <stdio.h>
#include <stdlib.h>

static unsigned int Counter;
int cataclan_recursive(int n);
int cataclan_dynamic(int n);
int cataclan_efficient(int n);

int main(void) {
    int n, result;    

    for (n = 0; n <= 12; n++) {
        printf("N = %d\n", n);
        Counter = 0;
        result = cataclan_recursive(n);
        printf("Recursive = %2d, div = %u\n", result, Counter);
        Counter = 0;
        result = cataclan_dynamic(n);
        printf("Dynamic   = %2d, div = %u\n", result, Counter);
        Counter = 0;
        result = cataclan_efficient(n);
        printf("Efficient = %2d, div = %u\n", result, Counter);
    }

    return 0;
}

int cataclan_recursive(int n) {
    if (!n)
        return 1;
    int i, sum = 0;
    for (i = 0; i < n; i++, Counter++)
        sum += cataclan_recursive(i) * cataclan_recursive(n - i - 1);
    return sum;
}

int cataclan_dynamic(int n) {
    if (!n)
        return 1;
    int i, j, *array = (int*)calloc(n + 1, sizeof(int));
    array[0] = 1; 

    for (i = 1; i <= n; i++) {
        for (j = 0; j < i; j++, Counter++)
            array[i] += array[j] * array[i - j - 1];
    }

    return array[n];            
}

int cataclan_efficient(int n) {
    if (!n)
        return 1;
    int i, j, *array = (int*)calloc(n + 1, sizeof(int));
    array[0] = 1; 

    for (i = 1; i <= n; i++) {
        for (j = 0; j < i / 2; j++, Counter++)
            array[i] += (array[j] * array[i - j - 1]) << 1;
        if (i % 2)
            array[i] += array[j] * array[i - j - 1];
    }

    return array[n];  
}

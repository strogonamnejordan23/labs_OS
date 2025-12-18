#include <math.h>
#include <stdlib.h>
#include "contract.h"

float SinIntegral(float A, float B, float e) {
    if (e <= 0.0f || B <= A) {
        return 0.0f;
    }
    
    float integral = 0.0f;
    int n = (int)((B - A) / e); 

    for (int i = 0; i < n; i++) {
        float x = A + i * e;
        integral += sinf(x) * e;
    }
    float remaining = B - (A + n * e);
    if (remaining > 0) {
        float x = A + n * e;  
        integral += sinf(x) * remaining;
    }
    
    return integral;
}
int* Sort(int* array) {
    if (array == NULL) return NULL;
    int n = array[0];
    if (n <= 1) return array;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n - i; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    return array;
}
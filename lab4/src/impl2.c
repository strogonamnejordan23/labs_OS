#include <math.h>
#include <stdlib.h>
#include "contract.h"

float SinIntegral(float A, float B, float e) {
    if (e <= 0.0f || B <= A) {
        return 0.0f;
    }

    float integral = 0.0f;
    int n = (int)((B - A) / e);
    float prev_value = sinf(A);
    for (int i = 1; i <= n; i++) {
        float x = A + i * e;
        float cur_value = sinf(x);
        integral += (prev_value + cur_value) * e / 2.0f;
        prev_value = cur_value;
    }
    float remaining = B - (A + n * e);
    if (remaining > 0) {
        float x = A + n * e;
        float cur_value = sinf(B);
        integral += (prev_value + cur_value) * remaining / 2.0f;
    }
    
    return integral;
}

static void quick_sort(int* arr, int left, int right) {
    if (left >= right) return;
    
    int pivot = arr[(left + right) / 2];
    int i = left, j = right;
    
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }
    quick_sort(arr, left, j);
    quick_sort(arr, i, right);
}

int* Sort(int* array) {
    if (array == NULL) return NULL;
    int n = array[0];
    if (n <= 1) return array;
    quick_sort(&array[1], 0, n - 1);
    return array;
}
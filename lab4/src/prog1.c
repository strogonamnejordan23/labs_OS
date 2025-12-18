#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contract.h"

int main() {
    char input[256];
    char command;
    float a, b, e;
    int *array = NULL;
    
    printf("Программа 1: Статическая линковка с impl1\n");
    printf("Команды:\n");
    printf("  0 - не используется в этой программе\n");
    printf("  1 A B e - вычислить интеграл sin(x) от A до B с шагом e\n");
    printf("  2 N x1 x2 ... xN - отсортировать массив из N элементов\n");
    printf("  q - выход\n\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (input[0] == 'q' && (input[1] == '\0' || input[1] == ' ')) {
            break;
        }
        
        if (sscanf(input, "%c", &command) != 1) {
            printf("Неверный формат команды\n");
            continue;
        }
        
        switch (command) {
            case '0':
                printf("В этой программе переключение реализаций не поддерживается\n");
                break;
                
            case '1':
                if (sscanf(input, "%*c %f %f %f", &a, &b, &e) != 3) {
                    printf("Ошибка: требуется 3 аргумента: A B e\n");
                    printf("Пример: 1 0 3.14159 0.01\n");
                    continue;
                }
                
                float result = SinIntegral(a, b, e);
                printf("Результат: %f\n", result);
                break;
                
            case '2': {
                int count = 0;
                char *token;
                char temp_input[256];
                strcpy(temp_input, input);
                
                token = strtok(temp_input, " ");
                while (token != NULL) {
                    count++;
                    token = strtok(NULL, " ");
                }
                
                int n = count - 1;
                if (n <= 0) {
                    printf("Ошибка: требуется хотя бы один элемент для сортировки\n");
                    printf("Пример: 2 5 9 3 7 1 5\n");
                    continue;
                }
                
                if (array != NULL) free(array);
                array = (int*)malloc((n + 1) * sizeof(int));
                if (array == NULL) {
                    printf("Ошибка выделения памяти\n");
                    continue;
                }
                
                array[0] = n;
                
                strcpy(temp_input, input);
                token = strtok(temp_input, " ");
                token = strtok(NULL, " ");
                
                for (int i = 1; i <= n && token != NULL; i++) {
                    array[i] = atoi(token);
                    token = strtok(NULL, " ");
                }
                
                array = Sort(array);
                
                printf("Отсортированный массив: ");
                for (int i = 1; i <= n; i++) {
                    printf("%d ", array[i]);
                }
                printf("\n");
                break;
            }
            
            default:
                printf("Неизвестная команда. Используйте 0, 1 или 2\n");
                break;
        }
    }
    
    if (array != NULL) free(array);
    return 0;
}
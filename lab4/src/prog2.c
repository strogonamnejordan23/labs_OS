#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

// Объявления типов функций
typedef float (*SinIntegralFunc)(float, float, float);
typedef int* (*SortFunc)(int*);

int main() {
    void *library = NULL;
    SinIntegralFunc SinIntegral = NULL;
    SortFunc Sort = NULL;
    
    int current_lib = 1;
    char lib_name[2][20] = {"./libimpl1.so", "./libimpl2.so"};
    char input[256];
    char command;
    float a, b, e;
    int *array = NULL;
    
    // Загружаем первую библиотеку
    printf("Загружаем библиотеку %s\n", lib_name[0]);
    library = dlopen(lib_name[0], RTLD_LAZY);
    if (!library) {
        fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
        return 1;
    }
    
    // Загружаем функции
    SinIntegral = (SinIntegralFunc)dlsym(library, "SinIntegral");
    Sort = (SortFunc)dlsym(library, "Sort");
    
    if (!SinIntegral || !Sort) {
        fprintf(stderr, "Ошибка загрузки функций: %s\n", dlerror());
        dlclose(library);
        return 1;
    }
    
    printf("Программа 2: Динамическая загрузка библиотек\n");
    printf("Команды:\n");
    printf("  0 - переключить реализацию (libimpl1.so <-> libimpl2.so)\n");
    printf("  1 A B e - вычислить интеграл sin(x) от A до B с шагом e\n");
    printf("  2 N x1 x2 ... xN - отсортировать массив из N элементов\n");
    printf("  q - выход\n\n");
    
    while (1) {
        printf("> ");
        
        // Читаем всю строку
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Убираем символ новой строки
        input[strcspn(input, "\n")] = 0;
        
        // Проверяем на выход
        if (input[0] == 'q' && (input[1] == '\0' || input[1] == ' ')) {
            break;
        }
        
        // Парсим команду
        if (sscanf(input, "%c", &command) != 1) {
            printf("Неверный формат команды\n");
            continue;
        }
        
        switch (command) {
            case '0': {
                // Переключение библиотеки
                dlclose(library);
                
                current_lib = (current_lib == 1) ? 2 : 1;
                printf("Переключаем на библиотеку %s\n", lib_name[current_lib - 1]);
                
                library = dlopen(lib_name[current_lib - 1], RTLD_LAZY);
                if (!library) {
                    fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
                    return 1;
                }
                
                SinIntegral = (SinIntegralFunc)dlsym(library, "SinIntegral");
                Sort = (SortFunc)dlsym(library, "Sort");
                
                if (!SinIntegral || !Sort) {
                    fprintf(stderr, "Ошибка загрузки функций: %s\n", dlerror());
                    dlclose(library);
                    return 1;
                }
                break;
            }
            
            case '1': {
                // Интеграл
                if (sscanf(input, "%*c %f %f %f", &a, &b, &e) != 3) {
                    printf("Ошибка: требуется 3 аргумента: A B e\n");
                    printf("Пример: 1 0 3.14159 0.01\n");
                    continue;
                }
                
                float result = SinIntegral(a, b, e);
                printf("Результат (библиотека %d): %f\n", current_lib, result);
                break;
            }
            
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
                token = strtok(NULL, " "); // пропускаем команду '2'
                
                for (int i = 1; i <= n && token != NULL; i++) {
                    array[i] = atoi(token);
                    token = strtok(NULL, " ");
                }
                array = Sort(array);
                printf("Отсортированный массив (библиотека %d): ", current_lib);
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
    if (library != NULL) dlclose(library);
    return 0;
}
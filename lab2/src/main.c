#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// #define ROWS1 2
// #define COLS1 3
// #define ROWS2 3
// #define COLS2 2

#define ROWS1 400
#define COLS1 400
#define ROWS2 400
#define COLS2 400
#define SIZE 400

int MAX_THREADS;

typedef struct{
        double Re;
        double Im;
    }complex_matrix;

typedef struct{
        int index;
        complex_matrix (*result)[COLS2];
        complex_matrix (*a)[COLS1];
        complex_matrix (*b)[COLS2];
    }args_for_thread;//вынес структуры для глобального значения


void* multiply_matrixes(void* arg){

    args_for_thread* args = (args_for_thread*)arg;
    int thread_index = args->index;
    complex_matrix (*result)[COLS2] = args->result;
    complex_matrix (*matrix_a)[COLS1] = args->a;
    complex_matrix (*matrix_b)[COLS2] = args->b;

    int per_thread=ROWS2/MAX_THREADS;

    if (thread_index!=MAX_THREADS-1){
        for (int i=per_thread*thread_index;i<per_thread*(thread_index+1);i++){
            for (int j=0;j<COLS2;j++){
                double real_sum = 0.0;
                double imag_sum = 0.0;
                for (int k=0;k<COLS1;k++){
                    double a_real = matrix_a[i][k].Re;
                    double a_imag = matrix_a[i][k].Im;
                    double b_real = matrix_b[k][j].Re;
                    double b_imag = matrix_b[k][j].Im;
                    real_sum += a_real * b_real - a_imag * b_imag;
                    imag_sum += a_real * b_imag + a_imag * b_real;
                }
                result[i][j].Re=real_sum;
                result[i][j].Im=imag_sum;
            }
        }
    }else{
        for (int i=per_thread*thread_index;i<ROWS1;i++){
            for (int j=0;j<COLS2;j++){
                double real_sum = 0.0;
                double imag_sum = 0.0;
                for (int k=0;k<COLS1;k++){
                    double a_real = matrix_a[i][k].Re;
                    double a_imag = matrix_a[i][k].Im;
                    double b_real = matrix_b[k][j].Re;
                    double b_imag = matrix_b[k][j].Im;
                    real_sum += a_real * b_real - a_imag * b_imag;
                    imag_sum += a_real * b_imag + a_imag * b_real;
                }
                result[i][j].Re=real_sum;
                result[i][j].Im=imag_sum;
            }
        }
    }
}

 void random_matrix(complex_matrix (*matrix)[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            (*matrix)[i][j].Re = rand() % 20;
            (*matrix)[i][j].Im = rand() % 20; // значения от 0 до 19
        }
    }
}
int main(int argc,char *argv[]){
    // clock_t start,end;
    // start=clock();
    struct timeval start, end;
    gettimeofday(&start, NULL);
    if (argc != 2){
        printf("False format\n");
    }
    MAX_THREADS=atoi(argv[1]);
    pthread_t threads[MAX_THREADS];
    // complex_matrix complex_matrix1[ROWS1][COLS1]={{{3,1},{1,1},{-3,1}},{{2,0},{-1,1},{-1,-1}}};
    // complex_matrix complex_matrix2[ROWS2][COLS2]={{{1,3},{2,1}},{{2,0},{1,3}},{{0,1},{-1,2}}};
    complex_matrix complex_matrix1[ROWS1][COLS1]={0};
    complex_matrix complex_matrix2[ROWS2][COLS2]={0};
    random_matrix(&complex_matrix1);
    random_matrix(&complex_matrix2);
    
    complex_matrix result_matrix[ROWS1][COLS2]={0};

    args_for_thread args[MAX_THREADS];

    for (int i=0;i<MAX_THREADS;i++){
        args[i].index=i;
        args[i].result=result_matrix;
        args[i].a=complex_matrix1;
        args[i].b=complex_matrix2;
        pthread_create(&threads[i],NULL,multiply_matrixes,&args[i]);
    }
    for (int i=0;i<MAX_THREADS;i++){
        pthread_join(threads[i],NULL);
    }
    // end=clock();
    // for (int i=0;i<ROWS1;i++){
    //     for (int j=0;j<COLS2;j++){
    //         if (result_matrix[i][j].Im>0){
    //         printf("%f+%fi ",result_matrix[i][j].Re,result_matrix[i][j].Im);
    //         }else if (result_matrix[i][j].Im==0){
    //         printf("%f ",result_matrix[i][j].Re);
    //         }else{
    //         printf("%f%fi ",result_matrix[i][j].Re,result_matrix[i][j].Im);
    //         }
    //     }
    //     printf("\n");
    // }
    gettimeofday(&end, NULL);    // Конец измерения
    
    double delta = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    // double delta = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Время выполнения программы c %d потоками: %f секунд\n",MAX_THREADS,delta);

    return 0;
}
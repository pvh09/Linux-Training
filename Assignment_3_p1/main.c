#include "matrix.h"

int main() {
    matrix_t* p_matrix_a = NULL;
    matrix_t* p_matrix_b = NULL;
    int row_matrix_a = 0; 
    int row_matrix_b = 0; 
    int col_matrix_a = 0; 
    int col_matrix_b = 0;
    int num_threads = 4;
    double time_spent = 0.0;

    printf(">> Enter matrix A size \n");
    input_number_of_array(&row_matrix_a, &col_matrix_a);
    printf(">> Enter matrix B size \n");
    input_number_of_array(&row_matrix_b, &col_matrix_b);

    p_matrix_a = init_matrix(row_matrix_a, col_matrix_a);
    p_matrix_b = init_matrix(row_matrix_b, col_matrix_b);

    printf(">> Enter matrix A:\n");
    create_data_matrix(p_matrix_a);
    printf(">> Enter matrix B:\n");
    create_data_matrix(p_matrix_b);
    
    // printf("\n==========================");
    // printf("\n>> matrix_t A:\n");
    // print_matrix(p_matrix_a);
    // printf(">> matrix_t B:\n");
    // print_matrix(p_matrix_b);
    
    clock_t begin = clock();
    matrix_t* p_product_matrix = calculate_product_matrix(p_matrix_a, p_matrix_b);
    clock_t end = clock();
    if(p_product_matrix != NULL) {
        printf("\n>> Calculate 2 matrix: NORMAL\n");
        //print_matrix(p_product_matrix);
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf(">> The elapsed time is %f seconds", time_spent);
        time_spent = 0.00;
    }
    else {
        printf(">> matrix_t A, matrix_t B can not multi together\n");
    }

    begin = clock();
    matrix_t* p_product_matrix_multi_threads = calculate_product_matrix_multi(p_matrix_a, p_matrix_b, num_threads);
    end = clock();
    if(p_product_matrix_multi_threads != NULL) {
        printf("\n\n>> Calculate 2 matrix: MULTI THREADS\n");
        //print_matrix(p_product_matrix_multi_threads);
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf(">> The elapsed time is %f seconds\n", time_spent);
        time_spent = 0.00;
    }
    else {
        printf(">> matrix_t A, matrix_t B can not multi together\n");
    }

    free_matrix(p_matrix_a);
    free_matrix(p_matrix_b);
    free_matrix(p_product_matrix);
    return 0;
}
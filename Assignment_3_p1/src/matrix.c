#include "matrix.h"

/*FUNCTION==========================================================
Function name: free_matrix
Description: release memory
===================================================================*/
void free_matrix(matrix_t *matrix)
{
    for (int i = 0; i < matrix->row; i++)
    {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
    matrix = NULL;
}
/*FUNCTION==========================================================
Function name: check_row_col_matrix
Description: check valid condition for row and column
===================================================================*/
static bool check_row_col_matrix(int row_matrix_a, int col_matrix_a, int row_matrix_b, int col_matrix_b, bool flag)
{
    if (flag == 1)
    {
        if (row_matrix_a != row_matrix_b || col_matrix_a != col_matrix_b)
        {
            return true;
        }
    }
    else
    {
        if (col_matrix_a != row_matrix_b)
        {
            return true;
        }
    }
    return false;
}

/*FUNCTION==========================================================
===================================================================*/
matrix_t *init_matrix(int row, int col)
{
    matrix_t *p_matrix = NULL;
    p_matrix = (matrix_t *)malloc(sizeof(matrix_t));
    p_matrix->row = row;
    p_matrix->col = col;

    p_matrix->data = (double **)malloc(row * sizeof(double *));
    for (int i = 0; i < row; i++)
    {
        p_matrix->data[i] = (double *)malloc(col * sizeof(double));
    }
    return p_matrix;
}

/*FUNCTION==========================================================
Function name: input_number_of_array
Description: Enter the number of matrix row and column
===================================================================*/
void input_number_of_array(int *row, int *col)
{
    printf("Enter row of matrix: ");
    *row = validate_num_input(1);
    printf("Enter column of matrix: ");
    *col = validate_num_input(1);
}

/*FUNCTION==========================================================
Function name: create_matrix
Description: Enter input of matrix
===================================================================*/
void create_data_matrix(matrix_t *matrix)
{
    int i = 0;
    int j = 0;
    for (i; i < matrix->row; i++)
    {
        for (j = 0; j < matrix->col; j++)
        {
            printf("Enter matrix[%d][%d]: ", i, j);
            matrix->data[i][j] = rand() % 10; // validate_num_input(0);
        }
    }
}

/*FUNCTION==========================================================
Function name: print_matrix
Description: Print output of matrix
===================================================================*/
void print_matrix(matrix_t *matrix)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < matrix->row; i++)
    {
        for (j = 0; j < matrix->col; j++)
        {
            printf("%f ", matrix->data[i][j]);
        }
        printf("\n");
    }
}

/*FUNCTION==========================================================
Function name: calculate_product_matrix
Description: Calculate product of matrix A and matrix B
===================================================================*/
matrix_t *calculate_product_matrix(matrix_t *p_matrix_a, matrix_t *p_matrix_b)
{
    int i = 0;
    int j = 0;
    matrix_t *p_product_matrix = NULL;
    if (check_row_col_matrix(p_matrix_a->row, p_matrix_a->col, p_matrix_b->row, p_matrix_b->col, 0))
    {
        return NULL;
    }

    p_product_matrix = init_matrix(p_matrix_a->row, p_matrix_b->col);
    for (i = 0; i < p_matrix_a->row; i++)
    {
        for (j = 0; j < p_matrix_b->col; j++)
        {
            p_product_matrix->data[i][j] = 0;
            for (int k = 0; k < p_matrix_a->col; k++)
            {
                p_product_matrix->data[i][j] += (p_matrix_a->data[i][k] * p_matrix_b->data[k][j]);
            }
        }
    }
    return p_product_matrix;
}

void *multiply_rows(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    matrix_t *matrix_a = data->matrix_a;
    matrix_t *matrix_b = data->matrix_b;
    matrix_t *result_matrix = data->result;
    int start_row = data->start_row;
    int end_row = data->end_row;

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < matrix_b->col; j++)
        {
            double sum = 0;
            for (int k = 0; k < matrix_a->col; k++)
            {
                sum += matrix_a->data[i][k] * matrix_b->data[k][j];
            }
            result_matrix->data[i][j] = sum;
        }
    }

    pthread_exit(NULL);
}

matrix_t *calculate_product_matrix_multi(matrix_t *matrix_a, matrix_t *matrix_b, int num_threads)
{
    matrix_t *result_matrix = NULL;
    pthread_t threads[num_threads];
    int rows_per_thread = 0;
    int remaining_rows = 0;
    int start_row = 0;

    thread_data_t thread_data[num_threads];

    if (matrix_a->col != matrix_b->row)
    {
        return NULL;
    }

    result_matrix = init_matrix(matrix_a->row, matrix_b->col);
    rows_per_thread = matrix_a->row / num_threads;
    remaining_rows = matrix_a->row % num_threads;
    
    for (int i = 0; i < num_threads; i++)
    {
        int end_row = start_row + rows_per_thread;
        if (i == num_threads - 1)
        {
            end_row += remaining_rows;
        }

        thread_data[i].matrix_a = matrix_a;
        thread_data[i].matrix_b = matrix_b;
        thread_data[i].result = result_matrix;
        thread_data[i].start_row = start_row;
        thread_data[i].end_row = end_row;

        pthread_create(&threads[i], NULL, multiply_rows, &thread_data[i]);

        start_row = end_row;
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return result_matrix;
}
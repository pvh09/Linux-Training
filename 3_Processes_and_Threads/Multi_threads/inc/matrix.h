#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "file_handling.h"

typedef struct {
    int row;
    int col;
    double **data;
} matrix_t;

typedef struct {
    int start_row;
    int end_row;
    matrix_t *matrix_a;
    matrix_t *matrix_b;
    matrix_t *result;
} thread_data_t;

void input_number_of_array(int *row, int *col);

/*!
 * @brief <Initialize dynamic memory for matrix>
 *
 * @param  row <matrix row>
 *
 * @param  col <matrix column>
 *
 * @param *p_matrix [In] <pointer point to struct Matrix of the matrix>
 */
matrix_t *init_matrix(int row, int col);

/*!
 * @brief <Enter the number of matrix row and  column>
 *
 * @param *p_matrix [In] <pointer point to struct Matrix of the matrix>
 */
void create_data_matrix(matrix_t *matrix);

/*!
 * @brief <Print the matrix to the terminal>
 *
 * @param *p_matrix [In] <pointer point to struct Matrix of the matrix>
 *
 *return None
 */
void print_matrix(matrix_t *matrix);

/*!
 * @brief <release memory>
 *
 * @param *p_matrix_a <pointer elements of matrix A>
 *
 * @param *p_matrix_b <pointer elements of matrix B>
 */
matrix_t *calculate_product_matrix(matrix_t *p_matrix_a, matrix_t *p_matrix_b);

matrix_t *calculate_product_matrix_multi(matrix_t *matrix_a, matrix_t *matrix_b, int num_threads);

/*!
 * @brief <release memory>
 *
 * @param *p_matrix_a <pointer elements of matrix>
 */
void free_matrix(matrix_t *matrix);

/*!
* @brief <check validate mumber of row, column>
*
@param *inputRequest <>
*/
#endif


#ifndef TENSOR_H
#define TENSOR_H

#include <stdio.h> 
#include <stdlib.h>
#include <math.h>   
#include <string.h> 

typedef struct {
    float **data;
    int rows;
    int columns;
} matrix;

matrix* init(int rows, int columns);
matrix* dot(matrix *matrix1, matrix *matrix2);
matrix* add(matrix *matrix1, matrix *matrix2, float sign);
void scale(matrix *matrix1, float k, int index, const char *check); 
void exponent(matrix* matrix1);
float sum(matrix *matrix1);
matrix* transpose(matrix* matrix1);
void free_matrix(matrix *matrix1);
void fill_matrix(matrix* matrix1, float data);
void z_normalize(matrix* matrix1);
void min_max_normalize(matrix* matrix1);
void max_normalize(matrix* matrix1);
matrix* split_matrix(matrix* matrix1, int row1, int row2, int col1, int col2);
void stack(matrix *matrix1, matrix *matrix2, const char* check); // Use const char*
void print_matrix(matrix *matrix1);
matrix* multiply(matrix *matrix1, matrix *matrix2);
void matrix_size(matrix *matrix1);
matrix* read_csv_to_matrix(const char* filename);
#endif 
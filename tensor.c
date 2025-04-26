#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "tensor.h" 

matrix* init(int rows, int columns){

    matrix* matrix3 = (matrix*)malloc(sizeof(matrix));
    if (matrix3 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    matrix3->rows=rows;
    matrix3->columns=columns;
    matrix3->data = (float**)malloc(matrix3->rows * sizeof(float*));
    if (matrix3->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    for (int i = 0; i < matrix3->rows; i++) {
        matrix3->data[i] = (float*)malloc(matrix3->columns * sizeof(float));
        if (matrix3->data[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++){
                free(matrix3->data[j]);
            }
            free(matrix3->data);
            free(matrix3);
            return NULL;
        }
    }
    return matrix3;
}
matrix* dot(matrix *matrix1,matrix *matrix2){
    if (matrix1->columns!= matrix2->rows){
    fprintf(stderr, "Invalid operation \n");
    return NULL;
    }
    matrix* matrix3=init(matrix1->rows,matrix2->columns);
    if (matrix3 == NULL) return NULL;
    for (int i = 0; i < matrix3->rows; i++){
        for (int j = 0; j < matrix3->columns; j++)
        {
            matrix3->data[i][j]=0;
            for (int k = 0; k < matrix1->columns; k++) matrix3->data[i][j]+= matrix1->data[i][k]*matrix2->data[k][j];
        }
    }
    return matrix3;
}
matrix* add(matrix *matrix1,matrix *matrix2,float sign){
    if (matrix1->columns != matrix2->columns || matrix1->rows!= matrix2 ->rows){
    fprintf(stderr, "Invalid operation \n");
    return NULL;
    }
    matrix* matrix3=init(matrix1->rows,matrix2->columns);
    if (matrix3 == NULL) return NULL;
    for (int i = 0; i < matrix3->rows; i++){
        for (int j = 0; j < matrix3->columns; j++)
        {
            matrix3->data[i][j]= matrix1->data[i][j]+ sign*matrix2->data[i][j];
        }
    }
    return matrix3;
}
void scale(matrix *matrix1,float k, int index,const char *check){
    if (strcmp(check, "row") == 0){
        for (int i = 0; i < matrix1->columns; i++) matrix1->data[index][i]*=k;
    }
    else if (strcmp(check, "column") == 0){
        for (int i = 0; i < matrix1->rows; i++) matrix1->data[i][index]*=k;
    }
    else{
        fprintf(stderr, "invalid parameter\n");
    }
}
void exponent(matrix* matrix1){
    for (int i = 0; i < matrix1->rows; i++){
        for (int j = 0; j < matrix1->columns; j++){
            matrix1->data[i][j]=exp(matrix1->data[i][j]);
        }
    }
}
float sum(matrix *matrix1){
    float sum = 0.0;
    for (int i = 0; i < matrix1->rows; i++){
        for (int j = 0; j < matrix1->columns; j++){
            sum += matrix1->data[i][j];
        }
    }
    return sum;
}
matrix* transpose(matrix* matrix1){
    matrix* matrix3 = init(matrix1->columns,matrix1->rows);
    if (matrix3 == NULL) return NULL;
    for (int i = 0; i < matrix3->rows; i++){
        for (int j = 0; j < matrix3->columns; j++)
        matrix3->data[i][j] =  matrix1->data[j][i];
    }
    return matrix3;
}

void free_matrix(matrix *matrix1) {
    if (matrix1 == NULL) {
        return; 
    }
    if (matrix1->data != NULL) {
        for (int j = 0; j < matrix1->rows; j++) {
            free(matrix1->data[j]);
        }
        free(matrix1->data);
    }
    free(matrix1);
}
void fill_matrix(matrix* matrix1, float data){
    for(int i = 0; i < matrix1->rows; i++){
        for(int j = 0; j < matrix1->columns; j++) matrix1->data[i][j]= data;
    }
}
void z_normalize(matrix* matrix1) {
    for (int j = 0; j < matrix1->columns; j++) {
        float sum = 0;
        for (int i = 0; i < matrix1->rows; i++) {
            sum += matrix1->data[i][j];
        }
        float mean = (double)sum / matrix1->rows;
        float variance = 0;
        for (int i = 0; i < matrix1->rows; i++) {
            variance += pow(matrix1->data[i][j] - mean, 2);
        }
        double stddev = sqrt(variance / matrix1->rows);

        for (int i = 0; i < matrix1->rows; i++) {
            matrix1->data[i][j] = (matrix1->data[i][j] - mean) / stddev;
        }
    }
}
matrix* split_matrix(matrix* matrix1, int row1,int row2, int col1, int col2)
{   
    matrix* matrix3=init(row2-row1,col2-col1);
    for(int i=0;i<row2-row1;i++){
        for(int j=0;j<col2-col1;j++){
            matrix3->data[i][j]=matrix1->data[row1+i][col1+j];
        }
    }
    return matrix3;
}
void stack(matrix *matrix1, matrix *matrix2,const char* check) {
    if (strcmp(check, "row") == 0) {
        if (matrix2->columns != matrix1->columns) {
            fprintf(stderr, "Invalid operation");
            return;
        }
        float **newdata = (float**)realloc(matrix1->data, (matrix1->rows + matrix2->rows) * sizeof(float*));
        if (newdata == NULL) {
            fprintf(stderr, "Error reallocating memory\n");
            return;
        }
        for (int i = matrix1->rows; i < matrix1->rows + matrix2->rows; i++) {
            newdata[i] = (float*)malloc(matrix2->columns * sizeof(float));
            if (newdata[i] == NULL) {
                fprintf(stderr, "Memory allocation failed \n");
                return;
            }
            memcpy(newdata[i], matrix2->data[i - matrix1->rows], matrix2->columns * sizeof(float));
        }
        matrix1->data = newdata;
        matrix1->rows += matrix2->rows;
    } else if (strcmp(check, "column") == 0) {
        if (matrix1->rows != matrix2->rows) {
            fprintf(stderr, "Invalid operation\n");
            return;
        }
        for (int i = 0; i < matrix1->rows; i++) {
            float* newdata = (float*)realloc(matrix1->data[i],(matrix1->columns + matrix2->columns) * sizeof(float));
            if (newdata == NULL) {
                fprintf(stderr, "Memory allocation failed \n");
                return;
            }
            memcpy(newdata + matrix1->columns, matrix2->data[i], matrix2->columns * sizeof(float));
            matrix1->data[i]=newdata;
        }
        matrix1->columns += matrix2->columns;
    } else {
        fprintf(stderr, "Invalid parameter: Use 'row' or 'column'\n");
    }
}
void print_matrix(matrix *matrix1){
    for(int i = 0; i < matrix1->rows; i++){
        for(int j = 0; j < matrix1->columns; j++){
            printf("%f ",matrix1->data[i][j]);
            if (j==matrix1->columns-1) printf("\n");
        }
    }
}
void min_max_normalize(matrix* matrix1){
    for (int i = 0; i < matrix1->columns; i++){
        float max=matrix1->data[0][i];
        float min=matrix1->data[0][i];
        for(int j = 0;j < matrix1->rows; j++){
            if(matrix1->data[j][i]>max) max =matrix1->data[j][i];
            if(matrix1->data[j][i]<min) min=matrix1->data[j][i];
        }
        float range=max-min;
        if(range>0.1){
            for(int j = 0;j < matrix1->rows; j++){
                matrix1->data[j][i]=(matrix1->data[j][i]-min)/range;
            }
        }
    }
}
void max_normalize(matrix* matrix1){
    for (int i = 0; i < matrix1->columns; i++){
        float max=matrix1->data[0][i];
        for(int j = 0;j < matrix1->rows; j++){
            if(matrix1->data[j][i]>max) max =matrix1->data[j][i];
        }
        if(max > 0.1 || max < 0.1){
            for(int j = 0;j < matrix1->rows; j++){
                matrix1->data[j][i]=matrix1->data[j][i]/max;
            }
        }
    }
}
void matrix_size(matrix *matrix1){
    printf("row: %d; collumns %d \n",matrix1->rows,matrix1->columns);
}
matrix* multiply(matrix *matrix1,matrix *matrix2){
    if (matrix1->rows!= matrix2->rows || matrix1->columns != matrix2->columns){
    fprintf(stderr, "Invalid operation \n");
    return NULL;
    }
    matrix* matrix3=init(matrix1->rows,matrix2->columns);
    if (matrix3 == NULL) return NULL;
    for (int i = 0; i < matrix3->rows; i++){
        for (int j = 0; j < matrix3->columns; j++)
        {
            matrix3->data[i][j]= matrix1->data[i][j]*matrix2->data[i][j];
        }
    }
    return matrix3;
}

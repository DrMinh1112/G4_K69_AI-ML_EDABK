#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <stdio.h> 
#include "tensor.h" 

typedef struct {
    char **column_names;
    matrix *data; 
    int rows;
    int cols;    
} DataFrame;
DataFrame* create_dataframe_from_csv(const char* filename, char delimiter_);
void free_dataframe(DataFrame* df);
void print_dataframe(const DataFrame* df);
matrix* dataframe_to_matrix(const DataFrame* df);

#endif
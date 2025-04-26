#include "dataframe.h" 
#include "tensor.h"    

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096
#define MAX_COL_NAME_LENGTH 256 

DataFrame* create_dataframe_from_csv(const char* filename, char delimiter_){
    FILE *file=fopen(filename,"r");
    if (!file){
        fprintf(stderr,"error opening file \n");
        return NULL;
    }
    char line[MAX_LINE_LENGTH];
    DataFrame *df = (DataFrame*)malloc(sizeof(DataFrame));
    if (!df) {
        fprintf(stderr,"Failed to allocate DataFrame struct \n");
        fclose(file);
        return NULL;
    }
    //rows and columns counting
    int rows=0;
    int cols=0;
    char delimiter[2]={delimiter_,'\0'};
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: Cannot read header line \n");
        fclose(file);
        return NULL;
    }
    line[strcspn(line, "\r\n")] = 0;
    char *line_copy = strdup(line);
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        if (strspn(line, " \t") != strlen(line)) {
            rows++;
        }
    }
    char *token=strtok(line_copy,delimiter);
    while(token){
        if (strlen(token) > 0) { 
            cols++;
       }
       token=strtok(NULL,delimiter);
    }
    free(line_copy);
    rewind(file);

    df->column_names=(char**)calloc(cols, sizeof(char*));
    if (!df->column_names) {
        perror("Memory allocation error for column names array \n");
        free(df);
        fclose(file);
        return NULL;
    }
    df->rows = rows;
    df->cols = cols;
    df->data=init(rows,cols);
    if (!df->data) {
        fprintf(stderr, "Error: Failed to initialize data matrix for file '%s'.\n", filename);
        free(df->column_names); // Free the array itself
        free(df);
        fclose(file);
        return NULL;
    }
    int i=0;
    if (fgets(line,sizeof(line),file)!=NULL){
        line[strcspn(line, "\r\n")] = 0;
        char* token= strtok(line,delimiter);
        while(token && i<cols){
            if(strlen(token)>0){
                df->column_names[i]=strdup(token);
                if(!df->column_names[i]){
                    fprintf(stderr,"Memory allocation error for column name string \n");
                    free_dataframe(df);
                    fclose(file);
                    return NULL;
                }
                i++;
            }
            token=strtok(NULL,delimiter);
        }
    }else{
        fprintf(stderr, "Error: Cannot read header line");
        fclose(file);
        return NULL;
    }
    if (i != cols) {
        fprintf(stderr, "Error: Header column count mismatch between passes (%d vs %d) in file '%s'.\n", cols, i, filename);
        free_dataframe(df); 
        fclose(file);
        return NULL;
    }
    int j=0;
    while(fgets(line,sizeof(line),file)!=NULL && j<rows){
        line[strcspn(line,"\r\n")]=0;
        if (strspn(line, " \t") == strlen(line)) {
            continue;
       }//skip empty line
       int i=0;
       token=strtok(line,delimiter);
       while(token!=NULL && i<cols){
            char *endptr;
            errno = 0; 
            float value = strtof(token, &endptr);
            if (errno != 0 || endptr == token || (*endptr != '\0' && *endptr != ' ' && *endptr != '\t')) {
                 fprintf(stderr, "Error converting value '%s' to float at row %d, col %d in file '%s'.\n",
                         token, j + 1, i + 1, filename);
                 free_dataframe(df);
                 fclose(file);
                 return NULL;
            }
            if (df->data) df->data->data[j][i] = value; 
            i++;
            token=strtok(NULL,delimiter);
       }
       j++;
    }
    fclose(file);
    return df;
}

void free_dataframe(DataFrame *df) {
    if (!df) {
        return;
    }
    if (df->column_names) {
        for (int i = 0; i < df->cols; i++) {
            free(df->column_names[i]);
        }
        free(df->column_names); 
    }
    if (df->data) {
        free_matrix(df->data);
    }
    free(df);
}
void print_dataframe(const DataFrame* df){
    for(int i=0;i<df->cols;i++){
        printf("%s ",df->column_names[i]);
        if(i==df->cols-1) printf("\n");
    }
    print_matrix(df->data);
}
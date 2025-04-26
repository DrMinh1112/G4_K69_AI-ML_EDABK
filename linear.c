#include <stdio.h>
#include "tensor.h" 
#include "dataframe.h"

#define iterations 500
#define learning_rate 0.6
#define espilon 1
//doc file va gop bias vao features
void preprocessor(const char* file,char delmiter,matrix** features,matrix** output){
    DataFrame *df=create_dataframe_from_csv(file,delmiter);
    *features=split_matrix(df->data,0,df->data->rows,0,df->data->columns-1);
    min_max_normalize(*features);
    *output = split_matrix(df->data,0,df->data->rows,df->data->columns-1,df->data->columns);
    matrix* bias=init(df->data->rows,1);
    fill_matrix(bias,1);
    stack(*features,bias,"column");
    free_matrix(bias);
    free_dataframe(df);
}
matrix* error(matrix* features, matrix* output, matrix* weight){
    matrix* prediction= dot(features,weight);
    matrix* error= add(prediction,output,-1);
    free_matrix(prediction);
    return error;
}
float cost_function(matrix* error){
    matrix* square_error=multiply(error,error);
    float cost= sum(square_error)/(float)(2*error->rows);
    free_matrix(square_error);
    return cost;
}
matrix* gradient(matrix* features,matrix* error){
    matrix* feature_T=transpose(features);
    matrix* gradient = dot(feature_T, error);
    scale(gradient,1.0/features->rows,0,"column");
    free_matrix(feature_T);
    return gradient;
}
void update_weight(matrix* gradient,matrix** weight){
    matrix* temp= add(*weight,gradient,-learning_rate);
    free_matrix(*weight);
    *weight=temp;
}
void linear_regression(matrix* features,matrix* output, matrix** weight){
    for(int i=0;i<iterations;i++){
        matrix* error_ = error(features,output,*weight);
        matrix* gradient_=gradient(features,error_);
        update_weight(gradient_,weight);
        matrix* check=multiply(gradient_,gradient_);
        if(sum(check)<espilon){
            printf("converged!\n");
            break;
        }
        float cost = cost_function(error_);
        if (i % 100 == 0) printf("cost= %f \n",cost);
        free_matrix(gradient_);
        free_matrix(check);
        free_matrix(error_);
    }
}
float prediction(matrix *features,matrix* weight){
    matrix* prediction=dot(features,weight);
    float predict= prediction->data[0][0];
    free_matrix(prediction);
    return predict;
}
int main(){
    matrix* features=NULL;
    matrix* output=NULL;
    preprocessor("coffee_shop_revenue.csv",',',&features,&output);
    matrix* weight=init(features->columns,1);
    fill_matrix(weight,0);
    linear_regression(features,output,&weight);
    matrix* test= split_matrix(features,2,3,0,features->columns);
    print_matrix(weight);
    printf("%f\n",prediction(test,weight));
    free_matrix(features);
    free_matrix(output);
    free_matrix(weight);
    free_matrix(test);
    return 0;
}
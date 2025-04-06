import numpy as np

learning_rate=0.6
iterations=5000 #ta co the thay doi iteration de biet xem su bien thien cost_function->dieu chinh learning_rate hoac chuan hoa features
bias=0 #init cho bias

def pre_loss_function(features,weight,targets,bias):
    lost= np.dot(features,weight)-targets+bias
    return lost

def loss_function(pre_lost):
    loss= np.dot(pre_loss,pre_loss.T)/(pre_loss.shape[0])
    return loss

def gradient(features,weight,targets,bias,pre_loss):
    d = np.dot(features.T,pre_loss)/(features.shape[0])
    d =d.astype(np.float32)
    d_bias=np.sum(pre_loss)/(features.shape[0])
    return d,d_bias

def update_parameters(weight,gradient,d_bias,bias):
    weight-=learning_rate*gradient
    bias=bias-learning_rate*d_bias
    return bias

def cost_function(pre_loss):
    cost= np.dot(pre_loss,pre_loss.T)/(2*pre_loss.shape[0])
    return cost

def linear_regression(features,weight,targets,bias):
    for i in range(0,iterations):
        pre_loss=pre_loss_function(features,weight,targets,bias)
        d,d_bias=gradient(features,weight,targets,bias,pre_loss)
        bias= update_parameters(weight,d,d_bias,bias)
        if np.linalg.norm(d)<0.1:
            print("converged")
            break
    print(weight)
    print(cost_function(pre_loss))
    #check xem cost co giam khong
    return bias

def normalization(data):
    pass

def preprocessor(csv_name):
    arr = np.loadtxt(csv_name,delimiter=",", dtype=str)
    data=arr[1::,:arr.shape[1]-1]
    data=data.astype(np.float32)
    result=arr[1:,arr.shape[1]-1].astype(np.float32) #ma tran cua targets
    return data,result

def prediction(features,bias):
    predict = np.dot(features,w)+bias
    return predict
#test
data,result=preprocessor("coffee_shop_revenue.csv")
w=np.zeros((data.shape[1],)) #init cho weights
#chuan hoa data
data[:,0]/=500
data[:,1]/=10
data[:,2]/=18
data[:,3]/=15
data[:,4]/=500
data[:,5]/=1000
bias=linear_regression(data,w,result,bias)
print(w)

#test du doan
x=np.array([398,9.09,6,6,91.76,636],dtype=np.float32)
x[0]/=500
x[1]/=10
x[2]/=18
x[3]/=15
x[4]/=500
x[5]/=1000
print(prediction(x,bias))

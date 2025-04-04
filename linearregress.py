import numpy as np

learning_rate=0.6
iterations=5000 #ta co the thay doi iteration de biet xem su bien thien cost_function->dieu chinh learning_rate hoac chuan hoa features

def loss_function(features,weight,targets):
    lost= np.dot(features,weight)-targets
    return lost

def gradient(features,weight,targets):
    temp=loss_function(features,weight,targets)
    d = np.dot(features.T,temp)/(features.shape[0])
    d =d.astype(np.float32)
    return d

def update_parameters(weight,gradient):
    weight-=learning_rate*gradient

def cost_function(loss):
    cost= np.dot(loss,loss.T)/(2*loss.shape[0])
    return cost

def linear_regression(features,weight,targets):
    for i in range(0,iterations):
        d=gradient(features,weight,targets)
        update_parameters(weight,d)
        if np.linalg.norm(d)<0.1:
            print("converged")
            break
    print(weight)
    print(cost_function(loss_function(features,weight,targets)))
    #check xem cost co giam khong

#test
arr = np.loadtxt("coffee_shop_revenue.csv",
                 delimiter=",", dtype=str)

data=arr[1::,:arr.shape[1]-1]
data=data.astype(np.float32)
temp=np.ones(data.shape[0])
data=np.column_stack((data,temp))
data=data.astype(np.float32) #data la ma tran cua features
result=arr[1:,arr.shape[1]-1].astype(np.float32) #ma tran cua targets

w=np.zeros((7,)) #init cho weights
#chuan hoa data
data[:,0]/=500
data[:,1]/=10
data[:,2]/=18
data[:,3]/=15
data[:,4]/=500
data[:,5]/=1000
linear_regression(data,w,result)
print(w)
def prediction(features):
    predict = np.dot(features,w)
    return predict
#test du doan
x=np.array([398,9.09,6,6,91.76,636,1],dtype=np.float32)
x[0]/=500
x[1]/=10
x[2]/=18
x[3]/=15
x[4]/=500
x[5]/=1000
print(prediction(x))

#!/usr/bin/python

from sklearn.neural_network import MLPClassifier
from sklearn.metrics import accuracy_score
import pandas as pd

# load the input train/test datasets from CSV
train_data = pd.read_csv("mnist_train.csv")
test_data = pd.read_csv("mnist_test.csv")

# get the input parameters of the samples
X_train = train_data.drop(['label'], axis=1)
X_test = test_data.drop(['label'], axis=1)

# get the classification labels of the samples
y_train = train_data['label']
y_test = test_data['label']

# # alternatively, split a single data set into test / training sets
# from sklearn.model_selection import train_test_split
# X_train, X_test, y_train, y_test = train_test_split(X_train, y_train, test_size= 0.25, shuffle=True, random_state=1)

# print for debugging purposes
print("train_data")
print(train_data.head(5))
print("test_data")
print(test_data.head(5))
print("X_train")
print(X_train.head(5))
print("X_test")
print(X_test.head(5))
print("y_train")
print(y_train.head(5))
print("y_test")
print(y_test.head(5))

# scale the training and testing data
from sklearn.preprocessing import StandardScaler
scaler = StandardScaler()
scaler.fit(X_train)
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

# perform the training
model = MLPClassifier(hidden_layer_sizes=(20, 10), max_iter=5000, alpha=0.0001, solver='lbfgs', verbose=10, random_state=1)
model.fit(X_train, y_train)

# compute testing results
y_pred = model.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)

# print results for debugging
print("TRAINING SAMPLES: " + str(len(X_train)))
print("TESTING SAMPLES: " + str(len(X_test)))
print("TESTING ACCURACY: " + str(accuracy))

# save the model and scaler
from joblib import dump
dump(model, 'mnist.model')
dump(scaler, 'mnist.scaler')


########## TEST ONLINE CLASSIFICATION ##########
from joblib import load
from sklearn.preprocessing import StandardScaler
import numpy as np

# load the saved model and scaler
model = load('mnist.model')
scaler = load('mnist.scaler')

# create a test sample (result should be 7)
test_sample = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,74,249,254,254,254,245,167,167,136,25,80,60,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,254,254,254,254,254,254,254,254,249,254,252,197,113,71,39,0,0,0,0,0,0,0,0,0,0,0,0,5,99,135,105,105,114,192,192,192,233,254,254,254,254,254,246,129,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,114,114,203,254,254,254,240,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,35,155,254,254,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,36,254,241,34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,115,254,254,118,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,243,254,240,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,111,254,254,139,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,37,243,254,244,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,176,254,254,113,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,140,254,254,220,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,253,254,243,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,241,254,254,83,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,243,254,254,147,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,111,254,254,203,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58,254,254,254,84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,237,254,255,194,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,82,254,254,194,27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39,230,193,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
x = np.array([test_sample])

# display the digit using matplotlib
import matplotlib.pyplot as plt
img = x.reshape((28,28))
plt.imshow(img, cmap="Greys")
plt.show()

# compute the classification result
result = int(model.predict(scaler.transform(x)))
print("Single test sample result: " + str(result))

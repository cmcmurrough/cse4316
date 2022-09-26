#!/usr/bin/python

from sklearn.neural_network import MLPClassifier
from sklearn.metrics import accuracy_score
import pandas as pd

# load the input train/test datasets from CSV
train_data = pd.read_csv("mnist_train.csv")
test_data = pd.read_csv("mnist_test.csv")

# get the input parameters of the samples (everything but the first column in the MNIST dataset)
X_train = train_data.drop(train_data.columns[0], axis=1)
X_test = test_data.drop(test_data.columns[0], axis=1)

# get the classification labels of the samples (the first column in the MNIST dataset)
y_train = train_data.iloc[:, 0]
y_test = test_data.iloc[:, 0]

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
scaler.fit(X_train.values)
X_train = scaler.transform(X_train.values)
X_test = scaler.transform(X_test.values)

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

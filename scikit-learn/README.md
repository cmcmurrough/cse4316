scikit-learn examples
========

Machine learning examples using scikit-learn. 

MNIST in CSV dataset located here: https://pjreddie.com/projects/mnist-in-csv/

Download the following two files:
https://pjreddie.com/media/files/mnist_train.csv
https://pjreddie.com/media/files/mnist_test.csv

MLPClassifier_train.py will train and test using the MNIST datasets. After the training and testing, scaler and model files will be exported for deployment use.

MLPClassifier_deploy.py will load the previously saved scaler and model files, perform one classification operation with a single test sample, display results, and terminate. This file is intented to demonstrate how to deploy a trained model to a generic python application.


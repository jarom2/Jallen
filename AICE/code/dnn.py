from pandas import DataFrame
from pandas import concat
import numpy as np
import pandas as pd
from keras.models import Sequential
import keras.layers
from keras.wrappers.scikit_learn import KerasRegressor
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold
from keras.layers import Dense
from keras.layers import LSTM
# get data
dataframe = pd.read_csv("cnn_combined_data.csv")
Y = dataframe['Solar_Power']
dataframe.drop('Solar_Power', 1, inplace=True)
X = dataframe
X.to_numpy()
Y.to_numpy()
# 1. define network
model = Sequential()
model.add(Dense(64, input_dim=33, activation='relu'))
model.add(Dense(64, activation='relu'))
model.add(Dense(64, activation='relu'))
model.add(Dense(1, kernel_initializer='normal'))
# 2. compile network
model.compile(optimizer='adam', loss='mean_squared_error')
# 3. fit network
estimator = KerasRegressor(model, epochs=100, batch_size=5, verbose=0)
kfold = KFold(n_splits=10)
results = cross_val_score(estimator, X, Y, cv=kfold)
print("Baseline: %.2f (%.2f) MSE" % (results.mean(), results.std()))

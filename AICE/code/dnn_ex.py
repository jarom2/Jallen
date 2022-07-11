import numpy as np
from pandas import read_csv
from keras.models import Sequential
from keras.layers import Dense
from keras.wrappers.scikit_learn import KerasRegressor
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold
# load dataset
dataframe = read_csv("cnn_combined_data.csv")
Y = dataframe['Solar_Power']
dataframe.drop('Solar_Power', 1, inplace=True)
X = dataframe.to_numpy()
Y = Y.to_numpy()
X = np.nan_to_num(X, copy=True)
Y = np.nan_to_num(Y, copy=True)
Y = Y/100
print(X.shape)
print(Y.shape)

# define base model
def baseline_model():
    # create model
    model = Sequential()
    model.add(Dense(64, input_dim=33, kernel_initializer='normal', activation='relu'))
    model.add(Dense(1, kernel_initializer='normal'))
    # Compile model
    model.compile(loss='mean_squared_error', optimizer='adam')
    return model
# evaluate model
estimator = KerasRegressor(build_fn=baseline_model, epochs=10, batch_size=5, verbose=0)
kfold = KFold(n_splits=10)
results = cross_val_score(estimator, X, Y, cv=kfold)
print(results)
print("Baseline: %.2f (%.2f) MSE" % (results.mean(), results.std()))
#estimator.save('saved_model')


import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import mean_absolute_error

data = pd.read_csv('../data/cnn_combined_data2.csv')
data2 = data[data['Solar_Power'] < 40000]
print(data2.shape[0])

y = data['Solar_Power']
data.drop('Solar_Power', 1, inplace=True)
print(data.columns)
x = data.to_numpy()
y = y.to_numpy()
x = np.nan_to_num(x, copy=True)
y = np.nan_to_num(y, copy=True)
y = y/1000

y2 = data2['Solar_Power']
data2.drop('Solar_Power', 1, inplace=True)
x2 = data2.to_numpy()
y2 = y2.to_numpy()
x2 = np.nan_to_num(x2, copy=True)
y2 = np.nan_to_num(y2, copy=True)
y2 = y2/1000

x_train = x[0:2999]
x_test = x[3000:3300]

y_train = y[0:2999]
y_test = y[3000:3300]



with np.errstate(invalid='ignore'):

    # TRAIN DATA
    regressor = RandomForestRegressor(n_estimators=100, random_state=0)
    regressor.fit(x_train, y_train)
    predictions = regressor.predict(x2)
    print(mean_absolute_error(y2, predictions))

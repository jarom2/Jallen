import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.ensemble import RandomForestRegressor

data = pd.read_csv('../data/cnn_combined_data1.csv')
data['Solar_Power'] = data['Solar_Power']/1000 #Convert to KW
x = data.to_numpy()
x = np.nan_to_num(x, copy=True)
data1 = data['Solar_Power']
data2 = data['td_avg']
correlation = np.corrcoef(data1, data2)
correlation = round(correlation[0][1], 4)
print(correlation)
title = "Solar Gen (Panels) vs Dewpoint Corr: {}".format(correlation)
x_axis_label = "Solar Generation (KW)"
y_axis_label = "Avg Dewpoint (Degrees)"
plt.scatter(data1, data2)
plt.title(title)
plt.xlabel(x_axis_label)
plt.ylabel(y_axis_label)
plt.savefig('../figures/solargen_dew.jpg')


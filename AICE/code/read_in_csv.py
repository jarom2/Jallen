import pandas as pd
import plotly.graph_objects as go

my_csv = pd.read_csv("combined_data_trimmed2.csv")
#my_csv.drop('Unnamed: 0', axis=1, inplace=True)
#del my_csv['Unnamed: 0']
#del my_csv['Unnamed: 0.1']
#del my_csv['station_id']
print(my_csv)
#my_csv.to_csv(r'combined_data_trimmed2.csv')

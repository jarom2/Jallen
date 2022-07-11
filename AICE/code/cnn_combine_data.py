import pandas as pd
import plotly.graph_objects as go

data = pd.read_csv("combined_data1.csv")
data.drop('rh_tmn', 1, inplace=True)
data.drop('rh_tmx', 1, inplace=True)
data.drop('td_tmn', 1, inplace=True)
data.drop('td_tmx', 1, inplace=True)
data.drop('airt_tmn', 1, inplace=True)
data.drop('airt_tmx', 1, inplace=True)
data.drop('winds_tmx', 1, inplace=True)
data.drop('timestamp', 1, inplace=True)

data.to_csv(r'cnn_combined_data.csv', index=False)
test_data = pd.read_csv("cnn_combined_data.csv")
print(test_data)
#solar_data = pd.read_csv("no_index_processed_solar.csv")
#weather_data = pd.read_csv("weather_data.csv")

#solar_data['timestamp'] = pd.to_datetime(solar_data['TIME'])
#weather_data['timestamp'] = pd.to_datetime(weather_data['date_time'])

#weather_data.drop('date_time', 1, inplace=True)
#solar_data.drop('TIME', 1, inplace=True)
#weather_data.drop('fan_rmp_avg', 1, inplace=True)
#weather_data.drop('batt_volt_min', 1, inplace=True)
#weather_data.drop('station_id', 1, inplace=True)

#new_data = pd.merge_asof(solar_data, weather_data, on='timestamp')
#new_data.set_index('timestamp', inplace=True)
#new_data.to_csv(r'combined_data1.csv')
#print(solar_data.shape)
#print(weather_data.shape)
#print(new_data.shape)
#print(new_data)


#fig = go.Figure(go.Scatter(x=solar_gen['TIME'], y=solar_gen['Solar_Power'], name='Solar Generation'))
#fig.update_layout(title='Averaged Solar Generation', showlegend=True)
##fig.write_image("avg_solar_gen.jpg")

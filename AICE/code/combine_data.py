import pandas as pd
import plotly.graph_objects as go

solar_data = pd.read_csv("no_index_processed_solar.csv")
weather_data = pd.read_csv("weather_data.csv")

solar_data['timestamp'] = pd.to_datetime(solar_data['TIME'])
weather_data['timestamp'] = pd.to_datetime(weather_data['date_time'])

weather_data.drop('date_time', 1, inplace=True)
solar_data.drop('TIME', 1, inplace=True)
weather_data.drop('fan_rmp_avg', 1, inplace=True)
weather_data.drop('batt_volt_min', 1, inplace=True)
weather_data.drop('station_id', 1, inplace=True)

new_data = pd.merge_asof(solar_data, weather_data, on='timestamp')
new_data.set_index('timestamp', inplace=True)
new_data.to_csv(r'combined_data1.csv')
print(solar_data.shape)
print(weather_data.shape)
print(new_data.shape)
print(new_data)


#fig = go.Figure(go.Scatter(x=solar_gen['TIME'], y=solar_gen['Solar_Power'], name='Solar Generation'))
#fig.update_layout(title='Averaged Solar Generation', showlegend=True)
#fig.write_image("avg_solar_gen.jpg")

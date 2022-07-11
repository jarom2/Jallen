import pandas as pd
import plotly.graph_objects as go

solar_data = pd.read_csv("new_processed_solar.csv")
weather_data = pd.read_csv("weather_data.csv")

solar_data['timestamp'] = pd.to_datetime(solar_data['TIME'])
weather_data['timestamp'] = pd.to_datetime(weather_data['date_time'])

new_data = pd.merge_asof(solar_data, weather_data, on='Timestamp')

del new_data['TIME']
del new_data['date_time']




#fig = go.Figure(go.Scatter(x=solar_gen['TIME'], y=solar_gen['Solar_Power'], name='Solar Generation'))
#fig.update_layout(title='Averaged Solar Generation', showlegend=True)
#fig.write_image("avg_solar_gen.jpg")

import pandas as pd
import plotly.graph_objects as go

solar_gen = pd.read_csv("weather_data.csv")

solar_gen2 = solar_gen[0:100]
fig = go.Figure(go.Scatter(x=solar_gen2['date_time'], y=solar_gen2['rso'], name='Solar Radiation'))
fig.update_layout(title='Solar Radiation', showlegend=True)
fig.write_image("fig3.jpg")
print(solar_gen.shape)
print(solar_gen2.shape)

import pandas as pd
import datetime
import plotly.graph_objects as go

solar_gen = pd.read_csv("processed_solar.csv")
date_time = solar_gen['TIME']
time_stamp = []

print("Processing time stamps...")
for i in range(date_time.shape[0]):
    date_time_str = str(date_time.iloc[i])
    date_time_obj = datetime.datetime.strptime(date_time_str, '%Y-%m-%d %H:%M:%S')
    time_stamp.append(date_time_obj)

generation = solar_gen['Solar_power']
new_solar = []
count = 0
solar_val = float(0)
stamp = time_stamp[0]

print("processing solar data...")
for i in range(len(time_stamp)):
    if time_stamp[i].hour != stamp.hour:
        new_solar.append([stamp, round(solar_val/count)])
        stamp = time_stamp[i]
        count = 0
        solar_val = float(0)
    solar_val += generation[i]
    count += 1

#new_solar = solar_gen[solar_gen['Solar_power'] > 0.0]
print(new_solar)
new_solar = pd.DataFrame(new_solar, columns=['TIME', 'Solar_Power'])
new_solar.to_csv(r'no_index_processed_solar.csv', index=False)

#fig = go.Figure(go.Scatter(x=solar_gen['TIME'], y=solar_gen['Solar_power'], name='Solar Generation'))
#fig.update_layout(title='Solar Generation', showlegend=True)

#fig.write_image("fig.jpg")

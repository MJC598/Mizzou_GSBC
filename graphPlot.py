from plotly import __version__
from plotly.offline import download_plotlyjs, init_notebook_mode, plot, iplot
import plotly.graph_objs as go

mq135Data = []
mq131Data = []
mq4Data = []
x_axis = []
x_a = []
xAxis = []
i = 1
j = 1
k = 1

mq135 = open("mq135.txt", "r")
mq131 = open("mq131.txt", "r")
mq4 = open("mq4.txt", "r")

for line in mq135:
    mq135Data.append(line)
    x_axis.append(i)
    i+=1
    
for line in mq131:
    mq131Data.append(line)
    x_a.append(j)
    j+=1
    
for line in mq4:
    mq4Data.append(line)
    xAxis.append(k)
    k+=1
    
    
gasSensor = go.Scatter(x=x_axis, y=mq135Data, name='MQ135 Gas Sensor (ppm)')
ozoneSensor = go.Scatter(x=x_a, y=mq131Data, name='MQ131 Ozone Sensor (ppb)')
methaneSensor = go.Scatter(x=xAxis, y=mq4Data, name='MQ4 Methane Sensor (ppm)')

data = [gasSensor, ozoneSensor, methaneSensor]
layout = go.Layout(title='Gasses', xaxis=dict(title='measurements'), yaxis=dict(title='Level'))
fig = go.Figure(data=data, layout=layout)
plot_url = plot(fig, filename='sensorReadings.html')

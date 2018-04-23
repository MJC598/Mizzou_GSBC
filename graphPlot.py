from plotly import __version__
from plotly.offline import download_plotlyjs, init_notebook_mode, plot, iplot
import plotly.graph_objs as go

mq135Data = []
mq131Data = []
mq4Data = []
altData = []
x_axis = []
x_a = []
xAxis = []
xA = []
i = 1
j = 1
k = 1
z = 1

mq135 = open("MQ135.txt", "r")
mq131 = open("MQ131.txt", "r")
mq4 = open("MQ4.txt", "r")
alt = open("ALTI.txt", "r")

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
    
for line in alt:
    altData.append(line)
    xA.append(z)
    z+=1
    
    
gasSensor = go.Scatter(x=x_axis, y=mq135Data, name='MQ135 Gas Sensor (ppm)')
ozoneSensor = go.Scatter(x=x_a, y=mq131Data, name='MQ131 Ozone Sensor (ppm)')
methaneSensor = go.Scatter(x=xAxis, y=mq4Data, name='MQ4 Methane Sensor (ppm)')
altSensor = go.Scatter(x=xAxis, y=altData, name='Altimeter (ft)')

data = [gasSensor, ozoneSensor, methaneSensor, altSensor]
layout = go.Layout(title='Gasses', xaxis=dict(title='measurements'), yaxis=dict(title='Level'))
fig = go.Figure(data=data, layout=layout)
plot_url = plot(fig, filename='sensorReadings.html')

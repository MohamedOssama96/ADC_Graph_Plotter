
import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib import style
from datetime import datetime
from statistics import mean

adc_data = []
time_data = []

for i in range(10):
    time_data.append(datetime.now())
    adc_data.append(2.5)

style.use('fivethirtyeight')

Fig, ax = plt.subplots()
ax.set_xlim(0, 100)
ax.set_ylim(0, 100)
line, = plt.plot([], [], 'ro')


def decompress(value):

    if value <= 15:
        return ((value & 0x80) << 4) | (value & 0x0f)
    elif value <= 31:
        return ((value & 0x80) << 4) | (1 << 4) | (value & 0x0f)
    elif value <= 47:
        return ((value & 0x80) << 4) | (1 << 5) | ((value & 0x0f) << 1) | 1
    elif value <= 63:
        return ((value & 0x80) << 4) | (1 << 6) | ((value & 0x0f) << 2) | 2
    elif value <= 79:
        return ((value & 0x80) << 4) | (1 << 7) | ((value & 0x0f) << 3) | 4
    elif value <= 95:
        return ((value & 0x80) << 4) | (1 << 8) | ((value & 0x0f) << 4) | 16
    elif value <= 111:
        return ((value & 0x80) << 4) | (1 << 9) | ((value & 0x0f) << 5) | 32
    else :
        return ((value & 0x80) << 4) | (1 << 10) | ((value & 0x0f) << 6) | 64


def adc():
    serial_port = serial.Serial("COM4", 110)

    while True:
        adc_value_compressed = serial_port.read(1)
        adc_signal = 5 * decompress(int.from_bytes(adc_value_compressed, "little")) / 4095
        time = datetime.now()

        yield [adc_signal, time]


def animate(frame):

    adc_data.append(frame[0])
    time_data.append(frame[1])

    line.set_data(mean(adc_data[-9:-1]), time_data)

    ax.clear()
    ax.plot(time_data, adc_data)


Ani = FuncAnimation(Fig, func=animate, frames=adc(), interval=10)
plt.show()







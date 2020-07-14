# ADC_Graph_Plotter

This a ADC plotter on using Matplotlip on python.
The ADC value are read from TM4C123 controller and sent over UART to the computer through a COM port.
I applied digital companding using uLaw on the ADC values as it was 12 bits and UART only sends 8 bits, then in the python script I decompressed the values and applied a moving average filter to smooth the optained values.

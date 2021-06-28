
import sys
import serial#pip install pyserial
if __name__=="__main__":
    port="/dev/cu.usbmodem14201"
    ser = serial.Serial(port, 115200, timeout=0.050)
    print(ser.name)
    ser.write(bytes('abcdefghijklmnopqrstuvwxyz','utf-8'))
    ser.close()
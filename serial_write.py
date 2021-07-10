
import sys
import serial#pip install pyserial
#  Function Definitions

#=====================================

def sendToArduino(sendStr):
    ser.write(sendStr.encode('utf-8')) # change for Python3


#======================================

def recvFromArduino():
    global startMarker, endMarker
    
    ck = ""
    x = "z" # any value that is not an end- or startMarker
    byteCount = -1 # to allow for the fact that the last increment will be one too many
    
    # wait for the start character
    while  ord(x) != startMarker: 
        x = ser.read()
    
    # save data until the end marker is found
    while ord(x) != endMarker:
        if ord(x) != startMarker:
            ck = ck + x.decode("utf-8") # change for Python3
            byteCount += 1
        x = ser.read()
    
    return(ck)


#============================

def waitForArduino():

    # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded
    
    global startMarker, endMarker
    
    msg = ""
    while msg.find("Arduino is ready") == -1:

        while ser.inWaiting() == 0:
            pass
        
        msg = recvFromArduino()

        print (msg) # python3 requires parenthesis
        print ()
        

if __name__=="__main__":
    startMarker = 60
    endMarker = 62
    port="/dev/cu.usbmodem14201"
    ser = serial.Serial(port, 9600)
    waitForArduino()
    sendToArduino("<abcdefghijklmnop>")
    while ser.inWaiting() == 0:
        pass
    
    dataRecvd = recvFromArduino()
    print ("Reply Received  " + dataRecvd)
    ser.close()
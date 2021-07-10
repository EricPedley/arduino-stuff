#define SER 2
#define RCLK 3//latch
#define SRCLK 4//clock
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

const int buffSize = 512;
int bytesRecvd = 0;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
boolean readInProgress = false;
boolean endOfInput = false;

void setup() {
    pinMode(SER,OUTPUT);
    pinMode(RCLK,OUTPUT);
    pinMode(SRCLK,OUTPUT);

    digitalWrite(WRITE_EN,HIGH);
    pinMode(WRITE_EN,OUTPUT);
    Serial.begin(9600);
    delay(500);
    Serial.println("<Arduino is ready>");
    printEEPROMContents();
}

void loop() {
  if(Serial.available()>0) {
    char x = Serial.read();
    if (x == endMarker) {
      readInProgress = false;
      endOfInput = true;
      inputBuffer[bytesRecvd] = 0;
      writeProgram(inputBuffer);
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      // if (bytesRecvd == buffSize) {
      //   bytesRecvd = buffSize - 1;
      // }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
  if (endOfInput) {
    endOfInput = false;
    Serial.print("<pong>");
  }
}

void writeProgram(const char* program) {
  for(int i=0;i<buffSize&&program[i]!=0;i++) {
    writeEEPROM(i,program[i]);
  }
}

void setAddress(int address, bool outputEnable) {
  shiftOut(SER,SRCLK,MSBFIRST,(address>>8) | (outputEnable? 0:0x80));
  shiftOut(SER,SRCLK,MSBFIRST,address);
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);
  digitalWrite(RCLK,LOW);
}

void printEEPROMContents() {
  for(int i=0;i<255;i++) {
    char buf[80];
    sprintf(buf,"%02x",readEEPROM(i));
    Serial.print(buf);
    Serial.print(" ");
    if(i%16==0)
      Serial.println();
  }

}

byte readEEPROM(int address) {
  for(int i=EEPROM_D0;i<=EEPROM_D7;i++)
    pinMode(i,INPUT);
  setAddress(address,true);
  byte data=0;
  for(int pin=EEPROM_D7;pin>=EEPROM_D0;pin--) {
    data=(data<<1)+digitalRead(pin);
  }
  return data;
}

void writeEEPROM(int address, byte data) {
  for(int i=EEPROM_D0;i<=EEPROM_D7;i++)
    pinMode(i,OUTPUT);
  setAddress(address,false);
  for(int pin=EEPROM_D0;pin<=EEPROM_D7;pin++) {
    digitalWrite(pin, data&1);
    data = data>>1;
  }
  digitalWrite(WRITE_EN,LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN,HIGH);
  delay(5);
}

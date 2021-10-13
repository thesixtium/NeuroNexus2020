// SPI ARDUINO AND ESP COMMUNICATION
#include <SPI.h>

// EEG DATA READING
// Mux Control Pins
int s0 = 2;
int s1 = 3;
int s2 = 4;
int s3 = 5;
int r0 = 6;
int r1 = 7;
int r2 = 8;
int r3 = 9;

// Mux in "SIG" Pins
int sig1 = A1;
int sig2 = A2;

// Output
byte outputArray[1][21];

void setup() {
  Serial.begin(9600);
  Serial.println("Master");
  
  // SPI ARDUINO AND ESP COMMUNICATION
  digitalWrite(SS, HIGH); // disable Slave Select
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);//divide the clock by 4

  // EEG DATA READING
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 
  pinMode(r0, OUTPUT); 
  pinMode(r1, OUTPUT); 
  pinMode(r2, OUTPUT); 
  pinMode(r3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  digitalWrite(r0, LOW);
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  digitalWrite(r3, LOW);
}

void loop() {
  // EEG DATA READING
   for(int i = 0; i < 16; i ++){
    outputArray[1][i] = readMux(i, 1);
    delay(100); }
   
   for(int i = 0; i < 5; i ++){
    outputArray[1][i+16] = readMux(i, 2);
    delay(100); }
  
  for(int i = 0; i < 25; i++){
    Serial.print(outputArray[1][i]);
    Serial.print(", "); }
  Serial.println();
  delay(1000);


// SPI ARDUINO AND ESP COMMUNICATION
  byte b;

  // enable Slave Select
  digitalWrite(SS, LOW);
  delayMicroseconds (200);
  
  SPI.transfer (0xCD); //are you there
  delayMicroseconds(20); //give the slave time to process
  byte x =SPI.transfer (0); //get response
  delayMicroseconds(20); //give the slave time to process
  
  if (x == 0xEF){
    for ( byte i = 0 ; i < 3 ; i++){
      for (byte j = 0 ; j < 3 ; j++){
        b =(outputArray[i][j]);
        SPI.transfer (b);
        delayMicroseconds(20); //give the slave time to process 
      }    }    }
  digitalWrite(SS, HIGH); // disable Slave Select
  delay(100);
}

int readMux(int channel, int mux){
  int controlPin[4];
  
  if(mux==1){
    controlPin[0] = s0;
    controlPin[1] = s1;
    controlPin[2] = s2;
    controlPin[3] = s3;
  }
  
  if(mux == 2){
    controlPin[0] = r0;
    controlPin[1] = r1;
    controlPin[2] = r2;
    controlPin[3] = r3;
  }

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  int val = analogRead(sig1);
  if(mux == 2){
    val = analogRead(sig2);
  }
  return val;
}

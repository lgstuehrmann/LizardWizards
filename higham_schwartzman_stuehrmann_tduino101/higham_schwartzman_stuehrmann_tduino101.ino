/*
 * Prototype code for Arduino 101
 * to be used as controller for
 * bahaman anole habitat.
 * 
 * Terraduino Design Team c. 2017
 * #lizardwizards
 * 
 */

#include <Wire.h>

const int atomizer = 2; //define ports
const int pump = 10;
const int dayLamp = 7;
const int nightLamp = 8;

boolean isDaytime? = true;
int percentHumidity = 10;
int desiredHumidity = 80;



void setup() {
  // put your setup code here, to run once:
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  
  pinMode(atomizer, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(dayLamp, OUTPUT);
  pinMode(nightLamp, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  heatlamp();
  humidifier();

}

void atomizerToggle(){
  digitalWrite(atomizer, HIGH);
}

void heatlamp(){
  if(isDaytime){
    digitalWrite(dayLamp, HIGH);
    digitalWrite(nightLamp, LOW);
  }
  else{
    digitalWrite(dayLamp, LOW);
    digitalWrite(nightLamp, HIGH);
  }
}

void humidifier(){
  if(percentHumidity < desiredHumidity){
    digitalWrite(atomizer, HIGH);
  }
  else{
    digitalWrite(atomizer, LOW);
  }
  
}

// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


void reqStat() {  //request status loop, run this once at the beginning of the main loop to 
  Wire.requestFrom(8, 1);    // request 1 byte from slave device #8

  while (Wire.available()) { // slave may send less than requested
    int stat = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  if (stat > 0){
    Wire.requestFrom(8, 1);
    int rangeTop = Wire.Read();
    delay(500);
    Wire.requestFrom(8, 1);
    int rangeBottom = Wire.Read()
  }

  delay(500);
}



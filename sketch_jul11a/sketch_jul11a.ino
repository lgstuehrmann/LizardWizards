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

bool day = true;
bool parametersSet = false;
int state;

int desHumidity[2]; //Arrays for storing desired environmental parameter ranges
int desTemp[2];
int desSoil[2];


void setup() {
  // put your setup code here, to run once:
  pinMode(atomizer, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(dayLamp, OUTPUT);
  pinMode(nightLamp, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly

  //ping
  // if ping is below a certain threshold, continue, otherwise, delay and loop through ping

  //initial value setup, receive values for each min and max for each fixture in a single function, first indicating which sensor, then global arrays for each range

  if (!parametersSet){  //if parameters have not been set yet
    rangeSet(); //set the parameters
  }

  state = readStatus(); //refresh state variable

  switch(state){
    case 0: //all clear
    break;  //continue

    case 1:
    //turn on humidity module

    case 2:
    //turn off humidity module

    case 3:
    //set day mode

    case 4:
    //set night mode

    case 5:
    //turn on pump

    case 6:
    //turn off pump
    
  }

  //loop through request of status; if all clear, continue

  

  //switch for all clear and other statuses, toggle modules

}

int readStatus(){
  Wire.requestFrom(8, 2);
  int conditions Wire.read();

  return conditions;
}

void rangeSet(){  // write values from photon to global arrays
  Wire.requestFrom(8, 4);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    desHumidity[1] = Wire.read(); // receive a byte as character
    Serial.print(desHumidity[1]);         // print the character
  }
  delay(500); //wait half a second
  while (Wire.available()) { // slave may send less than requested
    desHumidity[0] = Wire.read(); // receive a byte as character
    Serial.print(desHumidity[0]);         // print the character
  }
  delay(1000);

  while (Wire.available()) { // slave may send less than requested
    desTemp[1] = Wire.read(); // receive a byte as character
    Serial.print(desTemp[1]);         // print the character
  }
  delay(500); //wait half a second
  while (Wire.available()) { // slave may send less than requested
    desTemp[1] = Wire.read(); // receive a byte as character
    Serial.print(desTemp[1]);         // print the character
  }
  delay(1000);

  while (Wire.available()) { // slave may send less than requested
    desSoil[1] = Wire.read(); // receive a byte as character
    Serial.print(desSoil[1]);         // print the character
  }
  delay(500); //wait half a second
  while (Wire.available()) { // slave may send less than requested
    desSoil[0] = Wire.read(); // receive a byte as character
    Serial.print(desSoil[0]);         // print the character
  }
  delay(1000);

  parametersSet = true;
  
}

void atomizerToggle(){
  digitalWrite(atomizer, HIGH);
}

void heatlamp(){
  if(day){
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


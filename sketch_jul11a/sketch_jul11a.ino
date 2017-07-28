#include <SparkFun_RHT03.h>
#include <Wire.h>

/*
 * Prototype code for Arduino 101
 * to be used as controller for
 * bahaman anole habitat.
 * 
 * Terraduino Design Team c. 2017
 * #lizardwizards
 * 
 */



#define RHTpin 4 //insert digital pin # here
#define MISTpin 5//insert digital pin # here

RHT03 rht; 

const int atomizer = 2; //define ports
const int pump = 10;
const int dayLamp = 7;
const int nightLamp = 8;

bool day = true;
bool parametersSet = false;
int state = 1;
char tempstate;

int desHumidity[2]; //Arrays for storing desired environmental parameter ranges
int desTemp[2];
int desSoil[2];


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  rht.begin(RHTpin);
  Serial.begin(9600);
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

  state = readStatus(); //refresh state variable
    Serial.print(state);

  switch(state){
    case 0: //set values
    rangeSet();
    break;  //continue

    case 1:
    //  all clear, continue
    //adjustHumidity(checkHum());
    
    

    case 2:
    //turn on humidity module
    digitalWrite(atomizer, HIGH);

    case 3:
    //turn off humidity module
    digitalWrite(atomizer, LOW);

    case 4:
    //set day mode
    digitalWrite(dayLamp, HIGH);
    digitalWrite(nightLamp, LOW);

    break;

    case 5:
    //set night mode
    digitalWrite(nightLamp, HIGH);
    digitalWrite(dayLamp, LOW);
    break;

    case 6:
    //turn on pump
    digitalWrite(pump, HIGH);

    case 7:
    //turn off pump
    digitalWrite(pump, LOW);
    

  //loop through request of status; if all clear, continue

  //switch for all clear and other statuses, toggle modules

  }

}

int readStatus(){
  Wire.requestFrom(8, 1);
  int conditions = Wire.read();
  delay(500);
  return conditions;
}

void sendCurrentValues(){
  int truncHumidity = checkHum();
  Wire.beginTransmission(8);
  Wire.write(truncHumidity);
  Wire.endTransmission();
  delay(500);

  

  
}

void rangeSet(){  // write values from photon to global arrays
  Wire.requestFrom(8, 4);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    desHumidity[1] = Wire.read(); // receive a byte as character
    Serial.print(desHumidity[1]);         // print the character
  }
  delay(500); //wait half a second

  Wire.requestFrom(8, 4);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    desHumidity[0] = Wire.read(); // receive a byte as character
    Serial.print(desHumidity[0]);         // print the character
  }
  delay(1000);

  Wire.requestFrom(8, 4);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    desTemp[1] = Wire.read(); // receive a byte as character
    Serial.print(desTemp[1]);         // print the character
  }
  delay(500); //wait half a second

  Wire.requestFrom(8, 4);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    desTemp[1] = Wire.read(); // receive a byte as character
    Serial.print(desTemp[1]);         // print the character
  }
  delay(1000);
 
  Wire.requestFrom(8, 4);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    desSoil[1] = Wire.read(); // receive a byte as character
    Serial.print(desSoil[1]);         // print the character
  }
  delay(500); //wait half a second

  Wire.requestFrom(8, 4);    // request 6 bytes from slave device #8
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

void adjustHumidity(float currHumidity){
  if(currHumidity < desHumidity[0] && currHumidity != 0){
    state = 2;
    delay(2000);
    state = 3;
  }
}

float checkHum(){
  int updateRet = rht.update();
  
  // If successful, the update() function will return 1.
  // If update fails, it will return a value <0
  if (updateRet == 1)
  {
    float latestHumidity = rht.humidity();
    return latestHumidity;
  } 
  else{ return 0;}
}

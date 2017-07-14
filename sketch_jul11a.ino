/*
 * Prototype code for Arduino 101
 * to be used as controller for
 * bahaman anole habitat.
 * 
 * Terraduino Design Team c. 2017
 * #lizardwizards
 * 
 */

const int atomizer = 2; //define ports
const int pump = 10;
const int dayLamp = 7;
const int nightLamp = 8;

boolean day = true;
int percentHumidity = 10;
int desiredHumidity = 80;

void setup() {
  // put your setup code here, to run once:
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


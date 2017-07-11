                       

#define RELAY1  7 //Pin for daytime light
#define RELAY2  8 //Pin for night light

void setup(){    
  Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);       
  pinMode(RELAY2, OUTPUT);
}

void loop(){
  String dayorno = checkLight();
}

//gets a light reading from the Grove light sensor and 
//returns a boolean saying whether it is dark or not
boolean readLight(void){
  int lightVal =  analogRead(A0);
  Serial.println(lightVal);
  if(lightVal < 400){//if it is dark, return true
    return true;
  }else return false;//if it is light, return false
}

//Turns lights on and off based on Grove light sensor readings
//Returns a string stating whether it's dark or light out for the system to use
String checkLight(void){
   boolean value;
   value = readLight(); //get whether it's light or dark out
   if(!value){//if it is light out, turn OFF night light and turn ON day light
     digitalWrite(RELAY1,0);    // Turns ON day light
     digitalWrite(RELAY2,1);    // Turn OFF night light
     return "Day";
   }else{ //if it is not light out, turn OFF day light and turn ON night light
    digitalWrite(RELAY1,1);   // turn OFF day light
    digitalWrite(RELAY2,0);   // turn ON night light
    return "Night";
   }
   
}


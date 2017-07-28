
// This #include statement was automatically added by the Particle IDE.
#include <SparkFunPhant.h>

// This #include statement was automatically added by the Particle IDE.
#include <SparkFunRHT03.h>

#include <Wire.h>
/////////////////////
// Pin Definitions //
/////////////////////
const int RHT03_DATA_PIN = D3; // RHT03 data pin
const int LIGHT_PIN = A0; // Photocell analog output
const int LED_PIN = D7; // LED to show when the sensor's are being read

///////////////////////////
// RHT03 Object Creation //
///////////////////////////
RHT03 rht; // This creates a RTH03 object, which we'll use to interact with the sensor

///////////////////////////////////////////////
// Phant (data.sparkfun.com) Keys and Server //
///////////////////////////////////////////////
// These keys are given to you when you create a new stream:

// const char server[] = "http://phant.saintgimp.org/"; // Phant destination server
// const char publicKey[] = "e1BlEby7bMC7lRqmrwX2tmrZnxd"; // Phant public key
// const char privateKey[] = "R3gXdDE4DRu0LWMdPvyaFY8jpPg"; // Phant private key

const char server[] = "data.sparkfun.com"; // Phant destination server
const char publicKey[] = "LQLwdljGJ2TqXgxvAlGv"; // Phant public key
const char privateKey[] = "A1x5rDzw47HWPqyog0lo"; // Phant private key
Phant phant(server, publicKey, privateKey); // Create a P



///////////////////////
// Post Rate Control //

// This #include statement was automatically added by the Particle IDE.


// This #include statement was automatically added by the Particle IDE.



///////////////////////
// data.sparkfun.com limits how often you can post to the service. You are allowed up
// to 100 posts every 15 minutes.
unsigned long lastPost = 0; // lastPost keeps track of the last UNIX time we posted
const unsigned int POST_RATE_S = 60; // This sets the post rate to 60 seconds. Avoid setting it below 10s.

//////////////////////////
// Station Name Globals //
//////////////////////////
String stationName = ""; // String object to keep track of our Photon's name


int globalStatus = 1;

int humidityRange[2];
int tempRange[2];
int soilMoistureRange[2];

int currentHumidity = 0;
int val = 0;//variable to store soil value
int soil = A2;//Declare a variable for the soil moisture sensor 
int soilPower = D6;//Variable for Soil moisture Power


void setup() 
{
    Serial.begin(9600); // Start the serial interface at 9600 bps
    
    rht.begin(RHT03_DATA_PIN); // Initialize the RHT03 sensor
    
    pinMode(LIGHT_PIN, INPUT); // Set the photocell pin as an INPUT.
    pinMode(LED_PIN, OUTPUT); // Set the LED pin as an OUTPUT
    pinMode(soilPower, OUTPUT);//Set D6 as an OUTPUT
    digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
    digitalWrite(LED_PIN, LOW); // Initially set the LED pin low -- turn the LED off.
    
    // getDeviceName() -- defined at the bottom of this code -- polls Particle's
    // server to get the name of the Photon running this application.
    getDeviceName(); // Update the stationName String
    
    getDeviceName(); // Update the stationName String
  Particle.function("mostHumid", &mostHumid);   //Particle function pairings
  Particle.function("leastHumid", &leastHumid);
  Particle.function("mostHot", &mostHot);
  Particle.function("leastHot", &leastHot);
  Particle.function("mostMoist", &mostMoist);
  Particle.function("leastMoist", &leastMoist);
  
  Particle.function("misterOn", &misterOn);   //Particle function pairings
  Particle.function("misterOff", &misterOff);
  Particle.function("dayMode", &dayMode);
  Particle.function("nightMode", &nightMode);
  Particle.function("pumpOn", &pumpOn);
  Particle.function("pumpOff", &pumpOff);
  Particle.function("triggerFeed", &triggerFeed);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(sendStatus); // register event
}

void loop() 
{
    delay(100);
    //dataStream();
}

void dataStream(){
    // This conditional should only run when the last successful post to Phant
    // was POST_RATE_S (60 seconds) or longer ago.
    // Time.now() returns the current UNIX timestamp (number of seconds since January 1, 1970).
    // It should increase by 1 every second. On a successful POST, we set lastPost equal to Time.now().
    if (lastPost + POST_RATE_S <= Time.now())
    {
        digitalWrite(LED_PIN, HIGH); // Turn the LED on to indicate we're posting
        int update = rht.update(); // Get new values from the RHT03.
        
        if (update == 1) // If the RHT03 update was successful
        {
            int postResult = 0; // This variable will keep track of whether or not
            
            //while (postResult <= 0)
            // Phant posts aren't always successful. Our postToPhant() function,
            // defined below, will return 1 if it was successful. Or a negative
            // number if it failed.
            while (postToPhant() <= 0)
            {
                Serial.println("Phant post failed. Trying again."); // Debug statement
                // Delay 1s, so we don't flood the server. Little delay's allow the Photon time
                // to communicate with the Cloud.
                for (int i=0; i<1000; i++)
                    delay(1);
            }
            // After a successful Phant POST:
            Serial.println("Phant post success!"); // Debug print
            // Set lastPost to current time, so we don't post for another POST_RATE_S seconds:
            lastPost = Time.now(); 
        }
        else // If the RHT03 update failed:
        {
            delay(RHT_READ_INTERVAL_MS); // Delay to give the sensor time to reset
        }
        digitalWrite(LED_PIN, LOW); // Turn the LED off to indicate we're done posting (/trying to post)
    }
}

void sendStatus() { // tell 101 what the status is
  Wire.write(globalStatus); // respond to the 101's periodic requests for status
  // as expected by master
}

int readSoil()
{
    digitalWrite(soilPower, HIGH);//turn D6 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soil);
    digitalWrite(soilPower, LOW);//turn D6 "Off"
    return val;
}

// postToPhant() gathers all of our sensor data, bundles it into a Phant post,
// and sends it out to data.sparkfun.com.
// It'll return either 1 on success, or a negative number if the post fails
int postToPhant(void)
{
    Serial.println("Posting to Phant!"); // Debug statement
    
    // Use phant.add(<field>, <value>) to add data to each field.
    // Phant requires you to update each and every field before posting,
    // make sure all fields defined in the stream are added here.
    phant.add("humidity", rht.humidity(), 1); // These first three phant adds set a field value to float variable
    phant.add("tempc", rht.tempC(), 1); // The third parameter -- valid for float variables -- sets the number
    phant.add("tempf", rht.tempF(), 1); // of decimal points after the number.
    phant.add("light", analogRead(LIGHT_PIN)); // We can also add an integer
    //phant.add("soil", readSoil()); // We can also add an integer
    phant.add("station", stationName); // phant.add(<field>, <string>) is perfectly valid too!
    
    // phant.particlePost() performs all of the Phant server connection and HTTP POSTING for you.
    // It'll either return a 1 on success or negative number on fail.
    // It uses the field/value combinations added previously to send Phant its data.
    // MAKE SURE YOU COMMIT A phant.add() FOR EVERY FIELD IN YOUR STREAM BEFORE POSTING!
    return phant.particlePost();
}

///////////////////////////////
// Get Device Name Functions //
///////////////////////////////
// These sets of functions poll Particle's server for the name of our Photon.
// This method is described in Particle's documentation here:
// https://docs.particle.io/reference/firmware/photon/#get-device-name
// Function handlers are used here -- when the Spark.subscribe function
// returns, it will call the nameHandler([topic], [data]) function with our 
// Photon's name.
bool validName = false; // Boolean to track if we have a valid name or not

// nameHandler() is a function handler. It's passed to Spark.subscribe() and
// called when that function returns.
// The [data] variable should have the name of our Photon when it's done.
void nameHandler(const char *topic, const char *data)
{
    stationName = String(data);  // Store the name in the stationName global variable
    validName = true; // Set validName to true, so getDeviceName can stop blocking.
}

// getDeviceName manages the spark subscribing and publishing required to get our
// Photon's name. It'll block for up to 30 seconds. On success it'll return a
// positive number. On fail, it'll return 0.
int getDeviceName(void)
{
    Spark.subscribe("spark/", nameHandler);
    Spark.publish("spark/device/name");
    
    int timeout = 30;
    while ((!validName) && (timeout > 0))
    {
        Serial.println("Waiting for name..." + String(timeout--));
        delay(1000); // Spark.process() is called during delay()
    }
    
    Serial.println("Station name = " + stationName);
    
    return timeout;
}

// assign range values

int mostHumid(String input){
    humidityRange[1] = input.toInt();
}

int leastHumid(String input){
    humidityRange[0] = input.toInt();
}

int mostHot(String input){
    tempRange[1] = input.toInt();
}

int leastHot(String input){
    tempRange[0] = input.toInt();
}

int mostMoist(String input){
    soilMoistureRange[1] = input.toInt();
}

int leastMoist(String input){
    soilMoistureRange[0] = input.toInt();
}

//set global status values

int misterOn(String input){
    globalStatus = 2;
}

int misterOff(String input){
    globalStatus = 3;
}

int dayMode(String input){
    globalStatus = 4;
}

int nightMode(String input){
    globalStatus = 5;
}

int pumpOn(String input){
    globalStatus = 6;
}

int pumpOff(String input){
    globalStatus = 7;
}

int triggerFeed(String input){
    globalStatus = 8;
}

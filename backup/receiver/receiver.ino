#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266TrueRandom.h>


#define COMBINED_MODE 0u
#define VISUAL_MODE 1u
#define AUDIO_MODE 2u

#define MAX_SENSORS 4


bool isRunning = false;

int attemptCounter = 0; // not initializing this stole 2h of my life

// parameters

uint8_t command;
uint32_t intervalFrom;
uint32_t intervalTo;
uint8_t mode;
uint32_t attemptNum = 1;
uint32_t sensorNum;
#include "comunicationMacros.h"

// address

struct MacAddress {
  uint8_t address[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};


MacAddress addresses[MAX_SENSORS] = {
  {0x08, 0x3A, 0x8D, 0xE3, 0x37, 0x88},
  {0x23, 0x1A, 0x85, 0x23, 0x67, 0x28},
  {0x08, 0x3B, 0x86, 0x33, 0x37, 0x85},
  {0x35, 0x21, 0x8D, 0x52, 0x56, 0x34},
};


bool sensorStates[MAX_SENSORS] = {false, false, false, false};

struct struct_receive {
  uint32_t reactionTime;
  bool wasJustToLetYouKnowIAmAlive = false;
};

struct struct_message {
  uint32_t stimulationMode;
  bool startMeasuring;
  bool stop = false;
  bool check = false;
};



struct_receive dataReceived;
struct_message dataToSend;
struct_message breakMessage { STIMULATION_MODE_VISUAL, false, true, false };
struct_message checkMessage { STIMULATION_MODE_VISUAL, false, false, true };



// start the measuring cycles
void startLoop(){
  isRunning = true;

  getActiveSensors();

  // used to stop the loop when the desidered value has been reached
  attemptCounter = 0;

  // run loop will run as long as isRunning is true
  Serial.println("Do you receive this message?"); // -------------------------------------------------------------------------------------------------DEBUG
  Serial.println(attemptNum); // -------------------------------------------------------------------------------------------------DEBUG
  Serial.print(sensorStates[0]); // -------------------------------------------------------------------------------------------------DEBUG
  Serial.print(sensorStates[1]); // -------------------------------------------------------------------------------------------------DEBUG
  Serial.print(sensorStates[2]); // -------------------------------------------------------------------------------------------------DEBUG
  Serial.print(sensorStates[3]); // -------------------------------------------------------------------------------------------------DEBUG
  Serial.println("here are sensor states"); // -------------------------------------------------------------------------------------------------DEBUG




  //runLoop();
}



// wait, measure and repeat as long as attemptCounter < attemptNum
void runLoop(){

  // check if all attempts are done
  Serial.println("who called me??");//---------------------------------------------------------------------------------------dEBUG
  Serial.println(attemptCounter);//---------------------------------------------------------------------------------------dEBUG
  if ( attemptCounter >= attemptNum ) {
    isRunning = false;
  }
  // then register the new attempt
  attemptCounter++;

  // stop if it should
  if ( !isRunning ) {

    // tell the ui the attempt is completed
    //Serial.write(FINISHED); //---------------------------------------------------------------------------------------dEBUG
    Serial.println("AAAAAAAAAAAAAAAAAAAA");//---------------------------------------------------------------------------------------dEBUG
    return;
  }
  
  // choose a random sensor that is also active
  uint8_t randomSensor = 0;
  int not_found = 0;
  bool found = false;
  while (!found){
    randomSensor = ESP8266TrueRandom.random(sensorNum);
    if (not_found > 100){
      // timeout searching sensors
      Serial.println("Can't find acdtive sensors");
      return;
    }
    if (sensorStates[randomSensor]){
      found = true;
    }
    if (not_found > 20){
      getActiveSensors();
    }
    not_found++;
    //delay(0);
  }
  

  // get the randomized interval to wait
  uint32_t randomDelay = ESP8266TrueRandom.random(intervalFrom, intervalTo + 1);
  // and wait
  delay(randomDelay);

  // prepare the message to tell the ui wich sensor has been choosen
  uint8_t* msg = getActiveSensorMessage(randomSensor);
  Serial.write(msg, ACTIVE_SENSOR_MESSAGE_LENGTH);
  // free the memory
  delete[] msg;

  // finally request the measurement from the choosen sensor
  esp_now_send(addresses[randomSensor].address, (uint8_t *) &dataToSend, sizeof(dataToSend));
}



// stop loop from ui
void stopLoop(){
  isRunning = false;

  for (int i = 0; i < MAX_SENSORS; i++){
      esp_now_send(addresses[i].address, (uint8_t *) &breakMessage, sizeof(breakMessage));
  }
}


void OnDataSent(uint8_t *mac, uint8_t sendStatus) { // check which sensors are active
  // if the comunication wasn't succesful restart the current measurement
  Serial.println("sendStatus");
  Serial.println(sendStatus);
  Serial.println("??????");
  if (!sendStatus){
    updateSensorStatus(mac, false);


    for(int i = 0; i < 6; i++){
      Serial.print(mac[i]);
      Serial.print(".");
    }
    //--------------------------------------------------------------------------------------------------------------------DEBUG 
    Serial.println("is not there");//--------------------------------------------------------------------------------------------------------------------DEBUG 
    // rerun loop after deactivating the sensor and correct the attempt counter
    if (attemptCounter >= 1){
      attemptCounter -= 1;
    }
    
    /*if (isRunning){
      runLoop(); //--------------------------------------------------------------------------------------------------------------------DEBUG (commented)
    }*/
    
  }
  
}




// the measurement has been received from the sensor
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {

  memcpy(&dataReceived, incomingData, sizeof(dataReceived));

  // check if the message was just to notify the sensor's state

  if (dataReceived.wasJustToLetYouKnowIAmAlive){
    updateSensorStatus(mac, true);
    return;
  }

  // get the message to tell to the ui the measurement
  uint8_t* msg = getReactionTimeMessage(dataReceived.reactionTime);
  // send it and free the memory
  Serial.write(msg, REACTION_TIME_MESSAGE_LENGTH);
  delete[] msg;

  // call runLoop to continue the measurements
  if (isRunning){
    runLoop();
  }

}


// ------------------------------------------------------------------------------------------------------------------------------ SETUP AND LOOP ------------------------------------------------------------------------

void setup() {

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);


  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  for (int i = 0; i < MAX_SENSORS; i++){
     esp_now_add_peer(addresses[i].address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  }

  // check what sensors are active
  getActiveSensors();

  dataToSend.startMeasuring = true;
  dataToSend.stimulationMode = COMBINED_MODE;

  while (Serial.available()){ Serial.read(); }
}



void loop() {


  if (Serial.available()){ // a message is received from serial gui
    
    uint8_t buffer[INSTRUCTION_MESSAGE_LENGTH] = {0};

    for (int i = 0; i < sizeof(buffer); i++){
      while(!Serial.available()) {
        delay(0);
      }

      buffer[i] = Serial.read();
    }

    // write all new received parameters to local variables
    readInstructionMessage(buffer, &command, &intervalFrom, &intervalTo, &mode, &attemptNum, &sensorNum);
    
    // start loop

    if (command == START_LOOP){
      // tell the computer the message has been received and executed
      
      Serial.print(COPY);
      delay(10);
      startLoop();
    }

    // stop loop

    else if (command == BREAK_LOOP){
      // tell the computer the message has been received and executed
      Serial.print(COPY);
      stopLoop();
    }
  }
}













































#include <ESP8266WiFi.h>
#include <espnow.h>
//#include <EEPROM.h>



#define COMBINED_MODE 0u
#define VISUAL_MODE 1u
#define AUDIO_MODE 2u

#define MAX_SENSORS 4

#define LED1 14
#define LED2 12
#define LED3 13
#define LED4 5

#define CHECK_INTERVAL 250 /* ms */


bool isRunning = false;

int attemptCounter;

// parameters

uint8_t command;
uint32_t intervalFrom;
uint32_t intervalTo;
uint8_t mode;
uint32_t attemptNum = 1;
uint32_t sensorNum;
int last_used_sensor = 0; // used to remove from active ones if the sensor is not responding

// timers
unsigned long check_aliveness_timer = 0; 

unsigned long dt;
unsigned long old_time;

#include "comunicationMacros.h"

// address

struct MacAddress {
  uint8_t address[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};


/*MacAddress addresses[MAX_SENSORS] = {
  {0x08, 0x3A, 0x8D, 0xE3, 0x37, 0x88},
  {0x23, 0x1A, 0x85, 0x23, 0x67, 0x28},
  {0x08, 0x3B, 0x86, 0x33, 0x37, 0x85},
  {0x35, 0x21, 0x8D, 0x52, 0x56, 0x34},
};*/
MacAddress addresses[MAX_SENSORS] = {
  {0x48, 0xE7, 0x29, 0x54, 0xC3, 0x8C},
  {0x08, 0x3A, 0x8D, 0xE3, 0x50, 0x2D},
  {0x08, 0x3A, 0x8D, 0xE3, 0x38, 0xA8},
  {0x08, 0x3A, 0x8D, 0xE3, 0x37, 0x88},
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

// predefined messages

struct_receive dataReceived;
struct_message dataToSend;
struct_message breakMessage { STIMULATION_MODE_VISUAL, false, true, false };
struct_message checkMessage { STIMULATION_MODE_VISUAL, false, false, true };

// random number generation

//uint32_t random_seed = 12;
//
//
//
//
//uint32_t nextrandom()
//{
//  random_seed = (random_seed * 1103515245 + 12345) % 0x7FFFFFFF;
//  random_seed <<= 7;
//  return random_seed;
//}

// start the measuring cycles
void startLoop(){
  isRunning = true;

  // used to stop the loop when the desidered value has been reached
  attemptCounter = 0;

  // run loop will run as long as isRunning is true
  runLoop();
}



// wait, measure and repeat as long as attemptCounter < attemptNum
void runLoop(){

  // check if all attempts are done
  if ( attemptCounter >= attemptNum ) {
    isRunning = false;
  }
  // then register the new attempt
  attemptCounter++;

  // stop if it should
  if ( !isRunning ) {

    // tell the ui the attempt is completed
    //Serial.print("what the urpidurpiduppi");
    Serial.write(FINISHED);

    //EEPROM.put(0, random_seed);  // save current random number for next time
    //EEPROM.commit();

    return;
  }
  
  // choose a random sensor that is also active
  uint8_t randomSensor = 0;

  //just get the first available for testing
  /*randomSensor = MAX_SENSORS % nextrandom();
  randomSensor = 0;

  while(!sensorStates[randomSensor]){
    randomSensor += 1;
    if (randomSensor >= MAX_SENSORS){
      //Serial.print("what the bekceckcks");
      Serial.write(FINISHED);
      
      return;
    }
  }*/


  bool found = false;
  int timeout = 70; // after this many trials finished is returned and leds are blinked 2 times
  while (!found){
    timeout -= 1;
    randomSensor =  random(0, MAX_SENSORS);//nextrandom() % MAX_SENSORS;

    if (sensorStates[randomSensor]){
      found = true;
    }

    // no sensors were found
    if (timeout <= 0){
      Serial.write(FINISHED);
      int blinking_warning_state = 0;
      for (int i = 0; i < 5; i++){
        digitalWrite(LED1, blinking_warning_state);
        digitalWrite(LED2, blinking_warning_state);
        digitalWrite(LED3, blinking_warning_state);
        digitalWrite(LED4, blinking_warning_state);

        delay(1000);

        blinking_warning_state = !blinking_warning_state;
      }
      return;
    }
    
  }

  last_used_sensor = randomSensor;
  

  // get the randomized interval to wait
  uint32_t randomDelay = random(intervalFrom, intervalTo + 1);//intervalFrom + nextrandom() % (intervalTo + 1 - intervalFrom);//ESP8266TrueRandom.random(intervalFrom, intervalTo + 1);
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
  if (!sendStatus){
    //updateSensorStatus(mac, false); // non funziona?
    //updateSensorStatus(mac, false); // set it as ded
    //Serial.print("not even posibly this be the cause here ");
    //printMac(mac);
    //Serial.println();
    // rerun loop after deactivating the sensor and correct the attempt counter
    
    //if (isRunning){
    //  attemptCounter -= 1;------------------------------------------------------------------------------------------------------------
    //  runLoop();
    //}
  }
  
}

void printMac(uint8_t *mac){
  for (int i = 0; i < 6; i++){
    Serial.print(mac[i], HEX);
    Serial.print("-");
  }
}




// the measurement has been received from the sensor
void OnDataRecv(uint8_t* mac, uint8_t* incomingData, uint8_t len) {

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

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // get random number seed
 // EEPROM.begin(sizeof(random_seed)); // Set aside some memory
 // EEPROM.get(0, random_seed); // Gets data from address 0

  // first update of old time to prevent strange values on dt
  old_time = millis();

  
}



void loop() {

  // update timers

  unsigned long new_time = millis();
  dt = new_time - old_time;
  old_time = new_time;

  // check if sensors are still active 4 times per second 
  if (!isRunning){ // prevent ever setting the sensor state to low while running
    check_aliveness_timer += dt;
    if (check_aliveness_timer >= CHECK_INTERVAL){
      // these are put first as they would all get turned off immediatly after getActiveSensors
      digitalWrite(LED1, sensorStates[0]);
      digitalWrite(LED2, sensorStates[1]);
      digitalWrite(LED3, sensorStates[2]);
      digitalWrite(LED4, sensorStates[3]);

      check_aliveness_timer -= CHECK_INTERVAL;
      // check if sensors are still alive
      getActiveSensors();
    }
  }




  

  if (Serial.available()){ // a message is received from serial gui
    
    uint8_t buffer[INSTRUCTION_MESSAGE_LENGTH] = {0};

    for (int i = 0; i < sizeof(buffer); i++){
      while(!Serial.available()) { }

      buffer[i] = Serial.read();
    }

    // write all new received parameters to local variables
    readInstructionMessage(buffer, &command, &intervalFrom, &intervalTo, &mode, &attemptNum, &sensorNum);

    dataToSend.stimulationMode = mode;
    
    // start loop

    if (command == START_LOOP){
      // tell the computer the message has been received and executed
      randomSeed(millis());
      Serial.print(COPY);
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











































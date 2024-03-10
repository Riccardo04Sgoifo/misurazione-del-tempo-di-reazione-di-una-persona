
#include "ESP8266WiFi.h"
#include "espnow.h"

#define COMBINED_MODE 0u
#define VISUAL_MODE 1u
#define AUDIO_MODE 2u


#define IR_SENSOR_PIN 13

#define LED_R 4
#define LED_G 14
#define LED_B 12

#define BUZZ 5


uint8_t receiverAddress[] = {0x48, 0xE7, 0x29, 0x54, 0xBD, 0xB9};

struct struct_message {
  unsigned int reactionTime;
  bool wasJustToLetYouKnowIAmAlive = false;
};

struct struct_receive {
  unsigned int stimulationMode;
  bool startMeasuring;
  bool stop = false;
  bool check = true;
};



// response containing the measurement
struct_message dataToSend;
// just to let know the sensor is alive
struct_message aliveMessage { 0, true };
// received instructions
struct_receive dataReceived;

unsigned long startTime;

bool isMeasuring = false;

unsigned int stimulationMode = COMBINED_MODE;

unsigned long buzzerOldTime = 0;
unsigned long buzzerTime = 500; // us
unsigned long buzzerState = true;


// -------------------------------------------------------------------------------------------------------- SETUP & LOOP ----------------------------------------------------------------------------------------------------------------------------------

void setup() {

  WiFi.mode(WIFI_STA);

  Serial.begin(115200);

  if (esp_now_init() != 0){
    Serial.println("Error on inizialization of espnow");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);


  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  // for some reason the ir sensor is high when nothing is detected so FALLING is choosen
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_PIN), sensorTriggeredCallback, FALLING);

  delay(20);

  //notify the master this sensor is now alive
  esp_now_send(receiverAddress, (uint8_t * ) &aliveMessage, sizeof(aliveMessage));

  buzzerOldTime = micros();
}



void loop() {

  // activate the appropriate stimulation to prompt the subject to swiftly accelerate their upper limb towards the correct sensor


  if (isMeasuring){
    if ((stimulationMode == COMBINED_MODE) || (stimulationMode == VISUAL_MODE)) {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, HIGH); // test
    }

    if ((stimulationMode == COMBINED_MODE) || (stimulationMode == AUDIO_MODE)) {
      // write HIGH or LOW depending on the time
      digitalWrite(BUZZ, buzzerState);
    }
    

    // turn on and off the piezo at the right frequency
    unsigned long newTime = micros();
    unsigned long dt = newTime - buzzerOldTime;
    // set the buzzer state
    if ( dt > 500 ) {
      buzzerOldTime = newTime;
      buzzerState = !buzzerState;
    }

  }

  // if no measurement is needed turn off all the stimulations
  else{
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    digitalWrite(BUZZ, LOW);
  }
}





















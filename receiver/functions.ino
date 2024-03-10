
void readInstructionMessage(uint8_t buffer[INSTRUCTION_MESSAGE_LENGTH], uint8_t* command, uint32_t* intervalFrom, uint32_t* intervalTo, uint8_t* mode, uint32_t* attemptNum, uint32_t* sensorNum){
// read from buffer

  *command = buffer[0];

  *intervalFrom = buffer[1];
  *intervalFrom <<= 8;
  *intervalFrom |= buffer[2];
  *intervalFrom <<= 8;
  *intervalFrom |= buffer[3];
  *intervalFrom <<= 8;
  *intervalFrom |= buffer[4];

  *intervalTo = buffer[5];
  *intervalTo <<= 8;
  *intervalTo |= buffer[6];
  *intervalTo <<= 8;
  *intervalTo |= buffer[7];
  *intervalTo <<= 8;
  *intervalTo |= buffer[8];

  *mode = buffer[9];

  *attemptNum = buffer[10];

  *sensorNum = buffer[11];
}


uint8_t* getReactionTimeMessage(uint32_t reactionTime){
  
  uint8_t* msg = new uint8_t[REACTION_TIME_MESSAGE_LENGTH] { 0 };

  msg[0] = REACTION_TIME_PRINT;
  msg[4] |=  reactionTime & 0x000000ff;
  msg[3] |= (reactionTime & 0x0000ff00) >> 8;
  msg[2] |= (reactionTime & 0x00ff0000) >> 16;
  msg[1] |= (reactionTime & 0xff000000) >> 24;

  return msg;

}



void getActiveSensors(){

  for (int i = 0; i < MAX_SENSORS; i++){
    sensorStates[i] = false;
    // if a response is not received, the state will stay "offline"
    esp_now_send(addresses[i].address, (uint8_t * ) &checkMessage, sizeof(checkMessage));
    
  }

  // sensors should respond in less than 1ms so 20ms is way more than enough time
  // this delay also avoids led flickering
  delay(20);

  //for (int i = 0; i < MAX_SENSORS; i++){
  //  Serial.print("sensor ");
  //  Serial.print(i);
  //  Serial.print(" is ");
  //  Serial.println(sensorStates[i] ? "online" : "offline");
  //}
  
}



uint8_t* getActiveSensorMessage(uint8_t activeSensor){

  uint8_t* msg = new uint8_t[ACTIVE_SENSOR_MESSAGE_LENGTH] { 0 };

  msg[0] = ACTIVE_SENSOR;
  msg[1] = activeSensor;

  return msg;
}


// search for the correct index and update the state
void updateSensorStatus(uint8_t* mac_address, bool status){

  for (int i = 0; i < MAX_SENSORS; i++){
    bool same = true;

    for (int j = 0; j < 6; j++){
      if (addresses[i].address[j] != mac_address[j]){
        same = false;
        //Serial.print(addresses[i].address[j], HEX);
        //Serial.print(" is not the same as ");
        //Serial.println(mac_address[j], HEX);

      }
    }
      
    if (same){
      sensorStates[i] = status;
    }
  }

}
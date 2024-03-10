ICACHE_RAM_ATTR void sensorTriggeredCallback(){ // the ir sensor shouldn't give any fake readings unlesess there is sun

  // if it's not measuring then ignore the sensor
  if ( !isMeasuring ) {
    return;
  }

  // calculate how slow the tested human was
  unsigned long endTime = millis();
  unsigned int reactionTime = endTime - startTime;

  // write the measurement to the message buffer?
  dataToSend.reactionTime = reactionTime;
  // send it to the main board
  esp_now_send(receiverAddress, (uint8_t *) &dataToSend, sizeof(dataToSend));

  // disable measuring mode
  isMeasuring = false; 
  
}



void OnDataSent(uint8_t *mac_address, uint8_t sendStatus) { // need to remove prints
  // if the message has not been received idk
}




// this sensor has been choosen and should start measuring or should stop deafening everyone with the buzzer
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {

  // store the received instructions
  memcpy(&dataReceived, incomingData, sizeof(dataReceived));

  // stop the current measurement
  if (dataReceived.stop){
    isMeasuring = false;
    // digital write low on all pins to stop the screaming piezo if it's running
    return;
  }

  // start the measurement
  else if (dataReceived.startMeasuring){
    isMeasuring = true; // could add check to give error if hand is already over the sensor
    
    // take the first time
    startTime = millis();
  }
  

  // just tell the main board the sensor is currently active 
  else if (dataReceived.check){
    delay(20);
      esp_now_send(receiverAddress, (uint8_t * ) &aliveMessage, sizeof(aliveMessage));
  }
  stimulationMode = dataReceived.stimulationMode;
}
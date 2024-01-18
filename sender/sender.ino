
// -------------------------------------------------------------------------------------------------------- SETUP & LOOP ----------------------------------------------------------------------------------------------------------------------------------


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






















#ifndef MACROS_H
#define MACROS_H

#define START_LOOP 's'
#define REACTION_TIME_PRINT 'r'
#define FINISHED 'f'
#define COPY 'k'
#define END_PRINT 'e'
#define BREAK_LOOP 'b'
#define STIMULATION_MODE_BOTH 'b'
#define STIMULATION_MODE_VISUAL 'v'
#define STIMULATION_MODE_AUDITORY 'a'
//for comunication with esp
#define COMBINED_MODE 0u  /* both audio and visual */
#define VISUAL_MODE 1u    /* use LEDs */
#define AUDIO_MODE 2u     /* use buzzer */

#define ACTIVE_SENSOR 'l'

#define REACTION_TIME_MESSAGE_LENGTH 5
#define INSTRUCTION_MESSAGE_LENGTH 12
#define ACTIVE_SENSOR_MESSAGE_LENGTH 2

#endif // MACROS_H

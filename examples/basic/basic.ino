#include <midiRead.h>

bool velocidad=true;
void onMidiMsg(MidiMessageTypes type, int channel, int data1, int data2)
{   byte instr,speed;
  switch(type){
    case NoteOn: Serial.println("Event NoteOn Channel: " + String(channel) + " Note: " + String(data1) + " Velocity: " + String(data2));  break;
    case NoteOff: Serial.println("Event NoteOff Channel: " + String(channel) + " Note: " + String(data1) + " Velocity: " + String(data2));   break;
    case ProgramChange: Serial.println("Event Program Change program number:" + String(data1)); break;
    case ControlChange: Serial.println("Event Control Change controller number:" + String(data1) +  " Control Value: " + String(data2));  break;
    case PitchBend:  Serial.println("Event PitchBend Pitch Value:" + String(data2));  break;
  }
}
MidiRead midi(Serial1,115200);

void setup() {
  Serial.begin(115200);
  midi.handleMidi(onMidiMsg);
}

void loop() {
       midi.read();            // wait for a second
}

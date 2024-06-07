#ifndef MIDIREAD_H
#define MIDIREAD_H

// Define enum for message types
enum MidiMessageTypes {
  Invalid = 0x00,                 // zero as invalid message
  NoteOff = 0x80,                 // Data byte 1: note number, Data byte 2: velocity
  NoteOn = 0x90,                  // Data byte 1: note number, Data byte 2: velocity
  PolyphonicAftertouch = 0xA0,    // Data byte 1: note number, Data byte 2: pressure
  ControlChange = 0xB0,           // Data byte 1: controller number, Data byte 2: controller value
  ProgramChange = 0xC0,           // Data byte 1: program number
  ChannelAftertouch = 0xD0,       // Data byte 1: pressure
  PitchBend = 0xE0,               // Data byte 1: LSB, Data byte 2: MSB (14-bit value)
  SystemExclusive = 0xF0,         // Data bytes: variable length (sysex message)
  TimeCodeQuarterFrame = 0xF1,    // Data byte 1: time code value
  SongPositionPointer = 0xF2,     // Data byte 1: LSB, Data byte 2: MSB (14-bit value)
  SongSelect = 0xF3,              // Data byte 1: song number
  TuneRequest = 0xF6,             // No data bytes
  EndOfExclusive = 0xF7,          // No data bytes (end of sysex message)
  TimingClock = 0xF8,             // No data bytes
  Start = 0xFA,                   // No data bytes
  Continue = 0xFB,                // No data bytes
  Stop = 0xFC,                    // No data bytes
  ActiveSensing = 0xFE,           // No data bytes
  Reset = 0xFF                    // No data bytes
};

class MidiRead {
  private:
    // Callback function to notify the caller about received midi messages
    void (*onMsgMidi)(MidiMessageTypes, int, int, int);
    // Buffer for incoming midi messages
    int midiBuffer[3];
    uint8_t bufferIndex = 0;
    HardwareSerial *mSerial;
  public:
    bool running = true; //is false after midi stop message, and back true after start/continue
   MidiRead(HardwareSerial &inSerial,int baudios=31250) 
   {
    mSerial=&inSerial;
    mSerial->begin(baudios);
   };
  void handleMidi(void (*newMidiCallback)(MidiMessageTypes, int, int, int)) {
      onMsgMidi = newMidiCallback;
     }
    // Method to be called in the main loop
    void read() { 
      while (mSerial->available() > 0) {
        uint8_t incomingByte = mSerial->read();
        // Serial.print(incomingByte, HEX);
        // Serial.print("\n");
        if(incomingByte & 0x80){
          //is command byte, MSB == 1
          midiBuffer[0] = incomingByte;
          bufferIndex = 1;
        } else {
          // databyte, MSB == 0
          if(bufferIndex > 0){
            midiBuffer[bufferIndex] = incomingByte;
            bufferIndex++;
          } else {
            //error, databyte recieved without command byte
          }
        }
        int val = midiBuffer[0];
        MidiMessageTypes type;
        uint8_t channel = 0;
        //check if message complete
        if(midiBuffer[0] >= 0xF0){
          //is System Message 0xF0..0xFF
          type = (MidiMessageTypes)midiBuffer[0];
        } else{
          //is channel message
          type = (MidiMessageTypes)(midiBuffer[0] & 0xF0);
          channel = midiBuffer[0] & 0x0F;
        }
        switch(type){
          case Invalid:
            //Invalid Message
          break;
          case NoteOn:
          case NoteOff:
          case PolyphonicAftertouch:
          case ControlChange:
          case PitchBend:
          case SongPositionPointer:
          case SystemExclusive:
            //messages that require 2 data bytes
            if(bufferIndex == 3){
              //enough data for NoteOn Message available
              onMsgMidi(type, channel, midiBuffer[1], midiBuffer[2]); 
              bufferIndex = 0;             
            }
          break;
          case ProgramChange:
          case ChannelAftertouch:
          case SongSelect:
          case TimeCodeQuarterFrame:
            //messages that require 1 data byte
            if(bufferIndex == 2){
              //enough data for NoteOn Message available
              onMsgMidi(type, channel, midiBuffer[1], 0);  
              bufferIndex = 0;            
            }
          break;
          case ActiveSensing: break;
          case Reset:
          case Stop:
          case Start:
          case Continue:
          case TimingClock:
          case EndOfExclusive:
          case TuneRequest:
            if(bufferIndex == 1){
              //messages that require no data bytes
              onMsgMidi(type, 0, 0, 0);
              bufferIndex = 0;
              if(type == Start || type == Continue){
                running = true;
              } else if (type == Stop){
                running = false;
              }              
            }
          break;
          default:                break;
          
        }
      }
    }
};
#endif // MIDIREAD_H

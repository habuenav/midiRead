# midiRead

`midiRead` es una librería para Arduino que facilita la lectura de mensajes MIDI. <br>
Esta librería permite interpretar y manejar distintos tipos de mensajes MIDI de manera sencilla.

## Características

* Soporte para los tipos de mensajes MIDI más comunes, incluyendo Note On, Note Off, Control Change, Program Change, entre otros.
* Fácil integración con proyectos basados en la placa ESP32.
* Callback para manejar mensajes MIDI entrantes.

## Instalación

1. Descarga la última versión del repositorio o clona el repositorio:
    ```bash
    git clone https://github.com/habuenav/midiRead.git
    ```
2. Copia la carpeta `midiRead` en el directorio `libraries` de tu instalación de Arduino.

## Uso

### Ejemplo Básico

```
#include <midiRead.h>

void handleMidiMessage(MidiMessageTypes type, int channel, int data1, int data2) {
  // Manejar mensajes MIDI aquí
  Serial.print("MIDI Message: ");
  Serial.print(type);
  Serial.print(", Channel: ");
  Serial.print(channel);
  Serial.print(", Data1: ");
  Serial.print(data1);
  Serial.print(", Data2: ");
  Serial.println(data2);
}

MidiReader midi(Serial1);

void setup() {
  Serial.begin(115200);
  midi.handleMidi(handleMidiMessage);
}

void loop() {
  midi.read();
}
```


## Documentación
### Métodos de MidiReader
* <b> MidiReader(HardwareSerial &inSerial, int baudios = 31250):</b> Constructor que inicializa la comunicación MIDI.
* <b> void handleMidi(void (*newMidiCallback)(MidiMessageTypes, int, int, int)):</b> Registra el callback para manejar mensajes MIDI.
* <b> void read():</b> Lee y procesa mensajes MIDI desde el buffer serial.

## Contribuciones
¡Las contribuciones son bienvenidas! Si encuentras algún problema o tienes alguna sugerencia, por favor abre un issue o envía un pull request.

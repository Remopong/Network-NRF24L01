/*

  Radio    Arduino
  CE    -> 10
  CSN   -> 6 (Hardware SPI SS)
  MOSI  -> 11 (Hardware SPI MOSI)
  MISO  -> 12 (Hardware SPI MISO)
  SCK   -> 13 (Hardware SPI SCK)
  IRQ   -> No connection
  VCC   -> No more than 3.6 volts
  GND   -> GND

*/

#include <SPI.h>
#include <NRFLite.h>
const static uint8_t RADIO_ID = 5;
int DESTINATION_RADIO_ID;
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

int table[10];
int buff[10];

byte list[99];

struct sender {
  uint8_t from = RADIO_ID;
  uint8_t to;
  int number;
  String command;
};

struct receiver {
  uint8_t from;
  uint8_t to;
  int number;
  String command;
};

NRFLite _radio;
sender toSend;
receiver toReceive;

void setup()
{
  Serial.begin(9600);

  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("radio error");
    while (1);
  }
  
  in_range_detector();

  delay(1000);

}

void loop()
{
  _radio.startRx();
  if (_radio.hasData()) {
    _radio.readData(&toReceive);
    if(toReceive.command = "/add"){
      addMore(toReceive.number);
    }
  }

  _radio.startSend(toSend.to, &toSend, sizeof(toSend));
  _radio.send(toSend.to, &toSend, sizeof(toSend));
    
  delay(10);

}

void in_range_detector() {
  Serial.println("Scanning...");
  for(int detectorInt = 0; detectorInt <= (sizeof(table) / sizeof(table[0])); detectorInt++){
    if(detectorInt != RADIO_ID){
      if(_radio.send(detectorInt, &toSend, sizeof(toSend))){
        list[detectorInt] = 1;
      }else{
        list[detectorInt] = 0;
      }
    }
  }
}

void addMore(int variator){
  int buff[variator];
    for (int a = 0; a <= variator; a++) {
      buff[a] = table[a];
    }
    variator += toReceive.number;
    int table[variator];
    for (int z = 0; z <= sizeof(buff) / sizeof(buff[0]); z++) {
      table[z] = buff[z];
    }
    for (int j = sizeof(buff) / sizeof(buff[0]); j <= variator; j++) {
      table[j] =  j;
    }
  for (int i = 0; i <= (sizeof(table) / sizeof(table[0])); i++) {
    Serial.print(table[i]);
    Serial.print("  ");
  }
  Serial.println("Finished");
}

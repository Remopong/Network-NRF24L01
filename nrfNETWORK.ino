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

String echo = "echo";

struct sender {
  uint8_t from = RADIO_ID;
  uint8_t to;
};

struct receiver {
  uint8_t from;
  uint8_t to;
};

NRFLite _radio;
sender _Sender;
receiver _Receiver;

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
    _radio.readData(&_Receiver);
  }
  
  _radio.startSend(_Sender.to, &_Sender, sizeof(_Sender));
  
  if (_radio.send(_Sender.to, &_Sender, sizeof(_Sender))) {
    //Serial.println("SEND echo result");
  }
  delay(10);
}

void in_range_detector() {
  Serial.println("Scanning...");
}

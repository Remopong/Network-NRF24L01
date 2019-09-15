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
const static uint8_t RADIO_ID = 2;             // Our radio's id.
int DESTINATION_RADIO_ID; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

String moduleAroundID[11];

int len = 1;

struct sender {
  int FROM_send;
  int DESTINATION_send;
  String msg_send;
};

struct tester {
  int test;
};

struct receiver {
  int FROM_receive;
  int DESTINATION_receive;
  String msg_receive;
};
NRFLite _radio;
sender _Sender;
receiver _Receiver;
tester _Tester;


void setup()
{
  Serial.begin(9600);
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1);
  }
  in_range_detector();

  for (int i = 0; i <= len; i++) {
    if (moduleAroundID[i] != NULL) {
      if (_radio.send(int(moduleAroundID[i].toInt()), &_Sender, sizeof(_Sender))) {
        Serial.println("SENT to " + moduleAroundID[i]);
      }
    }
  }
}

void loop()
{
  
//  delay(5000);
//
//  if (_radio.hasData()) {
//    Serial.println("Receive");
//    _radio.readData(&_Receiver);
//    String msg = "";
//    msg += _Receiver.msg_receive;
//    Serial.println(msg);
//  }

}

void in_range_detector() {
  Serial.println("Scanning...");
  len = 1;
  for (int testRadio = 1; testRadio <= 10; testRadio++) {
    if (testRadio != RADIO_ID) {
      if (_radio.send(testRadio, &_Tester, sizeof(_Tester))) // Note how '&' must be placed in front of the variable name.
      {
        moduleAroundID[len] = String(testRadio);
        Serial.println(String(testRadio) + " is in range");
        len++;

      } else {
        Serial.println(String(testRadio) + " is not in range");
      }
    }
  }
}

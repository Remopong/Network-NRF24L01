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
const static uint8_t RADIO_ID = 7;             // Our radio's id.
int DESTINATION_RADIO_ID; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

String modulesAroundID[11];
String TAMPONmodulesAroundID[11];
String allModulesID[100];

int len = 1;
int lenALL = 1;
struct sender {
  int FROM_send = RADIO_ID;
  int DESTINATION_send;
  String msg_send;
};

struct receiver {
  int FROM_receive;
  int DESTINATION_receive;
  String msg_receive;
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

  for (int i = 0; i <= len; i++) {
    if (modulesAroundID[i] != NULL) {
      if (_radio.send(int(modulesAroundID[i].toInt()), &_Sender, sizeof(_Sender))) {
        Serial.println("SENT to " + modulesAroundID[i]);
      }
    }
  }
}

void loop()
{

  if (_radio.hasData()) {
    _radio.readData(&_Receiver);
    String msg = _Receiver.msg_receive;
    if (msg == "echo") {
      for (int i = 1; allModulesID[i] != NULL; i++) {
        for (int j = 1; j <= 10; j++) {
          if (allModulesID[i] == modulesAroundID[j]) {
            TAMPONmodulesAroundID[j] = "";
          }
        }
        lenALL++;
      }
      for (int i = 1; i <= lenALL; i++) {
        if (TAMPONmodulesAroundID[i] != "") {
          allModulesID[lenALL + i] = TAMPONmodulesAroundID[i];
        }
      }
    }
  }
}

void in_range_detector() {
  Serial.println("Scanning...");
  len = 1;
  for (int testRadio = 1; testRadio <= 10; testRadio++) {
    if (testRadio != RADIO_ID) {

      _Sender.msg_send = "echo";

      if (_radio.send(testRadio, &_Sender, sizeof(_Sender))) // Note how '&' must be placed in front of the variable name.
      {
        modulesAroundID[len] = String(testRadio);
        Serial.println(String(testRadio) + " in range");
        len++;

      } else {
        Serial.println(String(testRadio) + " not in range");
      }
    }
  }
}

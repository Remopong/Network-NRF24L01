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
const static uint8_t RADIO_ID = 5;             // Our radio's id.
int DESTINATION_RADIO_ID; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

int modulesAroundID[11];
int TAMPONmodulesAroundID[21];
int allModulesID[21];

int len = 1;
int lenALL = 1;

struct sender {
  int from = RADIO_ID;
  int to;
  String msg;
  int echoReturned[21];
};

struct receiver {
  int from;
  int to;
  String msg;
  int echoReturned[21];
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

  _Sender.to = 0;
  _Sender.msg = "echo";

  
  for (int i = 0; i <= len; i++) {
    if (modulesAroundID[i] != NULL) {
      if (_radio.send((modulesAroundID[i]), &_Sender, sizeof(_Sender))) {
        Serial.println("SENT to " + String(modulesAroundID[i]) + ", msg == echo");
      }
    }
  }
}

void loop()
{
  if (_radio.hasData()) {
    Serial.println("hasData");
    _radio.readData(&_Receiver);
    String msg = String(_Receiver.msg);
    Serial.println("Message is " + msg);
    if (msg == "echo") {
      Serial.println("**");
      for (int i = 1; allModulesID[i] != NULL; i++) {
        for (int j = 1; j <= 10; j++) {
          if (String(allModulesID[i]) == modulesAroundID[j]) {
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
    _Sender.to = _Receiver.from;
    for (int k = 1; k <= sizeof(allModulesID); k++) {
      _Sender.echoReturned[k] = allModulesID[k];
    }

    if (_radio.send(_Sender.to, &_Sender, sizeof(_Sender))) {
      Serial.println("SEND echo result");
    }
  }
  delay(10);
}

void in_range_detector() {
  Serial.println("Scanning...");
  len = 1;
  for (int testRadio = 1; testRadio <= 10; testRadio++) {
    if (testRadio != RADIO_ID) {
      if (_radio.send(testRadio, &_Sender, sizeof(_Sender))) // Note how '&' must be placed in front of the variable name.
      {
        modulesAroundID[len] = testRadio;
        Serial.println(String(testRadio) + " in range");
        len++;

      } else {
        Serial.println(String(testRadio) + " not in range");
      }
    }
  }
}

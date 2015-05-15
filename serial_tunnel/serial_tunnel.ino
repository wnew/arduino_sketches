#include <SoftwareSerial.h>

const int sw_rx_pin = 4;
const int sw_tx_pin = 5;

const int pc_baud = 57600;
const int device_baud = 9600;


SoftwareSerial mySerial(sw_rx_pin, sw_tx_pin); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(pc_baud);

  Serial.println("Starting serial tunnel....");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(device_baud);
}

void loop() {// run over and over
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}

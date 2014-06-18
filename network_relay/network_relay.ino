//===========================================================================//
//===========================================================================//
//                                                                           //
//    Sketch: Arduino Networked Relays                                       //
//    Created: June 2014                                                     //
//    Updated: June 2014                                                     //
//    Description: This sketchs allows control of relays via a webpage       //
//                 hosted on an arduino                                      //
//                                                                           //
//===========================================================================//
//===========================================================================//


//============
// #includes
//============
#include "etherShield.h"
#include "ETHER_28J60.h"

//=================
// Global ojbects
//=================
int i, rsize, relay[] = {8, 7, 6, 5, 3, 2, 1, 0};

static uint8_t mac[6] = {0x10, 0x10, 0x10, 0x02, 0x00, 0x00};
static uint8_t ip[4] = {10, 10, 10, 200};
static uint16_t port = 80;
unsigned long runtime, starttime;

ETHER_28J60 ether;


//================
// Initial setup
//================
void setup() {
  pinMode(4, OUTPUT);    // REQUIRED for eKitsZone.com ENC28J60 shield!!!
  digitalWrite(4, HIGH); // Same as above.

//  Serial.begin(57600);
  ether.setup(mac, ip, port);
  rsize = sizeof(relay) / 2;
  for (i = 0; i < rsize; i = i + 1) {
    pinMode(relay[i], OUTPUT);
  }
  runtime = 0;
  starttime = millis();
}


//============
// Main loop
//============
void loop() {
  int c;
  char * param;
  char * params;

  if (params = ether.serviceRequest()) {
    if (strstr(params, "?status")) {
      for (i = 0; i < rsize; i = i + 1) {
        ether.print(digitalRead(relay[i]));
      }
    } else {
      if((strlen(params) > 3) && (!(strstr(params, "favicon.ico")))) {
        runtime = 0;
        param = strtok(params, "?&");
        while (param != NULL) {
          if (strstr(param, "p=")) {
            i = atoi(param + 2) - 1;
          } else if (strstr(param, "c=")) {
            c = atoi(param + 2);
            if (c == 1) {
              for (c = 0; c < rsize; c = c + 1) { digitalWrite(relay[c], LOW); }
              digitalWrite(relay[i], HIGH);
            } else {
              for (c = 0; c < rsize; c = c + 1) { digitalWrite(relay[c], LOW); }
            }
          } else if (strstr(param, "t=")) {
            runtime = (unsigned long)(atoi(param + 2)) * 1000;
            if (runtime < 15000) { runtime = 15000; }
            starttime = millis();
          }
          param = strtok(NULL, "& ");
        }
        if (runtime == 0) {
          runtime = 60000;  // 60 sec. default run time (takes 10 seconds for valve to open).
          starttime = millis();
        }
      }

      ether.print("<tt>Network Relay System\n<br>\n");
      for (i = 0; i < rsize; i = i + 1) {
        ether.print("<br>Port #");
        ether.print(i+1);
        ether.print(": <a href='?p=");
        ether.print(i+1);
        if (digitalRead(relay[i]) == 1) {
          ether.print("&c=0'>Off</a> in ");
          ether.print((runtime - (millis() - starttime))/1000);
          ether.print(" seconds.\n");
        }
        else {
          ether.print("&c=1'>On</a>\n");
        }
      }
      ether.print("<p><a href='/'>Refresh</a>,<a href='/?status'>Status</a>\n");
    }
    ether.respond();
  }

  if ((unsigned long)(millis() - starttime) > runtime) {
    for (c = 0; c < rsize; c = c + 1) { digitalWrite(relay[c], LOW); }
  }
}


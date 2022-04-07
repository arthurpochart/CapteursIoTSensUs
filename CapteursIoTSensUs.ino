//---------Capteur CO2------------//
#include "MQ135.h"

#include <string>

const int mq135Pin = 0; // Pin sur lequel est branch├® de MQ135

MQ135 gasSensor = MQ135(mq135Pin); // Initialise l'objet MQ135 sur le Pin sp├®cifi├®
void motd();
//----Buetooth-----------//
int incomingByte = 0;

int alagogPin = 18;
int potVal = 0;
char dta[20];
int n;
String TRA, OBJ, REQ, TYP, NUM, VAL, TIM, CHK;

//Variables de trame//

// TRA  OBJ   REQ TYP  NUM   VAL   TIM    CHK

void setup() {
  Serial.begin(9600); // Initialise le port s├®rie ├á 9600 bps
  Serial1.begin(9600);

  //---------Capteur CO2------------//
  float rzero = gasSensor.getRZero();
  Serial.print("R0: ");
  Serial.println(rzero); // Valeur ├á reporter ligne 27 du fichier mq135.h apr├¿s 48h de pr├®chauffage
  motd();

}
///---Fonctions-------//
void capteurco2() {
  //-------Capteur CO2--------------//

  float ppm = gasSensor.getPPM();
  Serial.print("PA_6: ");
  Serial.print(analogRead(mq135Pin));
  Serial.print(" ppm CO2: ");
  Serial.println(ppm);
  delay(1000); // Actualise la mesure chaque seconde

}
void motd() {
  Serial.println("-------------------");
  Serial.println("Programmes utulisables:");
  Serial.println("A: Potentiometre");
  Serial.println("B: Co2");
  Serial.println("C: Initialiser communication passerelle");
  Serial.println("<Enter> Pour stopper un programme");
}
//-----Potentiometre-------

void pot() {
  float potVal = analogRead(27);
  float voltage = potVal * (3.3 / 4094.0);

  Serial.println("---------");
  Serial.println("Pot value:");
  Serial.println(potVal);
  Serial.println("Volts:");
  Serial.println(voltage);
  Serial.println("---------");
  Serial1.println("---------");

  delay(5000);
}

void envoiTrameCourante() {
  TRA = "1";
  OBJ = "0001";
  REQ = "1";
  TYP = "1";
  NUM = "01";
  TIM = "0000";
  CHK = "1B";
  int potVal = analogRead(27);

  String voltString = String(potVal);
  if (voltString.length() < 4) {
    String buffer = "";
    int bufferlength = 4 - voltString.length();
    for (int i = 0; i < bufferlength; i++) {
      buffer += "0";
    }
    voltString = buffer + voltString;

  }
  String potdata = TRA + OBJ + REQ + TYP + NUM + voltString + TIM + CHK;

  Serial1.print(potdata);
  Serial.println("Sent: " + potdata + " val: " + voltString);

  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();
    Serial.println(incomingByte);
  }
  delay(1000);
}

String stringToHex(String input) {
  String hex = “”;
  for (int i = 0; i < input.length(); i++) {
    char c = (char) input;
    hex += String(c, HEX);
  }
  return hex;
}
String hexToString(String hex) {
  String text = “”;
  for (int k = 0; k < hex.length(); k++) {
    if (k % 2 != 0) {
      char temp[3];
      sprintf(temp, "%c%c", hex[k - 1], hex[k]);
      int number = (int) strtol(temp, NULL, 16);
      text += char(number);
    }
  }
  return text;
}

void loop() {

  boolean running = false;

  //--------Interface Utulisateur---------//

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    if (incomingByte == 97) {
      running = true;
      while (running) {
        pot();
        if (Serial.available() > 0) {
          incomingByte = Serial.read();
          if (incomingByte == 13) {
            running = false;
            motd();
          }
        }
      }

    }
    if (incomingByte == 98) {
      running = true;
      while (running) {
        capteurco2();
        if (Serial.available() > 0) {
          incomingByte = Serial.read();
          if (incomingByte == 13) {
            running = false;
            motd();
          }
        }
      }

    }
    if (incomingByte == 67) {
      running = true;
      while (running) {
        envoiTrameCourante();
        if (Serial.available() > 0) {
          incomingByte = Serial.read();
          if (incomingByte == 13) {
            running = false;
            motd();
          }
        }
      }

    }
  }
}

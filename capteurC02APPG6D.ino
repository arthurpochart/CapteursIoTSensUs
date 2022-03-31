//---------Capteur CO2------------//
#include "MQ135.h"

const int mq135Pin = 0; // Pin sur lequel est branché de MQ135

MQ135 gasSensor = MQ135(mq135Pin); // Initialise l'objet MQ135 sur le Pin spécifié
void motd();
//----Buetooth-----------//
int incomingByte = 0;

//Potentiometre//
int alagogPin = 18;
int potVal = 0;

void setup() {
  Serial.begin(9600); // Initialise le port série à 9600 bps 
  Serial1.begin(9600);

  //---------Capteur CO2------------//
  float rzero = gasSensor.getRZero();
  Serial.print("R0: ");
  Serial.println(rzero); // Valeur à reporter ligne 27 du fichier mq135.h après 48h de préchauffage
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
  Serial.println("<Enter> Pour stopper un programme");
}
//-----Potentiometre-------//
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
  Serial1.println("Pot value:");
  Serial1.println(potVal);
  Serial1.println("Volts:");
  Serial1.println(voltage);
  Serial1.println("---------");
  delay(1000);
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
  }
}

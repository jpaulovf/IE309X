/*************************************
 * Tests.cpp - Testes unitários
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

#include "Tests.h"
#include <Arduino.h>
#include <DataStorage.h>
#include <ADE7758.h>
#include <stdint.h>

/* ------------------------------------- *
 * Funções                               *
 * ------------------------------------- */


/*
 * Testa o armazenamento na EEPROM
 */
void testStorage(){

  char msg[14] = "Jall Sondwich";
  char buf[14];

  // Clearing buffer
  for (int i=0; i<14; i++){
    buf[i] = 0;
  }

  Serial.begin(9600);

  Serial.println("EEPROM Storage Test!");
  Serial.println("Reading 14 bytes from EEPROM address 0...");

  getStoredData(0, &buf[0], 14);

  Serial.print(" Data = ");
  Serial.println(buf);

  Serial.println("Writing 14 bytes to EEPROM address 0...");

  storeData(0, &msg[0], 14);

  Serial.println("Reading 14 bytes from EEPROM address 0...");

  getStoredData(0, &buf[0], 14);
  Serial.print(" Data = ");
  Serial.println(buf);
  
}

/*
 * Testa a comunicação com o AD7758
 */
void testAD7758(){

  ADE7758Device *ade = new ADE7758Device();
  uint8_t reg_vrms[3];
  uint8_t reg_mode[2];
  uint32_t vrms = 0;
  uint64_t vrms_m = 0;
  uint16_t mode;
  uint8_t ch2os = 0x27;
  uint8_t vrmsos[2] = {0x00, 0x00};
  uint8_t vrmsos_read[2];

  for (int i=0; i<3; i++){
    reg_vrms[i] = 0;
  }

  Serial.begin(9600);

  Serial.println("AD7758 Communication Test!");

  // Lendo o reg MODE
  Serial.println("Reading MODE...");
  ade->readRegister(0x09, 2, reg_mode);
  mode = reg_mode[0] << 8 | reg_mode[1];
  Serial.print("Mode = ");
  Serial.println(mode, HEX);

  // Tentando corrigir o offset
  Serial.println("Tryying to correct offset...");
  ade->writeRegister(0x19, 2, vrmsos);
  Serial.println("Tryying to correct offset...");
  ade->writeRegister(0x0E, 1, &ch2os);

  ade->readRegister(0x19,2, vrmsos_read);
  Serial.println(vrmsos_read[0], HEX);
  Serial.println(vrmsos_read[1], HEX);


  Serial.println("Reading VRMS... 10 MEANS");


  for (int i=0; i<256; i++){
    // Lendo VRMS
    ade->readRegister(0x17, 3, reg_vrms);
    // Juntando
    vrms = reg_vrms[0] << 16 | reg_vrms[1] << 8 | reg_vrms[2];

    vrms_m += (uint64_t) vrms;

  }

  // Média
  vrms = (uint32_t) (vrms_m >> 8);

  // Printando
  Serial.println("Register VRMS = ");
  Serial.println((int) vrms);
  
}
 

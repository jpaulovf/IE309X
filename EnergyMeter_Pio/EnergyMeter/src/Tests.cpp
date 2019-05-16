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
 * Globais                               *
 * ------------------------------------- */
volatile uint8_t irqflag = 0;


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
void testADE7758(){

  ADE7758Device *ade = new ADE7758Device();
  uint8_t addr;
  uint8_t size;
  uint8_t operation = 0;
  uint8_t data[10];
  char inputBuffer[3];
  char key;
  uint32_t finalData;

  // Inicializando a serial
  Serial.begin(9600);
  Serial.setTimeout(60000);


  // Limpando os buffers
  for (int i=0; i<10; i++){
    data[i] = 0;
  }
  for (int i=0; i<3; i++){
    inputBuffer[i] = 0;
  }
  
  // Loop para pegar dados do usuário
  while (1){

    Serial.println("*** ENTER REGISTER PARAMETERS ***");

    // Escolhendo a operação (R/W)
    Serial.println("\nSelect operation: (W)rite, (R)ead");
    Serial.readBytes(&key, 1);
    if (key == 'w' || key == 'W'){
      operation = 1;
    }
    else{
      operation = 0;
    }
    Serial.print("You entered ");
    if (operation == 1){
      Serial.println("WRITE");
    }
    else{
      Serial.println("READ");
    }

    // Lendo o endereço do registrador
    Serial.println("\nRegister address (hexa): ");
    Serial.readBytesUntil('\n', (char *) inputBuffer, 2);
    while (Serial.available()){
      Serial.read();
    }
    inputBuffer[2] = 0;
    addr = (uint8_t) strtol(inputBuffer, 0, 16);
    Serial.print("You entered address 0x");
    Serial.println(addr, HEX);

    // Lendo o numero de bytes
    for (int i=0; i<3; i++){
      inputBuffer[i] = 0;
    }
    Serial.println("\nNumber of data bytes: ");
    Serial.readBytesUntil('\n', (char *) inputBuffer, 2);
    while (Serial.available()){
      Serial.read();
    }
    inputBuffer[2] = 0;
    size = (uint8_t) strtol(inputBuffer, 0, 10);
    if (size > 10){
      size = 10;
    }
    Serial.print("You entered ");
    Serial.println(size);

    // Lendo os dados, caso escrita
    if (operation == 1){
      for (int i=0; i<10; i++){
        data[i] = 0;
      }
      for (int i=0; i<3; i++){
        inputBuffer[i] = 0;
      }
      for (int i=0; i<size; i++){
        Serial.print("\nData[");
        Serial.print(i);
        Serial.println("] (hexa): ");
        Serial.readBytesUntil('\n', (char *) inputBuffer, 2);
        while (Serial.available()){
          Serial.read();
        }
        inputBuffer[2] = 0;
        data[i] = (uint8_t) strtol(inputBuffer, 0, 16);
        Serial.print("You entered 0x");
        Serial.println(data[i], HEX);
      }
    }

    // Iniciando operação
    if (operation == 1){
      Serial.println("---------------------------");
      Serial.print("Writing ");
      Serial.print(size);
      Serial.print(" bytes on register 0x");
      Serial.println(addr, HEX);
      ade->writeRegister(addr, size, data);
      Serial.println("DONE\n");
    }
    else{
      Serial.println("---------------------------");
      Serial.print("Reading ");
      Serial.print(size);
      Serial.print(" bytes from register 0x");
      Serial.println(addr, HEX);
      ade->readRegister(addr, size, data);
      finalData = 0;
      for (int i=0; i<size; i++){
        finalData |= data[i]<<(8*(size-1-i));
      }
      Serial.print("Read value is: ");
      Serial.print(finalData);
      Serial.print(" (0x");
      Serial.print(finalData, HEX);
      Serial.println(")\n");
    }

  }
    


  // uint8_t reg_vrms[3];
  // uint8_t reg_mode[2];
  // uint32_t vrms = 0;
  // uint64_t vrms_m = 0;
  // uint16_t mode;
  // uint8_t ch2os = 0x27;
  // uint8_t vrmsos[2] = {0x00, 0x00};
  // uint8_t vrmsos_read[2];

  // for (int i=0; i<3; i++){
  //   reg_vrms[i] = 0;
  // }

  // Serial.begin(9600);

  // Serial.println("AD7758 Communication Test!");

  // // Lendo o reg MODE
  // Serial.println("Reading MODE...");
  // ade->readRegister(0x09, 2, reg_mode);
  // mode = reg_mode[0] << 8 | reg_mode[1];
  // Serial.print("Mode = ");
  // Serial.println(mode, HEX);

  // // Tentando corrigir o offset
  // Serial.println("Tryying to correct offset...");
  // ade->writeRegister(0x19, 2, vrmsos);
  // Serial.println("Tryying to correct offset...");
  // ade->writeRegister(0x0E, 1, &ch2os);

  // ade->readRegister(0x19,2, vrmsos_read);
  // Serial.println(vrmsos_read[0], HEX);
  // Serial.println(vrmsos_read[1], HEX);


  // Serial.println("Reading VRMS... 10 MEANS");


  // for (int i=0; i<256; i++){
  //   // Lendo VRMS
  //   ade->readRegister(0x17, 3, reg_vrms);
  //   // Juntando
  //   vrms = reg_vrms[0] << 16 | reg_vrms[1] << 8 | reg_vrms[2];

  //   vrms_m += (uint64_t) vrms;

  // }

  // // Média
  // vrms = (uint32_t) (vrms_m >> 8);

  // // Printando
  // Serial.println("Register VRMS = ");
  // Serial.println((int) vrms);
  
}


/*
 * Testa a entrada de caracteres hexa
 */
void testHexInput(){

  char buf[3];
  uint8_t hex_val; 

  // Inicializando a serial
  Serial.begin(9600);
  Serial.setTimeout(60000);

  // Limpando buffer
  buf[0] = 0;
  buf[1] = 0;
  buf[2] = 0;

  while (1){
    Serial.println("Enter hex:");
    Serial.readBytesUntil('\n', (char *) buf, 2);
    
    // Flusheando o buffer
    while (Serial.available()){
      Serial.read();
    }

    buf[2] = 0;

    hex_val = (uint8_t) strtol(buf, 0, 16);

    Serial.print("You entered: ");
    Serial.println(buf);

    Serial.print("Number value is ");
    Serial.print(hex_val);
    Serial.print(" (");
    Serial.print(hex_val, HEX);
    Serial.println(")\n");
  }

}

void IRAM_ATTR IRQHandler(){

  irqflag = 1;

}

/*
 * Testa a medição de VRMS com interrupção de ZX
 */
void testVRMS(){

  ADE7758Device *ade = new ADE7758Device();
  const byte interruptPin = 39; //14
  uint8_t data[2];
  uint8_t vrms_data[3];
  uint32_t vrms;

  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);

  // Configurando os registradores
  Serial.println("Configuring registers...");

  // Registrador MODE
  data[0] = 0x60;
  data[1] = 0x0C;
  ade->writeRegister(0x09, 2, data);

  // Registrador INTERRUPT ENABLE
  data[0] = 0x00;
  data[1] = 0x10;
  ade->writeRegister(0x0A, 2, data);

  attachInterrupt(digitalPinToInterrupt(interruptPin), IRQHandler, FALLING);

  while (1){

    if (irqflag == 1){

      Serial.println("Interrupt detected. Reading VRMS...");

      ade->readRegister(0x17, 3, vrms_data);
      vrms = ((uint32_t) vrms_data[0]) << 16 |
             ((uint32_t) vrms_data[1]) << 8 |
             ((uint32_t) vrms_data[2]);

      Serial.print("VRMS = ");
      Serial.print(vrms);
      Serial.print(" (0x");
      Serial.print(vrms, HEX);
      Serial.println(")");

      ade->readRegister(0x0C, 2, data); 

    }

  }

}
 

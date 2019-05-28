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
#include <RGBLed.h>

/* ------------------------------------- *
 * Globais                               *
 * ------------------------------------- */
volatile uint8_t irqflag = 0;



/* ------------------------------------- *
 * Funções                               *
 * ------------------------------------- */

/*
 * Testa o LED RGB
 */
void testRGBLed(){

  RGBLed *myled = new RGBLed(PIN_LED_R, PIN_LED_G, PIN_LED_B);
  myled->write(0, 0, 0);  // Off

  while (1){

    myled->write(1, 0, 0);  // Red
    delay(500);
    myled->write(0, 1, 0);  // Green
    delay(500);
    myled->write(0, 0, 1);  // Blue
    delay(500);

  }


}


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
  
}

/*
 *  Interrupção de Zero-Crossing
 */ 
void IRAM_ATTR IRQHandler(){

  digitalWrite(DEBUG_LED, !digitalRead(DEBUG_LED));
  irqflag = 1;

}

/*
 * Testa a medição de VRMS com interrupção de ZX
 */
void testVRMS(){

  ADE7758Device *ade = new ADE7758Device();
  const byte interruptPin = 34; //14
  uint8_t data[2];
  uint8_t vrms_data[3];
  uint32_t vrms;
  uint32_t meas_counter = 0;
  uint64_t vrms_mean = 0;

  Serial.begin(9600);
  pinMode(interruptPin, INPUT);
  pinMode(DEBUG_LED, OUTPUT);
  digitalWrite(DEBUG_LED, HIGH);

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

  // Registrador de correção de offset VRMS
  data[0] = 0x00;
  data[1] = 0x00;
  ade->writeRegister(0x19, 2, data);

  // Lendo os registradores pra ver se tá certo
  ade->readRegister(0x09, 2, data);
  Serial.print("MODE = 0x");
  Serial.print(data[0], HEX);
  Serial.println(data[1], HEX);

  ade->readRegister(0x0B, 2, data);
  Serial.print("Interrupt Status = 0x");
  Serial.print(data[0], HEX);
  Serial.println(data[1], HEX);

  // Resetando flags
  ade->readRegister(0x0C, 2, data);

  attachInterrupt(digitalPinToInterrupt(interruptPin), IRQHandler, FALLING);

  while (1){

    if (irqflag == 1){

      //Serial.println("Interrupt detected. Reading VRMS...");

      ade->readRegister(0x17, 3, vrms_data);
      vrms = ((uint32_t) vrms_data[0]) << 16 |
             ((uint32_t) vrms_data[1]) << 8 |
             ((uint32_t) vrms_data[2]);

      /*
      Serial.print("VRMS = ");
      Serial.print(vrms);
      Serial.print(" (0x");
      Serial.print(vrms, HEX);
      Serial.println(")");
      */

      meas_counter++;
      vrms_mean += vrms;

      if (meas_counter == 256){
        meas_counter = 0;
        vrms = (uint32_t) (vrms_mean >> 8);
        vrms_mean = 0;
        Serial.print("VRMS = ");
        Serial.print(vrms);
        Serial.print(" (0x");
        Serial.print(vrms, HEX);
        Serial.println(")");
      }

      // Resetando o registrador de flags 
      ade->readRegister(0x0C, 2, data); 

    }

  }

}

/*
 *  Calibra o offset de tensão
 */
void voltCal(){

  ADE7758Device *ade = new ADE7758Device();
  uint8_t data[2];
  uint8_t wfm_data[3];  // Registrador WAVEFORM
  uint32_t volt;

  Serial.begin(9600);

  // Configurando o MODE
  data[0] = 0x60;
  data[1] = 0x0C;
  ade->writeRegister(0x09, 2, data);

  // Configurando o offset CH2OS
  data[0] = 0x00;
  ade->writeRegister(0x0E, 1, &data[0]);

  // Lendo o WAVEFORM em loop
  while (1){
    ade->readRegister(0x01, 3, wfm_data);
    volt = ((uint32_t) wfm_data[0]) << 16 |
           ((uint32_t) wfm_data[1]) << 8  |
           ((uint32_t) wfm_data[2]);
    Serial.print("Volt = ");
    Serial.print(volt);
    Serial.print(" (0x");
    Serial.print(volt, HEX);
    Serial.println(")");


  }

}
 

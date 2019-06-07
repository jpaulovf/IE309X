
/******************************************************
 * EnergyMeter - Firmware do medidor de energia 
 * Disciplina: IE309X - Oficina de IoT          
 * ----------------------------------------------------
 * Este arquivo contém o ponto de entrada do FW
 * do medidor de energia para o projeto da disciplina 
 * IE309X da FEEC/Unicamp, ministrada durante o primeiro 
 * semestre de 2019
 * 
 * Data de criação: 11/04/2019
 * 
 * Autores
 * - Fabio Bassan
 * - Felipe Marques
 * - Gabriel Legramanti
 * - Joao Fracarolli
 * 
 * Este arquivo está no GitHub
 * https://github.com/jpaulovf/IE309X
 * 
 * Microcontrolador: Espressif ESP32
 * Kit de desenvolvimento: ESP32-WROOM32
 * 
 * Desenvolvido na Arduino IDE 1.8.9
 * 
 ******************************************************/

/*-----------------------------------------------------
 *  Includes
 *-----------------------------------------------------*/

// Includes para o Blynk
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>
// Rotinas para configuração da SPI
//#include <SPI.h>
// DMA para salvar SSID e Pass
//#include <EEPROM.h>
// Lib do ADE7758 (clonado de engkan2kit)
//#include "ADE7758Lib.h" /* https://github.com/engkan2kit/ADE7758 */

// ----- Arquivos do projeto -----
#include <Arduino.h>
#include <RGBLed.h>
#include <UserInput.h>
#include <DataStorage.h>
#include "Tests.h"

/*-----------------------------------------------------
 *  Definições e macros
 *-----------------------------------------------------*/

#define PIN_IRQ     39  // IRQ no pino GPIO39 (input)
#define PIN_APCF    34  //  
#define PIN_VACPF   35  // 
//#define PIN_LED_R   32  // LED RBG
//#define PIN_LED_G   33  // LED RGB
//#define PIN_LED_B   25  // LED RGB
#define PIN_SCLK    14  // SPI Clock
#define PIN_DOUT    12  // SPI data out
#define PIN_DIN     13  // SPI data in
#define PIN_CS      15  // SPI chip select

#define PIN_DEVLED  2   // LED do devkit 

/*-----------------------------------------------------
 *  Globais
 *-----------------------------------------------------*/

/*-----------------------------------------------------
 *  Funções
 *-----------------------------------------------------*/

void setup() {

    // testStorage();
    // testHexInput();
    //testADE7758();
    //testVRMS();
    //testRGBLed();
    whCal();

   // checkIfAlive();
}

void loop() {
  
}

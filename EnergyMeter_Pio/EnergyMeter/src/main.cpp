
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
 * Desenvolvido no VSCode, usando o plugin PatformIO IDE
 * Framework: Arduino
 * Dependências: Blynk
 * 
 ******************************************************/

/*-----------------------------------------------------
 *  Includes
 *-----------------------------------------------------*/

#include <Arduino.h>
#include <RGBLed.h>
#include <UserInput.h>
#include <DataStorage.h>
#include <run.h>

// Descomentar para incluir o arquivo de testes
// #include <Tests.h>

/*-----------------------------------------------------
 *  Definições e macros
 *-----------------------------------------------------*/

/*-----------------------------------------------------
 *  Globais
 *-----------------------------------------------------*/

/*-----------------------------------------------------
 *  Funções
 *-----------------------------------------------------*/

/* Comente ou descomente para escolher qual função chamar 
 * O firmware principal está na função run() 
 */
void setup() {

    // testStorage();
    // testHexInput();
    // testADE7758();
    // testVRMS();
    // testRGBLed();
    // whCal();
    // testBlynk();
    // checkIfAlive();

   run();
}

/* A loop() ficará vazia pois o loop infinito está dentro da run() */
void loop() {
  
}

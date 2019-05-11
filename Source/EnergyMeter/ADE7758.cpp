/*************************************
 * ADE7758.cpp - Biblioteca do medidor
 * de energia ADE7758 com as funções
 * pertinente a este projeto
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

#include "ADE7758.h"
#include "Arduino.h"
#include <SPI.h>
#include <stdint.h>

/* ------------------------------------- *
 * Typedefs                              *
 * ------------------------------------- */


/* ------------------------------------- *
 * Globais                               *
 * ------------------------------------- */


/* ------------------------------------- *
 * Métodos private                       *
 * ------------------------------------- */

// =====================================
// Método para escrever no registrador
// =====================================
void ADE7758Device::writeRegister(uint8_t addr, 
                                  uint8_t dataSize, 
                                  uint8_t *data){

    // Iniciando comunicação SPI
    hspi->beginTransaction(SPISettings(SPICLK_HZ, MSBFIRST, SPI_MODE1));
    // Colocando o chip select em LOW
    digitalWrite(pin_cs, LOW);
    // Mandando o endereço
    hspi->transfer(0x80 | addr);
    // Mandando os dados
    for (int i=0; i<dataSize; i++){
        hspi->transfer(data[i]);
    }
    // Tempo de guarda
    delayMicroseconds(10);
    // Colocando o chip select em HIGH
    digitalWrite(pin_cs, HIGH);
    // Finalizando a comunicação SPI
    hspi->endTransaction();

}

// =====================================
// Método para ler de um registrador
// =====================================
void ADE7758Device::readRegister(uint8_t addr, 
                                 uint8_t dataSize, 
                                 uint8_t *data){
                                         // Iniciando comunicação SPI
    hspi->beginTransaction(SPISettings(SPICLK_HZ, MSBFIRST, SPI_MODE1));
    // Colocando o chip select em LOW
    digitalWrite(pin_cs, LOW);
    // Mandando o endereço
    hspi->transfer(addr);
    // Lendo os dados e colocando em data
    for (int i=0; i<dataSize; i++){
        data[i] = hspi->transfer(0x00);
    }
    // Tempo de guarda
    delayMicroseconds(10);
    // Colocando o chip select em HIGH
    digitalWrite(pin_cs, HIGH);
    // Finalizando a comunicação SPI
    hspi->endTransaction();
}

/* ------------------------------------- *
 * Métodos public                        *
 * ------------------------------------- */

// =====================================
// Construtor
// =====================================
ADE7758Device::ADE7758Device(uint8_t pin_reset,
                             uint8_t pin_ck,
                             uint8_t pin_miso,
                             uint8_t pin_mosi,
                             uint8_t pin_ss) {

    // Criando nova instância do objeto SPIClass
    hspi = new SPIClass(HSPI);
    hspi->begin(pin_ck, pin_miso, pin_mosi, pin_ss);
    pin_cs = pin_ss;

    // Setando o pino SS e reset como saídas
    pinMode(pin_ss, OUTPUT);
    pinMode(pin_reset, OUTPUT);

    // Colocando o reset em nível baixo
    pinMode(pin_reset, LOW);

    // Inserir aqui rotinas de inicialização do ADE7758

}

// =====================================
// Ler tensão RMS
// =====================================
float ADE7758Device::getRMSVoltage(uint8_t phase){

    // Bytes a serem lidos
    




}

// =====================================
// Ler corrente RMS
// =====================================
float ADE7758Device::getRMSCurrent(uint8_t phase){

}

// =====================================
// Destruidor
// =====================================
ADE7758Device::~ADE7758Device(){

    // Deletando o objeto hspi
    delete hspi;

}
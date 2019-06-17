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
#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>

/* ------------------------------------- *
 * Typedefs                              *
 * ------------------------------------- */


/* ------------------------------------- *
 * Globais                               *
 * ------------------------------------- */


/* ------------------------------------- *
 * Métodos                               *
 * ------------------------------------- */

// =====================================
// Método para escrever no registrador
// =====================================
void ADE7758Device::writeRegister(uint8_t addr, 
                                  uint8_t dataSize, 
                                  uint8_t *data){

    // Iniciando comunicação SPI
    hspi->beginTransaction(SPISettings(SPICLK_HZ, MSBFIRST, SPI_MODE2));
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
    hspi->beginTransaction(SPISettings(SPICLK_HZ, MSBFIRST, SPI_MODE2));
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

// =====================================
// Escreve em um registrador de 8 bits
// =====================================
void ADE7758Device::write8(uint8_t addr, uint8_t data){

    uint8_t data8;

    data8 = data;

    writeRegister(addr, 1, &data8);

}

// =====================================
// Escreve em um registrador de 16 bits
// =====================================
void ADE7758Device::write16(uint8_t addr, uint16_t data){

    uint8_t data16[2];

    data16[0] = (uint8_t) ((data & 0xFF00) >> 8);
    data16[1] = (uint8_t) ((data & 0x00FF) >> 0);

    writeRegister(addr, 2, data16);

}

// =====================================
// Escreve em um registrador de 24 bits
// =====================================
void ADE7758Device::write24(uint8_t addr, uint32_t data){

    uint8_t data24[3];

    data24[0] = (uint8_t) ((data & 0x00FF0000) >> 16);
    data24[1] = (uint8_t) ((data & 0x0000FF00) >> 8);
    data24[2] = (uint8_t) ((data & 0x000000FF) >> 0);

    writeRegister(addr, 3, data24);

}

// =====================================
// Lê um registrador de 8 bits
// =====================================
uint8_t ADE7758Device::read8(uint8_t addr){

    uint8_t regvalue;
    uint8_t data8;

    readRegister(addr, 1, &data8);

    regvalue = data8;

    return regvalue;

}

// =====================================
// Lê um registrador de 16 bits
// =====================================
uint16_t ADE7758Device::read16(uint8_t addr){

    uint16_t regvalue;
    uint8_t data16[2];

    readRegister(addr, 2, data16);

    regvalue = ((uint16_t) data16[0]) << 8 |
               ((uint16_t) data16[1]) << 0; 

    return regvalue;

}

// =====================================
// Lê um registrador de 24 bits
// =====================================
uint32_t ADE7758Device::read24(uint8_t addr){

    uint32_t regvalue;
    uint8_t data24[3];

    readRegister(addr, 3, data24);

    regvalue = ((uint32_t) data24[0]) << 16 |
               ((uint32_t) data24[1]) << 8 |
               ((uint32_t) data24[2]) << 0;

    return regvalue;

}

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
// Destruidor
// =====================================
ADE7758Device::~ADE7758Device(){

    // Deletando o objeto hspi
    delete hspi;

}
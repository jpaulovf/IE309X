/*************************************
 * ADE7758.h - Biblioteca do medidor
 * de energia ADE7758 com as funções
 * pertinente a este projeto
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

#ifndef ADE7758_H_
#define ADE7758_H_

#include <stdint.h>
#include <SPI.h>

/* ------------------------------------- *
 * Defines                               *
 * ------------------------------------- */

// Clock da SPI (Hz)
#define SPICLK_HZ 1000000   // 1 MHz

/* ------------------------------------- *
 * Classe ADE7758                        *
 * ------------------------------------- */
class ADE7758Device{

    private:

        // Pino do CS
        uint8_t pin_cs;

        // Ponteiro para um spi device
        SPIClass *hspi = NULL;

    public:

        // Método para escrever no registrador
        void writeRegister(uint8_t addr, uint8_t dataSize, uint8_t *data);

        // Método para ler de um registrador
        void readRegister(uint8_t addr, uint8_t dataSize, uint8_t *data);

        // Construtor com valores padrão
        ADE7758Device(uint8_t pin_reset = 17,
                      uint8_t pin_ck = 14,
                      uint8_t pin_miso = 12,
                      uint8_t pin_mosi = 13,
                      uint8_t pin_ss = 15);

        // Ler tensão RMS
        float getRMSVoltage(uint8_t phase);

        // Ler corrente RMS
        float getRMSCurrent(uint8_t phase);

        // Destruidor
        ~ADE7758Device();

};


#endif
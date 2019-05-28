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

// Seleção do tipo de ADE
#define TYPE_58 0
#define TYPE_53 1

// Endereços dos registradores
#if TYPE_53 == 1

    #define REG_WAVEFORM    0x01U   // Waveform
    #define REG_AENERGY     0x02U   // Active energy (acumulador 24 bits)
    #define REG_RAENERGY    0x03U   // Active energy RESET
    #define REG_LAENERGY    0x04U   // Line acumulation active energy
    #define REG_VAENERGY    0x05U   // Apparent energy
    #define REG_RVAENERGY   0x06U   // Apparent energy RESET
    #define REG_LVAENERGY   0x07U   // Line accumulation apparent energy
    #define REG_LVARENERGY  0x08U   // Line accumulation reactive energy
    #define REG_MODE        0x09U   // Configurações MODE
    #define REG_IRQEN       0x0AU   // Interrupt enable
    #define REG_STATUS      0x0BU   // Interrupt status
    #define REG_RSTSTATUS   0x0CU   // Interrupt status RESET
    #define REG_CH1OS       0x0DU   // CH1 Offset (corrente)
    #define REG_CH2OS       0x0EU   // CH2 Offset (tensão)
    #define REG_GAIN        0x0FU   // PGA Gain
    #define REG_PHCAL       0x10U   // Phase calibration
    #define REG_APOS        0x11U   // Active power offset
    #define REG_WGAIN       0x12U   // power gain adjust
    #define REG_WDIV        0x13U   // Active energy divider
    #define REG_CFNUM       0x14U   // CF frequency divider numerator
    #define REG_CFDEN       0x15U   // CF frequency divider denominator
    #define REG_IRMS        0x16U   // Corrente RMS
    #define REG_VRMS        0x17U   // Tensão RMS
    #define REG_IRMSOS      0x18U   // Corrente RMS offset
    #define REG_VRMSOS      0x19U   // Tensão RMS offset
    #define REG_VAGAIN      0x1AU   // Apparent gain
    #define REG_VADIV       0x1BU   // Apparent energy divider
    #define REG_LINECYC     0x1CU   // Line Cycle Energy Accumulation Mode Line-Cycle Register
    #define REG_ZXTOUT      0x1DU   // Zero-Crossing Timeout
    #define REG_SAGCYC      0x1EU   // Sag Line Cycle
    #define REG_SAGLVL      0x1FU   // Sag Voltage Level
    #define REG_IPKLVL      0x20U   // Pico CH1 threshold
    #define REG_VPKLVL      0x21U   // Pico CH2 threshold
    #define REG_IPEAK       0x22U   // Pico CH1
    #define REG_RSTIPEAK    0x23U   // Pico CH1 RESET
    #define REG_VPEAK       0x24U   // Pico CH2
    #define REG_RSTVPEAK    0x25U   // Pico CH2 RESET
    #define REG_TEMP        0x26U   // Temperatura
    #define REG_PERIOD      0x27U   // Período CH2
    #define REG_TMOD        0x3DU   // Modo de teste
    #define REG_CHKSUM      0x3EU   // Checksum
    #define REG_DIEREV      0x3FU   // Die revision number

#endif

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
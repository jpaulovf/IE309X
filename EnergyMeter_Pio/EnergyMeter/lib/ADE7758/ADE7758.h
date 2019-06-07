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
#define SPICLK_HZ 1000000       // 1 MHz

// Endereços dos registradores
#define REG_AWATTHR     0x01    // Leitura do W.h fase A (16)
#define REG_BWATTHR     0x02    // Leitura do W.h fase B (16)
#define REG_CWATTHR     0x03    // Leitura do W.h fase C (16)
#define REG_MMODE       0x14    // Configuração do modo de medição (8)
#define REG_WAVEFORM    0x15    // Configuração de medições de waveform
#define REG_LCYCMODE    0x17    // Modo line cycle (8)
#define REG_MASK        0x18    // Máscara de interrupções (24)
#define REG_STATUS      0x19    // Status das interrupções (24)
#define REG_RSTATUS     0x1A    // Reset dos flags de interrupção (24)
#define REG_LINECYC     0x1C    // Número de meio-ciclos para acumular medição de energia (16)
#define REG_AWG         0x2A    // Ganho em Watt da fase A (12)
#define REG_BWG         0x2B    // Ganho em Watt da fase B (12)
#define REG_CWG         0x2C    // Ganho em Watt da fase C (12)
#define REG_APCFNUM     0x45    // Numerador da escala de potência ativa (16)
#define REG_APCFDEN     0x46    // Denominador da escala de potência ativa (12)

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

        // Construtor com valores padrão
        ADE7758Device(uint8_t pin_reset = 17,
                      uint8_t pin_ck = 14,
                      uint8_t pin_miso = 12,
                      uint8_t pin_mosi = 13,
                      uint8_t pin_ss = 15);


        // Método para escrever no registrador (de tamanho variável)
        void writeRegister(uint8_t addr, uint8_t dataSize, uint8_t *data);

        // Método para ler de um registrador (de tamanho variável)
        void readRegister(uint8_t addr, uint8_t dataSize, uint8_t *data);
        
        // Escreve em um registrador de 8 bits
        void write8(uint8_t addr, uint8_t data);

        // Escreve em um registrador de 16 bits
        void write16(uint8_t addr, uint16_t data);

        // Escreve em um registrador de 24 bits
        void write24(uint8_t addr, uint32_t data);

        // Lê um registrador de 8 bits
        uint8_t read8(uint8_t addr);

        // Lê um registrador de 16 bits
        uint16_t read16(uint8_t addr);

        // Lê um registrador de 24 bits
        uint32_t read24(uint8_t addr);

        // Destruidor
        ~ADE7758Device();

};


#endif
/*********************************************
 * DataStorage.h - Rotinas para armazenamento
 * de dados de configuração na EEPROM
 * -------------------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *********************************************/

#ifndef DATA_STORAGE_H_
#define DATA_STORAGE_H_


/* ------------------------------------- *
 * Defines                               *
 * ------------------------------------- */

// Códigos de erro
#define EEPROM_OK        0  // Sem erro
#define EEPROM_BADADDR  -1  // Endereço inválido
#define EEPROM_BADSIZE  -2  // Tamanho inválido

// Tamanho máximo da EEPROM: 100 bytes (50 para SSID, 50 para pass)
#define EEPROM_SIZE 100

// Tamanho de cada campo: 50 bytes 
#define FIELD_SIZE 50;

// Posição inicial do SSID na EEPROM
#define SSID_START_ADDR 0

// Posição inicial do password na EEPROM
#define PASS_START_ADDR 99

/* ------------------------------------- *
 * Protótipos                            *
 * ------------------------------------- */

/*
 * Armazena dados na EEPROM
 *  param[in] addr
 *    endereço onde armazenar o dado
 *  param[in] data
 *    ponteiro para os dados a serem armazenados
 *  param[in] size
 *    quantidade de dados a serem armazenados
 *  return
 *    código de erro
 */
int storeData(unsigned int addr, char *data, unsigned int size);

/*
 * Lê dados armazenados na EEPROM
 *  param[in] addr
 *    endereço onde armazenar o dado
 *  param[out] data
 *    ponteiro para armazenar os dados lidos
 *  param[in] size
 *    quantidade de dados a serem lidos
 *  return
 *    código de erro
 */
int getStoredData(unsigned int addr, char *data, unsigned int size); 

#endif

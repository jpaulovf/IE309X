/*********************************************
 * DataStorage.cpp - Rotinas para armazenamento
 * de dados de configuração na EEPROM
 * -------------------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *********************************************/

#include "DataStorage.h"
#include "Arduino.h"
#include "EEPROM.h"

/* ------------------------------------- *
 * Globais                               *
 * ------------------------------------- */
unsigned int eeprom_ready = 0; 

/* ------------------------------------- *
 * Funções                               *
 * ------------------------------------- */

/*
 * Inicialização da EEPROM
 */
static void initEEPROM(){

  EEPROM.begin(EEPROM_SIZE);
  eeprom_ready = 1;
    
}

/*
 * Armazena dados na EEPROM
 */
int storeData(unsigned int addr, char *data, unsigned int size){

  int i;

  // Checando se a EEPROM foi inicializada
  if (eeprom_ready == 0){
    initEEPROM();
  }

  // Checando se o endereço é válido
  if (addr > 512){
    return EEPROM_BADADDR;
  }

  // Checando se o tamanho dos dados é válido
  if (size + addr > 512){
    return EEPROM_BADSIZE;
  }

  // Escrevendo
  for (i=0; i<size; i++){
    EEPROM.write(addr+i, data[i]);
  }

  // Enviando a mensagem para a EEPROM
  EEPROM.commit();

  return EEPROM_OK;
  
}

/*
 * Lê dados armazenados na EEPROM
 */
int getStoredData(unsigned int addr, char *data, unsigned int size){

  int i;
  char value;

  // Checando se a EEPROM foi inicializada
  if (eeprom_ready == 0){
    initEEPROM();
  }

  // Checando se o endereço é válido
  if (addr > 512){
    return EEPROM_BADADDR;
  }

  // Checando se o tamanho dos dados é válido
  if (size + addr > 512){
    return EEPROM_BADSIZE;
  }

  // Lendo
  for (i=0; i<size; i++){
    value = (char) EEPROM.read(addr+i);
    data[i] = value;
  }

  return EEPROM_OK;
  
}

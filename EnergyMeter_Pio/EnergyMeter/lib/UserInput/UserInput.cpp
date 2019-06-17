/*************************************
 * UserInput.cpp -- Rotinas para entrar 
 * com strings
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

 #include <Arduino.h>
 #include "UserInput.h"

 void initSerial(){
    Serial.begin(9600); 
    Serial.setTimeout(60000);
 }

 void getUserInput(char *buf, unsigned int size){

    unsigned int bufsize;
    int i;
    
    // Checando tamanho
    if (size > BUFMAX){
      bufsize = BUFMAX;
    }
    else{
      bufsize = size;
    }

    // Limpando buffer
    for (i=0; i<BUFMAX; i++){
      buf[i] = 0;
    }

    // Recebendo os dados da serial e colocando no buffer
    // Recebe até encontrar ENTER ou encher o buffer
    Serial.readBytesUntil(0x0A, buf, bufsize);
    
    // Lendo o restante dos caracteres (flush)
    while(Serial.available()){
      Serial.read();  // Esse read nao faz nada, só tira os chars do buffer
    }

 }

 

 
 
 

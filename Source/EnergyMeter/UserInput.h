/*************************************
 * UserInput.h -- Rotinas para entrar 
 * com strings
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

 #ifndef USER_INPUT_H_
 #define USER_INPUT_H_

 // Defines

 // Tamanho do buffer - 50 caracteres
 #define BUFMAX 50

 // Protótipos

 void initSerial();
 
 void getUserInput(char *buf, unsigned int size);

 #endif

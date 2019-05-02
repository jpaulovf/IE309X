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

/* ------------------------------------- *
 * Defines                               *
 * ------------------------------------- */

// Tamanho do buffer - 50 caracteres
#define BUFMAX 50

/* ------------------------------------- *
 * Protótipos                            *
 * ------------------------------------- */

/*
 * Inicializa a porta serial 
 *  param
 *    void
 *  return
 *    void
 */
void initSerial();

/*
 * Recebe entrada do usuário 
 *  param [in/out] buf
 *    ponteiro para o buffer de recepção
 *  param [in] size 
 *    tamanho do buffer passado
 *  return
 *    void
 */
void getUserInput(char *buf, unsigned int size);

#endif

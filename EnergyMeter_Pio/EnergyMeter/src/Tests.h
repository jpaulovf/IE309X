/*************************************
 * Tests.h - Testes unitários
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

#ifndef TESTS_H_
#define TESTS_H_

/* ------------------------------------- *
 * Defines                               *
 * ------------------------------------- */

/* ------------------------------------- *
 * Protótipos                            *
 * ------------------------------------- */

/*
 * Testa o armazenamento na EEPROM
 *  param
 *    void
 *  return
 *    void
 */
void testStorage();

/*
 * Testa a comunicação com o AD7758
 *  param
 *    void
 *  return
 *    void
 */
void testAD7758();
 

#endif

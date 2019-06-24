/*************************************
 * run.h - Função principal
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

// Modo debug (verbose)
#define DEBUGMODE 0

// Constante W.h/LSB
#define WHLSB 0.000172F

// Pino de interrupção do ADE
#define ADE_IRQ_PIN 26

// Intervalo de medição do Blynk em segundos
#define BLYNK_MEAS_TIMER 10000L

// Timeout em segundos
#define TOUT_S 10

void run();


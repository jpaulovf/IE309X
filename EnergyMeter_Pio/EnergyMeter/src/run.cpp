/*************************************
 * run.cpp - Função principal
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

#include <run.h>

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <ADE7758.h>
#include <DataStorage.h>
#include <RGBLed.h>

#include <cstdint>

// Variáveis globais
ADE7758Device *ade = new ADE7758Device();   // Dispositivo ADE7758 nos pinos padrão
BlynkTimer btimer;  // Timer para enviar dados ao Blynk
uint32_t errorCounter;
portMUX_TYPE syncMux = portMUX_INITIALIZER_UNLOCKED;   // Mux usado para sincronização com as ISRs

// Parâmetros da conexão
// @todo usar SmartConfig + Memória Interna
char auth[] = "a4079471987f4abd9c2a25ad8543e485";
char ssid[] = "AndroidAPDBD3";
char pass[] = "rdow3020";

// Flags
volatile uint8_t ade_ready;
volatile uint8_t tout_flag;

// Valores W.h acumulados durante um certo período
float awh_acc;
float bwh_acc;
float cwh_acc;

// Interrupção de aquisição de kW.h
void IRAM_ATTR IRQ_WH_Handler(){

    portENTER_CRITICAL_ISR(&syncMux);
    ade_ready = 1;
    portEXIT_CRITICAL_ISR(&syncMux);

}

// Interrupção do timer de timeout
void IRAM_ATTR onTimeout(){

    portENTER_CRITICAL_ISR(&syncMux);
    tout_flag = 1;
    portEXIT_CRITICAL_ISR(&syncMux);

}

// Rotina de envio periódico dos dados (float) medidos pelo ADE
// accWhx é o consumo acumulado em W.h da fase X
void timerEvent(){

    Blynk.virtualWrite(V0, awh_acc);
    Blynk.virtualWrite(V1, bwh_acc);
    Blynk.virtualWrite(V2, cwh_acc);

    awh_acc = 0;
    bwh_acc = 0;
    cwh_acc = 0;

}

// Função de configuração do ADE7758
static void ADEConfig(){

    // Variáveis para armazenar os dados dos registradores
    uint8_t data8;
    uint16_t data16;
    uint32_t data24;

    // Zerando ACPFNUM e ACPFDEN
    data16 = 0x0000;
    ade->write16(REG_APCFNUM, data16);
    ade->write16(REG_APCFDEN, data16);

    // Zerando xWG
    ade->write16(REG_AWG, data16);
    ade->write16(REG_BWG, data16);
    ade->write16(REG_CWG, data16);

    // Selecionando a fase
    data8 = 0xFC;   // Fase A
    ade->write8(REG_MMODE, data8);

    // Configurando o modo line accumulation cycle
    data8 = 0x09;
    ade->write8(REG_LCYCMODE, data8);

    // Configurando LINECYC para 500 ciclos
    data16 = 0x01F4;
    ade->write16(REG_LINECYC, data16);

    // Configurando a máscara de interrupções
    data24 = 0x001000;
    ade->write24(REG_MASK, data24);

}

// Função principal
void run(){

    // Valores de W.h "crus" para as 3 fases
    uint16_t awh_raw; 
    uint16_t bwh_raw; 
    uint16_t cwh_raw;

    // Valores convertidos para W.h
    float awh;
    float bwh;
    float cwh;

    // LED RGB
    RGBLed led(32, 25, 33);

    // Timer de timeout
    hw_timer_t *timeoutTimer = NULL;

    // Inicializando os valores acumulados
    awh_acc = 0;
    bwh_acc = 0;
    cwh_acc = 0;

    // Inicializando LED
    led.write(1,1,0);   // Amarelo

    // Inicializando o Blynk
    Blynk.begin(auth, ssid, pass);

    // Inicializando Serial
    Serial.begin(9600);

    // Inicializando o timer do Blynk
    btimer.setInterval(BLYNK_MEAS_TIMER, timerEvent);

    // Inicializando o timer de timeout
    timeoutTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(timeoutTimer, &onTimeout, true);
    timerAlarmWrite(timeoutTimer, TOUT_S*1000000, true);

    // Zerando o flag de interrupções
    portENTER_CRITICAL(&syncMux);
    ade_ready = 0;
    portEXIT_CRITICAL(&syncMux);

    // Zerando o flag de interrupções de timeout
    portENTER_CRITICAL(&syncMux);
    tout_flag = 0;
    portEXIT_CRITICAL(&syncMux);

    // Configurando os pinos e interrupções
    pinMode(ADE_IRQ_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ADE_IRQ_PIN), IRQ_WH_Handler, FALLING);

    // Configurando o ADE
    ADEConfig();

    // Rodando o Blynk e o seu timer
    Blynk.run();
    btimer.run();

    // Mudando o LED
    led.write(0, 1, 0); // Verde

    // Loop principal
    while (1){

        // Limpando o registrador de interrupções do ade
        ade->read24(REG_RSTATUS);

        // Espera pelo flag de medição
        while (ade_ready == 0 && tout_flag == 0){
             timerAlarmEnable(timeoutTimer);
        }     

        timerAlarmDisable(timeoutTimer);

        // Zerando o flag de interrupções de timeout
        portENTER_CRITICAL(&syncMux);
        tout_flag = 0;
        portEXIT_CRITICAL(&syncMux);

        // Zerando o flag de interrupções
        portENTER_CRITICAL(&syncMux);
        ade_ready = 0;
        portEXIT_CRITICAL(&syncMux);

        // Lendo as medições
        awh_raw = ade->read16(REG_AWATTHR);
        bwh_raw = ade->read16(REG_BWATTHR);
        cwh_raw = ade->read16(REG_CWATTHR);

        // Convertendo para W.h
        awh = ((float) awh_raw) * WHLSB;
        bwh = ((float) bwh_raw) * WHLSB;
        cwh = ((float) cwh_raw) * WHLSB;

        // Acumulando
        awh_acc += awh;
        bwh_acc += bwh;
        cwh_acc += cwh;

    }

}
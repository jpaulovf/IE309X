/*************************************
 * run.cpp - Função principal
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

/*-----------------------------------------------------
 *  Includes
 *-----------------------------------------------------*/

#include <run.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ADE7758.h>
#include <DataStorage.h>
#include <RGBLed.h>
#include <cstdint>


/*-----------------------------------------------------
 *  Globais
 *-----------------------------------------------------*/

ADE7758Device *ade = new ADE7758Device();               // Dispositivo ADE7758 nos pinos padrão
BlynkTimer btimer;                                      // Timer para enviar dados ao Blynk
portMUX_TYPE syncMux = portMUX_INITIALIZER_UNLOCKED;    // Mux usado para sincronização com as ISRs
int blynkSliderValue;                                   // Valor lido no Slider do Blynk
WidgetLED led_low(V14);                                 // LED verde do Blynk
WidgetLED led_wrn(V15);                                 // LED amarelo do Blynk   
WidgetLED led_hgh(V16);                                 // LED vermelho do Blynk

// Parâmetros da conexão
// @todo usar SmartConfig + Memória Interna
char auth[] = "a4079471987f4abd9c2a25ad8543e485";
char ssid[] = "TP-LINK_LAB-LSMO";
char pass[] = "sensores-lsmo";

// Flags
volatile uint8_t ade_ready;
volatile uint8_t tout_flag;

// Valores W.h acumulados durante um certo período
float awh_acc;
float bwh_acc;
float cwh_acc;

/*-----------------------------------------------------
 *  Funções
 *-----------------------------------------------------*/

// Interrupção de aquisição de kW.h
void IRAM_ATTR IRQ_WH_Handler(){

    Serial.println("Measurement complete");

    portENTER_CRITICAL_ISR(&syncMux);
    ade_ready = 1;
    portEXIT_CRITICAL_ISR(&syncMux);

}

// Interrupção do timer de timeout
void IRAM_ATTR onTimeout(){

    Serial.println("Timed out");

    portENTER_CRITICAL_ISR(&syncMux);
    tout_flag = 1;
    portEXIT_CRITICAL_ISR(&syncMux);

}

// Função de leitura do slider
BLYNK_WRITE(V10){
    blynkSliderValue = param.asInt();
    Serial.print("Slider value = ");
    Serial.println(blynkSliderValue);
}

// Rotina de envio periódico dos dados (float) medidos pelo ADE
// accWhx é o consumo acumulado em W.h da fase X
void blynkTimerEvent(){

    Serial.println("Sending data to WiFi");

    // Enviando para o gráfico
    Blynk.virtualWrite(V0, awh_acc);

    // Enviando para o indicador de valor
    Blynk.virtualWrite(V9, awh_acc);

    // Comparando o valor com o Slider e atualizando os LEDs
    if (awh_acc < blynkSliderValue - 50){
        led_low.on();
        led_wrn.off();
        led_hgh.off();
    }
    else if ( (awh_acc >= blynkSliderValue - 50) && (awh_acc < blynkSliderValue) ){
        led_low.off();
        led_wrn.on();
        led_hgh.off();
    }
    else{
        led_low.off();
        led_wrn.off();
        led_hgh.on();
    }

}

// Função de configuração do ADE7758
static void ADEConfig(){

    // Variáveis para armazenar os dados dos registradores
    uint8_t data8;
    uint16_t data16;
    uint32_t data24;

    Serial.println("Configuring ADE...");

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

    Serial.println("Done!");

}

// Função principal
void run(){

    // Valores de W.h "crus" para a fase A
    uint16_t awh_raw; 

    // Valores convertidos para W.h
    float awh;

    // LED RGB
    RGBLed led(32, 25, 33);

    // Timer de timeout
    hw_timer_t *timeoutTimer = NULL;

    // Inicializando Serial
    Serial.begin(9600);
    Serial.println("Running the CODE");

    // Inicializando os valores acumulados
    awh_acc = 0;

    // Inicializando LED
    led.write(1,1,0);   // Amarelo

    // Inicializando o Blynk
    Serial.println("Initializing Blynk...");
    Blynk.begin(auth, ssid, pass);
    Blynk.virtualWrite(V9, 0);      // Iniciando o indicador de kW.h em zero
    Serial.println("Done!");

    // Inicializando o timer do Blynk (10 s)
    btimer.setInterval(BLYNK_MEAS_TIMER, blynkTimerEvent);

    // Inicializando o timer de timeout (10 s)
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

    // Mudando o LED
    led.write(0, 1, 0); // Verde

    // Loop principal
    while (1){

        // Rodando o Blynk e o seu timer
        Blynk.run();
        btimer.run();

        // Zerando o valor de W.h lido pelo ADE
        awh = 0;

        // Limpando o registrador de interrupções do ade
        ade->read24(REG_RSTATUS);

        // Espera pelo flag de medição
        Serial.println("Waiting for interrupt flag...");
        #if DEBUGMODE == 1
            delay(2000);
        #else
        while (ade_ready == 0 && tout_flag == 0){
             timerAlarmEnable(timeoutTimer);
        }   
        #endif  

        timerAlarmDisable(timeoutTimer);

        // Zerando o flag de interrupções
        portENTER_CRITICAL(&syncMux);
        ade_ready = 0;
        portEXIT_CRITICAL(&syncMux);

        // Se não houve timeout, atualizar os valores
        if (tout_flag == 0){
            #if DEBUGMODE ==0
            // Lendo as medições
            awh_raw = ade->read16(REG_AWATTHR);

            // Convertendo para W.h
            awh = ((float) awh_raw) * WHLSB;

            // Convertendo para kW.h
            awh = awh/1000;
            #else
                awh = 10;
            #endif
        }

        // Zerando o flag de interrupções de timeout
        portENTER_CRITICAL(&syncMux);
        tout_flag = 0;
        portEXIT_CRITICAL(&syncMux);

        Serial.print("awh = ");
        Serial.print(awh);
        Serial.println(" kW.h");

        // Acumulando o valor de kW.h lido
        awh_acc += awh;

        Serial.print("awh_acc = ");
        Serial.print(awh_acc);
        Serial.println(" kW.h");

    }

}
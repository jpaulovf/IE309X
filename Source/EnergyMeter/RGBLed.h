/*************************************
 * RGBLed.h - Rotinas do LED RGB
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

 #ifndef LIB_RGB_LED_H
 #define LIB_RBG_LED_H

 class RGBLed{

  private:
    int _rpin;
    int _gpin;
    int _bpin;
    int _rval; // Valor R
    int _gval; // Valor G
    int _bval; // Valor B

  public:
    // Construtor
    RGBLed(int pin_r, int pin_g, int pin_b);   

    // Setar os valores do LED RGB
    void write(int r, int g, int b);

 };

 #endif

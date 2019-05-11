/*************************************
 * RGBLed.cpp - Rotinas do LED RGB
 * -----------------------------------
 * 
 * autor: jpaulovf@gmail.com
 * 
 *************************************/

 #include "RGBLed.h"
 #include <Arduino.h>

 /* ----------------------------------
  *  Métodos
  * ---------------------------------- */

 // Construtor
 RGBLed::RGBLed(int pin_r, int pin_g, int pin_b){

    _rpin = pin_r;
    _gpin = pin_g;
    _bpin = pin_b;
    _rval = LOW;
    _gval = LOW;
    _bval = LOW;
    

    pinMode(pin_r, OUTPUT);
    pinMode(pin_g, OUTPUT);
    pinMode(pin_b, OUTPUT);

    digitalWrite(_rpin, _rval);
    digitalWrite(_gpin, _gval);
    digitalWrite(_bpin, _bval);
        
 }

 // Setar os valores do LED RGB
 void RGBLed::write(int r, int g, int b){

    _rval = r;
    _gval = g;
    _bval = b;

    digitalWrite(_rpin, _rval);
    digitalWrite(_gpin, _gval);
    digitalWrite(_bpin, _bval);
  
 }

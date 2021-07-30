/* 
 * File:   dev_config.h
 * Author: Lin
 *
 * Created on 2021?7?20?, ??4:56
 */



#ifndef DEV_CONFIG_H
#define	DEV_CONFIG_H

#include "p32xxxx.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/*
 * DEV_CLK_PIN // ???
 * 
 */

#define DEV_CS_PIN  0 //LATDbits.LATD1 // control signal
#define DEV_DC_PIN  1 //LATDbits.LATD2 // command:0 data:1
#define DEV_RST_PIN 2 //LATDbits.LATD3 // reset, put 1
//#define DEV_BL_PIN  LATDbits.LATD4 // backlight

void DEV_SPI_Init(void);
    
void DEV_Pin_Init(void);

void DEV_Digital_Write(int pin, UBYTE value);
//void DEV_Digital_Read(UBYTE pin);

/**
 * SPI
**/
void DEV_SPI_WRITE(UBYTE dat);

/**
 * delay x ms
**/
void DEV_Delay_ms(UBYTE xms);
    
void DEV_Delay_us(UBYTE xus);

/**
 * PWM_BL
**/
//void DEV_Set_PWM(_Value)  analogWrite(DEV_BL_PIN, _Value)


#endif	/* DEV_CONFIG_H */


/* 
 * File:   main.c
 * Author: Lin
 *
 * Created on 2021?7?29?, ??1:42
 */

#include <stdio.h>
#include <stdlib.h>

#include "lcd_driver.h"
#include "dev_config.h"
#include "GUI.h"

#pragma config FCKSM = CSECME

void setSYSCLK80MHzAndPBDIV(uint8_t n){
    SYSKEY = 0x0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    OSCCONbits.PLLODIV = 0b000; // PLL output divisor = 1
    OSCCONbits.PBDIV = n;    // PMCLK = SYSCLK / 4
    OSCCONbits.PLLMULT = 0b101; // PLL output multiplier = 20
    OSCCONbits.NOSC = 0b001;    // FRCPLL
    OSCCONbits.OSWEN = 0b1;
    while(OSCCONbits.OSWEN);
    SYSKEY = 0x0;
}

/*
 * 
 */
int main() {
    setSYSCLK80MHzAndPBDIV(0b10);
    
    init_LCD();
    
    //DEV_Pin_Init();
    //DEV_SPI_Init();
    //asm("ei");
    //LCD_Init();
    //LATDbits.LATD6 = 1;
    //LCD_Clear(0x0000);
    //LATDbits.LATD6 = 0;
    //DEV_Delay_ms(200);
    
    UWORD color = 0x0000;
    
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    while(1){
        //if (count++ == 5){
        //    break;
        //}
        
        LATDINV = 0x40;
        //LCD_ClearWindow(50, 50, 200, 200, color);
        
        //DEV_Delay_ms(200);
        //write_char(100, 50, 'a');
        //write_val(100, 100, count0);
        //count0 += 1;
        //write_val(100, 150, count1);
        //count1 += 2;
        //write_val(100, 200, count2);
        //count2 += 4;
        //write_val(100, 250, count3);
        //count3 += 8;
        draw_color_block(40, 50, 180, 190, color);
        //draw_color_block(40, 0, 300, 230, color);
        color = rand() % 0x10000;
        write_str(200, 90, "Sic", color);
        write_str(200, 110, "mundus", color);
        write_str(200, 130, "creatus", color);
        write_str(200, 150, "est", color);
        color = rand() % 0x10000;
        DEV_Delay_ms(200);
        
    }
    
    
    
    
    
}


#include <xc.h>
#include "utils.h"

//#pragma interrupt adcISR ipl2 vector 27 // 27 for ADC1

int adcVal;

//void adcISR(){
//
//    IFS1bits.AD1IF = 0;
//}

void init_ADC(){
    TRISBSET = 1;
    
    AD1CON1 = 0x04E4;
    AD1CON2 = 0x0002;
    AD1CON3 = 0x0818;
    AD1CHS = 0x00000000;
    AD1PCFG = 0xFFFE;
    /*
     * ADCS=24
     * sample = 8
     * convert = 12
     * 8 buffer mode
     * SMPI = 0
     */
    
    //IPC6bits.AD1IP = 2;
    //IPC6bits.AD1IS = 0;
    //IFS1bits.AD1IF = 0;
    //IEC1bits.AD1IE = 1;
    
}

int read_ADC_val() {
    AD1CON1SET = 0x8000;
    while (!IFS1bits.AD1IF){
        if(AD1CON2bits.BUFS == 1){
            adcVal = ADC1BUF0;
        } else {
            adcVal = ADC1BUF8;
        }
    }
    AD1CON1CLR = 0x8000;
    IFS1bits.AD1IF = 0;
    return adcVal;
}
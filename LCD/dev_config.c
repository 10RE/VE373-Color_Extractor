
#include "dev_config.h"



void initTimerAndOC() {
    T2CON = 0x0000; // prescale = 1
    PR2 = 0x1387; // period = 5000
    TMR2 = 0;
    
    OC5CON = 0x0000;
    OC5R = 0x0;
    OC5RS = 0x0;
    OC5CON = 0x0006;
    T2CONSET = 0x8000;
    OC5CONSET = 0x8000;
}

void DEV_SPI_Init(){
    int rData;
    IEC0CLR=0x03800000;
    SPI1CON = 0x0;
    rData=SPI1BUF;
    SPI1BRG=0x0;
    SPI1STATCLR=0x40;
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.CKP = 0;
    SPI1CONbits.SMP = 0;
    //SPI1CONbits.DISSDO = 1;
    SPI1CONbits.CKE = 1;
    SPI1CONbits.ON = 1;
    
}

void DEV_Pin_Init() {
    INTCONbits.MVEC = 1;
    //TRISDCLR = 0x0E;
    TRISDCLR = 0xFF;
    //LATDCLR = 0x0E;
    LATDCLR = 0xFF;
    
    delay_init();
    
    initTimerAndOC();
    
    OC5RS = 4000;
    
    
}

void DEV_Digital_Write(int pin, UBYTE value) {
    if (value)
        LATDSET = 1 << (pin + 1);
    else
        LATDCLR = 1 << (pin + 1);
}

void DEV_SPI_WRITE(UBYTE dat) {
    UBYTE tmp;
    SPI1BUF = dat;
    //DEV_Delay_us(1);
    while(SPI1STATbits.SPITBF){}
    tmp = SPI1BUF;
    
}

void DEV_Delay_ms(UBYTE xms) {
    delay_ms(xms);
}


void DEV_Delay_us(UBYTE xus) {
    delay_us(xus);
}


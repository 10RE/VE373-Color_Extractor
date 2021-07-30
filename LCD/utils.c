
#include "utils.h"

#define CLK_FREQ_MS 20000 // 20Mhz / 1000 (1ms)
#define CLK_FREQ_US 20 // 20Mhz / 1000000 (1us)

uint8_t timer_n, timer_flag = 0;

/* timer interrupt handler */ 
#pragma interrupt T3_ISR ipl3AUTO vector 12
void T3_ISR (void) {
    IFS0bits.T3IF = 0;
    timer_n--;
    if(timer_n > 0)
        return;
    T3CONCLR = 0x8000;
    TMR3 = 0x0;
    timer_flag = 1;
}

void delay_init() {
    T3CON = 0x0;
    TMR3 = 0x0;

    IPC3bits.T3IP = 3;
    IPC3bits.T3IS = 0;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
}
    
void delay_ms(uint8_t xms) {
    timer_n = xms;
    timer_flag = 0;
    PR3 = CLK_FREQ_MS;
    T3CONSET = 0x8000;
    while (!timer_flag) {}
}


void delay_us(uint8_t xus) {
    timer_n = xus;
    timer_flag = 0;
    PR3 = CLK_FREQ_US;
    T3CONSET = 0x8000;
    while (!timer_flag) {}
}

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

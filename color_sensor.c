#include <stdio.h>
#include <stdlib.h>
#include "color_sensor.h"
#include "utils.h"
#include "dev_config.h"
#include "lcd_driver.h"

#define SENSOR_ADDR 0x29

#define R_CO_C 0.02465
#define R_CO_R 0.00465
#define R_CO_G -0.0611
#define R_CO_B -0.0128
#define R_INTE -11.911
#define G_CO_C -0.2058
#define G_CO_R 0.18774
#define G_CO_G 0.31938
#define G_CO_B 0.15645
#define G_INTE -42.349
#define B_CO_C 0.08618
#define B_CO_R -0.0927
#define B_CO_G -0.1753
#define B_CO_B 0.10713
#define B_INTE 57.202

void wait4Nms(int n){
    int pr4Backup = PR4;
    int t4conBackup = T4CON;
    T4CON = 0x0040;
    TMR4 = 0x0;
    PR4 = 4999;
    T4CONSET = 0x8000;
    int i;
    for(i = 0; i < n; i++){
        while(!IFS0bits.T4IF);
        IFS0bits.T4IF = 0;
    }
    T4CON = 0;
    TMR4 = 0;
    PR4 = pr4Backup;
    T4CON = t4conBackup;
}

uint8_t readOneByte(uint8_t addr){
    // PBCLK = 20MHz
    I2C2BRG = 0x060;
    // Baud Rate 100kHz
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.ON = 1;
    I2C2CONbits.SEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = SENSOR_ADDR << 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = 0b10000000 | addr;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.RSEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = (SENSOR_ADDR << 1) | 0b1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.RCEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.PEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    if(I2C2STATbits.RBF == 1){
        return I2C2RCV;
    }
    
    return 0xFF;
}

void readNBytes(uint8_t startAddr, int n, uint8_t* result){
    // PBCLK = 20MHz
    I2C2BRG = 0x060;
    // Baud Rate 100kHz
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.ON = 1;
    I2C2CONbits.SEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = SENSOR_ADDR << 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = 0b10100000 | startAddr;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.RSEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = (SENSOR_ADDR << 1) | 0b1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    int i;
    for(i = 0; i < n - 1; i++){
        I2C2CONbits.RCEN = 1;
        while(!IFS1bits.I2C2MIF);
        IFS1bits.I2C2MIF = 0;
    
        I2C2CONbits.ACKDT = 0;
        I2C2CONbits.ACKEN = 1;
        result[i] = I2C2RCV;
        while(!IFS1bits.I2C2MIF);
        IFS1bits.I2C2MIF = 0;
    }
    I2C2CONbits.RCEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
    result[n - 1] = I2C2RCV;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.PEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
}

void writeOneByte(uint8_t addr, uint8_t value){
    // PBCLK = 20MHz
    I2C2BRG = 0x060;
    // Baud Rate 100kHz
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.ON = 1;
    I2C2CONbits.SEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = SENSOR_ADDR << 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = 0b10000000 | addr;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2TRN = value;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
    
    I2C2CONbits.PEN = 1;
    while(!IFS1bits.I2C2MIF);
    IFS1bits.I2C2MIF = 0;
}

void getRGB(int* R, int* G, int* B){
    writeOneByte(0x01, 0xF6);
    writeOneByte(0x03, 0xAB);
    writeOneByte(0x0F, 0x03);
    writeOneByte(0x00, 0b00001011);
    delay_ms(40);
    uint8_t bytes[9];
    readNBytes(0x13, 9, bytes);
    writeOneByte(0x01, 0b00000000);
    int cValue, rValue, gValue, bValue;
    cValue = (bytes[2] << 8) + bytes[1];
    rValue = (bytes[4] << 8) + bytes[3];
    gValue = (bytes[6] << 8) + bytes[5];
    bValue = (bytes[8] << 8) + bytes[7];
    double rDouble, gDouble, bDouble;
    rDouble = R_CO_C * cValue + R_CO_R * rValue + R_CO_G * gValue + R_CO_B * bValue + R_INTE;
    gDouble = G_CO_C * cValue + G_CO_R * rValue + G_CO_G * gValue + G_CO_B * bValue + G_INTE;
    bDouble = B_CO_C * cValue + B_CO_R * rValue + B_CO_G * gValue + B_CO_B * bValue + B_INTE;
    if(rDouble < 0){
        *R = 0;
    } else if(rDouble > 255){
        *R = 255;
    } else {
        *R = (int)rDouble;
    }
    if(gDouble < 0){
        *G = 0;
    } else if(gDouble > 255){
        *G = 255;
    } else {
        *G = (int)gDouble;
    }
    if(bDouble < 0){
        *B = 0;
    } else if(bDouble > 255){
        *B = 255;
    } else {
        *B = (int)bDouble;
    }
}
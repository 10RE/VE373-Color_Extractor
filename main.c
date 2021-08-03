#include <stdio.h>
#include <stdlib.h>

#include "GUI.h"
#include "utils.h"
#include "color_sensor.h"
#include "ADC.h"

#pragma config FCKSM = CSECME

// TODO: define corresponding buttons
#define resetButton PORTEbits.RE4
#define confirmButton PORTEbits.RE5
#define selectButton PORTEbits.RE6
#define plusButton PORTEbits.RE7
#define minusButton PORTGbits.RG14

#define inkLevelMulti 

#define c_id 0
#define m_id 1
#define y_id 2
#define k_id 3

int init_button() {
    LATECLR = 0xFF;
    LATGbits.LATG14 = 0;
    TRISECLR = 0xF;
    TRISESET = 0xF0;
    TRISGbits.TRISG14 = 1;
    
}

void rgb2cmyk(const int r, const int g, const int b, int *c, int *m, int *y, int *k) {
    int rgb_max = max(max(r, b), g);
    *c = (int) (100 * (-(double)r + (double)rgb_max) / (double)rgb_max);
    *m = (int) (100 * (-(double)g + (double)rgb_max) / (double)rgb_max);
    *y = (int) (100 * (-(double)b + (double)rgb_max) / (double)rgb_max);
    *k = (int) (100 * (1.0 - (rgb_max / 255.0)));
}

void release_ink(const int val, int id){
    LATESET = 0x1 << id;
    delay_ms(val);
    //delay_ms(1000);
    LATECLR = 0x1 << id;
}

int main(){
    setSYSCLK80MHzAndPBDIV(0b10);
    init_LCD();
    init_ADC();
    init_button();
    int resetFlag = 0;
    while(1){
        // Init State
        if(resetFlag == 1){
            resetFlag = 0;
        }
        LCD_Clear(0x0000);
        write_str(40, 100, "C", 0xFC10);
        write_str(60, 100, "o", 0xFFF0);
        write_str(80, 100, "l", 0x87F0);
        write_str(100, 100, "o", 0x87FF);
        write_str(120, 100, "r", 0xFC1F);
        write_str(140, 100, " Extractor", 0xFFFF);
        write_str(40, 180, "You can adjust", 0xFFFF);
        write_str(40, 200, "screen brightness", 0xFFFF);
        write_str(40, 220, "with knob", 0xFFFF);
        //LCD_Clear(0x0000);
        while(!confirmButton){
            int adc_val = read_ADC_val();
            set_LCD_brightness(adc_val);
#if 0
            char adc_str[20];
            sprintf(adc_str, "%d", adc_val);
            write_str(80, 100, adc_str, 0xFFFF);
            delay_ms(10);
            write_str(80, 100, "    ", 0xFFFF);
            delay_ms(10);
#endif
        }
        
        // Color Extraction State
        int R, G, B, inkLevel;
        inkLevel = 10;
        getRGB(&R, &G, &B);
        //int i = 0;
        //for (i = 0; i < 3; i ++) {
        //    inkLevel = read_ADC_val() / 100.0 + 1;
        //}
        // TODO: Read ADC into inkLevel
        //inkLevel = (inkLevel / 100) + 1;
        //inkLevel = 5; // only for test
        char str[100];
        LCD_Clear(0x0000);
        draw_interface(R, G, B, inkLevel);
        int adjustingColor = 0;
        write_str(115, 35 + 30 * adjustingColor, "<", 0xFFFF);
        while(1){
            if(resetButton){
                delay_ms(300);
                if (resetButton) {
                    resetFlag = 1;
                    break;
                }
                else {
                    getRGB(&R, &G, &B);
                    draw_interface(R, G, B, inkLevel);
                }
            }
            if(selectButton){
                draw_color_block(115, 30 + 30 * adjustingColor, 135, 65 + 30 * adjustingColor, 0x0000, 0);
                adjustingColor = (adjustingColor + 1) % 3;
                write_str(115, 35 + 30 * adjustingColor, "<", 0xFFFF);
                while(selectButton);
                delay_ms(1);
                while(selectButton);
            }
            if(plusButton | minusButton){
                int count = 0;
                while(plusButton | minusButton){
                    count++;
                    int increment = count > 5 ? 10 : 1;
                    switch(adjustingColor){
                        case 0:
                            R = plusButton ? (R + increment > 255 ? 255 : R + increment) : (R - increment < 0 ? 0 : R - increment);
                            //draw_color_block(45, 30, 105, 60, 0x0000, 0);
                            write_str(45, 35, "     ", 0x0000);
                            sprintf(str, "%d", R);
                            write_str(45, 35, str, 0xF800);
                            break;
                        case 1:
                            G = plusButton ? (G + increment > 255 ? 255 : G + increment) : (G - increment < 0 ? 0 : G - increment);
                            //draw_color_block(45, 60, 105, 90, 0x0000, 0);
                            write_str(45, 65, "     ", 0x0000);
                            sprintf(str, "%d", G);
                            write_str(45, 65, str, 0x07E0);
                            break;
                        case 2:
                            B = plusButton ? (B + increment > 255 ? 255 : B + increment) : (B - increment < 0 ? 0 : B - increment);
                            //draw_color_block(45, 90, 105, 120, 0x0000, 0);
                            write_str(45, 95, "     ", 0x0000);
                            sprintf(str, "%d", B);
                            write_str(45, 95, str, 0x001F);
                            break;
                    }
                    draw_color_block(200, 80, 280, 160, ((R>>3)<<11) + ((G>>2)<<5) + ((B>>3)), 1);
                    T4CON = 0x0060; // PBCLK DIV by 64
                    PR4 = 62499;    // period = 62500, freq = 5Hz
                    TMR4 = 0;
                    T4CONSET = 0x8000;
                    while(!IFS0bits.T4IF){
                        if(!plusButton & !minusButton){
                            break;
                        }
                    }
                    T4CONCLR = 0x8000;
                    PR4 = 0;
                    IFS0bits.T4IF = 0;
                }
            }
            if(confirmButton){
                break;
            }
            // TODO: Read ADC
            int newInkLevel = 0;
            int i = 0;
            for (i = 0; i < 5; i ++) {
                newInkLevel += read_ADC_val() / 100;
#if 0
                char adc_str[20];
                sprintf(adc_str, "%d", inkLevel);
                write_str(80, 100, adc_str, 0xFFFF);
                delay_ms(10);
#endif
            }
            newInkLevel = newInkLevel / 5 + 1;
            if (newInkLevel == 11){
                newInkLevel -= 1;
            }
            if(newInkLevel != inkLevel){
                //draw_color_block(85, 120, 105, 155, 0x0000, 0);
                write_str(85, 125, "     ", 0xFFFF);
                sprintf(str, "%d", newInkLevel);
                write_str(85, 125, str, 0xFFFF);
                inkLevel = newInkLevel;
                //delay_ms(10);
            }
        }
        if(resetFlag == 1){
            continue;
        }
        
        // Inking State
        LCD_Clear(0x0000);
        write_str(40, 100, "Releasing Ink...", 0xFFFF);
        
        //break;
        // TODO: Inking...............
        int C, M, Y, K;
        rgb2cmyk(R, G, B, &C, &M, &Y, &K);
        write_str(30, 150, "C: ", 0x07FF);
        sprintf(str, "%d", C);
        write_str(75, 150, str, 0x07FF);
        write_str(130, 150, "M: ", 0xF81F);
        sprintf(str, "%d", M);
        write_str(175, 150, str, 0xF81F);
        write_str(30, 170, "Y: ", 0xFFE0);
        sprintf(str, "%d", Y);
        write_str(75, 170, str, 0xFFE0);
        write_str(130, 170, "K: ", 0xFFFF);
        sprintf(str, "%d", K);
        write_str(175, 170, str, 0xFFFF);
        write_str(30, 190, "Ink: ", 0xFFFF);
        sprintf(str, "%d", inkLevel);
        write_str(90, 190, str, 0xFFFF);
        
        //write_str(200, 175, "id: ", 0xFFFF);
        release_ink(C * inkLevel, c_id);
        delay_ms(100);
        //sprintf(str, "%d", c_id);
        //write_str(250, 175, str, 0xFFFF);
        release_ink(M * inkLevel, m_id);
        delay_ms(100);
        //sprintf(str, "%d", m_id);
        //write_str(250, 175, str, 0xFFFF);
        release_ink(Y * inkLevel, y_id);
        delay_ms(100);
        //sprintf(str, "%d", y_id);
        //write_str(250, 175, str, 0xFFFF);
        release_ink(K * inkLevel, k_id);
        delay_ms(100);
        //sprintf(str, "%d", k_id);
        //write_str(250, 175, str, 0xFFFF);
        
        while(!confirmButton){
            
            int newInkLevel = 0;
            int i = 0;
            for (i = 0; i < 5; i ++) {
                newInkLevel += read_ADC_val() / 100;
            }
            newInkLevel = newInkLevel / 5 + 1;
            if (newInkLevel == 11){
                newInkLevel -= 1;
            }
            if(newInkLevel != inkLevel){
                //draw_color_block(85, 120, 105, 155, 0x0000, 0);
                write_str(75, 190, "     ", 0xFFFF);
                sprintf(str, "%d", newInkLevel);
                write_str(75, 190, str, 0xFFFF);
                inkLevel = newInkLevel;
                //delay_ms(10);
            }
            
            if (resetButton) {
                delay_ms(300);
                if (resetButton){
                    //while (resetButton) {}
                    break;
                }
                else {
                    release_ink(C * inkLevel, c_id);
                    delay_ms(100);
                    release_ink(M * inkLevel, m_id);
                    delay_ms(100);
                    release_ink(Y * inkLevel, y_id);
                    delay_ms(100);
                    release_ink(K * inkLevel, k_id);
                    delay_ms(100);
                }
            }   
        }
        delay_ms(500);
        
    }
//    init_LCD();
//    TRISECLR = 0xFF;
//    LATE = 0x0;
//    while(1){
//        int R, G, B;
//        getRGB(&R, &G, &B);
//        delay_ms(10000);
//    }
}

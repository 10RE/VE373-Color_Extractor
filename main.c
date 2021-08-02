#include <stdio.h>
#include <stdlib.h>

#include "GUI.h"
#include "utils.h"
#include "color_sensor.h"

#pragma config FCKSM = CSECME

// TODO: define corresponding buttons
#define resetButton 0
#define confirmButton 1
#define selectButton 0
#define plusButton 0
#define minusButton 1

int main(){
    setSYSCLK80MHzAndPBDIV(0b10);
    init_LCD();
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
        while(!confirmButton){
            // TODO: Read ADC highest sample rate
            // TODO: Adjust OC
        }
        
        // Color Extraction State
        int R, G, B, inkLevel;
        getRGB(&R, &G, &B);
        // TODO: Read ADC into inkLevel
        //inkLevel = (inkLevel / 100) + 1;
        inkLevel = 5; // only for test
        LCD_Clear(0x0000);
        char str[100];
        write_str(5, 35, "R:", 0xF800);
        sprintf(str, "%d", R);
        write_str(45, 35, str, 0xF800);
        write_str(5, 65, "G:", 0x07E0);
        sprintf(str, "%d", G);
        write_str(45, 65, str, 0x07E0);
        write_str(5, 95, "B:", 0x001F);
        sprintf(str, "%d", B);
        write_str(45, 95, str, 0x001F);
        write_str(5, 125, "Ink:", 0xFFFF);
        sprintf(str, "%d", inkLevel);
        write_str(85, 125, str, 0xFFFF);
        draw_color_block(200, 80, 280, 160, ((R>>3)<<11) + ((G>>2)<<5) + ((B>>3)), 1);
        int adjustingColor = 0;
        write_str(115, 35 + 30 * adjustingColor, "<", 0xFFFF);
        while(1){
            if(resetButton){
                resetFlag = 1;
                break;
            }
            if(selectButton){
                draw_color_block(115, 30 + 30 * adjustingColor, 135, 65 + 30 * adjustingColor, 0x0000, 0);
                adjustingColor = (adjustingColor + 1) % 3;
                write_str(115, 35 + 30 * adjustingColor, "<", 0xFFFF);
            }
            if(plusButton | minusButton){
                int count = 0;
                while(plusButton | minusButton){
                    count++;
                    int increment = count > 5 ? 10 : 1;
                    switch(adjustingColor){
                        case 0:
                            R = plusButton ? (R + increment > 255 ? 255 : R + increment) : (R - increment < 0 ? 0 : R - increment);
                            draw_color_block(45, 30, 105, 60, 0x0000, 0);
                            sprintf(str, "%d", R);
                            write_str(45, 35, str, 0xF800);
                            break;
                        case 1:
                            G = plusButton ? (G + increment > 255 ? 255 : G + increment) : (G - increment < 0 ? 0 : G - increment);
                            draw_color_block(45, 60, 105, 90, 0x0000, 0);
                            sprintf(str, "%d", G);
                            write_str(45, 65, str, 0x07E0);
                            break;
                        case 2:
                            B = plusButton ? (B + increment > 255 ? 255 : B + increment) : (B - increment < 0 ? 0 : B - increment);
                            draw_color_block(45, 90, 105, 120, 0x0000, 0);
                            sprintf(str, "%d", B);
                            write_str(45, 95, str, 0x001F);
                            break;
                    }
                    draw_color_block(200, 80, 280, 160, ((R>>3)<<11) + ((G>>2)<<5) + ((B>>3)), 1);
                    delay_ms(200);
                }
            }
            if(confirmButton){
                break;
            }
            // TODO: Read ADC
            int newInkLevel = 10;
            if(newInkLevel != inkLevel){
                draw_color_block(85, 120, 105, 155, 0x0000, 0);
                sprintf(str, "%d", newInkLevel);
                write_str(85, 125, str, 0xFFFF);
                inkLevel = newInkLevel;
                delay_ms(500);
            }
        }
        if(resetFlag == 1){
            continue;
        }
        
        // Inking State
        LCD_Clear(0x0000);
        write_str(40, 100, "Releasing Ink...", 0xFFFF);
        break;
        // TODO: Inking...............
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

#include <stdio.h>
#include <stdlib.h>

#include "lcd_driver.h"
#include "dev_config.h"
#include "GUI.h"
#include "utils.h"
#include "color_sensor.h"

#pragma config FCKSM = CSECME

int main(){
    setSYSCLK80MHzAndPBDIV(0b10);
    
    init_LCD();
    TRISECLR = 0xFF;
    LATE = 0x0;
    while(1){
        int R, G, B;
        getRGB(&R, &G, &B);
        delay_ms(10000);
    }
}

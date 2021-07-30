
#include "GUI.h"

void init_LCD(){
    DEV_Pin_Init();
    DEV_SPI_Init();
    asm("ei");
    LCD_Init();
    LCD_Clear(0x0000);
}

void draw_color_block(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end, const uint16_t color){
    uint16_t xls = LCD_WIDTH - y_end - 1;
    uint16_t yls = x_start;
    uint16_t xle = LCD_WIDTH - y_start - 1;
    uint16_t yle = x_end;
    
    uint16_t tmp;
    for (tmp = xls; tmp <= xle; tmp ++) {
        LCD_DrawPaint(tmp, yls, 0xFFFF);
        LCD_DrawPaint(tmp, yls + 1, 0xFFFF);
        LCD_DrawPaint(tmp, yle, 0xFFFF);
        LCD_DrawPaint(tmp, yle - 1, 0xFFFF);
    }   
    for (tmp = yls; tmp <= yle; tmp ++){
        LCD_DrawPaint(xls, tmp, 0xFFFF);
        LCD_DrawPaint(xls + 1, tmp, 0xFFFF);
        LCD_DrawPaint(xle, tmp, 0xFFFF);
        LCD_DrawPaint(xle - 1, tmp, 0xFFFF);
    }
    
    LCD_ClearWindow(xls + 5, yls + 5, xle - 3, yle - 3, color);
}

void write_val(const uint16_t x, const uint16_t y, const uint8_t val) {
    if (x > LCD_WIDTH || y > LCD_HEIGHT) {
        return;
    }
    uint16_t fw = Font20.Width;
    uint8_t n2 = (val / 100);
    uint8_t n1 = (val - n2 * 100) / 10;
    uint8_t n0 = (val - n2 * 100 - n1 * 10);
    write_char(x, y, n2 + '0');
    write_char(x + fw, y, n1 + '0');
    write_char(x + 2 * fw, y, n0 + '0');
    return;
}

void write_str(const uint16_t x, const uint16_t y, const char *str, const UWORD color) {
    if (x > LCD_WIDTH || y > LCD_HEIGHT) {
        return;
    }
    uint16_t fw = Font20.Width;
    int pos = 0;
    for (pos = 0; pos < strlen(str); pos ++){
        write_char(x + pos * fw, y, str[pos], color);
    }
    return;
}

void write_char(const uint16_t x, const uint16_t y, const char val, const uint16_t color) {
    const uint8_t *ft = Font20.table;
    uint16_t fw = 16;
    uint16_t fh = 20;
    uint32_t s_addr = (val - 32) * 2 * 20;
    uint16_t yl = 0;
    for (yl = 0; yl < fh; yl ++) {
        uint16_t xl = 0;
        for (xl = 0; xl < fw; xl ++) {
            if (ft[s_addr + xl / 8] & (0x80 >> (xl % 8))) {
                LCD_DrawPaint(LCD_WIDTH - y - yl, x + xl, color);
            }
            else {
                LCD_DrawPaint(LCD_WIDTH - y - yl, x + xl, 0x0000);
            }
        }
        s_addr += 2;
    }
}
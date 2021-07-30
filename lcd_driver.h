/* 
 * File:   lcd_driver.h
 * Author: Lin
 *
 * Created on 2021?7?20?, ??4:52
 */

#ifndef LCD_DRIVER_H
#define	LCD_DRIVER_H

#include "dev_config.h"
#include <p32xxxx.h>

#define LCD_WIDTH   240 //LCD width
#define LCD_HEIGHT  320 //LCD height
    
void LCD_WriteData_Word(UWORD da);

void LCD_SetCursor(UWORD X, UWORD Y);
void LCD_SetWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
void LCD_DrawPaint(UWORD x, UWORD y, UWORD Color);

void LCD_Init(void);
void LCD_SetBackLight(UWORD Value);

void LCD_Clear(UWORD Color);
void LCD_ClearWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD color);

#endif	/* LCD_DRIVER_H */


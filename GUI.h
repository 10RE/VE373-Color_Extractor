/* 
 * File:   GUI.h
 * Author: Lin
 *
 * Created on 2021?7?30?, ??11:15
 */

#ifndef GUI_H
#define	GUI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "lcd_driver.h"
#include "dev_config.h"
#include "font.h"

void init_LCD();

void draw_interface(const uint16_t R, const uint16_t G, const uint16_t B, const uint16_t inkLevel);

void set_LCD_brightness(const uint16_t val);
    
void draw_color_block(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end, const uint16_t color, int frame);

void clear_LCD(const uint16_t color);

void write_str(const uint16_t x, const uint16_t y, const char *str, const UWORD color);
    
void write_val(const uint16_t x, const uint16_t y, const uint8_t val);

#endif	/* GUI_H */


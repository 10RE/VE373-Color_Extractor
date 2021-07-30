/* 
 * File:   utils.h
 * Author: Lin
 *
 * Created on 2021?7?30?, ??4:51
 */

#ifndef UTILS_H
#define	UTILS_H

#include <stdint.h>
#include <p32xxxx.h>

void delay_init();
    
void delay_ms(int xms);
    
void delay_us(int xus);

void setSYSCLK80MHzAndPBDIV(uint8_t n);

#endif	/* UTILS_H */


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

#ifdef	__cplusplus
extern "C" {
#endif

    void delay_init();
    
    void delay_ms(uint8_t xms);
    
    void delay_us(uint8_t xus);



#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */


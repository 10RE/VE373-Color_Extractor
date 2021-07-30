/* 
 * File:   font.h
 * Author: Lin
 *
 * Created on 2021?7?30?, ??11:02
 */

#ifndef FONT_H
#define	FONT_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

extern sFONT Font20;


#ifdef	__cplusplus
}
#endif

#endif	/* FONT_H */


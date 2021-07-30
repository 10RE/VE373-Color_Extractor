/* 
 * File:   font.h
 * Author: Lin
 *
 * Created on 2021?7?30?, ??11:02
 */

#ifndef FONT_H
#define	FONT_H

#include <stdint.h>

typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

extern sFONT Font20;


#endif	/* FONT_H */


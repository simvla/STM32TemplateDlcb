/*C Util functions lib by simvla*/

#ifndef _UTIL_H
#define _UTIL_H

#include <stdint.h>

/* UTIL lib functions -------------------------------------------------*/
char* uitoa(uint16_t n);
void strclr(char* str);
char* addDecPoint(uint16_t num);
uint16_t strToInt(char* str);
uint8_t searchStr(char*,char*);

#endif // _UTIL_H

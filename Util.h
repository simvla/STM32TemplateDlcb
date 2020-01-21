/*C Util functions lib by simvla*/

#ifndef _UTIL_H
#define _UTIL_H

#include "inttypes.h"
#include "string.h"

#ifdef __cplusplus
extern "C"{
#endif

// Public func:
char* uitoa(uint16_t n);
void strclr(char* str);
uint8_t addchl(char* str,char c,uint8_t len);
char* addDecPoint(uint16_t num);
uint16_t strToInt(char* str);
uint8_t searchStr(char*,const char*);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _UTIL_H

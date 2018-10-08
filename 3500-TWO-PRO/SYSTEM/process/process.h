
#ifndef __PROCESS_H_
#define __PROCESS_H_ 1

#include "stm32f4xx.h"

void CharToStr(u8 wei,u8 *str,u8 ivar);
void SPuti(u8 wei,u8 *str,u16 ivar);
void SPutl(u8 wei,u8 *str,u32 lvar);
void SPutf(const u8 *format,u8 *str,float fvar);

#endif /*__PROCESS_H_*/

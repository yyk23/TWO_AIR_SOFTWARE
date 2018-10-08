#ifndef __18B20_H__
#define	__18B20_H__	1

extern u8 wk_temperature_err;

void DS18b20_reset(u8 * pre);
void Temperature_read(float * tarray);
void DS18b20_init(void);
#endif


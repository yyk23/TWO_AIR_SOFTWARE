#ifndef __SAVERESUME_H__
#define	__SAVERESUME_H__	1

#include "sys.h"

void SavePara(u16 offset);
void ReadPara(u16 offset);
void ParaLimit(void);
void ParaInit(void);
//void ParaBackup(void);
void reset_para(void);
void save_sample_mode(void);
void save_sample_para(void);
#endif


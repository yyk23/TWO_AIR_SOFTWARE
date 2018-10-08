
#ifndef __FILE_H__
#define __FILE_H__
#include "datatype.h"

extern uint W25_erase_sector;
void FileInit(void);
void SaveSampleFile_TSP(void);
void ReadSampleFile_TSP(uint NumFile);
void SaveSampleFile_A(void);
void ReadSampleFile_A(uint NumFile);
void SaveSampleFile_B(void);
void ReadSampleFile_B(uint NumFile);
void SavePowerFile_TSP(void);
void ReadPowerFile_TSP(uint NumFile);
void SavePowerFile_A(void);
void ReadPowerFile_A(uint NumFile);
void SavePowerFile_B(void);
void ReadPowerFile_B(uint NumFile);
void SaveRunFile_TSP(void);
void ReadRunFile_TSP(uint NumFile);
void DeleteAllSampleFiles(void);
void DeleteAllRunFile(void);
void DeletAllPowerFile(void);
void test_file_write(void);
void DeleteAllFiles(void);
#endif

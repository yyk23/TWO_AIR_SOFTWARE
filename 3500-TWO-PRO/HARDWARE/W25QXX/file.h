
#ifndef __FILE_H__
#define __FILE_H__
//W25Q128共128Mbits(16Mbytes,4096个扇区，每扇区16页，65536页，每页256字节)
//注：以下定义的是页地址，地址必须是16的整数倍，每个页可以存储两个文件
#define         DEF_FIRST_START_FLAG_W25               				0         	//首次上电标志
#define         DEF_POWER_FILE_FIRST_START_PAGE_W25               	160         //
#define         DEF_POWER_FILE_SECOND_START_PAGE_W25              	1600        //预留了2000组掉电记录的地址
#define         DEF_POWER_FILE_LENGTH_W25           				16          //
#define         DEF_POWER_FILE_MAX_W25              				1000        //掉电记录数1000组
#define         DEF_SAMPLE_FILE_FIRST_START_PAGE_W25              	3200       	//
#define         DEF_SAMPLE_FILE_SECOND_START_PAGE_W25             	13200       //预留了20000组采样文件的地址
#define         DEF_SAMPLE_FILE_LENGTH_W25         					40          //
#define         DEF_SAMPLE_FILE_MAX_W25             				9999        //采样文件数9999组


extern u16 W25_erase_sector;
void FileInit(void);
void ReadSampleFile_AB(u16 NumFile);
void SaveSampleFile_A(void);
void SaveSampleFile_B(void);
void ReadPowerFile_AB(u16 NumFile);
void SavePowerFile_A(void);
void SavePowerFile_B(void);
void DeleteAllSampleFiles(void);
void DeletAllPowerFile(void);
void test_file_write(void);
void DeleteAllFiles(void);
#endif

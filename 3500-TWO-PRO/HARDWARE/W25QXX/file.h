
#ifndef __FILE_H__
#define __FILE_H__
//W25Q128��128Mbits(16Mbytes,4096��������ÿ����16ҳ��65536ҳ��ÿҳ256�ֽ�)
//ע�����¶������ҳ��ַ����ַ������16����������ÿ��ҳ���Դ洢�����ļ�
#define         DEF_FIRST_START_FLAG_W25               				0         	//�״��ϵ��־
#define         DEF_POWER_FILE_FIRST_START_PAGE_W25               	160         //
#define         DEF_POWER_FILE_SECOND_START_PAGE_W25              	1600        //Ԥ����2000������¼�ĵ�ַ
#define         DEF_POWER_FILE_LENGTH_W25           				16          //
#define         DEF_POWER_FILE_MAX_W25              				1000        //�����¼��1000��
#define         DEF_SAMPLE_FILE_FIRST_START_PAGE_W25              	3200       	//
#define         DEF_SAMPLE_FILE_SECOND_START_PAGE_W25             	13200       //Ԥ����20000������ļ��ĵ�ַ
#define         DEF_SAMPLE_FILE_LENGTH_W25         					40          //
#define         DEF_SAMPLE_FILE_MAX_W25             				9999        //�����ļ���9999��


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

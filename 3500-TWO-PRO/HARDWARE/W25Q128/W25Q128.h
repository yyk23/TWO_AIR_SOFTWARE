
#ifndef __W25Q128_H__
#define __W25Q128_H__
#include "datatype.h"

#define         DEF_FIRST_START_FLAG_W25               				0         	//�״��ϵ��־
#define         DEF_POWER_FILE_FIRST_START_PAGE_W25               	160         //�����ļ���ʼҳ(ҳ)
#define         DEF_POWER_FILE_SECOND_START_PAGE_W25              	1600        //�����ļ���ʼҳ(ҳ)
#define         DEF_POWER_FILE_LENGTH_W25           				16          //�����ļ�����(�ֽ�)ÿҳ256�ֽڴ洢2���ļ�
#define         DEF_POWER_FILE_MAX_W25              				1000        //�����ļ���(��)
#define         DEF_SAMPLE_FILE_FIRST_START_PAGE_W25              	3200       	//�����ļ���ʼҳ(ҳ)
#define         DEF_SAMPLE_FILE_SECOND_START_PAGE_W25             	4800       	//�����ļ���ʼҳ(ҳ)
#define         DEF_SAMPLE_FILE_LENGTH_W25         					40          //�����ļ�����(�ֽ�)ÿҳ256�ֽڴ洢2���ļ�
#define         DEF_SAMPLE_FILE_MAX_W25             				1000        //�����ļ���(��)
#define 		DEF_RUN_FILE_FIRST_START_PAGE_W25					8000		//˲ʱ�ļ���ʼҳ(ҳ)
#define 		DEF_RUN_FILE_SECOND_START_PAGE_W25					32000		//˲ʱ�ļ���ʼҳ(ҳ)
#define         DEF_RUN_FILE_LENGTH_W25          					22          //˲ʱ�ļ�����(�ֽ�)ÿҳ256�ֽڴ洢2���ļ�
#define         DEF_RUN_FILE_MAX_W25             					30000       //˲ʱ�ļ���(��)



void W25_Init(void);
void SPI_WR_Byte(u8 dat);

u8 SPI_RD_Byte(void);

/********************************************
** ��ȡSPI_FLASH��״̬�Ĵ���              ***
** BIT  6  5  4   3   2   1   0           ***
** SPR  RV TB BP2 BP1 BP0 WEL BUSY        ***
** SPR: Ĭ��0,״̬�Ĵ�������λ,���WPʹ�� ***
** TB,BP2,BP1,BP0:FLASH����д��������     ***
** WEL:дʹ������                         ***
** BUSY:æ���λ(1,æ;0,����)             ***
** Ĭ��:0x00                              ***
*********************************************/
u8 SPI_RD_Reg(void);

//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SPI_WR_SR(u8 sr);

//SPI FLASHдʹ��
//��WEL��λ
void SPI_WR_EN(void);

//SPI FLASHд��ֹ
//��WEL����
void SPI_WR_DIS(void);

//��ȡоƬID 
u16 SPI_ReadID(void);

//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SPI_Data_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead);

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ
//NumByteToWrite:Ҫд����ֽ���(���256),�������ܳ�����ҳ��ʣ���ֽ���
void SPI_DATA_WR_Page(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//�������дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0xFF,�����ڷ�0xFF��д������ݽ�ʧ��
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_Write_NoCheck(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
void SPI_Flash_Write(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite);

//��������оƬ
//��Ƭ����ʱ��:
//W25x16:25s
//W25x32:40s
//W25x64:40s
//�ȴ�ʱ�䳬��
void SPI_Flash_Erase_Chip(void);

//����һ������
//Dst Addr:������ַ 0~511 for w25x16 �˴������
//����һ����������ʱ��:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr);

/* �ȴ�оƬ���� */
void SPI_Wait_Busy(void);

//�������ģʽ
void SPI_Flash_PowerDown(void);

//����оƬ
void SPI_Flash_WakeUp(void);


#endif

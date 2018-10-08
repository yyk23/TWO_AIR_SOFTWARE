#ifndef __PRINT_H__
#define	__PRINT_H__	1


#define BLUETOOTH_LED 			PEin(15)
#define BLUETOOTH_KEY 			PEout(14)
#define BLUETOOTH_POWER_ON();	PBout(11) = 0;
#define BLUETOOTH_POWER_OFF();	PBout(11) = 1;
#define DF_PRINTER_PIN			0
extern u8 nDot;
extern u8 PrintErr;
extern u16 printer_password;
extern u8 bluetooth_ok_flag;
void PrintC(u8 num);
void PrintHZStr(u8 *pstr);
void InitPrinter(void);
//void USART_Init(void);
void PrintStart(void);
void PrintData(u16 FileNo);
void PrintSetUp(void);
u8 PrinterTest(void);
void PrintPower(u16 FileNo);
u8 printer_connect(void);
void bluetooth_init(void);
u8 bluetooth_set_master(void);
u8 bluetooth_set_printer_password(u16 password_tmp);
void UART2_SendStr(u8 * TxStr);
#endif


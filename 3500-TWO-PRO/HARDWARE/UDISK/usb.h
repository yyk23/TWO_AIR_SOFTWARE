#ifndef __USB_H__
#define	__USB_H__	1

#define DF_FILE_HEAD_LILE	1
#define DF_FILE_CONTENT		2
#define DF_FILE_CLOSED		3
extern u32 NewSize;
extern u16 total;
extern u8 FILE_DATA_BUF[712];
extern u8 	testbuf[200];
extern u8 usb_out_file_step;

u8 FileWrite(void);
u8 LoadFileBuffer(u8 * pFile,u8 flag);

void SampleFileOut(u16 start_file,u16 end_file);
void PowerFileOut(u16 start_file,u16 end_file);
void RunFileOut(u16 start_file,u16 end_file);
void SampleFileOut_CSV(u16 start_file,u16 end_file);
void PowerFileOut_CSV(u16 start_file,u16 end_file);
void RunFileOut_CSV(u16 start_file,u16 end_file);
#endif


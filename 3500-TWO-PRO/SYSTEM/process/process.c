
//--------include------------
#include "process.h"
#include "stm32f4xx.h"
//----------------------------------------
//        c��������ַ�������      (��С����)      
//���: λ��:wei; �ַ���:*str  ����:ivar  
//----------------------------------------
void CharToStr(u8 wei,u8 *str,u8 ivar) 
{
  u8 i,num=0;
  u8 intbuf;   // ���������� 
  intbuf = (u8)ivar;// ������     
  str[wei] = 0;       // �ַ������� 
  num = wei;
  for(i=0;i<wei;i++){
    
	num --;
	str[num] = intbuf % 10 + 0x30;// ��һ���ַ� 
    intbuf = intbuf / 10;         // ��һλ     
    }// for 
}


//----------------------------------------
//        ����������ַ�������            
//���: λ��:wei; �ַ���:*str  ����:ivar  
//----------------------------------------
void SPuti(u8 wei,u8 *str,u16 ivar) 
{
  u8 i,num=0;
  u16 intbuf;   // ���������� 

  intbuf = (u16)ivar;// ������     
  str[wei] = 0;       // �ַ������� 
  num = wei;
  for(i=0;i<wei;i++){
    num --;
    str[num] = intbuf % 10 + 0x30;// ��һ���ַ� 
    intbuf = intbuf / 10;         // ��һλ     
    }// for 
}

void SPutl(u8 wei,u8 *str,u32 lvar) 
{
  u8 i,num=0;
  u32 lbuf;   // ���������� 

  lbuf = (u32)lvar;// ������     
  str[wei] = 0;       // �ַ������� 
  num = wei;
  for(i=0;i<wei;i++){
    num --;
    str[num] = lbuf % 10 + 0x30;// ��һ���ַ� 
    lbuf = lbuf / 10;         // ��һλ     
    }// for 
}



//--------------------------------------------
// ������������ַ���                         
// ���: ˵�� *format, �ַ���:*str ����:fvar  
//--------------------------------------------
void SPutf(const u8 *format,u8 *str,float fvar)
{ 
  u8 a,b,i,num=0;
  unsigned long longbuf; // ����������   

  if (*(format) =='+'){
    if (fvar<0){ 
      fvar = -fvar;    // ������ת������ֵ  
      str[num]='-';   // �Ӹ���            
      }
    else{ 
      str[num]='+'; // ������            
      }
    num ++;
    format ++;
    }
  a = *(format) - 0x30;    // ȡ�ܳ���a     
  format ++;
  b = *(format) - 0x30;    // ȡС����λ��b 
  for (i=0;i<b;i++){
    fvar = fvar * 10;    // ����С��      
    }
  fvar = fvar + (float)0.50;     // ��������      
  num = num + a;
  if (b>0){ 
    num ++;                  // ��С����  
    }
  longbuf = (unsigned long)fvar;// ������ 
  str[num] = 0;             // �ַ������� 
  for(i=1;i<=a;i++){
    num --;
    str[num] = longbuf % 10 + 0x30; // ��һ���ַ�  
    longbuf = longbuf / 10;         // ��һλ      
    if (i==b){ 
      num --;
      str[num] = '.';               // ��С����    
      }
    }
}


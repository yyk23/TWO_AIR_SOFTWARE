/************************************************
              �������궨��
************************************************/
#ifndef _DEFINE_H_
#define _DEFINE_H_
/*page 1  �Լ�ҳ��   ��ַ����  0x0001--0x0040*/
#define DW_PAGE1_BIANHAO_ADD         0x0001   //�������       ����8
#define DW_PAGE1_BANBEN_ADD          0x0009   //�汾���       ����8
#define DW_PAGE1_ZIJIAZHONG_ADD      0x0011    //�Լ��� ��ͼ��ʾ��ַ  
#define DW_PAGE1_GUZHANG_ADD         0x0030    //��ʾ������Ϣ��ַ    ����30


//page 2  ������   
/////�����궨��
#define DW_KEY_ADD                         0x0040    //�������������� ��ַ
#define DW_KEY_ZHUYE_SHEZHI_DATA           0x0001    //��ҳ�����ð�����ֵ
#define DW_KEY_ZHUYE_CAIYANG_DATA          0x0002    //��ҳ�����������ֵ
#define DW_KEY_ZHUYE_WEIHUI_DATA           0x0003    //��ҳ��ά��������ֵ
#define DW_KEY_ZHUYE_CHAXUN_DATA           0x0004    //��ҳ���ѯ������ֵ
#define DW_KEY_ZHUYE_DAYIN_DATA            0x0005    //��ҳ���ӡ������ֵ
#define DW_KEY_ZHUYE_DAOCHU_DATA           0x0006    //��ҳ�浼��������ֵ
#define DW_KEY_ZHUYE_LANYA_DATA            0x0007    //��ҳ������������ֵ

//��ʾ�궨��
#define DW_WENBEN_TIMEANDDATA_ADD          0x0041     //��ʾʱ�������
#define DW_WENBEN_ATM_ADD                  0x005F     //��ʾ����ѹ
#define DW_WENBEN_ATM_UNIT_ADD             0x0065     //��ʾ����ѹ��λ kPa
#define DW_WENBEN_HUANWEN_ADD              0x0068     //��ʾ�����¶�
#define DW_WENBEN_HUANWEN_UNIT_ADD         0x006D     //��ʾ�¶ȵ�λ ��

//��ͼ
#define DW_ICON_LANYA                      0x006F      //��������ͼ����ͼ��ʾ��ַ



//page 6  ϵͳ����
/////�����궨��
//#define DW_KEY_ADD                         0x0040        //�������������� ��ַ
#define DW_KEY_SHEZHI_HUANWEN_CE_DATA            0x0008    //���ý��滷���¶Ȳ�������
#define DW_KEY_SHEZHI_HUANWEN_SHU_DATA           0x0009    //���ý��滷���¶����밴��
#define DW_KEY_SHEZHI_ATM_CE_DATA                0x000a    //���ý������ѹ��������
#define DW_KEY_SHEZHI_ATM_SHU_DATA               0x000b    //���ý������ѹ���밴��
#define DW_KEY_SHEZHI_WENKONG1_ON_DATA           0x000c    //���ý����¿�1�򿪰���
#define DW_KEY_SHEZHI_WENKONG1_OFF_DATA          0x000d    //���ý����¿�1�رհ���
#define DW_KEY_SHEZHI_WENKONG2_ON_DATA           0x000e    //���ý����¿�2�򿪰���
#define DW_KEY_SHEZHI_WENKONG2_OFF_DATA          0x000f    //���ý����¿�3�رհ���
#define DW_KEY_SHEZHI_FAN_HOME_DATA              0x0010    //�������˵�����

//��ʾ�궨��
#define DW_WENBEN_HUANWEN_SHE_ADD                0x00FB     //��ʾ�����еĻ����¶�
#define DW_WENBEN_ATM_SHE_ADD                    0x0101     //��ʾ�����еĴ���ѹ
#define DW_WENBEN_WENKONG1_SHE_ADD               0x0083     //��ʾ�����е��¿�1�¶�
#define DW_WENBEN_WENKONG2_SHE_ADD               0x0089     //��ʾ�����е��¿�2�¶�
#define DW_WENBEN_BEIGUANG_SHE_ADD               0x008f     //��ʾ�����еı���ʱ��
#define DW_WENBEN_NIAN_ADD                       0x0094     //��ʾ���� ��
#define DW_WENBEN_YUE_ADD                        0x0096     //��ʾ���� ��
#define DW_WENBEN_RI_ADD                         0x0098     //��ʾ���� ��
#define DW_WENBEN_SHI_ADD                        0x009a     //��ʾʱ�� ʱ
#define DW_WENBEN_FEN_ADD                        0x009c     //��ʾʱ�� ��
#define DW_WENBEN_MIAO_ADD                       0x009e     //��ʾʱ�� ��
//����¼���ַ
#define DW_DATA_HUANWEN_SHE                      0x00f0     //���뻷���¶ȵ�ַ
#define DW_DATA_ATM_SHE                          0x00f1     //�������ѹ��ַ
#define DW_DATA_WENKONG1_SHE                     0x00f2     //�����¿�1�¶ȵ�ַ
#define DW_DATA_WENKONG2_SHE                     0x00f3     //�����¿�2�¶ȵ�ַ
#define DW_DATA_BEIGUANG_SHE                     0x00f4     //���뱳��ʱ���ַ
#define DW_DATA_NIAN_SHE                         0x00f5     //�������ַ
#define DW_DATA_YUE_SHE                          0x00f6     //�����µ�ַ
#define DW_DATA_RI_SHE                           0x00f7     //�����յ�ַ
#define DW_DATA_SHI_SHE                          0x00f8     //����ʱ��ַ
#define DW_DATA_FEN_SHE                          0x00f9     //����ֵ�ַ
#define DW_DATA_MIAO_SHE                         0x00fa     //�������ַ

//��ͼ
#define DW_ICON_HUANWEN_CE_OR_SHU                      0x0107      //�����¶Ȳ�������ͼ����ͼ��ʾ��ַ
#define DW_ICON_ATM_CE_OR_SHU                          0x011b      //����ѹ��������ͼ����ͼ��ʾ��ַ
#define DW_ICON_WENKONG1_ON_OR_SHU                     0x012f      //�¿�1����ͼ����ͼ��ʾ��ַ
#define DW_ICON_WENKONG2_ON_OR_SHU                     0x0143      //�¿�2��ͼ����ͼ��ʾ��ַ

//��������  A·
//#define DW_KEY_ADD                             0x0040        //�������������� ��ַ
#define DW_KEY_ALU_CAIYANG_DATA                  0x0011    //��������A·��������
#define DW_KEY_BLU_CAIYANG_DATA                  0x0012    //��������B·��������
#define DW_KEY_CLU_CAIYANG_DATA                  0x0013    //��������C·��������
#define DW_KEY_DLU_CAIYANG_DATA                  0x0014    //��������D·��������
#define DW_KEY_CAIYANG_DATA                      0x0015        //��������
#define DW_KEY_CHAXUN_DATA                       0x0016        //��ѯ����
#define DW_KEY_DAYIN_DATA                        0x0017        //��ӡ����
#define DW_KEY_DAOCHU_DATA                       0x0018        //��������
#define DW_KEY_ALU_DINGSHI_CAI_DATA                  0x0019        //A·��ʱ��������
#define DW_KEY_ALU_DINGTI_CAI_DATA                   0x001a        //A·���ݲ�������
#define DW_KEY_ALU_DINGTI_QIDONG_DATA                0x001b        //A·��ʱ��������
#define DW_KEY_ALU_LIJI_QIDONG_DATA                  0x001C        //A·������������
#define DW_KEY_ZUHE_SHEZHI_DATA                      0x001d        //��ϲ������ð���

//Blu����
#define DW_KEY_BLU_DINGSHI_CAI_DATA                  0x001e        //B·��ʱ��������
#define DW_KEY_BLU_DINGTI_CAI_DATA                   0x001f        //B·���ݲ�������
#define DW_KEY_BLU_DINGTI_QIDONG_DATA                0x0020        //B·��ʱ��������
#define DW_KEY_BLU_LIJI_QIDONG_DATA                  0x0021        //B·������������

//Clu����
#define DW_KEY_CLU_DINGSHI_CAI_DATA                  0x0022        //C·��ʱ��������
#define DW_KEY_CLU_DINGTI_CAI_DATA                   0x0023        //C·���ݲ�������
#define DW_KEY_CLU_DINGTI_QIDONG_DATA                0x0024        //C·��ʱ��������
#define DW_KEY_CLU_LIJI_QIDONG_DATA                  0x0025        //C·������������
//Dlu����
#define DW_KEY_DLU_DINGSHI_CAI_DATA                  0x0026        //D·��ʱ��������
#define DW_KEY_DLU_DINGTI_CAI_DATA                   0x0027        //D·���ݲ�������
#define DW_KEY_DLU_DINGTI_QIDONG_DATA                0x0028        //D·��ʱ��������
#define DW_KEY_DLU_LIJI_QIDONG_DATA                  0x0029        //D·������������

//A��ʾ��ַ
#define DW_WENBEN_ALU_CAI_FLOE_SHE_ADD                        0x0157     //��ʾ�����е�A·��������
#define DW_WENBEN_ALU_CAI_SHICHANG_SHI_SHE_ADD                0x015b     //��ʾ�����е�A·����ʱ�� ʱ
#define DW_WENBEN_ALU_CAI_SHICHANG_FEN_SHE_ADD                0x015d     //��ʾ�����е�A·����ʱ�� ��
#define DW_WENBEN_ALU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x015f     //��ʾ�����е�A·����ʱ�� ʱ
#define DW_WENBEN_ALU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x0161     //��ʾ�����е�A·����ʱ�� ��
#define DW_WENBEN_ALU_CAI_JIANGE_SHI_SHE_ADD                  0x0163     //��ʾ�����е�A·���ʱ�� ʱ
#define DW_WENBEN_ALU_CAI_JIANGE_FEN_SHE_ADD                  0x0165     //��ʾ�����е�A·���ʱ�� ʱ
#define DW_WENBEN_ALU_CAI_CISHU_SHE_ADD                       0x0167     //��ʾ�����е�A·��������
#define DW_WENBEN_ALU_CAI_TIJI_SHE_ADD                        0x01C2     //��ʾ�����е�A·�������

//B��ʾ��ַ
#define DW_WENBEN_BLU_CAI_FLOE_SHE_ADD                        0x01CA     //��ʾ�����е�B·��������
#define DW_WENBEN_BLU_CAI_SHICHANG_SHI_SHE_ADD                0x01CE     //��ʾ�����е�B·����ʱ�� ʱ
#define DW_WENBEN_BLU_CAI_SHICHANG_FEN_SHE_ADD                0x01D0     //��ʾ�����е�B·����ʱ�� ��
#define DW_WENBEN_BLU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x01D2     //��ʾ�����е�B·����ʱ�� ʱ
#define DW_WENBEN_BLU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x01D4     //��ʾ�����е�B·����ʱ�� ��
#define DW_WENBEN_BLU_CAI_JIANGE_SHI_SHE_ADD                  0x01D6     //��ʾ�����е�B·���ʱ�� ʱ
#define DW_WENBEN_BLU_CAI_JIANGE_FEN_SHE_ADD                  0x01D8     //��ʾ�����е�B·���ʱ�� ʱ
#define DW_WENBEN_BLU_CAI_CISHU_SHE_ADD                       0x01DA     //��ʾ�����е�B·��������
#define DW_WENBEN_BLU_CAI_TIJI_SHE_ADD                        0x01DD     //��ʾ�����е�B·�������

//C��ʾ��ַ
#define DW_WENBEN_CLU_CAI_FLOE_SHE_ADD                        0x01EC     //��ʾ�����е�C·��������
#define DW_WENBEN_CLU_CAI_SHICHANG_SHI_SHE_ADD                0x01F0     //��ʾ�����е�C·����ʱ�� ʱ
#define DW_WENBEN_CLU_CAI_SHICHANG_FEN_SHE_ADD                0x01F2     //��ʾ�����е�C·����ʱ�� ��
#define DW_WENBEN_CLU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x01F4     //��ʾ�����е�C·����ʱ�� ʱ
#define DW_WENBEN_CLU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x01F6     //��ʾ�����е�C·����ʱ�� ��
#define DW_WENBEN_CLU_CAI_JIANGE_SHI_SHE_ADD                  0x01F8     //��ʾ�����е�C·���ʱ�� ʱ
#define DW_WENBEN_CLU_CAI_JIANGE_FEN_SHE_ADD                  0x01FA     //��ʾ�����е�C·���ʱ�� ʱ
#define DW_WENBEN_CLU_CAI_CISHU_SHE_ADD                       0x01FC    //��ʾ�����е�C·��������
#define DW_WENBEN_CLU_CAI_TIJI_SHE_ADD                        0x01FF     //��ʾ�����е�C·�������
//D��ʾ��ַ
#define DW_WENBEN_DLU_CAI_FLOE_SHE_ADD                        0x020E     //��ʾ�����е�D·��������
#define DW_WENBEN_DLU_CAI_SHICHANG_SHI_SHE_ADD                0x0212     //��ʾ�����е�D·����ʱ�� ʱ
#define DW_WENBEN_DLU_CAI_SHICHANG_FEN_SHE_ADD                0x0214     //��ʾ�����е�D·����ʱ�� ��
#define DW_WENBEN_DLU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x0216     //��ʾ�����е�D·����ʱ�� ʱ
#define DW_WENBEN_DLU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x0218     //��ʾ�����е�D·����ʱ�� ��
#define DW_WENBEN_DLU_CAI_JIANGE_SHI_SHE_ADD                  0x021a     //��ʾ�����е�D·���ʱ�� ʱ
#define DW_WENBEN_DLU_CAI_JIANGE_FEN_SHE_ADD                  0x021c     //��ʾ�����е�D·���ʱ�� ʱ
#define DW_WENBEN_DLU_CAI_CISHU_SHE_ADD                       0x021e    //��ʾ�����е�D·��������
#define DW_WENBEN_DLU_CAI_TIJI_SHE_ADD                        0x0221     //��ʾ�����е�D·�������

//��ͼ
#define DW_ICON_ALU_ZHUANGTAI                           0x016a      //A·����״̬��ͼ��ʾ
#define DW_ICON_BLU_ZHUANGTAI                           0x017e      //B·����״̬��ͼ��ʾ
#define DW_ICON_CLU_ZHUANGTAI                           0x0192      //C·����״̬��ͼ��ʾ
#define DW_ICON_DLU_ZHUANGTAI                           0x01a6      //D·����״̬��ͼ��ʾ

//A����¼���ַ
#define DW_DATA_ACAI_FLOW_SHE                                    0x01BA     //����A·��������
#define DW_DATA_ACAI_SHICHANG_SHI_SHE                            0x01bb     //����A·����ʱ�� ʱ
#define DW_DATA_ACAI_SHICHANG_FEN_SHE                            0x01bc     //����A·����ʱ�� ��
#define DW_DATA_ACAI_QIDONGSHIKE_SHI_SHE                         0x01bd     //����A·��������ʱ�� ʱ
#define DW_DATA_ACAI_QIDONGSHIKE_FEN_SHE                         0x01be     //����A·��������ʱ�� ��
#define DW_DATA_ACAI_JIANGE_SHI_SHE                              0x01bf     //����A·��������ʱ�� ʱ
#define DW_DATA_ACAI_JIANGE_FEN_SHE                              0x01c0     //����A·�������ʱ�� ��
#define DW_DATA_ACAI_CISHU_SHE                                   0x01c1     //����A·��������
#define DW_DATA_ACAI_TIJI_FEN_SHE                                0x01c9     //����A·�������
//B����¼���ַ
#define DW_DATA_BCAI_FLOW_SHE                                    0x01E3     //����B·��������
#define DW_DATA_BCAI_SHICHANG_SHI_SHE                            0x01e4     //����B·����ʱ�� ʱ
#define DW_DATA_BCAI_SHICHANG_FEN_SHE                            0x01e5     //����B·����ʱ�� ��
#define DW_DATA_BCAI_QIDONGSHIKE_SHI_SHE                         0x01e6     //����B·��������ʱ�� ʱ
#define DW_DATA_BCAI_QIDONGSHIKE_FEN_SHE                         0x01e7     //����B·��������ʱ�� ��
#define DW_DATA_BCAI_JIANGE_SHI_SHE                              0x01e8     //����B·��������ʱ�� ʱ
#define DW_DATA_BCAI_JIANGE_FEN_SHE                              0x01e9     //����B·�������ʱ�� ��
#define DW_DATA_BCAI_CISHU_SHE                                   0x01ea     //����B·��������
#define DW_DATA_BCAI_TIJI_FEN_SHE                                0x01eb     //����B·�������
//C����¼���ַ
#define DW_DATA_CCAI_FLOW_SHE                                    0x0205     //����C·��������
#define DW_DATA_CCAI_SHICHANG_SHI_SHE                            0x0206     //����C·����ʱ�� ʱ
#define DW_DATA_CCAI_SHICHANG_FEN_SHE                            0x0207     //����C·����ʱ�� ��
#define DW_DATA_CCAI_QIDONGSHIKE_SHI_SHE                         0x0208     //����C·��������ʱ�� ʱ
#define DW_DATA_CCAI_QIDONGSHIKE_FEN_SHE                         0x0209    //����C·��������ʱ�� ��
#define DW_DATA_CCAI_JIANGE_SHI_SHE                              0x020A     //����C·��������ʱ�� ʱ
#define DW_DATA_CCAI_JIANGE_FEN_SHE                              0x020B     //����C·�������ʱ�� ��
#define DW_DATA_CCAI_CISHU_SHE                                   0x020C    //����C·��������
#define DW_DATA_CCAI_TIJI_FEN_SHE                                0x020D     //����C·�������

//D����¼���ַ
#define DW_DATA_DCAI_FLOW_SHE                                    0x0227     //����D·��������
#define DW_DATA_DCAI_SHICHANG_SHI_SHE                            0x0228     //����D·����ʱ�� ʱ
#define DW_DATA_DCAI_SHICHANG_FEN_SHE                            0x0229     //����D·����ʱ�� ��
#define DW_DATA_DCAI_QIDONGSHIKE_SHI_SHE                         0x022a     //����D·��������ʱ�� ʱ
#define DW_DATA_DCAI_QIDONGSHIKE_FEN_SHE                         0x022b    //����D·��������ʱ�� ��
#define DW_DATA_DCAI_JIANGE_SHI_SHE                              0x022c     //����D·��������ʱ�� ʱ
#define DW_DATA_DCAI_JIANGE_FEN_SHE                              0x022d     //����D·�������ʱ�� ��
#define DW_DATA_DCAI_CISHU_SHE                                   0x022e    //����D·��������
#define DW_DATA_DCAI_TIJI_FEN_SHE                                0x022f     //����D·�������

//��ϲ�������  ����
#define DW_KEY_ZUHE_SHEZHI_A_B_DATA                   0x002a        //�������a���Ƶ�b
#define DW_KEY_ZUHE_SHEZHI_A_C_DATA                   0x002B        //�������a���Ƶ�c
#define DW_KEY_ZUHE_SHEZHI_A_D_DATA                   0x002C        //�������a���Ƶ�d
#define DW_KEY_ZUHE_SHEZHI_B_C_DATA                   0x002D        //�������b���Ƶ�c
#define DW_KEY_ZUHE_SHEZHI_B_D_DATA                   0x002E        //�������b���Ƶ�d
#define DW_KEY_ZUHE_SHEZHI_C_D_DATA                   0x002F        //�������c���Ƶ�d
#define DW_KEY_ZUHE_SHEZHI_A_DATA                     0x0030        //�������ѡ��a
#define DW_KEY_ZUHE_SHEZHI_B_DATA                     0x0031        //�������ѡ��b
#define DW_KEY_ZUHE_SHEZHI_C_DATA                     0x0032        //�������ѡ��c
#define DW_KEY_ZUHE_SHEZHI_D_DATA                     0x0033        //�������ѡ��d
#define DW_KEY_ZUHE_SHEZHI_DINGSHI_DATA               0x0034        //������� ��ʱ����
#define DW_KEY_ZUHE_SHEZHI_LIJI_DATA                  0x0035        //������� ��������
#define DW_KEY_ZUHE_SHEZHI_OFF_DATA                   0x0036        //�ر��������


//��ϲ�������  ��ͼ
#define DW_ICON_ZUHE_A                          0x0230      //A·ѡ����ͼ
#define DW_ICON_ZUHE_B                          0x0244      //b·ѡ����ͼ
#define DW_ICON_ZUHE_C                          0x0258      //c·ѡ����ͼ
#define DW_ICON_ZUHE_D                          0x026C      //d·ѡ����ͼ


//��������
#define DW_KEY_DIAODIAN_TIXIANG_SHI                   0x0037        //�����Ƿ��������  ��
#define DW_KEY_DIAODIAN_TIXIANG_FOU                   0x0038        //�����Ƿ��������  ��

//ABCD����
#define DW_WENBEN_ALU_TIAOLING_LIUYA_ADD                  0x0280     //A·������ѹ����
#define DW_WENBEN_ALU_TIAOLING_JIYA_ADD                   0x0286     //A·������ѹ����
#define DW_WENBEN_BLU_TIAOLING_LIUYA_ADD                  0x028C     //B·������ѹ����
#define DW_WENBEN_BLU_TIAOLING_JIYA_ADD                   0x0292     //B·������ѹ����
#define DW_WENBEN_CLU_TIAOLING_LIUYA_ADD                  0x0298     //C·������ѹ����
#define DW_WENBEN_CLU_TIAOLING_JIYA_ADD                   0x029E     //C·������ѹ����
#define DW_WENBEN_DLU_TIAOLING_LIUYA_ADD                  0x02A4     //D·������ѹ����
#define DW_WENBEN_DLU_TIAOLING_JIYA_ADD                   0x02AA     //D·������ѹ����

//A·���ڲ���
#define DW_WENBEN_ALU_CAI_SHUNSHI_FLOW_ADD                    0x02B0     //A·����˲ʱ����
#define DW_WENBEN_ALU_CAI_LEIJI_TIJI_ADD                      0x02B6     //A·�����ۼ����
#define DW_WENBEN_ALU_CAI_LEIJI_BIAOTI_ADD                    0x02c0     //A·�����ۼƱ���
#define DW_WENBEN_ALU_CAI_JIYA_ADD                            0x02ca     //A·�����ۼƱ���
#define DW_WENBEN_ALU_CAI_DAOJISHI_SHI_ADD                            0x02d2     //A·����ʱ ʱ
#define DW_WENBEN_ALU_CAI_DAOJISHI_FEN_ADD                            0x02d4   //A·����ʱ ��
#define DW_WENBEN_ALU_CAI_DAOJISHI_MIAO_ADD                           0x02d6    //A·����ʱ ��
#define DW_WENBEN_ALU_CAI_CISHU_N_ADD                                 0x02db    //A·��n�β���
#define DW_WENBEN_ALU_CAI_JIWEN_ADD                                   0x02de    //A·��ǰ�¶�
//B·���ڲ���
#define DW_WENBEN_BLU_CAI_SHUNSHI_FLOW_ADD                    0x02E3     //B·����˲ʱ����
#define DW_WENBEN_BLU_CAI_LEIJI_TIJI_ADD                      0x02E9     //B·�����ۼ����
#define DW_WENBEN_BLU_CAI_LEIJI_BIAOTI_ADD                    0x02F3     //B·�����ۼƱ���
#define DW_WENBEN_BLU_CAI_JIYA_ADD                            0x02FD     //B·�����ۼƱ���
#define DW_WENBEN_BLU_CAI_DAOJISHI_SHI_ADD                            0x0305     //B·����ʱ ʱ
#define DW_WENBEN_BLU_CAI_DAOJISHI_FEN_ADD                            0x0307   //B·����ʱ ��
#define DW_WENBEN_BLU_CAI_DAOJISHI_MIAO_ADD                           0x0309    //B·����ʱ ��
#define DW_WENBEN_BLU_CAI_CISHU_N_ADD                                 0x030B   //B·��n�β���
#define DW_WENBEN_BLU_CAI_JIWEN_ADD                                   0x030E    //B·��ǰ�¶�
//C·���ڲ���
#define DW_WENBEN_CLU_CAI_SHUNSHI_FLOW_ADD                    0x0313     //C·����˲ʱ����
#define DW_WENBEN_CLU_CAI_LEIJI_TIJI_ADD                      0x0319     //C·�����ۼ����
#define DW_WENBEN_CLU_CAI_LEIJI_BIAOTI_ADD                    0x0323     //C·�����ۼƱ���
#define DW_WENBEN_CLU_CAI_JIYA_ADD                            0x032D     //C·�����ۼƱ���
#define DW_WENBEN_CLU_CAI_DAOJISHI_SHI_ADD                            0x0335     //C·����ʱ ʱ
#define DW_WENBEN_CLU_CAI_DAOJISHI_FEN_ADD                            0x0337   //C·����ʱ ��
#define DW_WENBEN_CLU_CAI_DAOJISHI_MIAO_ADD                           0x0339    //C·����ʱ ��
#define DW_WENBEN_CLU_CAI_CISHU_N_ADD                                 0x033B   //C·��n�β���
#define DW_WENBEN_CLU_CAI_JIWEN_ADD                                   0x033E    //C·��ǰ�¶�

//D·���ڲ���
#define DW_WENBEN_DLU_CAI_SHUNSHI_FLOW_ADD                    0x0343     //D·����˲ʱ����
#define DW_WENBEN_DLU_CAI_LEIJI_TIJI_ADD                      0x0349     //D·�����ۼ����
#define DW_WENBEN_DLU_CAI_LEIJI_BIAOTI_ADD                    0x0353     //D·�����ۼƱ���
#define DW_WENBEN_DLU_CAI_JIYA_ADD                            0x035D     //D·�����ۼƱ���
#define DW_WENBEN_DLU_CAI_DAOJISHI_SHI_ADD                            0x0365     //D·����ʱ ʱ
#define DW_WENBEN_DLU_CAI_DAOJISHI_FEN_ADD                            0x0367   //D·����ʱ ��
#define DW_WENBEN_DLU_CAI_DAOJISHI_MIAO_ADD                           0x0369    //D·����ʱ ��
#define DW_WENBEN_DLU_CAI_CISHU_N_ADD                                 0x036B   //D·��n�β���
#define DW_WENBEN_DLU_CAI_JIWEN_ADD                                   0x036E    //D·��ǰ�¶�

//����
#define DW_KEY_ALU_STOP_CAI                                  0x0039        //A·ֹͣ����
#define DW_KEY_BLU_STOP_CAI                                  0x003a        //B·ֹͣ����
#define DW_KEY_CLU_STOP_CAI                                  0x003B        //C·ֹͣ����
#define DW_KEY_DLU_STOP_CAI                                  0x003C        //D·ֹͣ����

#define DW_KEY_ALU_JIXU_CAI                                  0x003D        //A·ֹͣ����
#define DW_KEY_BLU_JIXU_CAI                                  0x003E        //B·ֹͣ����
#define DW_KEY_CLU_JIXU_CAI                                  0x003F        //C·ֹͣ����
#define DW_KEY_DLU_JIXU_CAI                                  0x0040        //D·ֹͣ����

//A·�������
#define DW_WENBEN_ALU_JIANGE_XIACI_SHI_ADD                                   0x0371    //A·�����´β�������ʱ ʱ
#define DW_WENBEN_ALU_JIANGE_XIACI_FEN_ADD                                   0x0373    //A·�����´β�������ʱ ��
#define DW_WENBEN_ALU_JIANGE_XIACI_MIAO_ADD                                  0x0375    //A·�����´β�������ʱ ��
//B·�������
#define DW_WENBEN_BLU_JIANGE_XIACI_SHI_ADD                                   0x0377    //B·�����´β�������ʱ ʱ
#define DW_WENBEN_BLU_JIANGE_XIACI_FEN_ADD                                   0x0379    //B·�����´β�������ʱ ��
#define DW_WENBEN_BLU_JIANGE_XIACI_MIAO_ADD                                  0x037B    //B·�����´β�������ʱ ��
//C·�������
#define DW_WENBEN_CLU_JIANGE_XIACI_SHI_ADD                                   0x037D    //C·�����´β�������ʱ ʱ
#define DW_WENBEN_CLU_JIANGE_XIACI_FEN_ADD                                   0x037F    //C·�����´β�������ʱ ��
#define DW_WENBEN_CLU_JIANGE_XIACI_MIAO_ADD                                  0x0381    //C·�����´β�������ʱ ��

//D·�������
#define DW_WENBEN_DLU_JIANGE_XIACI_SHI_ADD                                   0x0383    //D·�����´β�������ʱ ʱ
#define DW_WENBEN_DLU_JIANGE_XIACI_FEN_ADD                                   0x0385    //D·�����´β�������ʱ ��
#define DW_WENBEN_DLU_JIANGE_XIACI_MIAO_ADD                                  0x0387    //D·�����´β�������ʱ ��

//����
#define DW_KEY_IF_FUZHI_B_THEN_STOP_B                                        0x0041        //B·���ڲ���  ��Ҫ��������ֹͣB
#define DW_KEY_IF_FUZHI_C_THEN_STOP_C                                        0x0042        //C·���ڲ���  ��Ҫ��������ֹͣC
#define DW_KEY_IF_FUZHI_D_THEN_STOP_D                                        0x0043        //D·���ڲ���  ��Ҫ��������ֹͣD
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_A                                   0x0044        //A·���ڲ���  ��Ҫ��·���� ����ֹͣA
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_B                                   0x0045        //B·���ڲ���  ��Ҫ��·���� ����ֹͣB
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_C                                   0x0046        //C·���ڲ���  ��Ҫ��·���� ����ֹͣC
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_D                                   0x0047        //D·���ڲ���  ��Ҫ��·���� ����ֹͣD


#define DW_KEY_STOP_A_CAI_SHI                                                0x0048        //�Ƿ����a����  ��
#define DW_KEY_STOP_A_CAI_FOU                                                0x0049        //�Ƿ����a����  ��

#define DW_KEY_STOP_B_CAI_SHI                                                0x004A        //�Ƿ����B����  ��
#define DW_KEY_STOP_B_CAI_FOU                                                0x004B        //�Ƿ����B����  ��

#define DW_KEY_STOP_C_CAI_SHI                                                0x004C        //�Ƿ����C����  ��
#define DW_KEY_STOP_C_CAI_FOU                                                0x004D        //�Ƿ����C����  ��

#define DW_KEY_STOP_D_CAI_SHI                                                0x004E        //�Ƿ����D����  ��
#define DW_KEY_STOP_D_CAI_FOU                                                0x004F        //�Ƿ����D����  ��

#define DW_KEY_STOP_CAUYANG_SHI                                              0x0050        //�Ƿ����ȫ������ ��
#define DW_KEY_STOP_CAUYANG_FOU                                              0x0051        //�Ƿ����ȫ������ ��


#define DW_KEY_ALU_CAIYANG_END_OK                                            0x0052        //A·��������ȷ��
#define DW_KEY_BLU_CAIYANG_END_OK                                            0x0053        //B·��������ȷ��
#define DW_KEY_CLU_CAIYANG_END_OK                                            0x0054        //C·��������ȷ��
#define DW_KEY_DLU_CAIYANG_END_OK                                            0x0055        //D·��������ȷ��


//��ѯ��ʾ
#define DW_WENBEN_DATA_CHAXUN_WENJIANHAO                                     0x0389     //�������ݲ�ѯ �ļ����
#define DW_WENBEN_DATA_CHAXUN_RIQI                                           0x038f     //�������ݲ�ѯ ��������
#define DW_WENBEN_DATA_CHAXUN_SHICHANG                                       0x03a3     //�������ݲ�ѯ ����ʱ��
#define DW_WENBEN_DATA_CHAXUN_HUANWEN                                        0x03bf     //�������ݲ�ѯ �����¶�
#define DW_WENBEN_DATA_CHAXUN_ATM                                            0x03ab     //�������ݲ�ѯ ����ѹ
#define DW_WENBEN_DATA_CHAXUN_FLOW                                           0x03c5     //�������ݲ�ѯ ����
#define DW_WENBEN_DATA_CHAXUN_JIQIANYA                                       0x03b1     //�������ݲ�ѯ ��ǰѹ��
#define DW_WENBEN_DATA_CHAXUN_JIQIANWEN                                      0x03ca     //�������ݲ�ѯ ��ǰ�¶�
#define DW_WENBEN_DATA_CHAXUN_LEITI                                          0x03b7     //�������ݲ�ѯ �ۼ����
#define DW_WENBEN_DATA_CHAXUN_LEIJIBIAOTI                                    0x03d0     //�������ݲ�ѯ �ۼƱ���

#define DW_WENBEN_DATA_DIAODIAN_WENJIANHAO                                   0x03ed     //�������ݲ�ѯ �ļ����
#define DW_WENBEN_DATA_DIAODIANSHIJIAN                                       0x0411     //�������� ����ʱ��
#define DW_WENBEN_DATA_LAIDIANSHIJIAN                                        0x042f     //�������� ����ʱ��
#define DW_WENBEN_DATA_LEIJIDIAODIAN                                         0x044d     //�������� �ۼƵ���


//��ͼ
#define DW_ICON_ABCD_DATA_CHAXUN                                             0x03d8     //ABCD��������
#define DW_ICON_ABCD_DATA_DIAODIAN                                           0x0458     //ABCD��������



//����
#define DW_KEY_CAIYANG_SHUJU_UP                                              0x0056        //����������һҳ
#define DW_KEY_CAIYANG_SHUJU_DOWN                                            0x0057        //����������һҳ
#define DW_KEY_CAIYANG_SHUJU_DAOCHU                                          0x0058        //��������U�̵���
#define DW_KEY_CAIYANG_SHUJU_DAYIN                                           0x0059        //�������ݴ�ӡ
#define DW_KEY_CAIYANG_SHUJU_CHAKAN                                          0x005a        //�鿴��������
#define DW_KEY_DIAODIAN_SHUJU_CHAKAN                                         0x005b        //�鿴��������

#define DW_KEY_DIAODIAN_SHUJU_UP                                              0x005c        //����������һҳ
#define DW_KEY_DIAODIAN_SHUJU_DOWN                                            0x005d        //����������һҳ
#define DW_KEY_DIAODIAN_SHUJU_DAOCHU                                          0x005e        //��������U�̵���
#define DW_KEY_DIAODIAN_SHUJU_DAYIN                                           0x005f        //�������ݴ�ӡ

//����¼��
#define DW_DATA_CHAXUN_CAIYANG_BIANHAO                                       0x03ec        //������������ļ����
#define DW_DATA_CHAXUN_DIAODIAN_BIANHAO                                      0x0457        //���������ļ����

//��ӡ ��ʾ
#define DW_WENBEN_DATA_DAYIN_CAIYANGWENJIAN_START                            0x046c     //��ӡ�����ļ� ��ʼ�ļ����
#define DW_WENBEN_DATA_DAYIN_CAIYANGWENJIAN_STOP                             0x0471     //��ӡ�����ļ� �����ļ����

#define DW_WENBEN_DATA_DAYIN_DIAODIANJILU_START                              0x0476     //��ӡ�����¼ ��ʼ�ļ����
#define DW_WENBEN_DATA_DAYIN_DIAODIANJILU_STOP                               0x047B     //��ӡ�����¼ �����ļ����

//����¼��
#define DW_DATA_DATA_DAYIN_CAIYANGWENJIAN_START                                      0x0480        //�����ӡ�����ļ� ��ʼ�ļ����
#define DW_DATA_DATA_DAYIN_CAIYANGWENJIAN_STOP                                       0x0481        //�����ӡ�����ļ� �����ļ����

#define DW_DATA_DATA_DAYIN_DIAODIANJILU_START                                        0x0482        //�����ӡ�����¼ ��ʼ�ļ����
#define DW_DATA_DATA_DAYIN_CDIAODIANJILU_STOP                                        0x0483        //�����ӡ�����¼ �����ļ����
//����
#define DW_KEY_CAIYANGWENJIAN_DAYIN                                          0x0060        //�����ļ���ӡ
#define DW_KEY_DIAODIANJILU_DAYIN                                            0x0061         //�������ݴ�ӡ


//���� ��ʾ
#define DW_WENBEN_DATA_DAOCHU_CAIYANGWENJIAN_START                            0x0484     //���������ļ� ��ʼ�ļ����
#define DW_WENBEN_DATA_DAOCHU_CAIYANGWENJIAN_STOP                             0x0489     //���������ļ� �����ļ����

#define DW_WENBEN_DATA_DAOCHU_DIAODIANJILU_START                              0x048e     //���������¼ ��ʼ�ļ����
#define DW_WENBEN_DATA_DAOCHU_DIAODIANJILU_STOP                               0x0493     //���������¼ �����ļ����

//����¼��
#define DW_DATA_DATA_DAOCHU_CAIYANGWENJIAN_START                              0x0498        //���뵼�������ļ� ��ʼ�ļ����
#define DW_DATA_DATA_DAOCHU_CAIYANGWENJIAN_STOP                               0x0499        //���뵼�������ļ� �����ļ����

#define DW_DATA_DATA_DAOCHU_DIAODIANJILU_START                                0x049a        //���뵼�������¼ ��ʼ�ļ����
#define DW_DATA_DATA_DAOCHU_CDIAODIANJILU_STOP                                0x049b        //���뵼�������¼ �����ļ����
//����
#define DW_KEY_CAIYANGWENJIAN_DAOCHU                                          0x0062        //�����ļ�����
#define DW_KEY_DIAODIANJILU_DAOCHU                                            0x0063         //�������ݵ���

//��ӡ���� ��ʾ
#define DW_WENBEN_DATA_DAYIN_JINDU                              0x049c     //��ӡ����

//�������� ��ʾ
#define DW_WENBEN_DATA_DAOCHU_JINDU                             0x04a1     //��������

//���������� ��ʾ
#define DW_WENBEN_MIMA_SHURU                                    0x04a6     //��������

 //����¼��
#define DW_DATA_DATA_MIMA                                       0x04a7        //��������


#define DW_KEY_WEUHUI_YALI_BIAODING                             0x0064//ѹ���궨����
#define DW_KEY_WEUHUI_WENDU_BIAODING                            0x0065//ѹ���궨����

#define DW_KEY_WEUHUI_FLOW_BIAODING                             0x0066//ѹ���궨����

#define DW_KEY_WEUHUI_SHEZHI_BIAODING                           0x0067//ѹ���궨����

#define DW_KEY_WEUHUI_JIAOLING_BIAODING                         0x0068//У�㰴��

//ѹ���궨
#define DW_WENBEN_ALU_LIUYA_BEILV                                     0x04a8     //A·��ѹ����
#define DW_WENBEN_BLU_LIUYA_BEILV                                     0x04ae     //B·��ѹ����
#define DW_WENBEN_CLU_LIUYA_BEILV                                     0x04b4     //C·��ѹ����
#define DW_WENBEN_DLU_LIUYA_BEILV                                     0x04ba     //D·��ѹ����
#define DW_WENBEN_ATM_BEILV                                           0x04c0     //����ѹ����

#define DW_WENBEN_ALU_LIUYA_ZERO                                      0x04c6     //A·��ѹ���
#define DW_WENBEN_BLU_LIUYA_ZERO                                      0x04cC     //B·��ѹ���
#define DW_WENBEN_CLU_LIUYA_ZERO                                      0x04D2     //C·��ѹ���
#define DW_WENBEN_DLU_LIUYA_ZERO                                      0x04D8     //D·��ѹ���
#define DW_WENBEN_ATM_ZERO                                            0x04DE     //����ѹ���


#define DW_WENBEN_ALU_JIYA_BEILV                                     0x04E4     //A·��ѹ����
#define DW_WENBEN_BLU_JIYA_BEILV                                     0x04EA     //B·��ѹ����
#define DW_WENBEN_CLU_JIYA_BEILV                                     0x04F0     //C·��ѹ����
#define DW_WENBEN_DLU_JIYA_BEILV                                     0x04F6     //D·��ѹ����

#define DW_WENBEN_ALU_JIYA_ZERO                                      0x04FC     //A·��ѹ���
#define DW_WENBEN_BLU_JIYA_ZERO                                      0x0502     //B·��ѹ���
#define DW_WENBEN_CLU_JIYA_ZERO                                      0x0508     //C·��ѹ���
#define DW_WENBEN_DLU_JIYA_ZERO                                      0x050E     //D·��ѹ���


//����¼��
#define DW_DATA_ALU_LIUYA_BEILV                                      0x050f       //����A·��ѹ����
#define DW_DATA_BLU_LIUYA_BEILV                                      0x0510       //����A·��ѹ����
#define DW_DATA_CLU_LIUYA_BEILV                                      0x0511       //����A·��ѹ����
#define DW_DATA_DLU_LIUYA_BEILV                                      0x0512       //����A·��ѹ����
#define DW_DATA_ATM_BEILV                                            0x0513       //����A·��ѹ����

#define DW_DATA_ALU_LIUYA_ZERO                                       0x0514       //����A·��ѹ���
#define DW_DATA_BLU_LIUYA_ZERO                                       0x0515       //����B·��ѹ���
#define DW_DATA_CLU_LIUYA_ZERO                                       0x0516       //����C·��ѹ���
#define DW_DATA_DLU_LIUYA_ZERO                                       0x0517       //����D·��ѹ���
#define DW_DATA_ATM_ZERO                                             0x0518       //�������ѹ���

#define DW_DATA_ALU_JIYA_BEILV                                      0x0519       //����A·��ѹ����
#define DW_DATA_BLU_JIYA_BEILV                                      0x051A       //����A·��ѹ����
#define DW_DATA_CLU_JIYA_BEILV                                      0x051B       //����A·��ѹ����
#define DW_DATA_DLU_JIYA_BEILV                                      0x051C       //����A·��ѹ����

#define DW_DATA_ALU_JIYA_ZERO                                       0x051D       //����A·��ѹ���
#define DW_DATA_BLU_JIYA_ZERO                                       0x051E       //����B·��ѹ���
#define DW_DATA_CLU_JIYA_ZERO                                       0x051F       //����C·��ѹ���
#define DW_DATA_DLU_JIYA_ZERO                                       0x0520       //����D·��ѹ���


//��ʾ
#define DW_WENBEN_ALU_JIWEN_ZERO                                       0x0533       //A·�������
#define DW_WENBEN_BLU_JIWEN_ZERO                                       0x0539       //B·�������
#define DW_WENBEN_CLU_JIWEN_ZERO                                       0x053F       //C·�������
#define DW_WENBEN_DLU_JIWEN_ZERO                                       0x0545       //D·�������

#define DW_WENBEN_BAOWEN_1_WENDU                                       0x0521       //������1�¶�
#define DW_WENBEN_BAOWEN_2_WENDU                                       0x0527       //������2�¶�
#define DW_WENBEN_HUANJING_WENDU                                       0x052d       //�����¶�

#define DW_WENBEN_BAOWEN_1_ZERO                                       0x054b       //������1���
#define DW_WENBEN_BAOWEN_2_ZERO                                       0x0551       //������2���
#define DW_WENBEN_HUANJING_WEN_ZERO                                   0x0557       //�����¶����



//����¼��
#define DW_DATA_ALU_JIWEN_ZERO                                      0x0558       //����A·�������
#define DW_DATA_BLU_JIWEN_ZERO                                      0x0559       //����B·�������
#define DW_DATA_CLU_JIWEN_ZERO                                      0x055a       //����C·�������
#define DW_DATA_DLU_JIWEN_ZERO                                      0x055b       //����D·�������

#define DW_DATA_BAOWEN_1_ZERO                                      0x055c       //���뱣����1���
#define DW_DATA_BAOWEN_2_ZERO                                      0x055d       //���뱣����2���
#define DW_DATA_HUANJING_WEN_ZERO                                  0x055e       //���뻷���¶����

//��ʾ
#define DW_WENBEN_ALU_SHE_FLOW                                       0x055f       //A·�趨����
#define DW_WENBEN_ALU_FLOE_BEILV                                     0x0565       //A·��������

#define DW_WENBEN_BLU_SHE_FLOW                                       0x056B       //B·�趨����
#define DW_WENBEN_BLU_FLOE_BEILV                                     0x0571       //B·��������

#define DW_WENBEN_CLU_SHE_FLOW                                       0x0577       //C·�趨����
#define DW_WENBEN_CLU_FLOE_BEILV                                     0x057D       //C·��������

#define DW_WENBEN_DLU_SHE_FLOW                                       0x0583       //D·�趨����
#define DW_WENBEN_DLU_FLOE_BEILV                                     0x0589       //D·��������

#define DW_WENBEN_KENWEN                                             0x058F        //�̶��¶�


//����
#define DW_KEY_WEUHUI_A_JIAOLING                                     0x0069//�����궨 a·ѹ������
#define DW_KEY_WEUHUI_B_JIAOLING                                     0x006A//�����궨 B·ѹ������
#define DW_KEY_WEUHUI_C_JIAOLING                                     0x006B//�����궨 C·ѹ������
#define DW_KEY_WEUHUI_D_JIAOLING                                     0x006C//�����궨 D·ѹ������

#define DW_KEY_WEUHUI_A_FLOW_ON_OFF                                  0x006D//�����궨 �Ƿ���a·
#define DW_KEY_WEUHUI_B_FLOW_ON_OFF                                  0x006E//�����궨 �Ƿ���B·
#define DW_KEY_WEUHUI_C_FLOW_ON_OFF                                  0x006F//�����궨 �Ƿ���C·
#define DW_KEY_WEUHUI_D_FLOW_ON_OFF                                  0x0070//�����궨 �Ƿ���D·

//��ͼ
#define DW_ICON_A_FLOW_ON_OFF                                      0x0595      //A·�Ƿ�����ͼ
#define DW_ICON_B_FLOW_ON_OFF                                      0x05A9      //B·�Ƿ�����ͼ
#define DW_ICON_C_FLOW_ON_OFF                                      0x05BD      //C·�Ƿ�����ͼ
#define DW_ICON_D_FLOW_ON_OFF                                      0x05D1      //D·�Ƿ�����ͼ

//����¼��
#define DW_DATA_ALU_BIAO_FLOW                                      0x05E6      //����A·�趨����
#define DW_DATA_ALU_BIAO_BEILV                                     0x05E7       //����A·����
#define DW_DATA_BLU_BIAO_FLOW                                      0x05E8       //����B·�趨����
#define DW_DATA_BLU_BIAO_BEILV                                     0x05E9       //����B·����
#define DW_DATA_CLU_BIAO_FLOW                                      0x05EA       //����C·�趨����
#define DW_DATA_CLU_BIAO_BEILV                                     0x05EB       //����C·����
#define DW_DATA_DLU_BIAO_FLOW                                      0x05EC       //����D·�趨����
#define DW_DATA_DLU_BIAO_BEILV                                     0x05ED       //����D·����
#define DW_DATA_KEWEN                                              0x05EE       //����̶��¶�

//����
#define DW_KEY_WEUHUI_XITONG_WENJIANSHANCHU                    0x0071//�ļ�ɾ��
#define DW_KEY_WEUHUI_XITONG_XITONGHUIFU                       0x0072//ϵͳ�ָ�
#define DW_KEY_WEUHUI_XITONG_MIMASHEZHI                        0x0073//��������
#define DW_KEY_WEUHUI_XITONG_WENJIANGESHI                      0x0074//�ļ���ʽ
#define DW_KEY_WEUHUI_XITONG_DAYINJISHEZHI                     0x0075//��ӡ������
#define DW_KEY_WEUHUI_XITONG_LIULIANGXIANSHISHEZHI             0x0076//������ʾ����

#define DW_KEY_WEUHUI_SHANCHUWENJIAN_SHI                    0x0077//�Ƿ�ɾ���ļ� ��
#define DW_KEY_WEUHUI_SHANCHUWENJIAN_FOU                    0x0078//�Ƿ�ɾ���ļ� ��

#define DW_KEY_WEUHUI_HUIFUMOREN_SHI                    0x0079//�Ƿ�ָ�Ĭ�� ��
#define DW_KEY_WEUHUI_HUIFUMOREN_FOU                    0x007a//�Ƿ�ָ�Ĭ�� ��



//��ʾ
#define DW_WENBEN_XIN_MIMA                                       0x05ef       //��������ʾ

//����¼��
#define DW_DATA_XIN_MIMA                                         0x05f5      //����������
//����
#define DW_KEY_WEUHUI_GUANBI_MIMA                                0x007b//�ر�����������
#define DW_KEY_WEUHUI_GUANBI_WENJIANGESHI                        0x007c//�ر��ļ���ʽ
#define DW_KEY_WEUHUI_TXT_GESHI                                  0x007d//TXT��ʽ
#define DW_KEY_WEUHUI_CSV_GESHI                                  0x007e//CSV��ʽ

//��ͼ
#define DW_ICON_TXT_GESHI                                        0x05f6      //TXT
#define DW_ICON_CSV_GESHI                                        0x060a      //CSV

//����
#define DW_KEY_WEUHUI_GUANBI_LANYASHEZHI                            0x007f//�ر���������
#define DW_KEY_WEUHUI_CHUSHIHUA_LANYA                               0x0080//��ʼ������
//����¼��
#define DW_DATA_LANYA_XIN_MIMA                                      0x061f      //��������������
//��ʾ
#define DW_WENBEN_LANYA_XIN_MIMA                                    0x061e       //������������ʾ



//��ͼ
#define DW_ICON_KELIU                                        0x0620      //�̶�����
#define DW_ICON_RUKOU                                        0x0634      //�������
#define DW_ICON_BIAOKUANG                                    0x0648      //�������


//����
#define DW_KEY_WEUHUI_GUANBI_LIULIANGXUANZHE                  0x0081//�ر���������
#define DW_KEY_WEUHU_FLOW_XUANZHE_KELIU                       0x0082//ѡ��̶�
#define DW_KEY_WEUHU_FLOW_XUANZHE_RUKOU                       0x0083//ѡ�����
#define DW_KEY_WEUHU_FLOW_XUANZHE_BIAOKUANG                   0x0084//ѡ����

#define DW_KEY_WEUHUI_CANSHUSHEZHI                            0x0086//��������
#define DW_KEY_WEUHUI_PIDSHEZHI                               0x0087//PID����
#define DW_KEY_WEUHUI_XITONGBEIFEN                            0x0088//ϵͳ����
#define DW_KEY_WEUHUI_YIQIBIANHAO                             0x0089//�������
//��ʾ
#define DW_WENBEN_A_QIDONG_MAIKUAN                            0x065c       //A·��������
#define DW_WENBEN_B_QIDONG_MAIKUAN                            0x0664       //B·��������
#define DW_WENBEN_C_QIDONG_MAIKUAN                            0x066c       //C·��������
#define DW_WENBEN_D_QIDONG_MAIKUAN                            0x0674       //D·��������
//����¼��
#define DW_DATA_A_QIDONG_MAIKUAN                                     0x067d      //����A·��������
#define DW_DATA_B_QIDONG_MAIKUAN                                     0x067e      //����B·��������
#define DW_DATA_C_QIDONG_MAIKUAN                                     0x067f      //����C·��������
#define DW_DATA_D_QIDONG_MAIKUAN                                     0x0680      //����D·��������

//��ʾ 
#define DW_WENBEN_PID_P                                       0x0681       //PID  P
#define DW_WENBEN_PID_I                                       0x0689       //PID  i
#define DW_WENBEN_PID_I_XIANZHI                               0x0691       //PID  i����
#define DW_WENBEN_PID_D                                       0x0699       //PID  d
#define DW_WENBEN_PID_SIQU                                    0x06a1       //PID  ����
#define DW_WENBEN_PID_QIDONG                                  0x06a9       //��������

//����¼��
#define DW_DATA_PID_P                               0x06b1      //����PID P
#define DW_DATA_PID_I                               0x06b2      //����PID I
#define DW_DATA_PID_I_XIANZHI                       0x06b3      //����PID I����
#define DW_DATA_PID_D                               0x06b4      //����PID D
#define DW_DATA_PID_SIQU                            0x06b5      //����PID ����
#define DW_DATA_PID_QIDONG                          0x06b6      //����PID ��������

//����
#define DW_KEY_WEUHUI_BEIFENXITONG                 0x008a//����ϵͳ
#define DW_KEY_WEUHUI_TUICHU_SHI                   0x008b//�˳�ϵͳά�� ��
#define DW_KEY_WEUHUI_TUICHU_FOU                   0x008c//�˳�ϵͳά�� ��
//����¼��
#define DW_DATA_XITONGBIANHAO                      0x06BF      //����ϵͳ���



#endif

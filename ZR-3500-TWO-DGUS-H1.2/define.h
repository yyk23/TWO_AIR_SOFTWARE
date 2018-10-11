/************************************************
              触摸屏宏定义
************************************************/
#ifndef _DEFINE_H_
#define _DEFINE_H_
/*page 1  自检页面   地址区间  0x0001--0x0040*/
#define DW_PAGE1_BIANHAO_ADD         0x0001   //仪器编号       长度8
#define DW_PAGE1_BANBEN_ADD          0x0009   //版本编号       长度8
#define DW_PAGE1_ZIJIAZHONG_ADD      0x0011    //自检中 切图显示地址  
#define DW_PAGE1_GUZHANG_ADD         0x0030    //显示故障信息地址    长度30


//page 2  主界面   
/////按键宏定义
#define DW_KEY_ADD                         0x0040    //触摸屏按键返回 地址
#define DW_KEY_ZHUYE_SHEZHI_DATA           0x0001    //主页面设置按键键值
#define DW_KEY_ZHUYE_CAIYANG_DATA          0x0002    //主页面采样按键键值
#define DW_KEY_ZHUYE_WEIHUI_DATA           0x0003    //主页面维护按键键值
#define DW_KEY_ZHUYE_CHAXUN_DATA           0x0004    //主页面查询按键键值
#define DW_KEY_ZHUYE_DAYIN_DATA            0x0005    //主页面打印按键键值
#define DW_KEY_ZHUYE_DAOCHU_DATA           0x0006    //主页面导出按键键值
#define DW_KEY_ZHUYE_LANYA_DATA            0x0007    //主页面蓝牙按键键值

//显示宏定义
#define DW_WENBEN_TIMEANDDATA_ADD          0x0041     //显示时间和日期
#define DW_WENBEN_ATM_ADD                  0x005F     //显示大气压
#define DW_WENBEN_ATM_UNIT_ADD             0x0065     //显示大气压单位 kPa
#define DW_WENBEN_HUANWEN_ADD              0x0068     //显示环境温度
#define DW_WENBEN_HUANWEN_UNIT_ADD         0x006D     //显示温度单位 ℃

//切图
#define DW_ICON_LANYA                      0x006F      //蓝牙开关图标切图显示地址



//page 6  系统设置
/////按键宏定义
//#define DW_KEY_ADD                         0x0040        //触摸屏按键返回 地址
#define DW_KEY_SHEZHI_HUANWEN_CE_DATA            0x0008    //设置界面环境温度测量按键
#define DW_KEY_SHEZHI_HUANWEN_SHU_DATA           0x0009    //设置界面环境温度输入按键
#define DW_KEY_SHEZHI_ATM_CE_DATA                0x000a    //设置界面大气压测量按键
#define DW_KEY_SHEZHI_ATM_SHU_DATA               0x000b    //设置界面大气压输入按键
#define DW_KEY_SHEZHI_WENKONG1_ON_DATA           0x000c    //设置界面温控1打开按键
#define DW_KEY_SHEZHI_WENKONG1_OFF_DATA          0x000d    //设置界面温控1关闭按键
#define DW_KEY_SHEZHI_WENKONG2_ON_DATA           0x000e    //设置界面温控2打开按键
#define DW_KEY_SHEZHI_WENKONG2_OFF_DATA          0x000f    //设置界面温控3关闭按键
#define DW_KEY_SHEZHI_FAN_HOME_DATA              0x0010    //返回主菜单按键

//显示宏定义
#define DW_WENBEN_HUANWEN_SHE_ADD                0x00FB     //显示设置中的环境温度
#define DW_WENBEN_ATM_SHE_ADD                    0x0101     //显示设置中的大气压
#define DW_WENBEN_WENKONG1_SHE_ADD               0x0083     //显示设置中的温控1温度
#define DW_WENBEN_WENKONG2_SHE_ADD               0x0089     //显示设置中的温控2温度
#define DW_WENBEN_BEIGUANG_SHE_ADD               0x008f     //显示设置中的背光时间
#define DW_WENBEN_NIAN_ADD                       0x0094     //显示日期 年
#define DW_WENBEN_YUE_ADD                        0x0096     //显示日期 月
#define DW_WENBEN_RI_ADD                         0x0098     //显示日期 日
#define DW_WENBEN_SHI_ADD                        0x009a     //显示时间 时
#define DW_WENBEN_FEN_ADD                        0x009c     //显示时间 分
#define DW_WENBEN_MIAO_ADD                       0x009e     //显示时间 秒
//数据录入地址
#define DW_DATA_HUANWEN_SHE                      0x00f0     //输入环境温度地址
#define DW_DATA_ATM_SHE                          0x00f1     //输入大气压地址
#define DW_DATA_WENKONG1_SHE                     0x00f2     //输入温控1温度地址
#define DW_DATA_WENKONG2_SHE                     0x00f3     //输入温控2温度地址
#define DW_DATA_BEIGUANG_SHE                     0x00f4     //输入背光时间地址
#define DW_DATA_NIAN_SHE                         0x00f5     //输入年地址
#define DW_DATA_YUE_SHE                          0x00f6     //输入月地址
#define DW_DATA_RI_SHE                           0x00f7     //输入日地址
#define DW_DATA_SHI_SHE                          0x00f8     //输入时地址
#define DW_DATA_FEN_SHE                          0x00f9     //输入分地址
#define DW_DATA_MIAO_SHE                         0x00fa     //输入秒地址

//切图
#define DW_ICON_HUANWEN_CE_OR_SHU                      0x0107      //环境温度测量输入图标切图显示地址
#define DW_ICON_ATM_CE_OR_SHU                          0x011b      //大气压测量输入图标切图显示地址
#define DW_ICON_WENKONG1_ON_OR_SHU                     0x012f      //温控1开关图标切图显示地址
#define DW_ICON_WENKONG2_ON_OR_SHU                     0x0143      //温控2关图标切图显示地址

//采样界面  A路
//#define DW_KEY_ADD                             0x0040        //触摸屏按键返回 地址
#define DW_KEY_ALU_CAIYANG_DATA                  0x0011    //采样界面A路采样按键
#define DW_KEY_BLU_CAIYANG_DATA                  0x0012    //采样界面B路采样按键
#define DW_KEY_CLU_CAIYANG_DATA                  0x0013    //采样界面C路采样按键
#define DW_KEY_DLU_CAIYANG_DATA                  0x0014    //采样界面D路采样按键
#define DW_KEY_CAIYANG_DATA                      0x0015        //采样按键
#define DW_KEY_CHAXUN_DATA                       0x0016        //查询按键
#define DW_KEY_DAYIN_DATA                        0x0017        //打印按键
#define DW_KEY_DAOCHU_DATA                       0x0018        //导出按键
#define DW_KEY_ALU_DINGSHI_CAI_DATA                  0x0019        //A路定时采样按键
#define DW_KEY_ALU_DINGTI_CAI_DATA                   0x001a        //A路定容采样按键
#define DW_KEY_ALU_DINGTI_QIDONG_DATA                0x001b        //A路定时启动按键
#define DW_KEY_ALU_LIJI_QIDONG_DATA                  0x001C        //A路立即启动按键
#define DW_KEY_ZUHE_SHEZHI_DATA                      0x001d        //组合采样设置按键

//Blu按键
#define DW_KEY_BLU_DINGSHI_CAI_DATA                  0x001e        //B路定时采样按键
#define DW_KEY_BLU_DINGTI_CAI_DATA                   0x001f        //B路定容采样按键
#define DW_KEY_BLU_DINGTI_QIDONG_DATA                0x0020        //B路定时启动按键
#define DW_KEY_BLU_LIJI_QIDONG_DATA                  0x0021        //B路立即启动按键

//Clu按键
#define DW_KEY_CLU_DINGSHI_CAI_DATA                  0x0022        //C路定时采样按键
#define DW_KEY_CLU_DINGTI_CAI_DATA                   0x0023        //C路定容采样按键
#define DW_KEY_CLU_DINGTI_QIDONG_DATA                0x0024        //C路定时启动按键
#define DW_KEY_CLU_LIJI_QIDONG_DATA                  0x0025        //C路立即启动按键
//Dlu按键
#define DW_KEY_DLU_DINGSHI_CAI_DATA                  0x0026        //D路定时采样按键
#define DW_KEY_DLU_DINGTI_CAI_DATA                   0x0027        //D路定容采样按键
#define DW_KEY_DLU_DINGTI_QIDONG_DATA                0x0028        //D路定时启动按键
#define DW_KEY_DLU_LIJI_QIDONG_DATA                  0x0029        //D路立即启动按键

//A显示地址
#define DW_WENBEN_ALU_CAI_FLOE_SHE_ADD                        0x0157     //显示设置中的A路采样流量
#define DW_WENBEN_ALU_CAI_SHICHANG_SHI_SHE_ADD                0x015b     //显示设置中的A路采样时长 时
#define DW_WENBEN_ALU_CAI_SHICHANG_FEN_SHE_ADD                0x015d     //显示设置中的A路采样时长 分
#define DW_WENBEN_ALU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x015f     //显示设置中的A路启动时刻 时
#define DW_WENBEN_ALU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x0161     //显示设置中的A路启动时刻 分
#define DW_WENBEN_ALU_CAI_JIANGE_SHI_SHE_ADD                  0x0163     //显示设置中的A路间隔时长 时
#define DW_WENBEN_ALU_CAI_JIANGE_FEN_SHE_ADD                  0x0165     //显示设置中的A路间隔时长 时
#define DW_WENBEN_ALU_CAI_CISHU_SHE_ADD                       0x0167     //显示设置中的A路采样次数
#define DW_WENBEN_ALU_CAI_TIJI_SHE_ADD                        0x01C2     //显示设置中的A路采样体积

//B显示地址
#define DW_WENBEN_BLU_CAI_FLOE_SHE_ADD                        0x01CA     //显示设置中的B路采样流量
#define DW_WENBEN_BLU_CAI_SHICHANG_SHI_SHE_ADD                0x01CE     //显示设置中的B路采样时长 时
#define DW_WENBEN_BLU_CAI_SHICHANG_FEN_SHE_ADD                0x01D0     //显示设置中的B路采样时长 分
#define DW_WENBEN_BLU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x01D2     //显示设置中的B路启动时刻 时
#define DW_WENBEN_BLU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x01D4     //显示设置中的B路启动时刻 分
#define DW_WENBEN_BLU_CAI_JIANGE_SHI_SHE_ADD                  0x01D6     //显示设置中的B路间隔时长 时
#define DW_WENBEN_BLU_CAI_JIANGE_FEN_SHE_ADD                  0x01D8     //显示设置中的B路间隔时长 时
#define DW_WENBEN_BLU_CAI_CISHU_SHE_ADD                       0x01DA     //显示设置中的B路采样次数
#define DW_WENBEN_BLU_CAI_TIJI_SHE_ADD                        0x01DD     //显示设置中的B路采样体积

//C显示地址
#define DW_WENBEN_CLU_CAI_FLOE_SHE_ADD                        0x01EC     //显示设置中的C路采样流量
#define DW_WENBEN_CLU_CAI_SHICHANG_SHI_SHE_ADD                0x01F0     //显示设置中的C路采样时长 时
#define DW_WENBEN_CLU_CAI_SHICHANG_FEN_SHE_ADD                0x01F2     //显示设置中的C路采样时长 分
#define DW_WENBEN_CLU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x01F4     //显示设置中的C路启动时刻 时
#define DW_WENBEN_CLU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x01F6     //显示设置中的C路启动时刻 分
#define DW_WENBEN_CLU_CAI_JIANGE_SHI_SHE_ADD                  0x01F8     //显示设置中的C路间隔时长 时
#define DW_WENBEN_CLU_CAI_JIANGE_FEN_SHE_ADD                  0x01FA     //显示设置中的C路间隔时长 时
#define DW_WENBEN_CLU_CAI_CISHU_SHE_ADD                       0x01FC    //显示设置中的C路采样次数
#define DW_WENBEN_CLU_CAI_TIJI_SHE_ADD                        0x01FF     //显示设置中的C路采样体积
//D显示地址
#define DW_WENBEN_DLU_CAI_FLOE_SHE_ADD                        0x020E     //显示设置中的D路采样流量
#define DW_WENBEN_DLU_CAI_SHICHANG_SHI_SHE_ADD                0x0212     //显示设置中的D路采样时长 时
#define DW_WENBEN_DLU_CAI_SHICHANG_FEN_SHE_ADD                0x0214     //显示设置中的D路采样时长 分
#define DW_WENBEN_DLU_CAI_QIDONGSHIKE_SHI_SHE_ADD             0x0216     //显示设置中的D路启动时刻 时
#define DW_WENBEN_DLU_CAI_QIDONGSHIKE_FEN_SHE_ADD             0x0218     //显示设置中的D路启动时刻 分
#define DW_WENBEN_DLU_CAI_JIANGE_SHI_SHE_ADD                  0x021a     //显示设置中的D路间隔时长 时
#define DW_WENBEN_DLU_CAI_JIANGE_FEN_SHE_ADD                  0x021c     //显示设置中的D路间隔时长 时
#define DW_WENBEN_DLU_CAI_CISHU_SHE_ADD                       0x021e    //显示设置中的D路采样次数
#define DW_WENBEN_DLU_CAI_TIJI_SHE_ADD                        0x0221     //显示设置中的D路采样体积

//切图
#define DW_ICON_ALU_ZHUANGTAI                           0x016a      //A路采样状态切图显示
#define DW_ICON_BLU_ZHUANGTAI                           0x017e      //B路采样状态切图显示
#define DW_ICON_CLU_ZHUANGTAI                           0x0192      //C路采样状态切图显示
#define DW_ICON_DLU_ZHUANGTAI                           0x01a6      //D路采样状态切图显示

//A数据录入地址
#define DW_DATA_ACAI_FLOW_SHE                                    0x01BA     //输入A路采样流量
#define DW_DATA_ACAI_SHICHANG_SHI_SHE                            0x01bb     //输入A路采样时长 时
#define DW_DATA_ACAI_SHICHANG_FEN_SHE                            0x01bc     //输入A路采样时长 分
#define DW_DATA_ACAI_QIDONGSHIKE_SHI_SHE                         0x01bd     //输入A路采样启动时刻 时
#define DW_DATA_ACAI_QIDONGSHIKE_FEN_SHE                         0x01be     //输入A路采样启动时刻 分
#define DW_DATA_ACAI_JIANGE_SHI_SHE                              0x01bf     //输入A路采样建个时长 时
#define DW_DATA_ACAI_JIANGE_FEN_SHE                              0x01c0     //输入A路采样间隔时长 分
#define DW_DATA_ACAI_CISHU_SHE                                   0x01c1     //输入A路采样次数
#define DW_DATA_ACAI_TIJI_FEN_SHE                                0x01c9     //输入A路采样体积
//B数据录入地址
#define DW_DATA_BCAI_FLOW_SHE                                    0x01E3     //输入B路采样流量
#define DW_DATA_BCAI_SHICHANG_SHI_SHE                            0x01e4     //输入B路采样时长 时
#define DW_DATA_BCAI_SHICHANG_FEN_SHE                            0x01e5     //输入B路采样时长 分
#define DW_DATA_BCAI_QIDONGSHIKE_SHI_SHE                         0x01e6     //输入B路采样启动时刻 时
#define DW_DATA_BCAI_QIDONGSHIKE_FEN_SHE                         0x01e7     //输入B路采样启动时刻 分
#define DW_DATA_BCAI_JIANGE_SHI_SHE                              0x01e8     //输入B路采样建个时长 时
#define DW_DATA_BCAI_JIANGE_FEN_SHE                              0x01e9     //输入B路采样间隔时长 分
#define DW_DATA_BCAI_CISHU_SHE                                   0x01ea     //输入B路采样次数
#define DW_DATA_BCAI_TIJI_FEN_SHE                                0x01eb     //输入B路采样体积
//C数据录入地址
#define DW_DATA_CCAI_FLOW_SHE                                    0x0205     //输入C路采样流量
#define DW_DATA_CCAI_SHICHANG_SHI_SHE                            0x0206     //输入C路采样时长 时
#define DW_DATA_CCAI_SHICHANG_FEN_SHE                            0x0207     //输入C路采样时长 分
#define DW_DATA_CCAI_QIDONGSHIKE_SHI_SHE                         0x0208     //输入C路采样启动时刻 时
#define DW_DATA_CCAI_QIDONGSHIKE_FEN_SHE                         0x0209    //输入C路采样启动时刻 分
#define DW_DATA_CCAI_JIANGE_SHI_SHE                              0x020A     //输入C路采样建个时长 时
#define DW_DATA_CCAI_JIANGE_FEN_SHE                              0x020B     //输入C路采样间隔时长 分
#define DW_DATA_CCAI_CISHU_SHE                                   0x020C    //输入C路采样次数
#define DW_DATA_CCAI_TIJI_FEN_SHE                                0x020D     //输入C路采样体积

//D数据录入地址
#define DW_DATA_DCAI_FLOW_SHE                                    0x0227     //输入D路采样流量
#define DW_DATA_DCAI_SHICHANG_SHI_SHE                            0x0228     //输入D路采样时长 时
#define DW_DATA_DCAI_SHICHANG_FEN_SHE                            0x0229     //输入D路采样时长 分
#define DW_DATA_DCAI_QIDONGSHIKE_SHI_SHE                         0x022a     //输入D路采样启动时刻 时
#define DW_DATA_DCAI_QIDONGSHIKE_FEN_SHE                         0x022b    //输入D路采样启动时刻 分
#define DW_DATA_DCAI_JIANGE_SHI_SHE                              0x022c     //输入D路采样建个时长 时
#define DW_DATA_DCAI_JIANGE_FEN_SHE                              0x022d     //输入D路采样间隔时长 分
#define DW_DATA_DCAI_CISHU_SHE                                   0x022e    //输入D路采样次数
#define DW_DATA_DCAI_TIJI_FEN_SHE                                0x022f     //输入D路采样体积

//组合采样设置  按键
#define DW_KEY_ZUHE_SHEZHI_A_B_DATA                   0x002a        //组合设置a复制到b
#define DW_KEY_ZUHE_SHEZHI_A_C_DATA                   0x002B        //组合设置a复制到c
#define DW_KEY_ZUHE_SHEZHI_A_D_DATA                   0x002C        //组合设置a复制到d
#define DW_KEY_ZUHE_SHEZHI_B_C_DATA                   0x002D        //组合设置b复制到c
#define DW_KEY_ZUHE_SHEZHI_B_D_DATA                   0x002E        //组合设置b复制到d
#define DW_KEY_ZUHE_SHEZHI_C_D_DATA                   0x002F        //组合设置c复制到d
#define DW_KEY_ZUHE_SHEZHI_A_DATA                     0x0030        //组合启动选择a
#define DW_KEY_ZUHE_SHEZHI_B_DATA                     0x0031        //组合启动选择b
#define DW_KEY_ZUHE_SHEZHI_C_DATA                     0x0032        //组合启动选择c
#define DW_KEY_ZUHE_SHEZHI_D_DATA                     0x0033        //组合启动选择d
#define DW_KEY_ZUHE_SHEZHI_DINGSHI_DATA               0x0034        //组合启动 定时启动
#define DW_KEY_ZUHE_SHEZHI_LIJI_DATA                  0x0035        //组合启动 立即启动
#define DW_KEY_ZUHE_SHEZHI_OFF_DATA                   0x0036        //关闭组合设置


//组合采样设置  切图
#define DW_ICON_ZUHE_A                          0x0230      //A路选择切图
#define DW_ICON_ZUHE_B                          0x0244      //b路选择切图
#define DW_ICON_ZUHE_C                          0x0258      //c路选择切图
#define DW_ICON_ZUHE_D                          0x026C      //d路选择切图


//掉电提醒
#define DW_KEY_DIAODIAN_TIXIANG_SHI                   0x0037        //掉电是否继续采样  是
#define DW_KEY_DIAODIAN_TIXIANG_FOU                   0x0038        //掉电是否继续采样  否

//ABCD调零
#define DW_WENBEN_ALU_TIAOLING_LIUYA_ADD                  0x0280     //A路采样动压调零
#define DW_WENBEN_ALU_TIAOLING_JIYA_ADD                   0x0286     //A路采样计压调零
#define DW_WENBEN_BLU_TIAOLING_LIUYA_ADD                  0x028C     //B路采样动压调零
#define DW_WENBEN_BLU_TIAOLING_JIYA_ADD                   0x0292     //B路采样计压调零
#define DW_WENBEN_CLU_TIAOLING_LIUYA_ADD                  0x0298     //C路采样动压调零
#define DW_WENBEN_CLU_TIAOLING_JIYA_ADD                   0x029E     //C路采样计压调零
#define DW_WENBEN_DLU_TIAOLING_LIUYA_ADD                  0x02A4     //D路采样动压调零
#define DW_WENBEN_DLU_TIAOLING_JIYA_ADD                   0x02AA     //D路采样计压调零

//A路正在采样
#define DW_WENBEN_ALU_CAI_SHUNSHI_FLOW_ADD                    0x02B0     //A路采样瞬时流量
#define DW_WENBEN_ALU_CAI_LEIJI_TIJI_ADD                      0x02B6     //A路采样累计体积
#define DW_WENBEN_ALU_CAI_LEIJI_BIAOTI_ADD                    0x02c0     //A路采样累计标体
#define DW_WENBEN_ALU_CAI_JIYA_ADD                            0x02ca     //A路采样累计标体
#define DW_WENBEN_ALU_CAI_DAOJISHI_SHI_ADD                            0x02d2     //A路倒计时 时
#define DW_WENBEN_ALU_CAI_DAOJISHI_FEN_ADD                            0x02d4   //A路倒计时 分
#define DW_WENBEN_ALU_CAI_DAOJISHI_MIAO_ADD                           0x02d6    //A路倒计时 秒
#define DW_WENBEN_ALU_CAI_CISHU_N_ADD                                 0x02db    //A路第n次采样
#define DW_WENBEN_ALU_CAI_JIWEN_ADD                                   0x02de    //A路计前温度
//B路正在采样
#define DW_WENBEN_BLU_CAI_SHUNSHI_FLOW_ADD                    0x02E3     //B路采样瞬时流量
#define DW_WENBEN_BLU_CAI_LEIJI_TIJI_ADD                      0x02E9     //B路采样累计体积
#define DW_WENBEN_BLU_CAI_LEIJI_BIAOTI_ADD                    0x02F3     //B路采样累计标体
#define DW_WENBEN_BLU_CAI_JIYA_ADD                            0x02FD     //B路采样累计标体
#define DW_WENBEN_BLU_CAI_DAOJISHI_SHI_ADD                            0x0305     //B路倒计时 时
#define DW_WENBEN_BLU_CAI_DAOJISHI_FEN_ADD                            0x0307   //B路倒计时 分
#define DW_WENBEN_BLU_CAI_DAOJISHI_MIAO_ADD                           0x0309    //B路倒计时 秒
#define DW_WENBEN_BLU_CAI_CISHU_N_ADD                                 0x030B   //B路第n次采样
#define DW_WENBEN_BLU_CAI_JIWEN_ADD                                   0x030E    //B路计前温度
//C路正在采样
#define DW_WENBEN_CLU_CAI_SHUNSHI_FLOW_ADD                    0x0313     //C路采样瞬时流量
#define DW_WENBEN_CLU_CAI_LEIJI_TIJI_ADD                      0x0319     //C路采样累计体积
#define DW_WENBEN_CLU_CAI_LEIJI_BIAOTI_ADD                    0x0323     //C路采样累计标体
#define DW_WENBEN_CLU_CAI_JIYA_ADD                            0x032D     //C路采样累计标体
#define DW_WENBEN_CLU_CAI_DAOJISHI_SHI_ADD                            0x0335     //C路倒计时 时
#define DW_WENBEN_CLU_CAI_DAOJISHI_FEN_ADD                            0x0337   //C路倒计时 分
#define DW_WENBEN_CLU_CAI_DAOJISHI_MIAO_ADD                           0x0339    //C路倒计时 秒
#define DW_WENBEN_CLU_CAI_CISHU_N_ADD                                 0x033B   //C路第n次采样
#define DW_WENBEN_CLU_CAI_JIWEN_ADD                                   0x033E    //C路计前温度

//D路正在采样
#define DW_WENBEN_DLU_CAI_SHUNSHI_FLOW_ADD                    0x0343     //D路采样瞬时流量
#define DW_WENBEN_DLU_CAI_LEIJI_TIJI_ADD                      0x0349     //D路采样累计体积
#define DW_WENBEN_DLU_CAI_LEIJI_BIAOTI_ADD                    0x0353     //D路采样累计标体
#define DW_WENBEN_DLU_CAI_JIYA_ADD                            0x035D     //D路采样累计标体
#define DW_WENBEN_DLU_CAI_DAOJISHI_SHI_ADD                            0x0365     //D路倒计时 时
#define DW_WENBEN_DLU_CAI_DAOJISHI_FEN_ADD                            0x0367   //D路倒计时 分
#define DW_WENBEN_DLU_CAI_DAOJISHI_MIAO_ADD                           0x0369    //D路倒计时 秒
#define DW_WENBEN_DLU_CAI_CISHU_N_ADD                                 0x036B   //D路第n次采样
#define DW_WENBEN_DLU_CAI_JIWEN_ADD                                   0x036E    //D路计前温度

//按键
#define DW_KEY_ALU_STOP_CAI                                  0x0039        //A路停止采样
#define DW_KEY_BLU_STOP_CAI                                  0x003a        //B路停止采样
#define DW_KEY_CLU_STOP_CAI                                  0x003B        //C路停止采样
#define DW_KEY_DLU_STOP_CAI                                  0x003C        //D路停止采样

#define DW_KEY_ALU_JIXU_CAI                                  0x003D        //A路停止采样
#define DW_KEY_BLU_JIXU_CAI                                  0x003E        //B路停止采样
#define DW_KEY_CLU_JIXU_CAI                                  0x003F        //C路停止采样
#define DW_KEY_DLU_JIXU_CAI                                  0x0040        //D路停止采样

//A路间隔采样
#define DW_WENBEN_ALU_JIANGE_XIACI_SHI_ADD                                   0x0371    //A路距离下次采样倒计时 时
#define DW_WENBEN_ALU_JIANGE_XIACI_FEN_ADD                                   0x0373    //A路距离下次采样倒计时 分
#define DW_WENBEN_ALU_JIANGE_XIACI_MIAO_ADD                                  0x0375    //A路距离下次采样倒计时 秒
//B路间隔采样
#define DW_WENBEN_BLU_JIANGE_XIACI_SHI_ADD                                   0x0377    //B路距离下次采样倒计时 时
#define DW_WENBEN_BLU_JIANGE_XIACI_FEN_ADD                                   0x0379    //B路距离下次采样倒计时 分
#define DW_WENBEN_BLU_JIANGE_XIACI_MIAO_ADD                                  0x037B    //B路距离下次采样倒计时 秒
//C路间隔采样
#define DW_WENBEN_CLU_JIANGE_XIACI_SHI_ADD                                   0x037D    //C路距离下次采样倒计时 时
#define DW_WENBEN_CLU_JIANGE_XIACI_FEN_ADD                                   0x037F    //C路距离下次采样倒计时 分
#define DW_WENBEN_CLU_JIANGE_XIACI_MIAO_ADD                                  0x0381    //C路距离下次采样倒计时 秒

//D路间隔采样
#define DW_WENBEN_DLU_JIANGE_XIACI_SHI_ADD                                   0x0383    //D路距离下次采样倒计时 时
#define DW_WENBEN_DLU_JIANGE_XIACI_FEN_ADD                                   0x0385    //D路距离下次采样倒计时 分
#define DW_WENBEN_DLU_JIANGE_XIACI_MIAO_ADD                                  0x0387    //D路距离下次采样倒计时 秒

//按键
#define DW_KEY_IF_FUZHI_B_THEN_STOP_B                                        0x0041        //B路正在采样  若要复制请先停止B
#define DW_KEY_IF_FUZHI_C_THEN_STOP_C                                        0x0042        //C路正在采样  若要复制请先停止C
#define DW_KEY_IF_FUZHI_D_THEN_STOP_D                                        0x0043        //D路正在采样  若要复制请先停止D
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_A                                   0x0044        //A路正在采样  若要多路启动 请先停止A
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_B                                   0x0045        //B路正在采样  若要多路启动 请先停止B
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_C                                   0x0046        //C路正在采样  若要多路启动 请先停止C
#define DW_KEY_IF_DUOLU_QIDONG_THEN_STOP_D                                   0x0047        //D路正在采样  若要多路启动 请先停止D


#define DW_KEY_STOP_A_CAI_SHI                                                0x0048        //是否结束a采样  是
#define DW_KEY_STOP_A_CAI_FOU                                                0x0049        //是否结束a采样  否

#define DW_KEY_STOP_B_CAI_SHI                                                0x004A        //是否结束B采样  是
#define DW_KEY_STOP_B_CAI_FOU                                                0x004B        //是否结束B采样  否

#define DW_KEY_STOP_C_CAI_SHI                                                0x004C        //是否结束C采样  是
#define DW_KEY_STOP_C_CAI_FOU                                                0x004D        //是否结束C采样  否

#define DW_KEY_STOP_D_CAI_SHI                                                0x004E        //是否结束D采样  是
#define DW_KEY_STOP_D_CAI_FOU                                                0x004F        //是否结束D采样  否

#define DW_KEY_STOP_CAUYANG_SHI                                              0x0050        //是否结束全部采样 是
#define DW_KEY_STOP_CAUYANG_FOU                                              0x0051        //是否结束全部采样 否


#define DW_KEY_ALU_CAIYANG_END_OK                                            0x0052        //A路采样结束确定
#define DW_KEY_BLU_CAIYANG_END_OK                                            0x0053        //B路采样结束确定
#define DW_KEY_CLU_CAIYANG_END_OK                                            0x0054        //C路采样结束确定
#define DW_KEY_DLU_CAIYANG_END_OK                                            0x0055        //D路采样结束确定


//查询显示
#define DW_WENBEN_DATA_CHAXUN_WENJIANHAO                                     0x0389     //采样数据查询 文件编号
#define DW_WENBEN_DATA_CHAXUN_RIQI                                           0x038f     //采样数据查询 采样日期
#define DW_WENBEN_DATA_CHAXUN_SHICHANG                                       0x03a3     //采样数据查询 采样时长
#define DW_WENBEN_DATA_CHAXUN_HUANWEN                                        0x03bf     //采样数据查询 环境温度
#define DW_WENBEN_DATA_CHAXUN_ATM                                            0x03ab     //采样数据查询 大气压
#define DW_WENBEN_DATA_CHAXUN_FLOW                                           0x03c5     //采样数据查询 流量
#define DW_WENBEN_DATA_CHAXUN_JIQIANYA                                       0x03b1     //采样数据查询 计前压力
#define DW_WENBEN_DATA_CHAXUN_JIQIANWEN                                      0x03ca     //采样数据查询 计前温度
#define DW_WENBEN_DATA_CHAXUN_LEITI                                          0x03b7     //采样数据查询 累计体积
#define DW_WENBEN_DATA_CHAXUN_LEIJIBIAOTI                                    0x03d0     //采样数据查询 累计标体

#define DW_WENBEN_DATA_DIAODIAN_WENJIANHAO                                   0x03ed     //掉电数据查询 文件编号
#define DW_WENBEN_DATA_DIAODIANSHIJIAN                                       0x0411     //掉电数据 掉电时间
#define DW_WENBEN_DATA_LAIDIANSHIJIAN                                        0x042f     //掉电数据 来电时间
#define DW_WENBEN_DATA_LEIJIDIAODIAN                                         0x044d     //掉电数据 累计掉电


//切图
#define DW_ICON_ABCD_DATA_CHAXUN                                             0x03d8     //ABCD采样数据
#define DW_ICON_ABCD_DATA_DIAODIAN                                           0x0458     //ABCD掉电数据



//按键
#define DW_KEY_CAIYANG_SHUJU_UP                                              0x0056        //采样数据上一页
#define DW_KEY_CAIYANG_SHUJU_DOWN                                            0x0057        //采样数据下一页
#define DW_KEY_CAIYANG_SHUJU_DAOCHU                                          0x0058        //采样数据U盘导出
#define DW_KEY_CAIYANG_SHUJU_DAYIN                                           0x0059        //采样数据打印
#define DW_KEY_CAIYANG_SHUJU_CHAKAN                                          0x005a        //查看采样数据
#define DW_KEY_DIAODIAN_SHUJU_CHAKAN                                         0x005b        //查看掉电数据

#define DW_KEY_DIAODIAN_SHUJU_UP                                              0x005c        //掉电数据上一页
#define DW_KEY_DIAODIAN_SHUJU_DOWN                                            0x005d        //掉电数据下一页
#define DW_KEY_DIAODIAN_SHUJU_DAOCHU                                          0x005e        //掉电数据U盘导出
#define DW_KEY_DIAODIAN_SHUJU_DAYIN                                           0x005f        //掉电数据打印

//数据录入
#define DW_DATA_CHAXUN_CAIYANG_BIANHAO                                       0x03ec        //输入采样数据文件编号
#define DW_DATA_CHAXUN_DIAODIAN_BIANHAO                                      0x0457        //输入掉电据文件编号

//打印 显示
#define DW_WENBEN_DATA_DAYIN_CAIYANGWENJIAN_START                            0x046c     //打印采样文件 开始文件编号
#define DW_WENBEN_DATA_DAYIN_CAIYANGWENJIAN_STOP                             0x0471     //打印采样文件 结束文件编号

#define DW_WENBEN_DATA_DAYIN_DIAODIANJILU_START                              0x0476     //打印掉电记录 开始文件编号
#define DW_WENBEN_DATA_DAYIN_DIAODIANJILU_STOP                               0x047B     //打印掉电记录 结束文件编号

//数据录入
#define DW_DATA_DATA_DAYIN_CAIYANGWENJIAN_START                                      0x0480        //输入打印采样文件 开始文件编号
#define DW_DATA_DATA_DAYIN_CAIYANGWENJIAN_STOP                                       0x0481        //输入打印采样文件 结束文件编号

#define DW_DATA_DATA_DAYIN_DIAODIANJILU_START                                        0x0482        //输入打印掉电记录 开始文件编号
#define DW_DATA_DATA_DAYIN_CDIAODIANJILU_STOP                                        0x0483        //输入打印掉电记录 结束文件编号
//按键
#define DW_KEY_CAIYANGWENJIAN_DAYIN                                          0x0060        //采样文件打印
#define DW_KEY_DIAODIANJILU_DAYIN                                            0x0061         //掉电数据打印


//导出 显示
#define DW_WENBEN_DATA_DAOCHU_CAIYANGWENJIAN_START                            0x0484     //导出采样文件 开始文件编号
#define DW_WENBEN_DATA_DAOCHU_CAIYANGWENJIAN_STOP                             0x0489     //导出采样文件 结束文件编号

#define DW_WENBEN_DATA_DAOCHU_DIAODIANJILU_START                              0x048e     //导出掉电记录 开始文件编号
#define DW_WENBEN_DATA_DAOCHU_DIAODIANJILU_STOP                               0x0493     //导出掉电记录 结束文件编号

//数据录入
#define DW_DATA_DATA_DAOCHU_CAIYANGWENJIAN_START                              0x0498        //输入导出采样文件 开始文件编号
#define DW_DATA_DATA_DAOCHU_CAIYANGWENJIAN_STOP                               0x0499        //输入导出采样文件 结束文件编号

#define DW_DATA_DATA_DAOCHU_DIAODIANJILU_START                                0x049a        //输入导出掉电记录 开始文件编号
#define DW_DATA_DATA_DAOCHU_CDIAODIANJILU_STOP                                0x049b        //输入导出掉电记录 结束文件编号
//按键
#define DW_KEY_CAIYANGWENJIAN_DAOCHU                                          0x0062        //采样文件导出
#define DW_KEY_DIAODIANJILU_DAOCHU                                            0x0063         //掉电数据导出

//打印进度 显示
#define DW_WENBEN_DATA_DAYIN_JINDU                              0x049c     //打印进度

//导出进度 显示
#define DW_WENBEN_DATA_DAOCHU_JINDU                             0x04a1     //导出进度

//请输入密码 显示
#define DW_WENBEN_MIMA_SHURU                                    0x04a6     //输入密码

 //数据录入
#define DW_DATA_DATA_MIMA                                       0x04a7        //输入密码


#define DW_KEY_WEUHUI_YALI_BIAODING                             0x0064//压力标定按键
#define DW_KEY_WEUHUI_WENDU_BIAODING                            0x0065//压力标定按键

#define DW_KEY_WEUHUI_FLOW_BIAODING                             0x0066//压力标定按键

#define DW_KEY_WEUHUI_SHEZHI_BIAODING                           0x0067//压力标定按键

#define DW_KEY_WEUHUI_JIAOLING_BIAODING                         0x0068//校零按键

//压力标定
#define DW_WENBEN_ALU_LIUYA_BEILV                                     0x04a8     //A路流压倍率
#define DW_WENBEN_BLU_LIUYA_BEILV                                     0x04ae     //B路流压倍率
#define DW_WENBEN_CLU_LIUYA_BEILV                                     0x04b4     //C路流压倍率
#define DW_WENBEN_DLU_LIUYA_BEILV                                     0x04ba     //D路流压倍率
#define DW_WENBEN_ATM_BEILV                                           0x04c0     //大气压倍率

#define DW_WENBEN_ALU_LIUYA_ZERO                                      0x04c6     //A路流压零点
#define DW_WENBEN_BLU_LIUYA_ZERO                                      0x04cC     //B路流压零点
#define DW_WENBEN_CLU_LIUYA_ZERO                                      0x04D2     //C路流压零点
#define DW_WENBEN_DLU_LIUYA_ZERO                                      0x04D8     //D路流压零点
#define DW_WENBEN_ATM_ZERO                                            0x04DE     //大气压零点


#define DW_WENBEN_ALU_JIYA_BEILV                                     0x04E4     //A路计压倍率
#define DW_WENBEN_BLU_JIYA_BEILV                                     0x04EA     //B路计压倍率
#define DW_WENBEN_CLU_JIYA_BEILV                                     0x04F0     //C路计压倍率
#define DW_WENBEN_DLU_JIYA_BEILV                                     0x04F6     //D路计压倍率

#define DW_WENBEN_ALU_JIYA_ZERO                                      0x04FC     //A路计压零点
#define DW_WENBEN_BLU_JIYA_ZERO                                      0x0502     //B路计压零点
#define DW_WENBEN_CLU_JIYA_ZERO                                      0x0508     //C路计压零点
#define DW_WENBEN_DLU_JIYA_ZERO                                      0x050E     //D路计压零点


//数据录入
#define DW_DATA_ALU_LIUYA_BEILV                                      0x050f       //输入A路流压倍率
#define DW_DATA_BLU_LIUYA_BEILV                                      0x0510       //输入A路流压倍率
#define DW_DATA_CLU_LIUYA_BEILV                                      0x0511       //输入A路流压倍率
#define DW_DATA_DLU_LIUYA_BEILV                                      0x0512       //输入A路流压倍率
#define DW_DATA_ATM_BEILV                                            0x0513       //输入A路流压倍率

#define DW_DATA_ALU_LIUYA_ZERO                                       0x0514       //输入A路流压零点
#define DW_DATA_BLU_LIUYA_ZERO                                       0x0515       //输入B路流压零点
#define DW_DATA_CLU_LIUYA_ZERO                                       0x0516       //输入C路流压零点
#define DW_DATA_DLU_LIUYA_ZERO                                       0x0517       //输入D路流压零点
#define DW_DATA_ATM_ZERO                                             0x0518       //输入大气压零点

#define DW_DATA_ALU_JIYA_BEILV                                      0x0519       //输入A路计压倍率
#define DW_DATA_BLU_JIYA_BEILV                                      0x051A       //输入A路计压倍率
#define DW_DATA_CLU_JIYA_BEILV                                      0x051B       //输入A路计压倍率
#define DW_DATA_DLU_JIYA_BEILV                                      0x051C       //输入A路计压倍率

#define DW_DATA_ALU_JIYA_ZERO                                       0x051D       //输入A路计压零点
#define DW_DATA_BLU_JIYA_ZERO                                       0x051E       //输入B路计压零点
#define DW_DATA_CLU_JIYA_ZERO                                       0x051F       //输入C路计压零点
#define DW_DATA_DLU_JIYA_ZERO                                       0x0520       //输入D路计压零点


//显示
#define DW_WENBEN_ALU_JIWEN_ZERO                                       0x0533       //A路计温零点
#define DW_WENBEN_BLU_JIWEN_ZERO                                       0x0539       //B路计温零点
#define DW_WENBEN_CLU_JIWEN_ZERO                                       0x053F       //C路计温零点
#define DW_WENBEN_DLU_JIWEN_ZERO                                       0x0545       //D路计温零点

#define DW_WENBEN_BAOWEN_1_WENDU                                       0x0521       //保温箱1温度
#define DW_WENBEN_BAOWEN_2_WENDU                                       0x0527       //保温箱2温度
#define DW_WENBEN_HUANJING_WENDU                                       0x052d       //环境温度

#define DW_WENBEN_BAOWEN_1_ZERO                                       0x054b       //保温箱1零点
#define DW_WENBEN_BAOWEN_2_ZERO                                       0x0551       //保温箱2零点
#define DW_WENBEN_HUANJING_WEN_ZERO                                   0x0557       //环境温度零点



//数据录入
#define DW_DATA_ALU_JIWEN_ZERO                                      0x0558       //输入A路计温零点
#define DW_DATA_BLU_JIWEN_ZERO                                      0x0559       //输入B路计温零点
#define DW_DATA_CLU_JIWEN_ZERO                                      0x055a       //输入C路计温零点
#define DW_DATA_DLU_JIWEN_ZERO                                      0x055b       //输入D路计温零点

#define DW_DATA_BAOWEN_1_ZERO                                      0x055c       //输入保温箱1零点
#define DW_DATA_BAOWEN_2_ZERO                                      0x055d       //输入保温箱2零点
#define DW_DATA_HUANJING_WEN_ZERO                                  0x055e       //输入环境温度零点

//显示
#define DW_WENBEN_ALU_SHE_FLOW                                       0x055f       //A路设定流量
#define DW_WENBEN_ALU_FLOE_BEILV                                     0x0565       //A路流量倍率

#define DW_WENBEN_BLU_SHE_FLOW                                       0x056B       //B路设定流量
#define DW_WENBEN_BLU_FLOE_BEILV                                     0x0571       //B路流量倍率

#define DW_WENBEN_CLU_SHE_FLOW                                       0x0577       //C路设定流量
#define DW_WENBEN_CLU_FLOE_BEILV                                     0x057D       //C路流量倍率

#define DW_WENBEN_DLU_SHE_FLOW                                       0x0583       //D路设定流量
#define DW_WENBEN_DLU_FLOE_BEILV                                     0x0589       //D路流量倍率

#define DW_WENBEN_KENWEN                                             0x058F        //刻度温度


//按键
#define DW_KEY_WEUHUI_A_JIAOLING                                     0x0069//流量标定 a路压力调零
#define DW_KEY_WEUHUI_B_JIAOLING                                     0x006A//流量标定 B路压力调零
#define DW_KEY_WEUHUI_C_JIAOLING                                     0x006B//流量标定 C路压力调零
#define DW_KEY_WEUHUI_D_JIAOLING                                     0x006C//流量标定 D路压力调零

#define DW_KEY_WEUHUI_A_FLOW_ON_OFF                                  0x006D//流量标定 是否开启a路
#define DW_KEY_WEUHUI_B_FLOW_ON_OFF                                  0x006E//流量标定 是否开启B路
#define DW_KEY_WEUHUI_C_FLOW_ON_OFF                                  0x006F//流量标定 是否开启C路
#define DW_KEY_WEUHUI_D_FLOW_ON_OFF                                  0x0070//流量标定 是否开启D路

//切图
#define DW_ICON_A_FLOW_ON_OFF                                      0x0595      //A路是否开启切图
#define DW_ICON_B_FLOW_ON_OFF                                      0x05A9      //B路是否开启切图
#define DW_ICON_C_FLOW_ON_OFF                                      0x05BD      //C路是否开启切图
#define DW_ICON_D_FLOW_ON_OFF                                      0x05D1      //D路是否开启切图

//数据录入
#define DW_DATA_ALU_BIAO_FLOW                                      0x05E6      //输入A路设定流量
#define DW_DATA_ALU_BIAO_BEILV                                     0x05E7       //输入A路倍率
#define DW_DATA_BLU_BIAO_FLOW                                      0x05E8       //输入B路设定流量
#define DW_DATA_BLU_BIAO_BEILV                                     0x05E9       //输入B路倍率
#define DW_DATA_CLU_BIAO_FLOW                                      0x05EA       //输入C路设定流量
#define DW_DATA_CLU_BIAO_BEILV                                     0x05EB       //输入C路倍率
#define DW_DATA_DLU_BIAO_FLOW                                      0x05EC       //输入D路设定流量
#define DW_DATA_DLU_BIAO_BEILV                                     0x05ED       //输入D路倍率
#define DW_DATA_KEWEN                                              0x05EE       //输入刻度温度

//按键
#define DW_KEY_WEUHUI_XITONG_WENJIANSHANCHU                    0x0071//文件删除
#define DW_KEY_WEUHUI_XITONG_XITONGHUIFU                       0x0072//系统恢复
#define DW_KEY_WEUHUI_XITONG_MIMASHEZHI                        0x0073//密码设置
#define DW_KEY_WEUHUI_XITONG_WENJIANGESHI                      0x0074//文件格式
#define DW_KEY_WEUHUI_XITONG_DAYINJISHEZHI                     0x0075//打印机设置
#define DW_KEY_WEUHUI_XITONG_LIULIANGXIANSHISHEZHI             0x0076//流量显示设置

#define DW_KEY_WEUHUI_SHANCHUWENJIAN_SHI                    0x0077//是否删除文件 是
#define DW_KEY_WEUHUI_SHANCHUWENJIAN_FOU                    0x0078//是否删除文件 否

#define DW_KEY_WEUHUI_HUIFUMOREN_SHI                    0x0079//是否恢复默认 是
#define DW_KEY_WEUHUI_HUIFUMOREN_FOU                    0x007a//是否恢复默认 否



//显示
#define DW_WENBEN_XIN_MIMA                                       0x05ef       //新密码显示

//数据录入
#define DW_DATA_XIN_MIMA                                         0x05f5      //输入新密码
//按键
#define DW_KEY_WEUHUI_GUANBI_MIMA                                0x007b//关闭新密码设置
#define DW_KEY_WEUHUI_GUANBI_WENJIANGESHI                        0x007c//关闭文件格式
#define DW_KEY_WEUHUI_TXT_GESHI                                  0x007d//TXT格式
#define DW_KEY_WEUHUI_CSV_GESHI                                  0x007e//CSV格式

//切图
#define DW_ICON_TXT_GESHI                                        0x05f6      //TXT
#define DW_ICON_CSV_GESHI                                        0x060a      //CSV

//按键
#define DW_KEY_WEUHUI_GUANBI_LANYASHEZHI                            0x007f//关闭蓝牙设置
#define DW_KEY_WEUHUI_CHUSHIHUA_LANYA                               0x0080//初始化蓝牙
//数据录入
#define DW_DATA_LANYA_XIN_MIMA                                      0x061f      //输入蓝牙新密码
//显示
#define DW_WENBEN_LANYA_XIN_MIMA                                    0x061e       //蓝牙新密码显示



//切图
#define DW_ICON_KELIU                                        0x0620      //刻度流量
#define DW_ICON_RUKOU                                        0x0634      //入口流量
#define DW_ICON_BIAOKUANG                                    0x0648      //标况流量


//按键
#define DW_KEY_WEUHUI_GUANBI_LIULIANGXUANZHE                  0x0081//关闭流量悬着
#define DW_KEY_WEUHU_FLOW_XUANZHE_KELIU                       0x0082//选择刻度
#define DW_KEY_WEUHU_FLOW_XUANZHE_RUKOU                       0x0083//选择入口
#define DW_KEY_WEUHU_FLOW_XUANZHE_BIAOKUANG                   0x0084//选择标况

#define DW_KEY_WEUHUI_CANSHUSHEZHI                            0x0086//参数设置
#define DW_KEY_WEUHUI_PIDSHEZHI                               0x0087//PID设置
#define DW_KEY_WEUHUI_XITONGBEIFEN                            0x0088//系统备份
#define DW_KEY_WEUHUI_YIQIBIANHAO                             0x0089//仪器编号
//显示
#define DW_WENBEN_A_QIDONG_MAIKUAN                            0x065c       //A路启动脉宽
#define DW_WENBEN_B_QIDONG_MAIKUAN                            0x0664       //B路启动脉宽
#define DW_WENBEN_C_QIDONG_MAIKUAN                            0x066c       //C路启动脉宽
#define DW_WENBEN_D_QIDONG_MAIKUAN                            0x0674       //D路启动脉宽
//数据录入
#define DW_DATA_A_QIDONG_MAIKUAN                                     0x067d      //输入A路启动脉宽
#define DW_DATA_B_QIDONG_MAIKUAN                                     0x067e      //输入B路启动脉宽
#define DW_DATA_C_QIDONG_MAIKUAN                                     0x067f      //输入C路启动脉宽
#define DW_DATA_D_QIDONG_MAIKUAN                                     0x0680      //输入D路启动脉宽

//显示 
#define DW_WENBEN_PID_P                                       0x0681       //PID  P
#define DW_WENBEN_PID_I                                       0x0689       //PID  i
#define DW_WENBEN_PID_I_XIANZHI                               0x0691       //PID  i限制
#define DW_WENBEN_PID_D                                       0x0699       //PID  d
#define DW_WENBEN_PID_SIQU                                    0x06a1       //PID  死区
#define DW_WENBEN_PID_QIDONG                                  0x06a9       //启动加热

//数据录入
#define DW_DATA_PID_P                               0x06b1      //输入PID P
#define DW_DATA_PID_I                               0x06b2      //输入PID I
#define DW_DATA_PID_I_XIANZHI                       0x06b3      //输入PID I限制
#define DW_DATA_PID_D                               0x06b4      //输入PID D
#define DW_DATA_PID_SIQU                            0x06b5      //输入PID 死区
#define DW_DATA_PID_QIDONG                          0x06b6      //输入PID 启动加热

//按键
#define DW_KEY_WEUHUI_BEIFENXITONG                 0x008a//备份系统
#define DW_KEY_WEUHUI_TUICHU_SHI                   0x008b//退出系统维护 是
#define DW_KEY_WEUHUI_TUICHU_FOU                   0x008c//退出系统维护 否
//数据录入
#define DW_DATA_XITONGBIANHAO                      0x06BF      //输入系统编号



#endif

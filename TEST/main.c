#include <REGX52.H>
#include "Buzzer.h"
#include "Key.h"
#include "Nixie.h"
#include "Timer0.h"
#include "XPT2046.h"
#include "Timer0.h"

//sbit Led_A_Red=P2^0;
//sbit Led_A_Yellow=P2^1;
//sbit Led_A_Green=P2^5;
//sbit Led_B_Red=P2^6;
//sbit Led_B_Yellow=P1^4;
//sbit Led_B_Green=P2^7;
sbit Led_A_Red=P1^1;
sbit Led_A_Yellow=P1^2;
sbit Led_A_Green=P1^3;
sbit Led_B_Red=P1^4;
sbit Led_B_Yellow=P1^5;
sbit Led_B_Green=P1^6;
sbit Buzzer=P2^5;

unsigned char mode=0;
unsigned char Light_Yellow=1;
unsigned char Buzzer_On;
unsigned char ATime=30;
unsigned char BTime=30;
unsigned char ADValue;
unsigned char KeyNum;


void Nixie_Clear()
{
	 Nixie_SetBuf(1,10);
    Nixie_SetBuf(2,10);
    Nixie_SetBuf(7,10);
    Nixie_SetBuf(8,10);
}

void Show_Led_Time() 
{
    if (mode == 5)// 夜间模式清空显示
	 { 
        Nixie_Clear();
    } 
	else // 正常模式显示时间
	{ 
        Nixie_SetBuf(7, ATime / 10);
        Nixie_SetBuf(8, ATime % 10);
        Nixie_SetBuf(1, BTime / 10);
        Nixie_SetBuf(2, BTime % 10);
    }
}

void Traffic_Lignt()
{
	if(KeyNum == 1) {mode = 4;ATime=10;BTime=10;}
	if(KeyNum == 2) mode = 5;
	switch(mode)
	{
		case 0:        //A green, B red
			Led_A_Red=0;
			Led_A_Yellow=0;
			Led_A_Green=1;
			Led_B_Red=1;
			Led_B_Yellow=0;
			Led_B_Green=0;
			Show_Led_Time();
			break;
		case 1:        //A yellow, B red
			
			Led_A_Yellow=Light_Yellow;
			Led_A_Green=0;
			
			Show_Led_Time();
			break;
		case 2:        //A red, B green
			Led_A_Red=1;
			Led_A_Yellow=0;
			
			Led_B_Red=0;
			
			Led_B_Green=1;
			Show_Led_Time();
			break;
		case 3:        //A red, B yellow
			
			
			Led_B_Yellow=Light_Yellow;
			Led_B_Green=0;
			Show_Led_Time();
			break;
		case 4:        //A red, B red
			Led_A_Red=1;
			Led_A_Yellow=0;
			Led_A_Green=0;
			Led_B_Red=1;
			Led_B_Yellow=0;
			Led_B_Green=0;
			Show_Led_Time();
			break;
		case 5:    //A,B yellow night mode
			Led_A_Red=0;
			Led_A_Yellow=Light_Yellow;
			Led_A_Green=0;
			Led_B_Red=0;
			Led_B_Yellow=Light_Yellow;
			Led_B_Green=0;
			Nixie_Clear();
			break;
	}
}



void main()
{
	Timer0Init();
	while(1)
	{
		
		KeyNum=Key();
		Traffic_Lignt();

	}
	

	
}

void Timer0_Routine(void) interrupt 1 
{
    static unsigned int T0Count0, T0Count1, T0Count2, T0Count3,T0Count4, T0CountMode;
    static bit NightMode = 0;

    TL0 = 0x66;     // 定时器重载值（假设中断周期为1ms）
    TH0 = 0xFC;

    // 公共计数器
    T0Count0++;
    T0Count1++;
    T0Count2++;
    T0Count3++;
	T0Count4++;
    T0CountMode++;

    // 数码管扫描
    if(T0Count0 >= 2) 
		{
        T0Count0 = 0;
        Nixie_Loop();
    }

    // 检测光照进入夜间模式
    if(T0Count1 >= 100) // 1ms秒检测一次光照
		{ 
        T0Count1 = 0;
        ADValue = XPT2046_ReadAD(XPT2046_XP);
				if(ADValue < 100) {NightMode = 1;}
				else {NightMode = 0;}
        if(NightMode && mode != 5) 
		{
            mode = 5;          // 进入夜间模式
            T0CountMode = 0;   // 重置模式计时器
        } 
				else if(!NightMode && mode == 5) 
				{
            mode = 0;          // 退出夜间模式
            T0CountMode = 0;   // 重置模式计时器
            ATime = 31;        // 恢复默认倒计时初值
            BTime = 31;
        }
    }

    // 按键扫描
    if(T0Count2 >= 20) // 20ms扫描一次按键
		{ 
        T0Count2 = 0;
        Key_Loop();
    }

    // 黄灯闪烁控制
    // 在黄灯闪烁控制的代码段：
		if (T0Count3 >= 500)
		{
				T0Count3 = 0;
				
				
				if (mode == 1 || mode == 3 || mode ==5)
						Buzzer_Time(100); // 每次闪烁触发一次短鸣
				
		}
			if(T0Count4>=500)
			{
				Light_Yellow=!Light_Yellow;
			}

    // 模式自动切换逻辑（仅在非夜间模式生效）
    if(!NightMode) 
		{
        switch(mode) 
				{
            case 0: // A绿-B红，持续30秒
                if(T0CountMode >= 31000) // 30秒后切到mode1
				{ 
                    mode = 1;
                    T0CountMode = 0;
                    ATime = 4;  // 黄灯倒计时初值3秒
                    BTime = 4;
                }
                break;
            case 1: // A黄-B红，持续3秒
                if(T0CountMode >= 4000) // 3秒后切到mode2
				{ 
                    mode = 2;
                    T0CountMode = 0;
                    ATime = 31; // 红灯倒计时初值30秒
                    BTime = 31;
                }
                break;
            case 2: // A红-B绿，持续30秒
                if(T0CountMode >= 31000) // 30秒后切到mode3
				{ 
                    mode = 3;
                    T0CountMode = 0;
                    ATime = 4;  // 黄灯倒计时初值3秒
                    BTime = 4;
                }
                break;
            case 3: // A红-B黄，持续3秒
                if(T0CountMode >= 4000) // 3秒后切回mode0
				{ 
                    mode = 0;
                    T0CountMode = 0;
                    ATime = 31; // 绿灯倒计时初值30秒
                    BTime = 31;
                }
                break;
			case 4:
				if(T0CountMode >= 13000 ) // 10秒后切到mode1
				{
					mode = 0;
					T0CountMode = 0;
					ATime = 31;  
					BTime = 31;
				}
        }

        // 更新倒计时（每秒递减）
        if(T0CountMode % 1000 == 0)// 每秒更新一次
				{ 
            switch(mode) {
                case 0:
                case 2:
                    ATime--;
                    BTime--;
                    break;
                case 1:
                case 3:
                    ATime--;
                    BTime--;
                    break;
								case 4:
									ATime--;
									BTime--;
            }
        }
    }
}
#include <REGX52.H>
#include <intrins.h>
#include "Buzzer.h"
#include "Timer0.h"
#include "Key.h"
#include "Nixie.h"
#include "AT24C02.h"
#include "I2C.h"
#include "Delay.h"

sbit LED_A_Red = P1^0;
sbit LED_A_Yellow = P1^1;
sbit LED_A_Green = P1^2;
sbit LED_B_Red = P1^3;
sbit LED_B_Yellow = P1^4;
sbit LED_B_Green = P1^5;
sbit Buzzer = P2^5;

enum {A_GREEN, A_YELLOW, B_GREEN, B_YELLOW} State;
unsigned char Emergency_Flag = 0;
unsigned char Sec = 66;
unsigned char Saved_Time;
unsigned char State,Saved_State;

void UpdateDisplay() {
    if (!Emergency_Flag) {
        switch(State) {
            case A_GREEN:
                Nixie(1, (Sec - 36)/10);
                Nixie(2, (Sec - 36)%10);
                break;
            case A_YELLOW:
                Nixie(1, (Sec - 33)/10);
                Nixie(2, (Sec - 33)%10);
                break;
            case B_GREEN:
                Nixie(1, (Sec - 3)/10);
                Nixie(2, (Sec - 3)%10);
                break;
            case B_YELLOW:
                Nixie(1, Sec/10);
                Nixie(2, Sec%10);
                break;
        }
    } else {
        Nixie(1, Sec/10);
        Nixie(2, Sec%10);
    }
}

void main() {
    Timer0Init();
    State = A_GREEN;
    while (1) {
        unsigned char KeyNum = Key();
        if (KeyNum == 1 && !Emergency_Flag) {
            Emergency_Flag = 1;
            Saved_State = State;
            Saved_Time = Sec;
            Sec = 10;
        }

        if (Emergency_Flag) {
            LED_A_Red = 0; LED_A_Green = 1; LED_A_Yellow = 1;
            LED_B_Red = 0; LED_B_Green = 1; LED_B_Yellow = 1;
            Buzzer = 1;
            UpdateDisplay();
            if (Sec == 0) {
                Emergency_Flag = 0;
                Sec = Saved_Time;
                State = Saved_State;
            }
        }
    }
}

void Timer0_Routine() interrupt 1 {
    static unsigned int T0Count;
    TL0 = 0x66; TH0 = 0xFC;
    T0Count++;
    if (T0Count >= 1000) {
        T0Count = 0;
        if (!Emergency_Flag) {
            Sec--;
            if (Sec > 66) Sec = 66;
            // 更新阶段状态
            if (Sec == 36) State = A_YELLOW;
            else if (Sec == 33) State = B_GREEN;
            else if (Sec == 3) State = B_YELLOW;
            else if (Sec == 0) { Sec = 66; State = A_GREEN; }
        } else {
            if (Sec > 0) Sec--;
        }

        // 控制LED和蜂鸣器
        switch(State) {
            case A_GREEN:
                LED_A_Green = 0; LED_B_Red = 0;
                LED_A_Red = 1; LED_B_Green = 1;
                Buzzer = 1;
                break;
            case A_YELLOW:
                LED_A_Yellow = !LED_A_Yellow;
                Buzzer = (LED_A_Yellow) ? 0 : 1; // 同步蜂鸣器
                break;
            case B_GREEN:
                LED_B_Green = 0; LED_A_Red = 0;
                LED_B_Red = 1; LED_A_Green = 1;
                Buzzer = 1;
                break;
            case B_YELLOW:
                LED_B_Yellow = !LED_B_Yellow;
                Buzzer = (LED_B_Yellow) ? 0 : 1;
                break;
        }
        UpdateDisplay();
    }
}
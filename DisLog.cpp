#include"DisLog.h"
#include"CtrFun.h"
#include"ParDef.h"


void Log(int Sta, float *avrSwap, char *Msg) //日志记录函数
{
    static int FirLog;
    if (Sta == 0)
    {
        avrSwap[64]=2; //输出的日志变量个数
       FirLog=NINT(avrSwap[62])-1; //日志变量首地址
        strcpy(Msg, "Speed:A;T;Pitch:A;"); //名字和单位
    }
    avrSwap[FirLog]=WinTur.Log1; //记录第一个日志变量
    avrSwap[FirLog+1]=WinTur.Log2; //记录第二个日志变量
}
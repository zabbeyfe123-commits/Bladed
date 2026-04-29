//Bladed 外部控制器
#include"DISCON.h"
#include"ParDef.h"


void __declspec(dllexport) __cdecl DISCON(float *avrSwap, int *aviFail, char *accInfile, char *avcOutname, char *avcMsg)
{
    //变量定义和赋值
    accInfile[NINT(avrSwap[49])+1]='\0';
    avcOutname[NINT(avrSwap[50])]='\0';
    avcMsg[0]='\0';   //添加字符串结束符

    //风机正常运行
    if (*aviFail >= 0)
    {
        //第一个调用周期读取Bladed输入的变量数据及初始化输入相关的变量
        //之后的周期读取Bladed输入的变量数据
        InPar(avrSwap);

        //判断是否为第一个调用周期
        if (WinTur.WinSta==0)
        {
            //读取外控参数列表并初始化相关变量
            ParIni(accInfile);
            avrSwap[28]=0;   //设置偏航控制为偏航速率控制
        }

        //转矩和桨距角控制
        *aviFail=Calcs(WinTur.WinSta);

        //向Blaed输出量给定值，第一个调用周期向Bladed输出变量给定值及初始化输出相关的变量
        OutPar(avrSwap);

        //日志变量记录
        Log(WinTur.WinSta, avrSwap, avcOutname);
        //向Bladed发送消息
        if (WinTur.WinSta==0)
        {
            strcpy(avcMsg, "Discon v1.0");
        }
    }
        else
        {
            //出错，向Bladed发送消息    
            strcpy(avcMsg, "DLL has error");
            *aviFail = -1;
        }
    
}
    
    
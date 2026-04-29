#ifndef DISCON_H //头文件包含宏定义
#define DISCON_H

#include<stdio.h>
#include<string.h>
#include"BlaInOut.h"
#include "ParDef.h"
#include "CtrFun.h"
#include "CalPow.h"
#include "DisLog.h"
extern ParTab WinTur;

#pragma comment(lib, "ExternalControllerApi.lib") //导入静态链接库文件
#include "ExternalControllerApi.h"    // 定义dll与GH交互数据的函数
using namespace GHTurbineInterface;  //使用名命名空间GHTurbineInterface
#define NINT(a) ((a) >= 0.0 ? (int)((a)+0.5) : (int)((a)-0.5))


extern "C"
{
    void __declspec(dllexport) __cdecl DISCON(float *avrSwap,int *aviFail,char *accInfile,char *avcOutname,char *avcMsg);
    
}
#endif
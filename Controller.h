#ifndef CONTROLLER_H //头文件包含宏定义
#define CONTROLLER_H

#include<stdio.h>
#include "ParDef.h"

#pragma comment(lib, "ExternalControllerApi.lib") //导入静态链接库文件
#include "ExternalControllerApi.h"    // 定义dll与GH交互数据的函数
using namespace GHTurbineInterface;  //使用名命名空间GHTurbineInterface

extern "C"
{
    int __declspec(dllexport) __cdecl CONTROLLER(const turbine turbine_id);//外部控制器主函数
    int __declspec(dllexport) __cdecl CONTROLLER_INIT(const turbine turbine_id);//仿真第一个周期调用该函数进行初始化
}
#endif
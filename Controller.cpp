/* ===================
API接口函数编程
====================== */

#include "Controller.h"
#include"BlaInOut.h"

ParTab WinTur;//定义风电机组数据结构

int CONTROLLER_INIT(const turbine turbine_id)
{
    //外部控制器初始化函数，用于初始化外部控制器使用的相关变量，滤波器等，仅在第一个调用周期被调用。
    InPar(turbine_id);//调用Bladed V4.8输入变量函数的返回值

    ParIni(GetInfileFilepath(turbine_id));//调用输入数据读取及参数初始化函数

    //打印读入的参数
    printf("偏航速率为：%f\n", WinTur.DemYawRat);

    //添加日志变量
    AddLogValue(turbine_id, "MeaYawErr", "A");//实测偏航误差
    AddLogValue(turbine_id, "MeaNacAngFromNor", "A");//实测机舱角度

    return CONTROLLER(turbine_id);
    //调用CONTROLLER (const turbine turbine_id)函数
}

int CONTROLLER(const turbine turbine_id)
{
    //变量定义
    int RetSig = 0;
    //调用转矩、桨距角需求值设置函数的返回值，Bladed V4.8，代表调用成功，-1代表调用不成功

    InPar(turbine_id);//调用Bladed V4.8输入变量函数的返回值

    RetSig = OutPar(turbine_id);//调用转矩、桨距角需求值设置函数，Bladed V4.8

    printf("机舱风向为：%f\t",WinTur.MeaNacAngFromNor);
    printf("偏航误差为：%f\n",WinTur.MeaYawErr);
    printf("调用时间为：%f\n", WinTur.SimCurTim);

    //输出日志变量数值
    SetLoggingValue(turbine_id, 0, WinTur.MeaYawErr);//实测偏航误差
    SetLoggingValue(turbine_id, 1, WinTur.MeaNacAngFromNor);//实测机舱角度

    if (RetSig == 0)
    {
        return GH_DISCON_SUCCESS;//外部控制器调用成功，返回
    }
    else
    {
        return GH_DISCON_ERROR;//外部控制器调用不成功，返回-1
    }
}
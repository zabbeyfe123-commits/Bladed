#include"CtrFun.h"
#include"ParDef.h"


/******************第二种PI控制器******************/
float PI2(int Sta, PiDat2*Pi, float Inp, float LasDem, float RefVal, float PMax, float PMin, float OutFac)
{
    float GaiFac;
    float Err;
    float DPIDem;
    float PIDem;

    Err =Inp-RefVal;

    if (Sta==0)
    {
        Pi->LasErr=Err;
        Pi->LasPIDem=LasDem;
        Pi->Coe1=Pi->Kp+Pi->Ki*Pi->T/2.0f;
        Pi->Coe2=-Pi->Kp+Pi->Ki*Pi->T/2.0f;
    }
    if (Pi->PiParCha == 1)
    {
      // 根据上一周期计算的叶片桨距角需求值计算变桨控制器增益修正系数
        GaiFac=1.0f/(1.0f + LasDem/Pi->GaiPit);
        Pi->Coe1=GaiFac*Pi->Kp+GaiFac*Pi->Ki*Pi->T/2.0f;
        Pi->Coe2=-GaiFac*Pi->Kp+GaiFac*Pi->Ki*Pi->T/2.0f;
    }
    DPIDem=Pi->Coe1*Err + Pi->Coe2*Pi->LasErr;
    PIDem = DPIDem + Pi->LasPIDem;
    PIDem = Min(Max(PIDem, PMin), PMax);
    if (OutFac<0.95)
    {
        PIDem = PMin;
    }
    Pi->LasErr = Err;
    Pi->LasPIDem = PIDem;

    return PIDem;
}
//Sta: 整型 用于程序第一个调用周期标志
//Pi: PiDat2型指针，用于存放PI控制相关变量
//Inp:浮点型，本次的输入值，通常为实测发电机转速
//PiDat2*Pi:PiDat2的指针，用于存放Pi相关的变量
//RefVal:输入量的目标值
//PMax;位置最大限值
//PMin:位置最小限值
//OutFac:转矩控制器和桨距角控制器解耦的一个系数

/******************第一种PI控制器******************/
float PI1(int Sta, PiDat1 *Pi, float InP, float ElaTim, float LasDem, float RefVal, float PMax, float PMin)
{
    float GaiFac;
    float ProDem;
    float IntDem;
    float PIDem;

    // 根据上一周期计算的叶片桨距角需求值计算变桨控制器增益修正系数
    GaiFac = 1.0f/(1.0f+LasDem/Pi->GaiPit);

    if (Sta==0)
    {
        Pi->IntErr=LasDem/(GaiFac * Pi->Ki); // 初始化发电机转速积分误差
    }

    Pi->Err = InP-RefVal; // 实测发电机转速误差
    Pi->IntErr = Pi->IntErr + Pi->Err*ElaTim; // 发电机转速积分误差
    // 考虑最大最小桨距角位置限定后的发电机转速积分误差
    Pi->IntErr = Min(Max(Pi->IntErr, PMin / (GaiFac * Pi->Ki)), PMax / (GaiFac * Pi->Ki));
    ProDem = GaiFac * Pi->Kp * Pi->Err; // 桨距角需求值比例分量
    IntDem = GaiFac * Pi->Ki * Pi->IntErr; // 桨距角需求值积分分量
    PIDem = ProDem + IntDem; // 桨距角需求值比例+积分分量
    // 考虑最大最小桨距角位置限定后的桨距角需求值比例+积分分量
    PIDem = Min(Max(PIDem, PMin), PMax);

    return PIDem;
}
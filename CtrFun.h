// 各类控制器及滤波器函数
#ifndef CtrlFun_H // 避免重复引用
#define CtrlFun_H

#include "ParDef.h"
#include <math.h>
#include <stdio.h>

#define Max(a, b) (a>b?a:b)
#define Min(a, b) (a>b?b:a)
#define Limit(x,a,b) (x<a ? a:(x>b?b:x))
#define NINT(a) ((a) >= 0.0 ? (int)((a) + 0.5) : (int)((a) - 0.5))

/*******第一种PI控制器*********/
float PI1(int Sta, PiDat1*Pi, float InP, float ElaTim, float LasDem, float RefVal, float PMax, float PMin);

/*******第二种PI控制器*********/
float PI2(int Sta,PiDat2*Pi, float Inp, float LasDem, float RefVal, float PMax, float PMin, float OutFac);

void TorCtr(int Sta, float FilMeaGenSpe, float ElaTim, float &GenTorDem, float &LastGenTorDem); // 转矩控制函数

void PitCtr(int Sta,float FilMeaGenSpe,float ElaTim,float *PitDem,float OutFac); // 桨距角控制函数

#endif
#ifndef Calcs_H // 避免重复引用
#define Calcs_H

#include <stdio.h>
#include "ParDef.h"
#include "CtrFun.h"

int Calcs(int Sta); // 调用转矩控制和桨距角控制的函数
void TorCtr(int Sta, float FilMeaGenSpe, float ElaTim, float &GenTorDem, float &LastGenTorDem); // 转矩控制函数
void YawCtr(float &YawRat); // 偏航控制函数

#endif
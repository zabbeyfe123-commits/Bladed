//GH bladed 输入输出函数及外控读取函数
#ifndef BlaInOut_H //避免重复引用
#define BlaInOut_H
#include"ParDef.h"
#include "DISCON.h"
#include <math.h>
#include<stdlib.h>

void InPar(float *avrSwap);//外控输入变量函数
void OutPar(float *avrSwap);//外控输出变量函数
void ParIni(const char *inFile);//输入数据读取及参数初始化函数
#endif
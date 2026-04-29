
#ifndef ParDef_H // 避免重复引用
#define ParDef_H
#define NINT(a) ((a) >= 0.0 ? (int)((a) + 0.5):(int)((a) - 0.5))//浮点数转化成小数

struct PiDat1   // PI结构体，对应第一种PI控制器的实现
{
    float Kp;
    float Ki;
    float GaiPit; // 计算变桨控制器增益修正系数时使用的桨距角, rad.
    float Err;    // 比例误差
    float IntErr; // 积分误差
  
};

struct PiDat2   //第二种PI控制器结构体
{
    float Kp;
    float Ki;
    float T; //采样时间
    float LasErr;
    float LasPIDem;
    float Coe1;
    float Coe2;
    int PiParCha; //PI参数是否改变标志，比如随着桨距角值增大，Kp,Ki的值要变小
    float GaiPit; //计算变桨控制器增益修正系数时使用的桨距角, rad
};
struct FirFil // 一阶低通滤波器
{
    float Alp; // 滤波器参数
    float Nt;
    float Lt;
    float Cf; // 转角频率
    float yl; // 转角频率
};

struct ParTab
{
    int ConMod;     // 转矩和桨距角控制方式：1代表第一种查表控制及第一种PI控制，2代表PI控制
    int WinSta;     // 指示风机状态，0代表初始状态，1代表之后的状态
    int BlaNum;     // 叶片个数，(-)
    int PcsSyn;     // bladed输入的接触器标志
    float NowTim;    // 当前调用外部控制器的时刻，sec.
    float ComTim;    //通信时间
    float OutTim;     //开始输出时间
    float MeaPit[3];      // 三叶片实测桨距角, rad.
    float MeaGenSpe;       // 实测发电机转速, rad/s.
    float MeaRotSpe;       // 实测风轮转速, rad/s.
    float MeaGenTor;      // 实测发电机转矩, Nm.
    float MeaGenPow;      // 实测发电机功率, W.
    float MeaHorWinV;     // 轮毂中心高度纵向风速, m/s.
    float CorFre;        // 转角频率
    float OnePluEps;    // 时间系数
    float PitCtrDT;     // 桨距角控制器调用间隔, sec.
    float MaxPit;       // 最大桨距角, rad.
    float PitMaxRat;    // 最大变桨速率, rad/s.
    float MinPit;       // 最小桨距角, rad.
    float PitRefSpe;      // 变桨控制器目标转速, rad/s.
    float CutInSpe;       // 并网转速
    float TorCtrDT;      // 转矩控制器调用间隔
    float TorMaxRat;     // 最大转矩变化速率, N-m/s.
    float PitCtrMaxTor;  // 变桨控制中的最大发电机转矩, N-m. 等于1.1倍的额定转矩
    float KopGai;          // 最优模型增益, N-m/(rad/s)^2.
    float TorCtrRan2Spe;    // 区域为1 1/2时的上限转速, rad/s.
    float PitRan3;         // 额定以上桨距角标志, rad. 取1deg
    float RatGenSpe;          // 额定发电机转速, rad/s. -- 为0.99倍的PitRefSpe
    float RatPow;          // 额定功率, W. 需考虑电气效率
    float GenSlo;          // 计算区域为2 1/2时的转矩斜率时需要的发电机转速滑差百分比, %.
    float Slo15;            // 区域为1 1/2时的转矩斜率, N-m/(rad/s).
    float Slo25;             // 区域为2 1/2时的转矩斜率, N-m/(rad/s).
    float SySp;       // 计算区域为2 1/2时的转矩斜率时需要的发电机下限转速, rad/s.
    float TrGnSp;     // 区域为2 1/2时的转速下限, rad/s.
    float PitDem[3];  // 桨距角需求值, rad.
    float GenTorDem;  // 发电机转矩需求值, N-m
    PiDat1 PitPI1;    // 桨距角控制PI结构, 第一种实现方式
    PiDat2 PitPI2;    // 桨距角控制PI结构, 第二种实现方式
    PiDat2 TorPI2;    // 转矩控制PI结构, 第二种实现方式
    float YawCtrDT;    //偏航控制器调用间隔，sec
    float YawRatDem;  // 偏航速率需求值, rad/s

    //日志变量
    float Log1;
    float Log2;
};
extern ParTab WinTur;
#endif
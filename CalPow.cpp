#include"CalPow.h"
#include"ParDef.h"

int Calcs(int Sta)//调用转矩控制和桨距角控制的函数
{
    float ElaTim;                 //前后两次调用控制器的时间间隔, sec.
    static float PitDem[3];        //桨距角需求值
    static float GenTorDem;        //转矩需求值, N-m.
    static float LastGenTorDem;    //上次调用外部控制器的时刻的发电机转矩需求值, N-m.
    static float YawRatDem;       //偏航速率需求值
    static float FilMeaGenSpe;      //滤波后的发电机转速, rad
    static float LasTimCal;        //上次调用外部控制器的时刻, sec.
    static float LasTimCalTor;       //上次调用转矩控制器的时刻, sec.
    static float LasTimCalPit;       //上次调用变桨控制器的时刻, sec.
    static float LasTimCalYaw;       //上次调用偏航控制器的时刻, sec.

    if (Sta==0)
    {
        PitDem[0]=WinTur.MeaPit[0];
        PitDem[1]=WinTur.MeaPit[1];
        PitDem[2]=WinTur.MeaPit[2];
        GenTorDem=WinTur.MeaGenTor;
        LastGenTorDem=WinTur.MeaGenTor;              //上次调用外部控制器的时刻的发电机转矩需求值
        FilMeaGenSpe=WinTur.MeaGenSpe;
        LasTimCal=WinTur.NowTim;                    //上次调用外部控制器的时刻
        LasTimCalPit=WinTur.NowTim-WinTur.PitCtrDT; //初始化上次调用变桨控制器的时刻
        LasTimCalTor=WinTur.NowTim-WinTur.TorCtrDT; //初始化上次调用转矩控制器的时刻
        LasTimCalYaw=WinTur.NowTim-WinTur.YawCtrDT; //初始化上次调用偏航控制器的时刻
            printf("GenSpeed=%.2f, CutIn=%.2f, Rated=%.2f, Mode=%s\n",
                FilMeaGenSpe, WinTur.CutInSpe, WinTur.RatGenSpe,
                (FilMeaGenSpe >= (WinTur.RatGenSpe + WinTur.CutInSpe) / 2.0f) ? "RatedZone" : "CutInZone");
        

    }

    FilMeaGenSpe=WinTur.MeaGenSpe; //发电机转速
    //转矩控制
    ElaTim = WinTur.NowTim-LasTimCalTor; // 前后两次调用控制器的时间间隔
    if ((ElaTim * WinTur.OnePluEps)>= WinTur.TorCtrDT) // 调用转矩控制器
    {
        TorCtr(Sta, FilMeaGenSpe, ElaTim, GenTorDem, LastGenTorDem); // 转矩控制函数

        WinTur.GenTorDem=LastGenTorDem;

        LasTimCalTor=WinTur.NowTim; // 保存调用转矩控制器的时刻，供下一个调用周期使用
    }

    //桨距角控制
   // ElaTim = WinTur.NowTim - LasTimCalPit;//前后两次调用控制器的时间间隔
    //if ((ElaTim*WinTur.OnePluEps) >= WinTur.PitCtrDT)//调用桨距角控制器
    //{
       // PitCtr(Sta, FilMeaGenSpe, ElaTim, PitDem, GenTorDem / (WinTur.RatPow / WinTur.PitRefSpe));//桨距角控制函数
      //  WinTur.PitDem[0] = PitDem[0];
      //  WinTur.PitDem[1] = PitDem[1];
      //  if (WinTur.BlaNum == 3)
      //  {
      //      WinTur.PitDem[2] = PitDem[2];
      //  }
      //  LasTimCalPit = WinTur.NowTim;
   // }
    // 偏航控制
    ElaTim = WinTur.NowTim - LasTimCalYaw; // 前后两次调用控制器的时间间隔
    if ((ElaTim * WinTur.OnePluEps) >= WinTur.YawCtrDT) // 调用偏航控制器
    {
        YawCtr(YawRatDem); // 偏航函数

        WinTur.YawRatDem = YawRatDem;

        LasTimCalYaw = WinTur.NowTim;
        // 保存调用偏航速率控制器的时刻，供下一个调用周期使用
    }
    LasTimCal =WinTur.NowTim; // 保存调用外部控制器的时刻，供下一个调用周期使用
    
 
    return 0;
}

void TorCtr(int Sta, float FilMeaGenSpe, float ElaTim, float &GenTorDem, float &LastGenTorDem) // 转矩控制函数
{
    float TorRat; //转矩需求值变化速率, N-m/s.

    if (WinTur.ConMod==1)
    {
        // 查表法
        // 按转速区域计算发电机转矩
        if ((FilMeaGenSpe >= WinTur.RatGenSpe) || (WinTur.PitDem[0]>=WinTur.PitRan3)) // 区域3，滤波后的发电机转速大于额定转速或者叶片1的桨距角需求值大于额定风速以上的桨距角
            GenTorDem = WinTur.RatPow / FilMeaGenSpe;
        // 根据额定功率及滤波后的发电机转速计算转矩需求值
        else if (FilMeaGenSpe <= WinTur.CutInSpe)          // 区域1，滤波后的发电机转速小于并网转速
            GenTorDem = 0.0f;                              // 转矩需求值设置为0
        else if (FilMeaGenSpe < WinTur.TorCtrRan2Spe)      // 区域1 1/2，滤波后的发电机转速小于区域为1 1/2时的上限转速
            GenTorDem = WinTur.Slo15 * (FilMeaGenSpe - WinTur.CutInSpe);
        // 根据区域为1 1/2时的转矩斜率及滤波后的发电机转速计算转矩需求值
        else if (FilMeaGenSpe < WinTur.TrGnSp)            // 区域为2，滤波后的发电机转速小于区域为2 1/2时的转速下限
            GenTorDem = WinTur.KopGai * FilMeaGenSpe * FilMeaGenSpe;
        // 根据滤波后的发电机转速计算转矩需求值设置，t=kw^2
        else // 区域为2 1/2
            GenTorDem = WinTur.Slo25 * (FilMeaGenSpe - WinTur.SySp);
        // 根据区域为2 1/2时的转矩斜率及滤波后的发电机转速计算转矩需求值
    
    }
    else
    {
         //PI法，第二类PI控制
        float TMax;
        float TMin;
        float TKopt;            //最佳叶尖速比的转矩
        float TorRefSpe;
        TKopt = WinTur.KopGai*FilMeaGenSpe*FilMeaGenSpe;          //由Kopt求取的发电机转矩需求值
        if (FilMeaGenSpe >=(WinTur.RatGenSpe + WinTur.CutInSpe)/2.0f)
        {
            //发电机转速接近于额定转速
            TMax=WinTur.RatPow/WinTur.RatGenSpe;          //最大转矩是等于额定功率除以额定转速
            TMin=TKopt;                                   //最小转矩是等于最佳叶尖速比下的转矩
            TorRefSpe =WinTur.RatGenSpe;
        }
        else
        {
            //发电机转速接近于并网转速
            TMax=TKopt;
            TMin=0;
            TorRefSpe=WinTur.CutInSpe;
        }
        GenTorDem = PI2(Sta, &WinTur.TorPI2,FilMeaGenSpe,LastGenTorDem,TorRefSpe,TMax,TMin,1.0f);
       }
    GenTorDem = Min(GenTorDem,WinTur.PitCtrMaxTor);
    // 发电机转矩需求值最大值限定
    TorRat=(GenTorDem - LastGenTorDem)/ElaTim;
    // 计算转矩需求值变化速率
    TorRat=Min(Max(TorRat,-WinTur.TorMaxRat), WinTur.TorMaxRat);
    // 转矩需求值变化速率限定
    GenTorDem=LastGenTorDem+TorRat*ElaTim;
    // 根据限定后的转矩需求值变化速率重新计算发电机转矩需求值
    LastGenTorDem=GenTorDem;
    // 保存调用外部控制器的时刻的发电机转矩需求值，供下一个调用周期使用
}
//FilMeaGenSpe:实测发电机转速
//ElaTim:前后两次调用控制器的时间间隔
//GenTorDem:计算本次发电机调用的给定值
//LastGenTorDem；上一次发电机调用的给定值

void PitCtr(int Sta, float FilMeaGenSpe, float ElaTim, float *PitDem, float OutFac) // 桨距角控制函数
{
    float PitRat[3]; // 变桨速率给定值, rad/s.
    float PiOutPut; // PI函数返回值

    if (WinTur.ConMod == 1)
    {
        // PI法，第一类PI控制
        //PI法,第一类PI控制
        PiOutPut = PI1(Sta, &WinTur.PitPI1, FilMeaGenSpe, ElaTim,WinTur.PitDem[0], WinTur.PitRefSpe, WinTur.MaxPit, WinTur.MinPit);
    }
    else
    {
        // PI法，第二类PI控制
        PiOutPut =PI2(Sta, &WinTur.PitPI2, FilMeaGenSpe, ElaTim, WinTur.PitDem[0], WinTur.PitRefSpe, WinTur.MaxPit, WinTur.MinPit);
    }
    for (int i = 0; i < WinTur.BlaNum; i++)
    {
        PitDem[i] = PiOutPut;
        PitRat[i] = (PitDem[i] - WinTur.MeaPit[i]) / ElaTim;
        //计算变桨速率需求值
        PitRat[i] = Min(Max(PitRat[i], -WinTur.PitMaxRat), WinTur.PitMaxRat);
        PitDem[i] = WinTur.MeaPit[i] + PitRat[i] * ElaTim;        //计算桨距角需求值
        PitDem[i] = Min(Max(PitDem[i], -WinTur.MinPit), WinTur.MaxPit);
        //考虑变桨速率限定之后的桨距角需求值
    }

    //日志变量记录
    WinTur.Log1 = FilMeaGenSpe;
    WinTur.Log1 = PitDem[0];
}
void YawCtr(float &YawRat)//偏航控制函数
{
    float pi = 3.141592653f;
    //计算偏航速率
    YawRat = 50.0f/1000.0f/180.0f*pi;
}
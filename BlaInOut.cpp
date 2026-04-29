#include"BlaInOut.h"
#include"ParDef.h"
ParTab WinTur;


void InPar(float *avrSwap)
{
    //*******从Bladed中读取变量******/
    WinTur.NowTim = avrSwap[1];           //当前调用外部控制器的时刻
    WinTur.WinSta = NINT(avrSwap[0]);     //状态标志

    //**************第一次运行，初始化**************/
    if (WinTur.WinSta == 0)
    {
        WinTur.ComTim = avrSwap[2];         //当前仿真时间
        WinTur.OutTim = avrSwap[117];       //开始保存数据的时刻
        WinTur.BlaNum = NINT(avrSwap[60]);  //叶片个数
    }
    //**************读取风机状态数据**************/
    WinTur.MeaGenSpe = avrSwap[19];      //额定功率时的发电机转速设定点(仅变速变桨调节机组适用)
    WinTur.MeaRotSpe = avrSwap[20];      //实测发电机转速
    WinTur.MeaPit[0] = avrSwap[3];       //实测叶片1桨距角
    WinTur.MeaPit[1] = avrSwap[32];      //实测叶片2桨距角
    if (WinTur.BlaNum == 3)
    {
        WinTur.MeaPit[2] = avrSwap[33];    //叶片3的实测桨距角
    }
    WinTur.MeaGenTor = avrSwap[22];       //额定功率时的发电机转矩设定点(仅变转速机组适用)
    WinTur.MeaGenPow = avrSwap[14];       //实测主轴功率(仅变转速机组适用)
    WinTur.PcsSyn = NINT(avrSwap[34]);    //blade轴输入的接触器标志
    WinTur.MeaHorWinV = avrSwap[26];      //输入风速
}

  
void OutPar(float *avrSwap)
{
        //设置变量值
    avrSwap[35] = 0.0f;           //主轴刹车标志，0代表不刹车
    avrSwap[40] = 0.0f;           //偏航执行机构转矩给定值
    avrSwap[45] = 0.0f;           //三叶片变桨速率给定值(统一变桨)
    avrSwap[64] = 0.0f;           //日志变量的数目
    avrSwap[71] = 0.0f;           //发电机启动电阻
    avrSwap[78] = 0.0f;           //载荷请求
    avrSwap[79] = 0.0f;           //变滑差发电机电流给定值标志
    avrSwap[54] = 0.0f;           //桨距角内外部控制器切换标志，0：外部；1：内部
    avrSwap[55] = 0.0f;           //发电机转矩内外部控制器切换标志，0：外部；1：内部
    avrSwap[34] = float(WinTur.PcsSyn);
    avrSwap[47] = WinTur.YawRatDem;    //偏航速率给定值
    
    ////未考虑延时的发电机转矩需求值及桨距角需求值输出
    avrSwap[46]=WinTur.GenTorDem;       //将电机转矩需求值输出到Bladed
    if (NINT(avrSwap[9])==0)            //0代表位置控制，1代表速率控制
    {
        avrSwap[41]=WinTur.PitDem[0];       //独立变桨中叶片1桨距角需求值
        avrSwap[42]=WinTur.PitDem[1];        //独立变桨中叶片2桨距角需求值
        if (WinTur.BlaNum==3)
            avrSwap[43]=WinTur.PitDem[2];  //独立变桨中叶片3桨距角需求值
        avrSwap[44]=WinTur.PitDem[0];      //统一变桨中三叶片的桨距角需求值
    }

}
        

void ParIni(const char *inFile)
{
        //读取参数文件中的变量
        FILE *ParFilFid;
        ParFilFid = fopen(inFile, "r");

        //1.整机
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.CorFre);    //转角频率
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.OnePluEps); //时间系数
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitCtrDT);  //桨距角控制器调用间隔, sec.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.TorCtrDT);  //转矩控制器调用间隔
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.YawCtrDT);  //偏航控制器调用间隔
        fscanf(ParFilFid, "%d%*[^\n]", &WinTur.ConMod);    //转矩和桨距角控制方式，1代表第一种查表控制及第一种PI控制，

        //2.发电机转速
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.CutInSpe);      //并网转速
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitRefSpe);     //变桨控制器目标转速, rad/s.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.TorCtrRan2Spe); //区域为1 1/2时的上限转速, rad/s.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.RatGenSpe);     //额定发电机转速, rad/s. -- 为0.99倍的PitRefSpe
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.GenSlo);        //计算区域为2 1/2时的转矩斜率时需要的发电机转速滑差百分比,

        //3. 转矩控制
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.KopGai);       //最优模型增益, N-m/(rad/s)^2.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.TorMaxRat);    //最大转矩变化速率, N-m/s.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitCtrMaxTor); //变桨控制中的最大发电机转矩, N-m. 等于1.1倍的额定转
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.TorPI2.Kp);    //转矩控制器比例增益, sec. 第二种PI控制
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.TorPI2.Ki);    //转矩控制器积分增益, (-). 第二种PI控制

        //4.桨距角控制
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%*c%*[^\n]");
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.MinPit);     //最小桨距角, rad.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.MaxPit);     //最大桨距角, rad.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitMaxRat);  //最大变桨速率, rad/s.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitRan3);    //额定以上桨距角标志, rad.取1deg
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.RatPow);     //额定功率, W.需考虑电气效率
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitPI1.GaiPit);//计算变桨控制器增益修正系数时使用的桨距角, rad.
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitPI1.Kp);//桨距角控制器比例增益, sec.第一种PI控制
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitPI1.Ki);//桨距角控制器积分增益, (-).第一种PI控制
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitPI2.Kp);//桨距角控制器比例增益, sec.第二种PI控制
        fscanf(ParFilFid, "%f%*[^\n]", &WinTur.PitPI2.Ki);//桨距角控制器积分增益, (-).第二种PI控制
        
        fclose(ParFilFid);

        //直接赋值的变量
        WinTur.SySp = WinTur.RatGenSpe / (1.0f + 0.01f*WinTur.GenSlo);//计算区域为2 1/2时的转矩斜率时需要的发电机下限转速
        WinTur.Slo15 = (WinTur.KopGai*WinTur.TorCtrRan2Spe*WinTur.TorCtrRan2Spe)/(WinTur.TorCtrRan2Spe-WinTur.CutInSpe);//计算区域为1 1/2时的斜率
        WinTur.Slo25 = (WinTur.RatPow/WinTur.RatGenSpe)/(WinTur.RatGenSpe-WinTur.SySp);//计算区域为2 1/2时的斜率
        if (WinTur.KopGai==0.0)//最优模型增益等于零判断，因为else中需要除以最优模型增益
            WinTur.TrGnSp=WinTur.SySp;
        else                    //计算最优模型增益控制和区域为2 1/2时斜率控制的交点，为了转矩控制过渡平滑，计算时使得kw^2=V25(w-SySp)
            WinTur.TrGnSp =(WinTur.Slo25-sqrt(WinTur.Slo25*(WinTur.Slo25-4.0f*WinTur.KopGai*WinTur.SySp)))/(2.0f/WinTur.KopGai);    //区域为2 1/2时的转速下限，计算时使得kw^2=V25(w-SySp)
            
            for (int i=0; i<WinTur.BlaNum; i++)
            { 
                  WinTur.PitDem[i] = WinTur.MeaPit[i];  //初始化桨距角需求值
             }
        WinTur.GenTorDem = WinTur.MeaGenTor;         //发电机转矩需求值，N-m.
        WinTur.PitPI2.PiParCha=1;
        WinTur.PitPI2.T=WinTur.ComTim;
        WinTur.PitPI2.GaiPit=WinTur.PitPI1.GaiPit;

        WinTur.TorPI2.PiParCha=0;
        WinTur.TorPI2.T=WinTur.ComTim;
        WinTur.TorPI2.GaiPit=1.0f;
}
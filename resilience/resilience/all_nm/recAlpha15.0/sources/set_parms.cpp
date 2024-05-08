/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           set_parms.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        set_parms.cpp
********************************************************************************************************************************/
#include "../headers/config.h"

SPD g_Spd;
int g_taskNum;
/********************************************************************************************************************************
** Function name:       Double_Into_Int
** Descriptions:        Double_Into_Int
** input parameters:    oldValue
** output parameters:   newValue
** Returned value:      newValue
********************************************************************************************************************************/
int Double_Into_Int(const double oldValue)
{
	int newValue = int(oldValue);
	if (newValue - oldValue > 0.99)  
	{
		newValue--;
	}
	else if (oldValue - newValue > 0.99)
	{
		newValue++;
	}
	return newValue;
}
/********************************************************************************************************************************
** Function name:       Set_Spd_Parm
** Descriptions:        Set_Spd_Parm
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Set_Spd_Parm(void)
{
	double aveRec_Rate = 0.5;

#ifdef PARM_BETA_INF_1D
	g_Spd.BETA_INF_2D = 1.425815;
#elif defined PARM_BETA_INF_2D
	g_Spd.BETA_INF_1D = 1.0;	//2D		//or 0.5, 1.0, 2.0, 4.0
#endif
	g_Spd.ALPHA_REC = 15.0;								//recovery
	g_Spd.BETA_REC = 1.0 / (Gamma_Fun1(1.0 + 1.0 / g_Spd.ALPHA_REC)*aveRec_Rate);	//to keep the same aveRec_Rate between the markovian and the non-markovian processes.
	//infect
	if (g_taskNum == 0)
	{
		g_Spd.ALPHA_INF_1D = 1.0;							//1D	//or 0.5, 1.0, 2.0, 4.0		//0.5 + double(g_taskNum) * 0.5;
	}
	else if (g_taskNum == 1)
	{
		g_Spd.ALPHA_INF_1D = 5.0;
	}
	else if (g_taskNum == 2)
	{
		g_Spd.ALPHA_INF_1D = 10.0;
	}
	else if (g_taskNum == 3)
	{
		g_Spd.ALPHA_INF_1D = 15.0;
	}
	
	g_Spd.ALPHA_INF_2D = 15.0;
	printf("g_Spd.ALPHA_INF_1D = %f\n", g_Spd.ALPHA_INF_1D);

	g_Spd.REAL_TIME_LEN = 1000;


	clock_t startTime = clock();
	Cal_LamEff1D_LamEff2D_Parm();
	clock_t endTime = clock();

	cout << "run time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s"
		<< "(" << (double)(endTime - startTime) / (CLOCKS_PER_SEC * 60) << "min)"
		<< "(" << (double)(endTime - startTime) / (CLOCKS_PER_SEC * 60 * 60) << "h)" << endl;
	Output_Spd_Parm();
}
/********************************************************************************************************************************
** Function name:       Set_Network_Parm
** Descriptions:        Set_Network_Parm
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Set_Network_Parm(void)
{
#ifdef ER_NETWORK
	g_Net.SERIAL = 1;
	g_Net.N = 1000;
	g_Net.P = 0.006;

#elif defined WS_NETWORK
	g_Net.SERIAL = 2;
	g_Net.N = 1000;
	g_Net.HALF_DEGREE = 6;
	g_Net.P = 1;

#elif defined BA_NETWORK
	g_Net.SERIAL = 3;
	g_Net.N = 100;
	g_Net.M0 = 4;
	g_Net.M = 3;

#elif defined COMPLETE_NETWORK
	g_Net.SERIAL = 4;
	g_Net.N = 100;

#elif defined UCM_NETWORK
	g_Net.SERIAL = 5;
	g_Net.N = 1000;
	g_Net.KMIN = 15;
	g_Net.KMAX = 31;
	g_Net.GAMMA = 3.0;  //range limit: gamma > 0

#elif defined REGULAR_RANDOM_NETWORK
	g_Net.SERIAL = 6;
	g_Net.N = 5000;
	g_Net.DEGREE = 20;

#elif defined REGULAR_NETWORK
	g_Net.SERIAL = 7;
	g_Net.N = 100;
	g_Net.DEGREE = 10;

#elif defined OTHER_NETWORK
	g_Net.SERIAL = 9;
	g_Net.N = 2000;
#endif
	Output_Network_Parm();
}
/********************************************************************************************************************************
** Function name:       Set_Rd_Parm
** Descriptions:        设置随机数发生器所依赖的是哪个种子,以及均匀int分布和均匀real分布的分布范围参数
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Set_Rd_Parm(void)
{
	g_Rd.mt.seed(g_Rd.rd());
	g_Rd.distF.param(std::uniform_real_distribution<double>::param_type(0, 1));
	g_Rd.distD.param(std::uniform_int_distribution<int>::param_type(0, g_Net.N - 1));
	Output_Rd_Parm();
}
/***********************************************End Of File*********************************************************************/
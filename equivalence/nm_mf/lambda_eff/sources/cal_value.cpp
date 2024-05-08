/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           cal_value.cpp
** Created by:          linzhaohua
** Created date:        2020-1-3
** Version:             V1.0
** Descriptions:        Simplex_NonMarkovian_QMF，一阶淬火平均场理论，画rhoParm图，迭代式算法
********************************************************************************************************/
#include "../headers/config.h"

FPARM g_Fun;
int g_taskNum;

static void Set_Fun_Parm(void);
static void Set_Network_Parm(void);
static double Gamma_Fun(double x);
static void Cal_SteadyRho_Of_Diff_Rho0_And_Diff_Parm(void);
/********************************************************************************************************************************
** Function name:       Draw_SteadyRho_Of_Parm_Diagram
** Descriptions:        Draw_SteadyRho_Of_Parm_Diagram
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Draw_SteadyRho_Of_Parm_Diagram(void)
{
	Set_Fun_Parm();
	Set_Network_Parm();
	Cal_SteadyRho_Of_Diff_Rho0_And_Diff_Parm();
}
/********************************************************************************************************************************
** Function name:       Set_Fun_Parm
** Descriptions:        初始化g_Fun
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Set_Fun_Parm(void)
{
	double aveRec_Rate = 0.5;

#ifdef PARM_BETA_INF_1D
	g_Fun.BETA_INF_2D = 1.425815;
#elif defined PARM_BETA_INF_2D
	g_Fun.BETA_INF_1D = 1.0;	//2D		//or 0.5, 1.0, 2.0, 4.0		//0.0 + double(g_taskNum)*0.1
#endif
	g_Fun.ALPHA_REC = 15.0;								//recovery
	g_Fun.BETA_REC = 1.0 / (Gamma_Fun(1.0 + 1.0 / g_Fun.ALPHA_REC)*aveRec_Rate);	//to keep the same aveRec_Rate between the markovian and the non-markovian processes.
														//infect
	//infect
	if (g_taskNum == 0)
	{
		g_Fun.ALPHA_INF_1D = 1.0;							//1D	//or 0.5, 1.0, 2.0, 4.0		//0.5 + double(g_taskNum) * 0.5;
	}
	else if (g_taskNum == 1)
	{
		g_Fun.ALPHA_INF_1D = 5.0;
	}
	else if (g_taskNum == 2)
	{
		g_Fun.ALPHA_INF_1D = 10.0;
	}		
	g_Fun.ALPHA_INF_2D = 15.0;

	g_Fun.REAL_TIME_LEN = 80;
	g_Fun.UPDATE_STEP_LEN = 0.01;

	clock_t startTime = clock();
	Cal_LamEff1D_LamEff2D_Parm();
	clock_t endTime = clock();

	cout << "run time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s"
		<< "(" << (double)(endTime - startTime) / (CLOCKS_PER_SEC * 60) << "min)"
		<< "(" << (double)(endTime - startTime) / (CLOCKS_PER_SEC * 60 * 60) << "h)" << endl;

	Output_Fun_Parm();
}

/********************************************************************************************************************************
** Function name:       Gamma_Fun
** Descriptions:        计算gamma函数的函数值（实数域，x>0）
** input parameters:    x(is x of gamma(x))
** output parameters:   gamma(x)
** Returned value:      gamma(x)
********************************************************************************************************************************/
double Gamma_Fun(double x)
{
	double sol = 0.0;
	double dt = 0.00001;
	double tMax = 500.0;

	if (x - 1.0 >= 0.0)
	{
		sol += pow(0.0, x - 1.0)*exp(-0.0)*dt;
	}
	for (double t = dt; t <= tMax; t += dt)
	{
		sol += pow(t, x - 1.0)*exp(-t)*dt;
	}

	return sol;
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
#elif defined RSC_NETWORK
	g_Net.SERIAL = 9;
	g_Net.N = 2000;
	g_Net.avek = 14;
	g_Net.avekTri = 6;
#endif
	Output_Network_Parm();
}
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
** Function name:       Cal_SteadyRho_Of_Diff_Rho0_And_Diff_Parm
** Descriptions:        不读取网络数据的函数
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_SteadyRho_Of_Diff_Rho0_And_Diff_Parm(void)
{
	int index = 0;
	double stRho;
	vector<vector<double> > rhoParm;
	clock_t tick, tock;														//record time;

	for (double rho0 = INITIAL_RHO_FROM; rho0 <= INITIAL_RHO_TO; rho0 += INITIAL_RHO_DELTA)
	{
#ifdef PARM_UP
		for (auto it = g_Fun.lamEff_Parm[1].begin(); it != g_Fun.lamEff_Parm[1].end(); it++)
#elif defined PARM_DOWN
		for (auto it = g_Fun.lamEff_Parm[1].end() - 1; it != g_Fun.lamEff_Parm[1].begin(); it--)
#else
#error "no defined PARM_UP or PARM_DOWN"
#endif // defined
		{
#ifdef PARM_BETA_INF_1D
			g_Fun.BETA_INF_1D = *it;
#elif defined PARM_BETA_INF_2D
			g_Fun.BETA_INF_2D = *it;
#endif
			printf("p = %f\n", *it);
			printf("lamEff = %f\n", g_Fun.lamEff_Parm[0][index]);
			tick = clock();

			rhoParm.resize(3);
			stRho = Simplex_NonMarkovian_MF(rho0);

			rhoParm[0].push_back(g_Fun.lamEff_Parm[0][index]);		//lam_eff
			rhoParm[1].push_back(*it);
			rhoParm[2].push_back(stRho);
			cout << "stRho = " << stRho << endl;

			tock = clock();															//record time;
			cout << "One parameter simulation time was " << 1.0*(tock - tick) / CLOCKS_PER_SEC << endl;
			index++;
		}
		Output_Rho_Parm_To_File(rhoParm, rho0);
		Free_2DDouble_Vector(3, rhoParm);
	}
}
/*****************************************************End Of File***************************************/
/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           cal_rho_abscissa_value.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        simplex非马模型(Gibson异步更新)，
						画lambdaEffC_1D~I0图（直接由每个rhoT求出stRho值，然后每张parm_rho图算临界值lambdaEffC）
						是lambda_eff_1D的程序修改来的
********************************************************************************************************/
#include "../headers/config.h"

static void Cal_LamC_Of_Diff_Networks(void);
static void Cal_BetaC_Of_Diff_Rho0(LIST **net, TRILIST **netTri, int netIndex);
static double Cal_BetaC_Of_Diff_Parm_Values(LIST **net, TRILIST **netTri, double rho0);
static double Cal_BetaC_Of_Diff_Parm_Values(LIST **net, TRILIST **netTri, double rho0);
void Cal_AveStRho_Value_And_Write(double lamEff, double parm, vector<vector<double> >& rhoT, vector<vector<double> >& aveStRho_Parm);
/********************************************************************************************************************************
** Function name:       Draw_Rho_Vs_AbscissaParm_Diagram1
** Descriptions:        Draw_Rho_Vs_AbscissaParm_Diagram1
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Draw_Rho_Vs_AbscissaParm_Diagram1(void)
{
	Set_Network_Parm();
	Set_Spd_Parm();
	Set_Rd_Parm();
	Cal_LamC_Of_Diff_Networks();
}
/********************************************************************************************************************************
** Function name:       Cal_AveRho_Of_Diff_Networks
** Descriptions:        Cal_AveRho_Of_Diff_Networks
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_LamC_Of_Diff_Networks(void)
{
	LIST *netAdjList;
	TRILIST *netTriList;
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	for (int i = 0; i < TOTAL_NETWORK_NUM; i++)
	{
		clock_t tick, tock;														//record time;
		tick = clock();

		sprintf(name, NETWORK_FILE, i, g_Net.SERIAL);							//read netwotk data
		Read_Network_File(&netAdjList, name);

		sprintf(name, NETWORK_TRI_FILE, i, g_Net.SERIAL);						//read netwotk_tri_list data
		Read_Net_TriList_File(&netTriList, name);

		g_Spd.aveK_1D = Avenk_NK(g_Net.N, netAdjList);							
		g_Spd.aveK_2D = Avenk_TriNK(g_Net.N, netTriList);

		Cal_BetaC_Of_Diff_Rho0(&netAdjList, &netTriList, i);

		Free2D_NK(&netAdjList, g_Net.N);
		Free2D_TriNK(&netTriList, g_Net.N);

		tock = clock();															//record time;
		cout << "One parameter simulation time was " << 1.0*(tock - tick) / CLOCKS_PER_SEC << endl;
	}
	free(name);
}
/********************************************************************************************************************************
** Function name:       Cal_BetaC_Of_Diff_Rho0
** Descriptions:        Cal_BetaC_Of_Diff_Rho0
** input parameters:    netIndex
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_BetaC_Of_Diff_Rho0(LIST **net, TRILIST **netTri, int netIndex)
{
	vector<vector<double> > betaC_Rho0;
	double betaC;
	int i = 0;

	for (double rho0 = INITIAL_RHO_FROM; rho0 <= (INITIAL_RHO_TO + EPSILON); rho0 += INITIAL_RHO_DELTA)
	{
		if (rho0 == 0.0)
		{
			rho0 = 0.02;
		}
		cout << "rho0 = " << rho0 << endl;
		g_Spd.INTIAL_RHO_NUM = Double_Into_Int(g_Net.N * rho0);
		cout << "INTIAL_RHO_NUM = " << g_Spd.INTIAL_RHO_NUM << endl;
		betaC_Rho0.push_back({ rho0 });
		for (int j = 0; j < TOTAL_DIFFUSION_NUM; j++)
		{
			betaC = Cal_BetaC_Of_Diff_Parm_Values(net, netTri, rho0);
			if (betaC >= 0)
			{
				betaC_Rho0[i].push_back(betaC);
			}
		}
		i++;
		if (rho0 == 0.02)
		{
			rho0 = 0.0;
		}
	}
	Output_BetaC_Of_Diff_Rho0_To_File(netIndex, betaC_Rho0);
	Free_2DDouble_Vector(i, betaC_Rho0);
}
/********************************************************************************************************************************
** Function name:       Cal_BetaC_Of_Diff_Parm_Values
** Descriptions:        Cal_BetaC_Of_Diff_Parm_Values
** input parameters:    rho0
** output parameters:   sol
** Returned value:      betaC or -1(表示没找到betaC)
********************************************************************************************************************************/
double Cal_BetaC_Of_Diff_Parm_Values(LIST **net, TRILIST **netTri, double rho0)
{
	vector<vector<double> > aveStRho_Parm(3);
	int index = 0;
	vector<vector<double> > rhoT;
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));
	double sol = -1;

#ifdef PARM_UP
	for (auto it = g_Spd.lamEff_Parm[1].begin(); it != g_Spd.lamEff_Parm[1].end(); it++)
#elif defined PARM_DOWN
	for (auto it = g_Spd.lamEff_Parm[1].end() - 1; it != g_Spd.lamEff_Parm[1].begin(); it--)
#else
#error "no defined PARM_UP or PARM_DOWN"
#endif // defined
	{
#ifdef PARM_BETA_INF_1D
		g_Spd.BETA_INF_1D = *it;
#elif defined PARM_BETA_INF_2D
		g_Spd.BETA_INF_2D = *it;
#endif
		printf("\nbeta_inf = %f\n", *it);
		printf("lamEff = %f\n", g_Spd.lamEff_Parm[0][index]);
		rhoT.resize(2, vector<double>(stepTotal));
		One_Diffusion_AS(g_Spd.lamEff_Parm[0][index], net, netTri, rhoT);
		Cal_AveStRho_Value_And_Write(g_Spd.lamEff_Parm[0][index], *it, rhoT, aveStRho_Parm);

		if (it > g_Spd.lamEff_Parm[1].begin())		//here is PARM_UP
		{
			if ((*(aveStRho_Parm[2].end() - 1) - *(aveStRho_Parm[2].end() - 2)) > (TURNING_PHASE))
			{
				sol = *(aveStRho_Parm[0].end() - 2);
				break;
			}
			else if ((*(aveStRho_Parm[2].end() - 2) - *(aveStRho_Parm[2].end() - 1)) > (TURNING_PHASE))
			{
				sol = *(aveStRho_Parm[0].end() - 1);
				break;
			}
		}

		Free_2DDouble_Vector(2, rhoT);
		index++;
	}
	Output_AveSteadyRho_Parm_To_File(rho0, aveStRho_Parm);
	Free_2DDouble_Vector(3, aveStRho_Parm);

	return sol;
}
/********************************************************************************************************************************
** Function name:       Cal_AveStRho_Value_And_Write
** Descriptions:        Cal_AveStRho_Value_And_Write
** input parameters:    lamEff,parm,rhoT,aveStRho_Parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveStRho_Value_And_Write(double lamEff, double parm, vector<vector<double> >& rhoT, vector<vector<double> >& aveStRho_Parm)
{
	int tFrom = Double_Into_Int((PCT_OF_STEP_BEGIN_OF_CAL_STEADY_RHO)*g_Spd.REAL_TIME_LEN);
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));
	double stRho = 0.0;
	int num = 0;

	for (int i = tFrom; i < stepTotal; i++)
	{
		if (rhoT[0][i] != 0.0)
		{
			stRho += rhoT[1][i];
			num++;
		}
	}
	stRho = (num == 0) ? 0.0 : (stRho / double(num));
	aveStRho_Parm[0].push_back(lamEff);
	aveStRho_Parm[1].push_back(parm);
	aveStRho_Parm[2].push_back(stRho);
	printf("stRho = %f\n", stRho);
}
/*****************************************************End Of File***************************************/
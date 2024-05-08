/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           cal_rho_abscissa_value.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        simplex非马模型(Gibson异步更新)
********************************************************************************************************/
#include "../headers/config.h"

void Cal_AveStRho_Of_Diff_Networks(void);
void Cal_AveStRho_Of_Diff_Diffusions(LIST **net, TRILIST **netTri, int netCnt, vector<vector<double> >& aveStRho_Parm);
void Cal_AveSteadyRho_Of_Diff_Parm_Values(LIST **net, TRILIST **netTri, vector<vector<double> >& aveStRho_Parm);
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
	Cal_AveStRho_Of_Diff_Networks();
}
/********************************************************************************************************************************
** Function name:       Cal_AveStRho_Of_Diff_Networks
** Descriptions:        Cal_AveStRho_Of_Diff_Networks
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveStRho_Of_Diff_Networks(void)
{
	vector<vector<double> > rhoParm;
	vector<vector<double> > aveStRho_Parm(3);
	LIST *netAdjList;
	TRILIST *netTriList;
	int index;
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

		Cal_AveStRho_Of_Diff_Diffusions(&netAdjList, &netTriList, i, rhoParm);
		if (i == 0)
		{
			aveStRho_Parm[0].resize(rhoParm[0].size());
			aveStRho_Parm[1].resize(rhoParm[1].size());
			aveStRho_Parm[2].resize(rhoParm[2].size());
		}
		index = 0;
		for (vector< double >::iterator it1 = rhoParm[1].begin(); it1 != rhoParm[1].end(); it1++)
		{
			aveStRho_Parm[0][index] += rhoParm[0][index] / TOTAL_NETWORK_NUM;
			aveStRho_Parm[1][index] += rhoParm[1][index] / TOTAL_NETWORK_NUM;
			aveStRho_Parm[2][index] += rhoParm[2][index] / TOTAL_NETWORK_NUM;
			index++;
		}
		Free_2DDouble_Vector(3, rhoParm);
		Free2D_NK(&netAdjList, g_Net.N);
		Free2D_TriNK(&netTriList, g_Net.N);

		tock = clock();															//record time;
		cout << "One parameter simulation time was " << 1.0*(tock - tick) / CLOCKS_PER_SEC << endl;
	}
	free(name);
	Output_AveSteadyRho_Parm_To_File(aveStRho_Parm);
}
/********************************************************************************************************************************
** Function name:       Cal_AveStRho_Of_Diff_Diffusions
** Descriptions:        Cal_AveStRho_Of_Diff_Diffusions
** input parameters:    net,netTri,netCnt,aveStRho_Parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveStRho_Of_Diff_Diffusions(LIST **net, TRILIST **netTri, int netCnt, vector<vector<double> >& aveStRho_Parm)
{
	vector<vector<double> > rhoParm;
	int index;

	for (int i = 0; i < TOTAL_DIFFUSION_NUM; i++)								//into cycle  
	{
		Cal_AveSteadyRho_Of_Diff_Parm_Values(net, netTri, rhoParm);
		if (i == 0)
		{
			aveStRho_Parm.resize(3);
			aveStRho_Parm[0].resize(rhoParm[0].size());
			aveStRho_Parm[1].resize(rhoParm[1].size());
			aveStRho_Parm[2].resize(rhoParm[2].size());
		}
		index = 0;
		for (vector< double >::iterator it1 = rhoParm[1].begin(); it1 != rhoParm[1].end(); it1++)
		{
			aveStRho_Parm[0][index] += rhoParm[0][index] / TOTAL_DIFFUSION_NUM;
			aveStRho_Parm[1][index] += rhoParm[1][index] / TOTAL_DIFFUSION_NUM;
			aveStRho_Parm[2][index] += rhoParm[2][index] / TOTAL_DIFFUSION_NUM;
			index++;
		}
		Output_NotAveStRho_Parm_To_File(netCnt, i, rhoParm);
		Free_2DDouble_Vector(3, rhoParm);
	}
}
/********************************************************************************************************************************
** Function name:       Cal_AveSteadyRho_Of_Diff_Parm_Values
** Descriptions:        Cal_AveSteadyRho_Of_Diff_Parm_Values
** input parameters:    net,netTri,aveStRho_Parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveSteadyRho_Of_Diff_Parm_Values(LIST **net, TRILIST **netTri, vector<vector<double> >& aveStRho_Parm)
{
	int index = 0;
	vector<vector<double> > rhoT;
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));

	aveStRho_Parm.resize(3);
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
		printf("p = %f\n", *it);
		printf("lamEff = %f\n", g_Spd.lamEff_Parm[0][index]);
		rhoT.resize(2, vector<double>(stepTotal));
		One_Diffusion_AS(g_Spd.lamEff_Parm[0][index], net, netTri, rhoT);
		Cal_AveStRho_Value_And_Write(g_Spd.lamEff_Parm[0][index], *it, rhoT, aveStRho_Parm);

		Free_2DDouble_Vector(2, rhoT);
		index++;
	}
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
}
/*****************************************************End Of File***************************************/
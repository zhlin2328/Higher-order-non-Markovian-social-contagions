/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           cal_rho_abscissa_value2.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        simplex非马模型(Gibson异步更新)，画parm_rho图，可以产生平均后的aveRho_T图
********************************************************************************************************/
#include "../headers/config.h"

void Cal_AveSteadyRho_Of_Diff_Parm_Values(void);
void Cal_Steady_Value_And_Write_Into_AveStRho_Parm(double lamEff, double parm, vector<vector<double> >& aveRho_T, vector<vector<double> >& aveStRho_Parm);
vector<vector<double> > Cal_AveRho_Of_Diff_Networks(double lamEff);
void Cal_AveRho_Of_Diff_Diffusions(double lamEff, LIST **net, TRILIST **netTri, vector<vector<double> >& aveRho_T, vector<int>& num);
void Cal_Mean_Value_Of_AveRho_T(vector<vector<double> >& aveRho_T, vector<int>& num);
void Add_Up_Into_AveRho_T(vector<vector<double> >& aveRho_T, vector<int>& num, vector<vector<double> >& rhoT);
/********************************************************************************************************************************
** Function name:       Draw_Rho_Vs_AbscissaParm_Diagram2
** Descriptions:        Draw_Rho_Vs_AbscissaParm_Diagram2
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Draw_Rho_Vs_AbscissaParm_Diagram2(void)
{
	Set_Network_Parm();
	Set_Spd_Parm();
	Set_Rd_Parm();
	Cal_AveSteadyRho_Of_Diff_Parm_Values();
}
/********************************************************************************************************************************
** Function name:       Cal_AveSteadyRho_Of_Diff_Parm_Values
** Descriptions:        Cal_AveSteadyRho_Of_Diff_Parm_Values
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveSteadyRho_Of_Diff_Parm_Values(void)
{
	int index = 0;
	vector<vector<double> > aveStRho_Parm(3);

	clock_t tick, tock;																	//record time;
	tick = clock();
#ifdef PARM_UP
	for (auto it = g_Spd.lamEff_Parm[1].begin(); it != g_Spd.lamEff_Parm[1].end(); it++)
#elif defined PARM_DOWN
	for (auto it = g_Spd.lamEff_Parm[1].end() - 1; it != g_Spd.lamEff_Parm[1].begin(); it--)
#else
#error "no defined PARM_UP or PARM_DOWN"
#endif // defined
	{
#ifdef PARM_INF_2D
		g_Spd.BETA_INF_2D = *it;
#elif defined PARM_INF_1D
		g_Spd.BETA_INF_1D = *it;
#endif
		printf("\nbeta_inf = %f\n", *it);
		printf("lamEff = %f\n", g_Spd.lamEff_Parm[0][index]);
		vector<vector<double> > aveRho_T = Cal_AveRho_Of_Diff_Networks(g_Spd.lamEff_Parm[0][index]);
		Cal_Steady_Value_And_Write_Into_AveStRho_Parm(g_Spd.lamEff_Parm[0][index], *it, aveRho_T, aveStRho_Parm);
		Free_2DDouble_Vector(2, aveRho_T);
		index++;
		tock = clock();																	//record time;
		cout << "One parameter simulation time was " << 1.0*(tock - tick) / CLOCKS_PER_SEC << endl;
	}
	Output_AveSteadyRho_Parm_To_File(aveStRho_Parm);
	Free_2DDouble_Vector(3, aveStRho_Parm);
}
/********************************************************************************************************************************
** Function name:       Cal_Steady_Value_And_Write_Into_AveStRho_Parm
** Descriptions:        Cal_Steady_Value_And_Write_Into_AveStRho_Parm
** input parameters:    lamEff,parm,aveRho_T,aveStRho_Parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Steady_Value_And_Write_Into_AveStRho_Parm(double lamEff, double parm, vector<vector<double> >& aveRho_T, vector<vector<double> >& aveStRho_Parm)
{
	double stRho = 0;
	int num = 0;
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));
	//e.g.REAL_TIME_LEN=3000,则只记录[0,1)-[2999,3000)时刻内的传播，所以在spread的One_Diffusion_AS中为while (tNow < g_Spd.REAL_TIME_LEN)
	int stepBegin_Of_Cal_Steady_Rho = Double_Into_Int((PCT_OF_STEP_BEGIN_OF_CAL_STEADY_RHO)*double(stepTotal));

	for (int i = stepBegin_Of_Cal_Steady_Rho; i < stepTotal; i++)
	{
		if (aveRho_T[0][i] != 0.0)
		{
			stRho += aveRho_T[1][i];
			num++;
		}
		
	}
	stRho = (num == 0) ? 0.0 : (stRho / double(num));
	aveStRho_Parm[0].push_back(lamEff);
	aveStRho_Parm[1].push_back(parm);
	aveStRho_Parm[2].push_back(stRho);
	printf("stRho = %f\n", stRho);
}
/********************************************************************************************************************************
** Function name:       Cal_AveRho_Of_Diff_Networks
** Descriptions:        Cal_AveRho_Of_Diff_Networks
** input parameters:    None
** output parameters:   aveRho_T
** Returned value:      aveRho_T
********************************************************************************************************************************/
vector<vector<double> > Cal_AveRho_Of_Diff_Networks(double lamEff)
{
	char *name;
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));
	vector<vector<double> > aveRho_T(2, vector<double>(stepTotal));
	vector<int> num(stepTotal, 0);
	LIST *netAdjList;
	TRILIST *netTriList;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	for (int i = 0; i < TOTAL_NETWORK_NUM; i++)
	{
		sprintf(name, NETWORK_FILE, i, g_Net.SERIAL);
		Read_Network_File(&netAdjList, name);

		sprintf(name, NETWORK_TRI_FILE, i, g_Net.SERIAL);								//read netwotk_tri_list data
		Read_Net_TriList_File(&netTriList, name);

		g_Spd.aveK_1D = Avenk_NK(g_Net.N, netAdjList);									//这个目前没什么作用了
		g_Spd.aveK_2D = Avenk_TriNK(g_Net.N, netTriList);

		Cal_AveRho_Of_Diff_Diffusions(lamEff, &netAdjList, &netTriList, aveRho_T, num);
		Free2D_NK(&netAdjList, g_Net.N);
		Free2D_TriNK(&netTriList, g_Net.N);
	}
	free(name);

	Cal_Mean_Value_Of_AveRho_T(aveRho_T, num);
	Output_aveRho_t_To_File(lamEff, aveRho_T);

	return aveRho_T;
}
/********************************************************************************************************************************
** Function name:       Cal_AveRho_Of_Diff_Diffusions
** Descriptions:        Cal_AveRho_Of_Diff_Diffusions
** input parameters:    net,netTri,aveRho_T,num
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveRho_Of_Diff_Diffusions(double lamEff, LIST **net, TRILIST **netTri, vector<vector<double> >& aveRho_T, vector<int>& num)
{
	vector<vector<double> > rhoT;
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));

	for (int i = 0; i < TOTAL_DIFFUSION_NUM; i++)
	{
		rhoT.resize(2, vector<double>(stepTotal));
		One_Diffusion_AS(lamEff, net, netTri, rhoT);
		Add_Up_Into_AveRho_T(aveRho_T, num, rhoT);

		Free_2DDouble_Vector(2, rhoT);
	}
}
/********************************************************************************************************************************
** Function name:       Cal_Mean_Value_Of_AveRho_T
** Descriptions:        Cal_Mean_Value_Of_AveRho_T
** input parameters:    aveRho_T,num
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Mean_Value_Of_AveRho_T(vector<vector<double> >& aveRho_T, vector<int>& num)
{
	int index = 0;
	for (auto it = aveRho_T[0].begin(); it != aveRho_T[0].end(); it++)
	{
		if (num[index] != 0)
		{
			*it = (*it) / double(num[index]);
			aveRho_T[1][index] /= double(num[index]);
		}
		else
		{
			//如果该rescaled时间步num=0，即这里假设认为无事件发生，则rhoT值与上一个rescale的值相同，时间为居中值(即+0.5*RESCALE_TIME_STEP)
			*it = (double(index) + 0.5) * double(RESCALE_TIME_STEP);	
			aveRho_T[1][index] = aveRho_T[1][index - 1];
			//*it = 0.0;
			//aveRho_T[1][index] = 0.0;
			cout << "RESCALE_TIME_STEP is too small !" << endl;
		}
		index++;
	}
}
/********************************************************************************************************************************
** Function name:       Add_Up_Into_AveRho_T
** Descriptions:        Add_Up_Into_AveRho_T
** input parameters:    aveRho_T,num,rhoT
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Add_Up_Into_AveRho_T(vector<vector<double> >& aveRho_T, vector<int>& num, vector<vector<double> >& rhoT)
{
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));
	for (int i = 0; i < stepTotal; i++)
	{
		if (i == 0 || rhoT[0][i] != 0.0)
		{
			aveRho_T[0][i] += rhoT[0][i];
			aveRho_T[1][i] += rhoT[1][i];
			num[i]++;
		}
	}
}
/*****************************************************End Of File***************************************/
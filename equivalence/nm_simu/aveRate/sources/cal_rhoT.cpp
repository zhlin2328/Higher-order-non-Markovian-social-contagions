/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           cal_rhoT.cpp
** Created by:          linzhaohua
** Created date:        2020-7-12
** Version:             V1.0
** Descriptions:        simplex����ģ��(Gibson�첽����)�������в����̶��µ���ƽ����ε�aveRho_Tͼ
********************************************************************************************************/
#include "../headers/config.h"

static void Set_Spd_Parm2(void);
static vector<vector<double> > Cal_AveRho_Of_Diff_Networks();
static void Cal_AveRho_Of_Diff_Diffusions(LIST **net, TRILIST **netTri, vector<vector<double> >& aveRho_T, vector<int>& num);
static void Cal_Mean_Value_Of_AveRho_T(vector<vector<double> >& aveRho_T, vector<int>& num);
static void Add_Up_Into_AveRho_T(vector<vector<double> >& aveRho_T, vector<int>& num, vector<vector<double> >& rhoT);
/********************************************************************************************************************************
** Function name:       Draw_Rho_Vs_AbscissaParm_Diagram2
** Descriptions:        Draw_Rho_Vs_AbscissaParm_Diagram2
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Draw_Rho_Vs_AbscissaParm_Diagram3(void)
{
	Set_Network_Parm();
	Set_Spd_Parm2();
	Set_Rd_Parm();
	Cal_AveRho_Of_Diff_Networks();
}
/********************************************************************************************************************************
** Function name:       Set_Spd_Parm
** Descriptions:        Set_Spd_Parm
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Set_Spd_Parm2(void)
{
	g_Spd.ALPHA_INF_2D = 3.0 + double(g_taskNum) * 0.5;	//2D		//or 0.5, 1.0, 2.0, 4.0
	g_Spd.BETA_INF_2D = 1.0;
	
	g_Spd.ALPHA_REC = 2.0;								//recovery
	g_Spd.BETA_REC = 0.5;
	//infect
	g_Spd.ALPHA_INF_1D = 2.5;							//1D	//or 0.5, 1.0, 2.0, 4.0		//0.5 + double(g_taskNum) * 0.5;
	g_Spd.BETA_INF_1D = 1.0;

	g_Spd.REAL_TIME_LEN = 6;
	g_Spd.INTIAL_RHO_NUM = 40;

	Output_Spd_Parm();
}
/********************************************************************************************************************************
** Function name:       Cal_AveRho_Of_Diff_Networks
** Descriptions:        Cal_AveRho_Of_Diff_Networks
** input parameters:    None
** output parameters:   aveRho_T
** Returned value:      aveRho_T
********************************************************************************************************************************/
vector<vector<double> > Cal_AveRho_Of_Diff_Networks()
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

		g_Spd.aveK_1D = Avenk_NK(g_Net.N, netAdjList);									
		g_Spd.aveK_2D = Avenk_TriNK(g_Net.N, netTriList);

		Cal_AveRho_Of_Diff_Diffusions(&netAdjList, &netTriList, aveRho_T, num);
		Free2D_NK(&netAdjList, g_Net.N);
		Free2D_TriNK(&netTriList, g_Net.N);
	}
	Cal_Mean_Value_Of_AveRho_T(aveRho_T, num);

	FILE *fp;			//output_aveRhoT into_file

	sprintf(name, "task%d_aveRhoT.csv", g_taskNum);
	fp = fopen(name, "w");
	fprintf(fp, "x,y1,y2\n");
	for (int i = 0; i < stepTotal; i++)
	{
		fprintf(fp, "%f,%f\n", aveRho_T[0][i], aveRho_T[1][i]);
	}
	fclose(fp);
	free(name);

	return aveRho_T;
}
/********************************************************************************************************************************
** Function name:       Cal_AveRho_Of_Diff_Diffusions
** Descriptions:        Cal_AveRho_Of_Diff_Diffusions
** input parameters:    net,netTri,aveRho_T,num
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveRho_Of_Diff_Diffusions(LIST **net, TRILIST **netTri, vector<vector<double> >& aveRho_T, vector<int>& num)
{
	vector<vector<double> > rhoT;
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));

	for (int i = 0; i < TOTAL_DIFFUSION_NUM; i++)
	{
		rhoT.resize(2, vector<double>(stepTotal));
		One_Diffusion_AS(0.0, net, netTri, rhoT);			//ֻҪspread.cpp�в����rhoT����0.0û���κκ��壬����������һ��ֵ
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
			//�����rescaledʱ�䲽num=0�������������Ϊ���¼���������rhoTֵ����һ��rescale��ֵ��ͬ��ʱ��Ϊ����ֵ(��+0.5*RESCALE_TIME_STEP)
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
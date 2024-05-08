/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           file.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        file.cpp
********************************************************************************************************/
#include "../headers/config.h"

#define NETWORK_INPUT_FILE_INFO_LEN			300
/********************************************************************************************************************************
** Function name:       Output_Network_Parm
** Descriptions:        Output_Network_Parm
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Network_Parm(void)
{
	cout << "Network Parameter Information: " << endl;
#ifdef ER_NETWORK
	cout << "ER_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.P: " << g_Net.P << endl;

#elif defined WS_NETWORK
	cout << "WS_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.HALF_DEGREE: " << g_Net.HALF_DEGREE << endl;
	cout << "g_Net.P: " << g_Net.P << endl;

#elif defined BA_NETWORK
	cout << "BA_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.M0: " << g_Net.M0 << endl;
	cout << "g_Net.M: " << g_Net.M << endl;

#elif defined COMPLETE_NETWORK
	cout << "COMPLETE_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;

#elif defined UCM_NETWORK
	cout << "UCM_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.KMIN: " << g_Net.KMIN << endl;
	cout << "g_Net.KMAX: " << g_Net.KMAX << endl;
	cout << "g_Net.GAMMA: " << g_Net.GAMMA << endl;

#elif defined REGULAR_RANDOM_NETWORK
	cout << "REGULAR_RANDOM_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.DEGREE: " << g_Net.DEGREE << endl;

#elif defined REGULAR_NETWORK
	cout << "REGULAR_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.DEGREE: " << g_Net.DEGREE << endl;

#elif defined ER_NETWORK_WITH_M_EDGES
	cout << "ER_NETWORK_WITH_M_EDGES" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.M: " << g_Net.M << endl;

#elif defined OTHER_NETWORK
	cout << "OTHER_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;

#endif
	cout << endl;
}
/********************************************************************************************************************************
** Function name:       Output_Spd_Parm
** Descriptions:        Output_Spd_Parm
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Spd_Parm(void)
{
	cout << "Spread Parmeter Information: " << endl;
	cout << "Recovery:" << endl;
	cout << "g_Spd.ALPHA_REC: " << g_Spd.ALPHA_REC << endl;
	cout << "g_Spd.BETA_REC: " << g_Spd.BETA_REC << endl;

	cout << "Infection:" << endl;
	cout << "g_Spd.ALPHA_INF_1D: " << g_Spd.ALPHA_INF_1D << endl;
	cout << "g_Spd.ALPHA_INF_2D: " << g_Spd.ALPHA_INF_2D << endl;
#ifdef PARM_BETA_INF_1D
	cout << "g_Spd.BETA_INF_2D: " << g_Spd.BETA_INF_2D << endl;
#elif defined PARM_BETA_INF_2D
	cout << "g_Spd.BETA_INF_1D: " << g_Spd.BETA_INF_1D << endl;
#endif
	
	cout << "g_Spd.REAL_TIME_LEN: " << g_Spd.REAL_TIME_LEN << endl;
	cout << "g_Spd.INTIAL_RHO_NUM: " << g_Spd.INTIAL_RHO_NUM << endl;
	cout << endl;
}
/********************************************************************************************************************************
** Function name:       Output_Rd_Parm
** Descriptions:        输出均匀随机数分布的范围参数
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Rd_Parm(void)
{
	cout << "g_Rd.distF.min = " << g_Rd.distF.min() << endl;
	cout << "g_Rd.distF.max = " << g_Rd.distF.max() << endl;
	cout << "g_Rd.distD.min = " << g_Rd.distD.min() << endl;
	cout << "g_Rd.distD.max = " << g_Rd.distD.max() << endl;
}
/********************************************************************************************************************************
** Function name:       Output_AveSteadyRho_Parm_To_File
** Descriptions:        Output_AveSteadyRho_Parm_To_File
** input parameters:    aveStRho_Parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_AveSteadyRho_Parm_To_File(vector<vector<double> >& aveStRho_Parm)
{
	FILE *fp;
	int index = 0;
	//char name[NAME_SIZE];
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	sprintf(name, "task%d_AveSteadyRho_Parm.csv", g_taskNum);
	fp = fopen(name, "w");
	fprintf(fp, "x,y1,y2,y3\n");
	for (vector<double>::iterator it = aveStRho_Parm[1].begin(); it != aveStRho_Parm[1].end(); it++)
	{
		fprintf(fp, "%f,%f,%f\n", aveStRho_Parm[0][index], *it, aveStRho_Parm[2][index]);
		index++;
	}
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Output_NotAveStRho_Parm_To_File
** Descriptions:        Output_NotAveStRho_Parm_To_File
** input parameters:    netCnt,diffusionCnt,rhoParm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_NotAveStRho_Parm_To_File(int netCnt, int diffusionCnt, vector<vector<double> >& rhoParm)
{
	FILE *fp;
	int index = 0;
	//char name[NAME_SIZE];
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	sprintf(name, "task%d_NotAveStRho_Parm.csv", g_taskNum);							
	if (netCnt == 0 && diffusionCnt == 0)							//clear file content
	{
		fp = fopen(name, "w+");
		fprintf(fp, "x,y1,y2,y3\n");
	}
	else
	{
		fp = fopen(name, "a+");
	}
	for (auto it = rhoParm[1].begin(); it != rhoParm[1].end(); it++)
	{
		fprintf(fp, "%f,%f,%f\n", rhoParm[0][index], *it, rhoParm[2][index]);
		index++;
	}
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Output_aveRho_t_To_File
** Descriptions:        Output_aveRho_t_To_File
** input parameters:    aveRho_t
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_aveRho_t_To_File(double lamEff, vector<vector<double> >& aveRho_t)
{
	FILE *fp;
	//char name[NAME_SIZE];
	char *name;
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));

	name = (char *)malloc(NAME_SIZE * sizeof(char));
#ifdef PARM_ALPHA_INF_2D
	sprintf(name, "task%d_aveRho_t_lamEff=%0.5f.csv", g_taskNum, lamEff);
#elif defined PARM_BETA_INF_2D
	sprintf(name, "task%d_aveRho_t_lamEff=%0.5f.csv", g_taskNum, lamEff);
#endif
	fp = fopen(name, "w");
	fprintf(fp, "x,y1,y2\n");
	for (int i = 0; i < stepTotal; i++)
	{
		fprintf(fp, "%f,%f\n", aveRho_t[0][i], aveRho_t[1][i]);
	}
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Output_rhoT_To_File
** Descriptions:        Output_rhoT_To_File
** input parameters:    rhoT
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_rhoT_To_File(double lamEff, vector<vector<double> >& rhoT)
{
	FILE *fp;
	//char name[NAME_SIZE];
	char *name;
	int index = 0;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
#ifdef PARM_ALPHA_INF_2D
	sprintf(name, "task%d_rhoT_lamEff=%0.5f.csv", g_taskNum, lamEff);
#elif defined PARM_BETA_INF_2D
	sprintf(name, "task%d_rhoT_lamEff=%0.5f.csv", g_taskNum, lamEff);
#endif
	fp = fopen(name, "w");
	fprintf(fp, "x,y1,y2\n");
	for (auto it = rhoT[0].begin(); it != rhoT[0].end(); it++)
	{
		if (index == 0 || *it != 0.0)
		{
			fprintf(fp, "%f,%f\n", *it, rhoT[1][index]);
		}
		index++;
	}
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Read_Network_File
** Descriptions:        Read_Network_File
** input parameters:    net, file_name
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Read_Network_File(LIST **net, char *file_name)
{
	int temp;
	int degree;
	char words[NETWORK_INPUT_FILE_INFO_LEN];
	FILE *fp;

	Init_NK(net);
	fp = fopen(file_name, "r");
	fgets(words, NETWORK_INPUT_FILE_INFO_LEN, fp);			//read network information
	printf("Network Input File Information: %s\n", words);
	for (int i = 0; i < g_Net.N; i++)						//read node_edge
	{
		fscanf(fp, "%d,", &temp);
		degree = temp;
		for (int j = 1; j <= degree; j++)
		{
			fscanf(fp, "%d,", &temp);
			Push_Back_NK(net, temp, i);
		}
		//fscanf(fp,"\n");									//have or not have is ok
	}
	fclose(fp);
	Set_Counterpart_NK(net);								//遍历网络中所有的边，并尝试更新对应的counterpart指针值
	//Output_NK(g_Net.N);
}
/********************************************************************************************************************************
** Function name:       Read_Net_TriList_File
** Descriptions:        Read_Net_TriList_File
** input parameters:    netTri,file_name
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Read_Net_TriList_File(TRILIST **netTri, char *file_name)
{
	int temp1;
	int temp2;
	int degree;
	char words[NETWORK_INPUT_FILE_INFO_LEN];
	FILE *fp;

	Init_TriNK(netTri);
	fp = fopen(file_name, "r");
	fgets(words, NETWORK_INPUT_FILE_INFO_LEN, fp);			//read network information
	printf("Network Input File Information: %s\n", words);
	for (int i = 0; i < g_Net.N; i++)						//read tri_motif
	{
		fscanf(fp, "%d,", &temp1);
		degree = temp1;
		for (int j = 1; j <= degree; j++)
		{
			fscanf(fp, "{%d,%d},", &temp1, &temp2);			//{%d,%d}每个这个，代表一个三角形模体结构，即总的结构为{i,temp1,temp2}
			Push_Back_TriNK(netTri, temp1, temp2, i);
		}
		//fscanf(fp,"\n");									//have or not have is ok
	}
	fclose(fp);

	char *name = (char *)malloc(300 * sizeof(char));		//output_test
	sprintf(name, "net_read_test000000.csv");
	Output_TriNK(netTri, name, g_Net.N);
}
/*****************************************************End Of File***************************************/

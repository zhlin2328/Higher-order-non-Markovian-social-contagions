/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           file.cpp
** Created by:          linzhaohua
** Created date:        2018-5-9
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

#elif defined RSC_NETWORK
	cout << "RSC_NETWORK" << endl;
	cout << "g_Net.SERIAL: " << g_Net.SERIAL << endl;
	cout << "g_Net.N: " << g_Net.N << endl;
	cout << "g_Net.avek: " << g_Net.avek << endl;
	cout << "g_Net.avekTri: " << g_Net.avekTri << endl;

#endif
	cout << endl;
}
/********************************************************************************************************************************
** Function name:       Output_Fun_Parm
** Descriptions:        Output_Fun_Parm
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Fun_Parm(void)
{
	cout << "Spread Parmeter Information: " << endl;
	cout << "Recovery:" << endl;
	cout << "g_Fun.BETA_REC: " << g_Fun.BETA_REC << endl;
	cout << "g_Fun.ALPHA_REC: " << g_Fun.ALPHA_REC << endl;

	cout << "Infection:" << endl;
	cout << "g_Fun.ALPHA_INF_1D: " << g_Fun.ALPHA_INF_1D << endl;
	cout << "g_Fun.ALPHA_INF_2D: " << g_Fun.ALPHA_INF_2D << endl;
#ifdef PARM_INF_1D
	cout << "g_Fun.BETA_INF_2D: " << g_Fun.BETA_INF_2D << endl;
#elif defined PARM_INF_2D
	cout << "g_Fun.BETA_INF_1D: " << g_Fun.BETA_INF_1D << endl;
#endif

	cout << "g_Fun.REAL_TIME_LEN: " << g_Fun.REAL_TIME_LEN << endl;
	cout << "g_Fun.UPDATE_STEP_LEN: " << g_Fun.UPDATE_STEP_LEN << endl;
	cout << endl;
}
/********************************************************************************************************************************
** Function name:       Output_RhoT_To_File
** Descriptions:        Output_RhoT_To_File
** input parameters:	rhoT
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_RhoT_To_File(vector<vector<double> >& rhoT)
{
	int i = 0;
	FILE *fp;
	//char name[NAME_SIZE];
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
#ifdef PARM_INF_1D
	sprintf(name, "task%d_rhoT_parm=%0.5f.csv", g_taskNum, g_Fun.BETA_INF_1D);
#elif defined PARM_INF_2D
	sprintf(name, "task%d_rhoT_parm=%0.5f.csv", g_taskNum, g_Fun.BETA_INF_2D);
#endif
	fp = fopen(name, "w");
	fprintf(fp, "x,y1,y2\n");  //plot
	for (auto it = rhoT[0].begin(); it != rhoT[0].end(); it++)
	{
		fprintf(fp, "%f,%f\n", *it, rhoT[1][i]);
		++i;
	}
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Output_Rho_Parm_To_File
** Descriptions:        Output_Rho_Parm_To_File
** input parameters:    rhoParm,rho0
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Rho_Parm_To_File(vector<vector<double> >& rhoParm, double rho0)
{
	int i = 0;
	FILE *fp;
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	sprintf(name, "task%d_rhoParm_Of_rho0=%0.5f.csv", g_taskNum, rho0);
	fp = fopen(name, "w");
	fprintf(fp, "x,y1,y2,y3\n");  //plot
	for (vector<double>::iterator it = rhoParm[0].begin(); it != rhoParm[0].end(); it++)
	{
		fprintf(fp, "%f,%f,%f\n", *it, rhoParm[1][i], rhoParm[2][i]);
		++i;
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
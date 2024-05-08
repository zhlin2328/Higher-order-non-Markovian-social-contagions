/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           cal_value.h
** Created by:          linzhaohua
** Created date:        2019-10-4
** Version:             V1.0
** Descriptions:        cal_value.h
********************************************************************************************************/
#pragma warning(disable:4996)
#ifndef __CAL_VALUE_H
#define __CAL_VALUE_H

#define EPSILON					0.00001
#define NAME_SIZE				300

//#define NETWORK_FILE			"D:/output_network/ER_N2000_avek10/No0_NetSerial1_nk.csv"
#define NETWORK_FILE			"/public/home/zhlin/Desktop/network_file/simplex_networks/RSC_Network_N2000_avek14_avekTri6/No%d_NetSerial%d_nk.csv"
#define NETWORK_TRI_FILE		"/public/home/zhlin/Desktop/network_file/simplex_networks/RSC_Network_N2000_avek14_avekTri6/TRI_List_No%d_NetSerial%d_nk.csv"

#define PARM_INF_1D
//#define PARM_INF_2D

#define PARM_UP
//#define PARM_DOWN

//***时会修改的值***//
#define LAMEFF_MIN				0.0
#define LAMEFF_MAX				2.0
#define LAMEFF_DELTA			0.02				//这个是间隔精度

#define INITIAL_RHO_FROM		0.02
#define INITIAL_RHO_TO			0.02
#define INITIAL_RHO_DELTA		0.05

#define EQUAL_EPSILON			0.000000001
#define COMPARE_EPSILON			0.000000001		//比较大小的精度

typedef struct Fun_Parm
{
	double BETA_REC;				//Recovery's parameter beta 
	double ALPHA_REC;				//Recovery's parameter alpha
	double BETA_INF_1D;				//1D
	double ALPHA_INF_1D;			//1D
	double BETA_INF_2D;				//2D
	double ALPHA_INF_2D;			//2D

	double REAL_TIME_LEN;			//time_end
	double UPDATE_STEP_LEN;
	vector<vector<double> > lamEff_Parm;
	//double aveK_1D;
	//double aveK_2D;
}FPARM;
extern FPARM g_Fun;
extern int g_taskNum;

void Draw_SteadyRho_Of_Parm_Diagram(void);
int Double_Into_Int(const double oldValue);
#endif // __CAL_VALUE_H
/*****************************************************End Of File***************************************/

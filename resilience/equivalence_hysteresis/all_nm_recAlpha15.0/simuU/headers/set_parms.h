/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           set_parms.h
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        set_parms.h
********************************************************************************************************************************/
#ifndef __SET_PARMS_H
#define __SET_PARMS_H

#define NAME_SIZE							300
#define WARNING_STRING_SIZE					500
#define EPSILON								0.00001
#define TAU_INF								99999.0

#define RESCALE_TIME_STEP					1.0
#define PCT_OF_STEP_BEGIN_OF_CAL_STEADY_RHO 0.9		//t1  //PCT:percentage
#define NETWORK_FILE						"/public/home/zhlin/Desktop/network_file/simplex_networks/RSC_Network_N2000_avek14_avekTri6/No%d_NetSerial%d_nk.csv"
#define NETWORK_TRI_FILE					"/public/home/zhlin/Desktop/network_file/simplex_networks/RSC_Network_N2000_avek14_avekTri6/TRI_List_No%d_NetSerial%d_nk.csv"

#define TOTAL_DIFFUSION_NUM					100		//times
#define TOTAL_NETWORK_NUM					1		//TIMES

#define PARM_BETA_INF_1D
//#define PARM_BETA_INF_2D

//#define PARM_UP							
#define PARM_DOWN							

//***时会修改的值***//
#define LAMEFF_MIN							0.0
#define LAMEFF_MAX							0.15
#define LAMEFF_DELTA						0.005				

typedef struct Spread
{
	double BETA_REC;				//Recovery's parameter beta 
	double ALPHA_REC;				//Recovery's parameter alpha
	double BETA_INF_1D;				//1D
	double ALPHA_INF_1D;			//1D
	double BETA_INF_2D;				//2D
	double ALPHA_INF_2D;			//2D

	double REAL_TIME_LEN;			//time_end
	int INTIAL_RHO_NUM;
	double aveK_1D;
	double aveK_2D;
	vector<vector<double> > lamEff_Parm;
}SPD;
extern SPD g_Spd;
extern int g_taskNum;

void Set_Network_Parm(void);
void Set_Spd_Parm(void);
void Set_Rd_Parm(void);
int Double_Into_Int(const double old_value);

#endif //__SET_PARMS_H
/***********************************************End Of File*********************************************************************/
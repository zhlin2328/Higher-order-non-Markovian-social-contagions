/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           file.h
** Created by:          linzhaohua
** Created date:        2020-3-2
** Version:             V1.0
** Descriptions:        fun.h
********************************************************************************************************/
//#pragma warning(disable:4996) 
#ifndef __FUN_H
#define __FUN_H

#define E_VALUE							2.7182818			//自然常数
#define BISECTION_EPSILON				0.001				//找到的lamEff值如果误差在e.g.0.001以内，就认为ok(要比LAMEFF_DELTA小一个量级才行！！)

#define PARM_SEARCH_FROM				0.01				//这个值要足够小或者足够大
#define PARM_SEARCH_STEP				0.0001
#define SEARCH_SPEED					3.0					//确定范围的变步长速度

#define DIST_TIME_STEP					0.01
#define TOTAL_TIME						300.0
typedef struct TempS
{
	int TOTAL_TIME_STEPS;
	double deltaEff;
	vector<double> recSurvival;
	vector<double> cumRec_Survival;		//integrate Psi(tau'') with tau'' from tau' to inf
	vector<double> inf_1D;
	vector<double> inf_2D;
	vector<double> ita_1D;
	vector<double> ita_2D;
}TEMP;

void Cal_LamEff1D_LamEff2D_Parm(void);
#endif //__FUN_H
/*****************************************************End Of File***************************************/

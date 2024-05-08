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

#define E_VALUE							2.7182818			//��Ȼ����
#define BISECTION_EPSILON				0.001				//�ҵ���lamEffֵ��������e.g.0.001���ڣ�����Ϊok(Ҫ��LAMEFF_DELTAСһ���������У���)

#define PARM_SEARCH_FROM				0.01				//���ֵҪ�㹻С�����㹻��
#define PARM_SEARCH_STEP				0.0001
#define SEARCH_SPEED					3.0					//ȷ����Χ�ı䲽���ٶ�

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

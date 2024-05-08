/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           function.h
** Created by:          linzhaohua
** Created date:        2018-4-16
** Version:             V3.1
** Descriptions:        function.h
********************************************************************************************************/
#pragma warning(disable:4996)
#ifndef __FUNCTION_H
#define __FUNCTION_H

typedef struct Theory_Temp
{
	int TOTAL_TIME_STEPS;

	vector<double> rec;
	vector<double> recProb;
	vector<double> inf_1D;
	vector<double> inf_2D;
	vector<double> ita_1D;
	vector<double> ita_2D;

	vector<double> iValue;
	vector<double> sValue;
	vector<double> iValue_New;
	vector<double> sValue_New;
}TTEMP;

double Simplex_NonMarkovian_MF(double rho0);
#endif // __FUNCTION_H
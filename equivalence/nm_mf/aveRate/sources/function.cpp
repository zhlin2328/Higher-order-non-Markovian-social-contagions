/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           function.cpp
** Created by:          linzhaohua
** Created date:        2020-1-3
** Version:             V3.1
** Descriptions:        Simplex_NonMarkovian_QMF，一阶淬火平均场理论，画rhoParm图，迭代式算法
********************************************************************************************************/
#include "../headers/config.h"

#define E_VALUE	2.7182818			//自然常数


static void Init_Temp(TTEMP& Ttemp);
static void Cal_Inf_1D_2D_And_Rec_Fun(TTEMP& Ttemp);
static void Cal_Ita_1D_2D(TTEMP& Ttemp);
static void Init_Diffusion_Process(TTEMP& Ttemp, vector<vector<double> >& rhoT, double rho0);
static double Cal_Phi(TTEMP& Ttemp, int curStep);
static double Cal_Rho_And_Write_Into_RhoT(TTEMP& Ttemp, vector<vector<double> >& rhoT, int curStep);
static int Is_Equal(double num1, double num2);
static int Compare_Size(double numLeft, double numRight);
/********************************************************************************************************************************
** Function name:       Simplex_NonMarkovian_QMF
** Descriptions:        Simplex_NonMarkovian_QMF
** input parameters:    net,netTri,rho0
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
double Simplex_NonMarkovian_MF(double rho0)
{
	TTEMP Ttemp;
	double capPhi;
	double preRho = rho0;
	double curRho;
	double deltaRho;
	double decS;											//the decreasing fraction of S
	double decI;
	double cntS;											//the new S with age 0
	double cntI;
	vector<vector<double> > rhoT(2);

	Init_Temp(Ttemp);
	Init_Diffusion_Process(Ttemp, rhoT, rho0);
	for (int i = 1; i < Ttemp.TOTAL_TIME_STEPS; i++)			//i is curStep(new)
	{
		//cout << "step = " << i << endl;
		
		cntS = 0;
		cntI = 0;
		for (int j = 1; j <= i; j++)
		{
			capPhi = Cal_Phi(Ttemp, i);		
			if (capPhi > 1.0 or capPhi < 0.0)
			{
				cout << "capPhi is wrong,= " << capPhi << endl;
			}
			decS = (capPhi)* Ttemp.sValue[j - 1];			
			decI = Ttemp.recProb[j - 1] * Ttemp.iValue[j - 1];

			Ttemp.sValue_New[j] = Ttemp.sValue[j - 1] - decS;		//Ttemp.sValue_New: means next step value
			Ttemp.iValue_New[j] = Ttemp.iValue[j - 1] - decI;		//debug

			cntS += decS;
			cntI += decI;
		}
		Ttemp.sValue_New[0] = cntI;
		Ttemp.iValue_New[0] = cntS;


		Ttemp.sValue = Ttemp.sValue_New;
		Ttemp.iValue = Ttemp.iValue_New;

		curRho = Cal_Rho_And_Write_Into_RhoT(Ttemp, rhoT, i);
		//cout << std::setprecision(16) << "curRho = " << curRho << endl;
		deltaRho = curRho - preRho;
		if ((1.0*g_Fun.UPDATE_STEP_LEN*i) > 3.0 && Is_Equal(deltaRho, 0))
		{
			cout << "end_step i=" << i << endl;
			break; 
		}
		else if (i == Ttemp.TOTAL_TIME_STEPS)
		{
			cout << "Not Reach Steady State" << endl;
			cout << "deltaRho = " << deltaRho << endl;
		}
		preRho = curRho;
	}
	//Output_RhoT_To_File(rhoT);

	return curRho;
}
/********************************************************************************************************************************
** Function name:       Init_Temp
** Descriptions:        Init_Temp
** input parameters:    Ttemp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_Temp(TTEMP& Ttemp)
{
	Ttemp.TOTAL_TIME_STEPS = Double_Into_Int(g_Fun.REAL_TIME_LEN / g_Fun.UPDATE_STEP_LEN);		//step from 0~Ttemp.TOTAL_TIME_STEPS-1，总共Ttemp.TOTAL_TIME_STEPS个步长

	Ttemp.inf_1D.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);
	Ttemp.inf_2D.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);
	Ttemp.rec.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);
	Ttemp.recProb.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);

	Ttemp.ita_1D.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);
	Ttemp.ita_2D.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);

	Ttemp.iValue.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);
	Ttemp.sValue.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);
	Ttemp.iValue_New.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);
	Ttemp.sValue_New.resize(Ttemp.TOTAL_TIME_STEPS, 0.0);

	Cal_Inf_1D_2D_And_Rec_Fun(Ttemp);
	Cal_Ita_1D_2D(Ttemp);
}
/********************************************************************************************************************************
** Function name:       Cal_Inf_1D_2D_And_Rec_Fun
** Descriptions:        Cal_Inf_1D_2D_And_Rec_Fun
** input parameters:    Ttemp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Inf_1D_2D_And_Rec_Fun(TTEMP& Ttemp)
{
	double tmp1;
	double tmp2;
	double tmp3;
	
	for (int i = 0; i < Ttemp.TOTAL_TIME_STEPS; i++)
	{
		tmp1 = pow((E_VALUE), -pow(((double)i)*g_Fun.UPDATE_STEP_LEN / g_Fun.BETA_INF_1D, g_Fun.ALPHA_INF_1D));		//inf_1D
		tmp2 = pow((E_VALUE), -pow(((double)(i + 1))*g_Fun.UPDATE_STEP_LEN / g_Fun.BETA_INF_1D, g_Fun.ALPHA_INF_1D));
		Ttemp.inf_1D[i] = tmp1 - tmp2;	

		tmp1 = pow((E_VALUE), -pow(((double)i)*g_Fun.UPDATE_STEP_LEN / g_Fun.BETA_INF_2D, g_Fun.ALPHA_INF_2D));		//inf_2D
		tmp2 = pow((E_VALUE), -pow(((double)(i + 1))*g_Fun.UPDATE_STEP_LEN / g_Fun.BETA_INF_2D, g_Fun.ALPHA_INF_2D));
		Ttemp.inf_2D[i] = tmp1 - tmp2;

		tmp1 = pow((E_VALUE), -pow(((double)i)*g_Fun.UPDATE_STEP_LEN / g_Fun.BETA_REC, g_Fun.ALPHA_REC));			//rec
		tmp2 = pow((E_VALUE), -pow(((double)(i + 1))*g_Fun.UPDATE_STEP_LEN / g_Fun.BETA_REC, g_Fun.ALPHA_REC));
		Ttemp.rec[i] = tmp1 - tmp2;
		tmp3 = (g_Fun.ALPHA_REC / g_Fun.BETA_REC)*pow(((double)i)*g_Fun.UPDATE_STEP_LEN / g_Fun.BETA_REC, g_Fun.ALPHA_REC - 1.0)*g_Fun.UPDATE_STEP_LEN;
		Ttemp.recProb[i] = (tmp3 >= 1.0) ? 1.0 : tmp3;
	}
}
/********************************************************************************************************************************
** Function name:       Cal_Ita_1D_2D
** Descriptions:        Cal_Ita_1D_2D
** input parameters:    Ttemp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Ita_1D_2D(TTEMP& Ttemp)
{
	for (int i = 0; i < Ttemp.TOTAL_TIME_STEPS; i++)						
	{
		Ttemp.ita_1D[i] = Ttemp.inf_1D[i];								//psi_inf 1D
		Ttemp.ita_2D[i] = Ttemp.inf_2D[i];								//psi_inf 2D
		for (int j = 0; j < i; j++)
		{
			Ttemp.ita_1D[i] += Ttemp.ita_1D[j] * Ttemp.inf_1D[i - j - 1];		
			Ttemp.ita_2D[i] += Ttemp.ita_2D[j] * Ttemp.inf_2D[i - j - 1];		
		}
	}
}
/********************************************************************************************************************************
** Function name:       Init_Diffusion_Process
** Descriptions:        Init_Diffusion_Process
** input parameters:    Ttemp,rhoT,rho0
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_Diffusion_Process(TTEMP& Ttemp, vector<vector<double> >& rhoT, double rho0)
{
	Ttemp.iValue[0] = rho0;
	Ttemp.sValue[0] = 1.0 - rho0;

	rhoT[0].push_back(0.0);
	rhoT[1].push_back(rho0);
}
/********************************************************************************************************************************
** Function name:       Cal_Phi
** Descriptions:        Cal_Phi
** input parameters:    net,netTri,Ttemp,node,curStep
** output parameters:   phi
** Returned value:      phi
********************************************************************************************************************************/
double Cal_Phi(TTEMP& Ttemp, int curStep)
{
	double phi = 0.0;
	double temp1;
	double aValue_2D;
	double tmp1D = 1.0;
	double tmp2D = 1.0;

	for (int i = 0; i < curStep; i++)			
	{
		tmp1D *= (1.0 - Ttemp.ita_1D[i] * Ttemp.iValue[i]);		
	}

	tmp1D = pow(tmp1D, 1.0*g_Net.avek);

	temp1 = 0.0;
	for (int i = curStep - 1; i >= 0; i--)			
	{
		temp1 += Ttemp.iValue[i];
		aValue_2D = Ttemp.iValue[i] * temp1 + Ttemp.iValue[i] * temp1 - Ttemp.iValue[i] * Ttemp.iValue[i];
		tmp2D *= (1.0 - Ttemp.ita_2D[i] * aValue_2D);
	}
	tmp2D = pow(tmp2D, 1.0*g_Net.avekTri);

	phi += 1.0 - tmp1D*tmp2D;
	

	return phi;
}
/********************************************************************************************************************************
** Function name:       Cal_Rho_And_Write_Into_RhoT
** Descriptions:        Cal_Rho_And_Write_Into_RhoT
** input parameters:    Ttemp,rhoT,curStep
** output parameters:   rho
** Returned value:      rho
********************************************************************************************************************************/
double Cal_Rho_And_Write_Into_RhoT(TTEMP& Ttemp, vector<vector<double> >& rhoT, int curStep)
{
	double rho = 0.0;

	for (int i = 0; i <= curStep; i++)
	{
		rho += Ttemp.iValue[i];
	}
	rhoT[0].push_back(curStep * g_Fun.UPDATE_STEP_LEN);
	rhoT[1].push_back(rho);

	return rho;
}
/********************************************************************************************************************************
** Function name:       Is_Equal
** Descriptions:        判断是否在EQUAL_EPSILON精度内num1=num2
** input parameters:    num1,num2
** output parameters:   0(false),1(true)
** Returned value:      0,1
********************************************************************************************************************************/
int Is_Equal(double num1, double num2)
{
	double sol = num1 - num2;

	if ((sol >= -(EQUAL_EPSILON)) && (sol <= EQUAL_EPSILON))
	{
		return 1;
	}
	return 0;
}
/********************************************************************************************************************************
** Function name:       Compare_Size
** Descriptions:        判断numLeft与numRight的大小关系
** input parameters:    numLeft,numRight
** output parameters:   0(numLeft less than numRight),1(greater),2(equal)
** Returned value:      0,1,2
********************************************************************************************************************************/
int Compare_Size(double numLeft, double numRight)
{
	double sol = numLeft - numRight;

	if ((sol >= -(COMPARE_EPSILON)) && (sol <= COMPARE_EPSILON))	//numLeft=numRight
	{
		return 2;
	}
	else if (sol > COMPARE_EPSILON)									//numLeft>numRight
	{
		return 1;
	}
	else															//numLeft<numRight
	{
		return 0;
	}
}
/*****************************************************End Of File***************************************/

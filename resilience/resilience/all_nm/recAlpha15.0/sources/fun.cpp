/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           fun.cpp
** Created by:          linzhaohua
** Created date:        2020-3-2
** Version:             V1.0
** Descriptions:        fun.cpp，计算1D的值，存入g_Spd.lamEff_Parm数组中，
						g_Spd.lamEff_Parm[0]对应的为lamEff1D,g_Spd.lamEff_Parm[1]对应为parm
********************************************************************************************************/
#include "../headers/config.h"

static void Cal_LamEff_And_Parm(TEMP& Temp);
static void Method_Of_Bisection(TEMP& Temp, double initLeft, double initRight, double lamEff,
	double& parmSol, double& lamEff_Sol);
static void Update_Var_Of_Temp(TEMP& Temp, double parm);
static void Cal_Inf_1D(TEMP& Temp);
static void Cal_Ita_1D(TEMP& Temp);
static double Cal_LamEff_1D(TEMP& Temp);

static void Init_Temp(TEMP& Temp);
static double Cal_LamEff_2D(TEMP& Temp);
static void Cal_Var_Of_Temp(TEMP& Temp);
static void Cal_Ita_2D(TEMP& Temp);

void Output_LamEff2D_To_File(double lamEff_2D);
void Output_LamEff1D_To_File(void);
/********************************************************************************************************************************
** Function name:       Cal_LamEff1D_LamEff2D_Parm
** Descriptions:        这个函数是总的，计算lamEff2D（输出到文件中）
						以及lamEff1D,parm of lamEff1D的值（存入g_Spd.lamEff_Parm数组中，[0]对应的为lamEff1D,[1]对应为parm）
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_LamEff1D_LamEff2D_Parm(void)
{
	TEMP Temp;
	double lamEff_2D;
	g_Spd.lamEff_Parm.resize(2);

	Init_Temp(Temp);
	lamEff_2D = Cal_LamEff_2D(Temp);
	Output_LamEff2D_To_File(lamEff_2D);
	cout << "lamEff_2D = " << lamEff_2D << endl;

	Cal_LamEff_And_Parm(Temp);
	Output_LamEff1D_To_File();
}
/********************************************************************************************************************************
** Function name:       Cal_Effective_Rate_And_Output_Into_File
** Descriptions:        这个函数是固定平均时间一定，改变beta_inf值，计算aveInf_Rate_1D,aveRec_Rate,aveInf_Rate_2D（输出到文件中）
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Effective_Rate_And_Output_Into_File(void)
{
	double x1 = 1.0 + 1.0 / g_Spd.ALPHA_REC;
	double x2;
	double aveRec_Rate = 1.0 / (g_Spd.BETA_REC * Gamma_Fun1(x1));
	double temp;

	g_Spd.lamEff_Parm.resize(2);
#ifdef PARM_INF_2D
	x2 = 1.0 + 1.0 / g_Spd.ALPHA_INF_2D;
#elif defined PARM_INF_1D
	x2 = 1.0 + 1.0 / g_Spd.ALPHA_INF_1D;
#endif
	for (double effDif_Rate = (LAMEFF_MIN); effDif_Rate < ((LAMEFF_MAX)+((0.001)*(LAMEFF_DELTA))); effDif_Rate += (LAMEFF_DELTA))
	{
		temp = 1.0 / (effDif_Rate*aveRec_Rate*Gamma_Fun1(x2));
		
		g_Spd.lamEff_Parm[0].push_back(effDif_Rate);
		g_Spd.lamEff_Parm[1].push_back(temp);
	}

	FILE *fp;
	char *name = (char *)malloc(NAME_SIZE * sizeof(char));

	sprintf(name, "task%d_lamDif_Eff_1D.csv", g_taskNum);
	fp = fopen(name, "w");
#ifdef PARM_INF_2D
	fprintf(fp, "aveInf_Rate_1D,alphaI_1D,betaI_1D,aveRec_Rate,alphaR,betaR\n");
	fprintf(fp, "%f,%f,%f,%f,%f,%f\n", 1.0 / (g_Spd.BETA_INF_1D * Gamma_Fun1(1.0 + 1.0 / g_Spd.ALPHA_INF_1D)),
		g_Spd.ALPHA_INF_1D, g_Spd.BETA_INF_1D, aveRec_Rate, g_Spd.ALPHA_REC, g_Spd.BETA_REC);
#elif defined PARM_INF_1D
	fprintf(fp, "aveInf_Rate_2D,alphaI_2D,betaI_2D,aveRec_Rate,alphaR,betaR\n");
	fprintf(fp, "%f,%f,%f,%f,%f,%f\n", 1.0 / (g_Spd.BETA_INF_2D * Gamma_Fun1(1.0 + 1.0 / g_Spd.ALPHA_INF_2D)),
		g_Spd.ALPHA_INF_2D, g_Spd.BETA_INF_2D, aveRec_Rate, g_Spd.ALPHA_REC, g_Spd.BETA_REC);
#endif
	fclose(fp);

	int index = 0;
	sprintf(name, "task%d_lamDif_Eff_2D_parm.csv", g_taskNum);
	fp = fopen(name, "w");
#ifdef PARM_INF_2D
	fprintf(fp, "effDif_Rate,aveInf_Rate_2D\n");
#elif defined PARM_INF_1D
	fprintf(fp, "effDif_Rate,aveInf_Rate_1D\n");
#endif
	for (auto it = g_Spd.lamEff_Parm[0].begin(); it != g_Spd.lamEff_Parm[0].end(); it++)
	{
		fprintf(fp, "%f,%f\n", *it, g_Spd.lamEff_Parm[1][index]);
		index++;
	}
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Cal_LamEff_And_Parm
** Descriptions:        根据初始的PARM_SEARCH_FROM开始不断+deltaParm来计算对应的lamEff，
						最终计算出[0.0:LAMEFF_DELTA:1.0]之间的所有目标desLamEff与对应的parm

						(其中：不断+deltaParm时会得到每一组curParm与nextParm(也得到对应的curLamEff，nextLamEff)，
						若desLamEff属于区间[curLamEff,nextLamEff]，则调用二分法函数，计算得出最终的desLamEff对应的parm值)
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_LamEff_And_Parm(TEMP& Temp)
{
	double lamEff_Min = (LAMEFF_MIN);									//求出[lamEff_Min,lamEff_Max]范围内的lamEff_parm值
	double lamEff_Max = (LAMEFF_MAX);

	double curParm = (PARM_SEARCH_FROM);
	double nextParm;
	double parmSol;

	double curLamEff;
	double nextLamEff;
	double lamEff_Sol;
	double desLamEff;

	double deltaLam = (LAMEFF_DELTA);
	double searchStep = (PARM_SEARCH_STEP);
	double speed = (SEARCH_SPEED);
	double deltaParm;													//deltaParm = searchStep * speed^n(n次方)
	double value;
	int flag;
	int isNear_lamEff_Min;
	int isInc;

	g_Spd.lamEff_Parm.resize(2);
	//***计算初始的curParm对应的curLamEff***//
	Update_Var_Of_Temp(Temp, curParm);									//根据初始的curParm = (PARM_SEARCH_FROM)来计算这一步curLamEff
	curLamEff = Cal_LamEff_1D(Temp);

	//***计算下一步的nextParm对应的nextLamEff***//						
	nextParm = curParm + searchStep;									//nextParm += curParm + searchStep;来计算下一步nextLamEff
	Update_Var_Of_Temp(Temp, nextParm);
	nextLamEff = Cal_LamEff_1D(Temp);

	//***判断初始值parm和lamEff分别朝增加 or 降低的方向***//
	isNear_lamEff_Min = (abs(curLamEff - lamEff_Min) < abs(curLamEff - lamEff_Max)) ? 1 : 0;
	isInc = (nextLamEff - curLamEff > 0.0) ? 1 : 0;						//判断lamEff是单调递增(>0)/减(<0)方向
	if (nextLamEff - curLamEff == 0.0)
	{
		cout << "searchStep is too low and thus cannot cal value of isInc_Direction!" << endl;
	}
	if (isNear_lamEff_Min == 1)
	{
		if (isInc == 0)
		{
			searchStep = -searchStep;
		}
		desLamEff = lamEff_Min + deltaLam;
	}
	else
	{
		if (isInc == 1)
		{
			searchStep = -searchStep;
		}
		deltaLam = -deltaLam;
		desLamEff = lamEff_Max + deltaLam;
	}

	do
	{
		cout << endl;
		cout << "curParm = " << curParm << endl;
		cout << "curLamEff = " << setprecision(8) << curLamEff << endl;
		cout << "desLamEff = " << setprecision(8) << desLamEff << endl;
		deltaParm = searchStep;
		//***找到desLamEff对应parm区间[curParm,nextParm]***//				//不断循环计算新的nextParm，直到找到desLamEff所对应parm的区间[curParm,nextParm]
		do
		{
			nextParm += deltaParm;
			Update_Var_Of_Temp(Temp, nextParm);
			nextLamEff = Cal_LamEff_1D(Temp);
			value = (nextLamEff - desLamEff) * (curLamEff - desLamEff);
			flag = (value > 0.0) ? 1 : 0;
			if (flag)
			{
				curParm = nextParm;
				curLamEff = nextLamEff;
				deltaParm *= speed;
			}
			//cout << "nextParm = " << nextParm << endl;
			//cout << "nextLamEff = " << nextLamEff << endl;
		} while (flag);

		//***调用二分法算出desLamEff对应的parmSol值***//	
		Method_Of_Bisection(Temp, curParm, nextParm, desLamEff, parmSol, lamEff_Sol);
		g_Spd.lamEff_Parm[0].push_back(lamEff_Sol);
		g_Spd.lamEff_Parm[1].push_back(parmSol);

		curParm = parmSol;
		curLamEff = lamEff_Sol;
		nextParm = curParm;
		desLamEff += deltaLam;
	} while (desLamEff > lamEff_Min && desLamEff < lamEff_Max);
}
/********************************************************************************************************************************
** Function name:       Method_Of_Bisection
** Descriptions:        Method_Of_Bisection
** input parameters:    Temp,initLeft,initRight,lamEff,parmSol,lamEff_Sol
						(其中parmSol和lamEff_Sol是通过二分法最后找出的结果)
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Method_Of_Bisection(TEMP& Temp, double initLeft, double initRight, double lamEff,
	double& parmSol, double& lamEff_Sol)
{
	double middle;
	double precise = (BISECTION_EPSILON);
	double left = initLeft;
	double right = initRight;
	double solL;
	double solR;
	double solM;
	int iterationNum = 0;

	Update_Var_Of_Temp(Temp, left);
	solL = Cal_LamEff_1D(Temp) - lamEff;
	Update_Var_Of_Temp(Temp, right);
	solR = Cal_LamEff_1D(Temp) - lamEff;
	if (fabs(solL) < precise)				//若left对应lamEff已满足误差范围内的值，则停止运算并return
	{
		parmSol = left;
		lamEff_Sol = solL + lamEff;
	}
	else if (fabs(solR) < precise)			//若right对应lamEff已满足误差范围内的值，则停止运算并return
	{
		parmSol = right;
		lamEff_Sol = solR + lamEff;
	}
	else									//否则，一直二分法直到得出最后的结果
	{
		while (1)
		{
			middle = (left + right) / 2.0;
			Update_Var_Of_Temp(Temp, middle);
			solM = Cal_LamEff_1D(Temp) - lamEff;
			if (fabs(solM) <= precise)		//已找到，存储下来并结束
			{
				parmSol = middle;
				lamEff_Sol = solM + lamEff;
				break;
			}
			else
			{
				if (solM*solL < 0)
				{
					right = middle;
				}
				else if (solM*solR < 0)
				{
					left = middle;
				}
			}
			iterationNum++;
		}
		//cout << "iterationNum = " << iterationNum << endl;
	}
}
/********************************************************************************************************************************
** Function name:       Update_Var_Of_Temp
** Descriptions:        由于这里我们改变BETA_INF_2D（2维的）或者BETA_INF_1D（1维的），所以更新的时候只要改变inf_1D和ita_1D的值
** input parameters:    Temp,parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Update_Var_Of_Temp(TEMP& Temp, double parm)
{
#ifdef PARM_BETA_INF_1D
	g_Spd.BETA_INF_1D = parm;
#elif defined PARM_BETA_INF_2D
	g_Spd.BETA_INF_2D = parm;
#endif

	vector<double>().swap(Temp.inf_1D);
	vector<double>().swap(Temp.ita_1D);
	Temp.inf_1D.resize(Temp.TOTAL_TIME_STEPS, 0.0);
	Temp.ita_1D.resize(Temp.TOTAL_TIME_STEPS, 0.0);

	Cal_Inf_1D(Temp);
	Cal_Ita_1D(Temp);											//ita_2D
}
/********************************************************************************************************************************
** Function name:       Cal_Inf_1D
** Descriptions:        Cal_Inf_1D
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Inf_1D(TEMP& Temp)
{
	double tmp1;
	double tmp2;
	double timeStep = (DIST_TIME_STEP);

	double sum_debug1 = 0.0;		//debug_test

	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)				//inf_2D
	{
		tmp1 = pow((E_VALUE), -pow(((double)i)*timeStep / g_Spd.BETA_INF_1D, g_Spd.ALPHA_INF_1D));
		tmp2 = pow((E_VALUE), -pow(((double)(i + 1))*timeStep / g_Spd.BETA_INF_1D, g_Spd.ALPHA_INF_1D));
		Temp.inf_1D[i] = tmp1 - tmp2;

		sum_debug1 += Temp.inf_1D[i];		//debug_test
	}
	cout << "sum_of_Inf_1D = " << sum_debug1 << endl;		//debug_test
}
/********************************************************************************************************************************
** Function name:       Cal_Ita_1D
** Descriptions:        Cal_Ita_1D
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Ita_1D(TEMP& Temp)
{
	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)
	{
		Temp.ita_1D[i] = Temp.inf_1D[i];								//psi_inf 1D
		for (int j = 0; j < i; j++)
		{
			Temp.ita_1D[i] += Temp.ita_1D[j] * Temp.inf_1D[i - j - 1];
		}
	}
}
/********************************************************************************************************************************
** Function name:       Cal_LamEff_1D
** Descriptions:        根据ita_1D（通过inf_1D可得出），recSurvival和deltaEff即可算出lamEff_1D
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
double Cal_LamEff_1D(TEMP& Temp)
{
	double lamEff_1D = 0.0;
	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)
	{
		lamEff_1D += Temp.ita_1D[i] * Temp.recSurvival[i];				//ita_1D[i]的值已经自带了dt
	}

	return lamEff_1D;
}



/********************************************************************************************************************************
** Function name:       Init_Temp
** Descriptions:        Init_Temp
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_Temp(TEMP& Temp)
{
	Temp.TOTAL_TIME_STEPS = Double_Into_Int((TOTAL_TIME) / (DIST_TIME_STEP));		//step from 0~Temp.TOTAL_TIME_STEPS-1，总共Temp.TOTAL_TIME_STEPS个步长

	Temp.inf_1D.resize(Temp.TOTAL_TIME_STEPS, 0.0);
	Temp.inf_2D.resize(Temp.TOTAL_TIME_STEPS, 0.0);
	Temp.recSurvival.resize(Temp.TOTAL_TIME_STEPS, 0.0);
	Temp.cumRec_Survival.resize(Temp.TOTAL_TIME_STEPS, 0.0);

	Temp.ita_1D.resize(Temp.TOTAL_TIME_STEPS, 0.0);
	Temp.ita_2D.resize(Temp.TOTAL_TIME_STEPS, 0.0);

	Cal_Var_Of_Temp(Temp);
}
/********************************************************************************************************************************
** Function name:       Cal_LamEff_2D
** Descriptions:        根据ita_2D（通过inf_2D可得出）和recSurvival即可算出lamEff_2D
** input parameters:    Temp
** output parameters:   lamEff_2D
** Returned value:      lamEff_2D
********************************************************************************************************************************/
double Cal_LamEff_2D(TEMP& Temp)		//dai ding
{
	double lamEff_2D = 0.0;
	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)
	{
		lamEff_2D += 2.0*Temp.ita_2D[i] * Temp.recSurvival[i]
			* Temp.deltaEff*Temp.cumRec_Survival[i];				//ita_2D[i]的值已经自带了dt
	}

	return lamEff_2D;
}
/********************************************************************************************************************************
** Function name:       Cal_Var_Of_Temp
** Descriptions:        计算inf_2D，recSurvival，deltaEff，ita_2D

						（备注：这里没有计算inf_1D和ita_1D，因为之后要不断更新parm值来找对应的lamEff，
						所以是在那时候通过调用 Update_Var_Of_Temp() 和 Cal_LamEff_1D() 才计算这个值，
						然后求出对应的lamEff）
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Var_Of_Temp(TEMP& Temp)
{
	double tmp1;
	double tmp2;
	double timeStep = (DIST_TIME_STEP);
	Temp.deltaEff = 0.0;

	double sum_debug1 = 0.0;		//debug_test

	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)
	{
		tmp1 = pow((E_VALUE), -pow(((double)(i))*timeStep / g_Spd.BETA_INF_2D, g_Spd.ALPHA_INF_2D));		//inf_2D
		tmp2 = pow((E_VALUE), -pow(((double)(i + 1))*timeStep / g_Spd.BETA_INF_2D, g_Spd.ALPHA_INF_2D));
		Temp.inf_2D[i] = tmp1 - tmp2;																	//这个已经自带dt了，因为是i的量减去i+1的
		//这里的tau~tau+dt时间内感染的概率inf_2D(即psi_inf*dt)是用相邻时间的存活概率相减得到的

		sum_debug1 += Temp.inf_2D[i];		//debug_test

		tmp1 = pow((E_VALUE), -pow(((double)i)*timeStep / g_Spd.BETA_REC, g_Spd.ALPHA_REC));			//recSurvival probability
		Temp.recSurvival[i] = tmp1;
	}
	cout << "sum_of_Inf_2D = " << sum_debug1 << endl;		//debug_test
	cout << "sum_of_Rec = " << Temp.recSurvival.back() << endl;		//debug_test

	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)														//deltaEff
	{
		Temp.deltaEff += Temp.recSurvival[i];
		for (int j = i; j < Temp.TOTAL_TIME_STEPS; j++)
		{
			Temp.cumRec_Survival[i] += Temp.recSurvival[j];
		}
		Temp.cumRec_Survival[i] *= timeStep;
	}
	Temp.deltaEff = 1.0 / (Temp.deltaEff * timeStep);

	Cal_Ita_2D(Temp);
}
/********************************************************************************************************************************
** Function name:       Cal_Ita_2D
** Descriptions:        Cal_Ita_2D
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_Ita_2D(TEMP& Temp)
{
	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)
	{
		Temp.ita_2D[i] = Temp.inf_2D[i];									//psi_inf 2D
		for (int j = 0; j < i; j++)
		{
			Temp.ita_2D[i] += Temp.ita_2D[j] * Temp.inf_2D[i - j - 1];		//这个Temp.ita_2D[j] * Temp.inf_2D[i - j]里面自带dt*dt
		}
	}
}





/********************************************************************************************************************************
** Function name:       Output_LamEff2D_To_File
** Descriptions:        Output_LamEff2D_To_File
** input parameters:    lamEff_1D
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_LamEff2D_To_File(double lamEff_2D)
{
	FILE *fp;
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	sprintf(name, "task%d_lamEff_2D.csv", g_taskNum);
	fp = fopen(name, "w");
	fprintf(fp, "lamEff_2D,alphaI_2D,betaI_2D,alphaR,betaR\n");
	fprintf(fp, "%f,%f,%f,%f,%f\n", lamEff_2D, g_Spd.ALPHA_INF_2D, g_Spd.BETA_INF_2D, g_Spd.ALPHA_REC, g_Spd.BETA_REC);
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Output_LamEff1D_To_File
** Descriptions:        Output_LamEff1D_To_File
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_LamEff1D_To_File(void)
{
	FILE *fp;
	char *name;
	int index = 0;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	sprintf(name, "task%d_lamEff_1D_parm.csv", g_taskNum);
	fp = fopen(name, "w");
	for (auto it = g_Spd.lamEff_Parm[0].begin(); it != g_Spd.lamEff_Parm[0].end(); it++)
	{
		fprintf(fp, "%f,%f\n", *it, g_Spd.lamEff_Parm[1][index]);
		index++;
	}
	fclose(fp);
	free(name);
}
/********************************************************************************************************************************
** Function name:       Gamma_Fun
** Descriptions:        计算gamma函数的函数值（实数域，x>0）
** input parameters:    x(is x of gamma(x))
** output parameters:   gamma(x)
** Returned value:      gamma(x)
********************************************************************************************************************************/
double Gamma_Fun1(double x)
{
	double sol = 0.0;
	double dt = 0.00001;		//step
	double tMax = 500.0;		//tmax

	if (x - 1.0 >= 0.0)
	{
		sol += pow(0.0, x - 1.0)*exp(-0.0)*dt;
	}
	for (double t = dt; t <= tMax; t += dt)
	{
		sol += pow(t, x - 1.0)*exp(-t)*dt;
	}

	return sol;
}
//别人写的gamma函数
double Gamma_Fun2(const double xx)
{
	int j;
	double x, y, tmp, ser;
	double sol;
	static const double cof[6] = { 76.18009172947146,
	-86.50532032941677,24.0140982408391,-1.231739572450155,
	0.1208650713866179e-2,-0.5395239384953e-5 };

	y = x = xx;
	tmp = x + 5.5;
	tmp -= (x + 0.5)*log(tmp);
	ser = 1.000000000190015;
	for (j = 0; j < 6; j++) ser += cof[j] / ++y;
	sol = -tmp + log(2.5066282746310005*ser / x);
	sol = exp(sol);

	return sol;
}
/*****************************************************End Of File***************************************/
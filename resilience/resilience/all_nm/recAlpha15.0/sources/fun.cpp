/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           fun.cpp
** Created by:          linzhaohua
** Created date:        2020-3-2
** Version:             V1.0
** Descriptions:        fun.cpp������1D��ֵ������g_Spd.lamEff_Parm�����У�
						g_Spd.lamEff_Parm[0]��Ӧ��ΪlamEff1D,g_Spd.lamEff_Parm[1]��ӦΪparm
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
** Descriptions:        ����������ܵģ�����lamEff2D��������ļ��У�
						�Լ�lamEff1D,parm of lamEff1D��ֵ������g_Spd.lamEff_Parm�����У�[0]��Ӧ��ΪlamEff1D,[1]��ӦΪparm��
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
** Descriptions:        ��������ǹ̶�ƽ��ʱ��һ�����ı�beta_infֵ������aveInf_Rate_1D,aveRec_Rate,aveInf_Rate_2D��������ļ��У�
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
** Descriptions:        ���ݳ�ʼ��PARM_SEARCH_FROM��ʼ����+deltaParm�������Ӧ��lamEff��
						���ռ����[0.0:LAMEFF_DELTA:1.0]֮�������Ŀ��desLamEff���Ӧ��parm

						(���У�����+deltaParmʱ��õ�ÿһ��curParm��nextParm(Ҳ�õ���Ӧ��curLamEff��nextLamEff)��
						��desLamEff��������[curLamEff,nextLamEff]������ö��ַ�����������ó����յ�desLamEff��Ӧ��parmֵ)
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_LamEff_And_Parm(TEMP& Temp)
{
	double lamEff_Min = (LAMEFF_MIN);									//���[lamEff_Min,lamEff_Max]��Χ�ڵ�lamEff_parmֵ
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
	double deltaParm;													//deltaParm = searchStep * speed^n(n�η�)
	double value;
	int flag;
	int isNear_lamEff_Min;
	int isInc;

	g_Spd.lamEff_Parm.resize(2);
	//***�����ʼ��curParm��Ӧ��curLamEff***//
	Update_Var_Of_Temp(Temp, curParm);									//���ݳ�ʼ��curParm = (PARM_SEARCH_FROM)��������һ��curLamEff
	curLamEff = Cal_LamEff_1D(Temp);

	//***������һ����nextParm��Ӧ��nextLamEff***//						
	nextParm = curParm + searchStep;									//nextParm += curParm + searchStep;��������һ��nextLamEff
	Update_Var_Of_Temp(Temp, nextParm);
	nextLamEff = Cal_LamEff_1D(Temp);

	//***�жϳ�ʼֵparm��lamEff�ֱ����� or ���͵ķ���***//
	isNear_lamEff_Min = (abs(curLamEff - lamEff_Min) < abs(curLamEff - lamEff_Max)) ? 1 : 0;
	isInc = (nextLamEff - curLamEff > 0.0) ? 1 : 0;						//�ж�lamEff�ǵ�������(>0)/��(<0)����
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
		//***�ҵ�desLamEff��Ӧparm����[curParm,nextParm]***//				//����ѭ�������µ�nextParm��ֱ���ҵ�desLamEff����Ӧparm������[curParm,nextParm]
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

		//***���ö��ַ����desLamEff��Ӧ��parmSolֵ***//	
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
						(����parmSol��lamEff_Sol��ͨ�����ַ�����ҳ��Ľ��)
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
	if (fabs(solL) < precise)				//��left��ӦlamEff��������Χ�ڵ�ֵ����ֹͣ���㲢return
	{
		parmSol = left;
		lamEff_Sol = solL + lamEff;
	}
	else if (fabs(solR) < precise)			//��right��ӦlamEff��������Χ�ڵ�ֵ����ֹͣ���㲢return
	{
		parmSol = right;
		lamEff_Sol = solR + lamEff;
	}
	else									//����һֱ���ַ�ֱ���ó����Ľ��
	{
		while (1)
		{
			middle = (left + right) / 2.0;
			Update_Var_Of_Temp(Temp, middle);
			solM = Cal_LamEff_1D(Temp) - lamEff;
			if (fabs(solM) <= precise)		//���ҵ����洢����������
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
** Descriptions:        �����������Ǹı�BETA_INF_2D��2ά�ģ�����BETA_INF_1D��1ά�ģ������Ը��µ�ʱ��ֻҪ�ı�inf_1D��ita_1D��ֵ
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
** Descriptions:        ����ita_1D��ͨ��inf_1D�ɵó�����recSurvival��deltaEff�������lamEff_1D
** input parameters:    Temp
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
double Cal_LamEff_1D(TEMP& Temp)
{
	double lamEff_1D = 0.0;
	for (int i = 0; i < Temp.TOTAL_TIME_STEPS; i++)
	{
		lamEff_1D += Temp.ita_1D[i] * Temp.recSurvival[i];				//ita_1D[i]��ֵ�Ѿ��Դ���dt
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
	Temp.TOTAL_TIME_STEPS = Double_Into_Int((TOTAL_TIME) / (DIST_TIME_STEP));		//step from 0~Temp.TOTAL_TIME_STEPS-1���ܹ�Temp.TOTAL_TIME_STEPS������

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
** Descriptions:        ����ita_2D��ͨ��inf_2D�ɵó�����recSurvival�������lamEff_2D
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
			* Temp.deltaEff*Temp.cumRec_Survival[i];				//ita_2D[i]��ֵ�Ѿ��Դ���dt
	}

	return lamEff_2D;
}
/********************************************************************************************************************************
** Function name:       Cal_Var_Of_Temp
** Descriptions:        ����inf_2D��recSurvival��deltaEff��ita_2D

						����ע������û�м���inf_1D��ita_1D����Ϊ֮��Ҫ���ϸ���parmֵ���Ҷ�Ӧ��lamEff��
						����������ʱ��ͨ������ Update_Var_Of_Temp() �� Cal_LamEff_1D() �ż������ֵ��
						Ȼ�������Ӧ��lamEff��
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
		Temp.inf_2D[i] = tmp1 - tmp2;																	//����Ѿ��Դ�dt�ˣ���Ϊ��i������ȥi+1��
		//�����tau~tau+dtʱ���ڸ�Ⱦ�ĸ���inf_2D(��psi_inf*dt)��������ʱ��Ĵ���������õ���

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
			Temp.ita_2D[i] += Temp.ita_2D[j] * Temp.inf_2D[i - j - 1];		//���Temp.ita_2D[j] * Temp.inf_2D[i - j]�����Դ�dt*dt
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
** Descriptions:        ����gamma�����ĺ���ֵ��ʵ����x>0��
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
//����д��gamma����
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
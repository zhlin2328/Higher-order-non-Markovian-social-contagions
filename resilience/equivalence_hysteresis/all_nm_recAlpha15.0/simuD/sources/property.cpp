/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           property.cpp
** Created date:        2018-11-7
** Version:             V1.0
** Descriptions:        property.cpp
********************************************************************************************************/
#include "../headers/config.h"
/********************************************************************************************************************************
** Function name:       Nkmax_NK
** Descriptions:        caculate nkmax(degree maximum) for nk
** input parameters:    N,net_nk
** output parameters:   nkmax
** Returned value:      nkmax
********************************************************************************************************************************/
int Nkmax_NK(const int N, LIST *net_nk)
{
	int nkmax = 0;
	int temp;

	for (int i = 0; i < N; i++)
	{
		temp = net_nk[i].degree;
		nkmax = (nkmax > temp) ? nkmax : temp;
	}
	return nkmax;
}
/********************************************************************************************************************************
** Function name:       Avenk_NK
** Descriptions:        calculate avenk_nk
** input parameters:    N,net_nk
** output parameters:   avenk
** Returned value:      avenk
********************************************************************************************************************************/
double Avenk_NK(const int N, LIST *net_nk)
{
	double avenk = 0;
	for (int i = 0; i < N; i++)
	{
		avenk += net_nk[i].degree;
	}
	avenk /= N;
	return avenk;
}
/********************************************************************************************************************************
** Function name:       Avenk_TriNK
** Descriptions:        calculate avek of netTri
** input parameters:    N,netTri
** output parameters:   avenk
** Returned value:      avenk
********************************************************************************************************************************/
double Avenk_TriNK(const int N, TRILIST *netTri)
{
	double avenk = 0;
	for (int i = 0; i < N; i++)
	{
		avenk += netTri[i].degree;
	}
	avenk /= N;
	return avenk;
}
/********************************************************************************************************************************
** Function name:       Pk_NK
** Descriptions:        calculate pk_nk(degree distribution)
** input parameters:    N,net_nk
** output parameters:   pk_nk
** Returned value:      pk
********************************************************************************************************************************/
double *Pk_NK(const int N, LIST *net_nk)
{
	int nkmax;
	double *pk;
	int temp;

	nkmax = Nkmax_NK(N, net_nk);
	pk = (double *)malloc((nkmax + 1) * sizeof(double));
	for (int i = 0; i < nkmax + 1; i++)
	{
		pk[i] = 0;
	}
	for (int i = 0; i < N; i++)
	{
		temp = net_nk[i].degree;
		pk[temp]++;
	}
	for (int i = 0; i < nkmax + 1; i++)
	{
		pk[i] /= double(N);
	}
	return pk;
}
/********************************************************************************************************************************
** Function name:       Cupk_NK
** Descriptions:        calculate cumulation pk(degree distribution)
** input parameters:    N,net_nk
** output parameters:   cupk(cumulation pk)
** Returned value:      cupk
********************************************************************************************************************************/
double *Cupk_NK(const int N, LIST *net_nk)
{
	int nkmax;
	double *cupk;
	int temp;

	nkmax = Nkmax_NK(N, net_nk);
	cupk = (double *)malloc((nkmax + 1) * sizeof(double));
	for (int i = 0; i < nkmax + 1; i++)
	{
		cupk[i] = 0;
	}
	for (int i = 0; i < N; i++)
	{
		temp = net_nk[i].degree;
		cupk[temp]++;
	}
	cupk[nkmax] /= double(N);
	for (int i = nkmax - 1; i >= 0; i--)
	{
		cupk[i] = cupk[i] / double(N) + cupk[i + 1];
	}
	//printf("cupk[0]=%f\n", cupk[0]);
	return cupk;
}
/********************************************************************************************************************************
** Function name:       S_Avepath_NK
** Descriptions:        S_Avepath_NK
** input parameters:    s,N,net_nk
** output parameters:   s_avepath
** Returned value:      s_avepath
********************************************************************************************************************************/
double S_Avepath_NK(const int s, const int N, LIST *net_nk)
{
	int *u_color;
	int *u_d;
	int *u_pai;
	int *q;  //queue
	int V;
	int tail = 0;
	int head = 0;
	double s_avepath = 0;
	NODE *work;

	u_color = (int *)malloc(N * sizeof(int));
	u_d = (int *)malloc(N * sizeof(int));
	u_pai = (int *)malloc(N * sizeof(int));
	q = (int *)malloc((N + 1) * sizeof(int));

	for (int i = 0; i < N; i++)  //initialization
	{
		u_color[i] = 0;  //white:0,gray:1,black:2
		u_d[i] = -9999;
		u_pai[i] = -9999;
		q[i] = -9999;
	}
	q[N] = -9999;

	u_color[s] = 1;  //gray
	u_d[s] = 0;
	u_pai[s] = -9999;

	q[head] = s;
	tail++;
	while (!(head == tail))
	{
		work = net_nk[q[head]].p_head;
		for (int i = 1; i <= net_nk[q[head]].degree; i++)
		{
			work = work->next;
			V = work->desNode;
			//V = net_nk[q[head]][i];
			if (u_color[V] == 0)
			{
				u_color[V] = 1;
				u_d[V] = u_d[q[head]] + 1;
				u_pai[V] = q[head];
				q[tail++] = V;
			}
		}
		u_color[q[head++]] = 2;
	}
	for (int i = 0; i < N; i++)
	{
		s_avepath += double(u_d[i]);
	}
	s_avepath /= double(N - 1);

	free(u_color);
	free(u_d);
	free(u_pai);
	free(q);

	return s_avepath;
}
/********************************************************************************************************************************
** Function name:       S_Hm_Avepath_NK
** Descriptions:        harmonic mean
** input parameters:    s,N,net_nk
** output parameters:   s_avepath
** Returned value:      s_avepath
********************************************************************************************************************************/
double S_Hm_Avepath_NK(const int s, const int N, LIST *net_nk)
{
	int *u_color;
	int *u_d;
	int *u_pai;
	int *q;  //queue
	int V;
	int tail = 0;
	int head = 0;
	double s_avepath = 0;
	NODE *work;

	u_color = (int *)malloc(N * sizeof(int));
	u_d = (int *)malloc(N * sizeof(int));
	u_pai = (int *)malloc(N * sizeof(int));
	q = (int *)malloc((N + 1) * sizeof(int));

	for (int i = 0; i < N; i++)  //initialization
	{
		u_color[i] = 0;  //white:0,gray:1,black:2
		u_d[i] = -9999;
		u_pai[i] = -9999;
		q[i] = -9999;
	}
	q[N] = -9999;

	u_color[s] = 1;  //gray
	u_d[s] = 0;
	u_pai[s] = -9999;

	q[head] = s;
	tail++;
	while (!(head == tail))
	{
		work = net_nk[q[head]].p_head;
		for (int i = 1; i <= net_nk[q[head]].degree; i++)
		{
			work = work->next;
			V = work->desNode;
			//V = net_nk[q[head]][i];
			if (u_color[V] == 0)
			{
				u_color[V] = 1;
				u_d[V] = u_d[q[head]] + 1;
				u_pai[V] = q[head];
				q[tail++] = V;
			}
		}
		u_color[q[head++]] = 2;
	}
	for (int i = 0; i < N; i++)
	{
		if ((i != s) && (u_d[i] != -9999))
		{
			s_avepath += 1.0 / double(u_d[i]);
		}

	}
	s_avepath /= double(N - 1);

	free(u_color);
	free(u_d);
	free(u_pai);
	free(q);

	return s_avepath;
}
/********************************************************************************************************************************
** Function name:       Is_Connected_NK
** Descriptions:        Is_Connected_NK
** input parameters:    N,net_nk
** output parameters:   connected: 1 or unconnected: 0
** Returned value:      1 or 0
********************************************************************************************************************************/
int Is_Connected_NK(const int N, LIST *net_nk)
{
	int *u_color;
	int *q;  //queue
	int V;
	int tail = 0;
	int head = 0;
	NODE *work;

	u_color = (int *)malloc(N * sizeof(int));
	q = (int *)malloc((N + 1) * sizeof(int));

	for (int i = 0; i < N; i++)  //initialization
	{
		u_color[i] = 0;  //white:0,gray:1,black:2
		q[i] = -9999;
	}
	q[N] = -9999;
	u_color[0] = 1;  //gray,initial node=0

	q[head] = 0;
	tail++;
	while (!(head == tail))
	{
		work = net_nk[q[head]].p_head;
		for (int i = 1; i <= net_nk[q[head]].degree; i++)
		{
			work = work->next;
			V = work->desNode;
			//V = net_nk[q[head]][i];
			if (u_color[V] == 0)
			{
				u_color[V] = 1;
				q[tail++] = V;
			}
		}
		u_color[q[head++]] = 2;
	}
	free(u_color);
	free(q);
	if (tail == N)
	{
		return 1;  //connected
	}
	else
	{
		return 0;  //unconnected
	}
}
/********************************************************************************************************************************
** Function name:       Avepath_NK
** Descriptions:        Avepath_NK
** input parameters:    N,net_nk
** output parameters:   avepath
** Returned value:      avepath
********************************************************************************************************************************/
double Avepath_NK(const int N, LIST *net_nk)  //need "fun_s_avepath_nk"
{
	double sum = 0;
	for (int i = 0; i < N; i++)
	{
		sum += S_Avepath_NK(i, N, net_nk);
	}
	return sum / double(N);
}
/********************************************************************************************************************************
** Function name:       Hm_Avepath_NK
** Descriptions:        Hm_Avepath_NK
** input parameters:    N,net_nk
** output parameters:   avehmpath
** Returned value:      avehmpath
********************************************************************************************************************************/
double Hm_Avepath_NK(const int N, LIST *net_nk)
{
	double sum = 0;
	for (int i = 0; i < N; i++)
	{
		sum += S_Hm_Avepath_NK(i, N, net_nk);
	}
	return sum / double(N);
}





/********************************************************************************************************************************
** Function name:       Nkmax_NN
** Descriptions:        Nkmax_NN
** input parameters:    N,*nk
** output parameters:   nkmax
** Returned value:      nkmax
********************************************************************************************************************************/
int Nkmax_NN(const int N, int *nk)   //find nk max
{
	int nkmax = 0;
	for (int i = 0; i < N; i++)
	{
		nkmax = (nk[i] > nkmax) ? nk[i] : nkmax;
	}
	return nkmax;
}
/********************************************************************************************************************************
** Function name:       Nk_NN
** Descriptions:        Nk_NN
** input parameters:    N,net_nn
** output parameters:   nk
** Returned value:      nk
********************************************************************************************************************************/
int *Nk_NN(const int N, int **net_nn)  //caculate node degree for nn
{
	int *nk;

	nk = (int *)malloc(N * sizeof(int));

	for (int i = 0; i < N; i++)
	{
		nk[i] = 0;
		for (int j = 0; j < N; j++)
		{
			nk[i] += net_nn[i][j];
		}
	}
	return nk;
}
/********************************************************************************************************************************
** Function name:       Avenk_NN
** Descriptions:        Avenk_NN
** input parameters:    N,nk_nn
** output parameters:   avenk
** Returned value:      avenk
********************************************************************************************************************************/
double Avenk_NN(const int N, int *nk_nn)
{
	double avenk = 0;
	for (int i = 0; i < N; i++)
	{
		avenk += nk_nn[i];
	}
	avenk /= N;
	return avenk;

}
/********************************************************************************************************************************
** Function name:       Pk_NN
** Descriptions:        Pk_NN
** input parameters:    N,*nk
** output parameters:   pk
** Returned value:      pk
********************************************************************************************************************************/
double *Pk_NN(const int N, int *nk)  //caculate pk(degree distribution)
{
	double *pk;
	int nkmax;

	nkmax = Nkmax_NN(N, nk);
	pk = (double *)malloc((nkmax + 1) * sizeof(double));
	for (int i = 0; i < nkmax + 1; i++)
	{
		pk[i] = 0;
	}
	for (int i = 0; i < N; i++)
	{
		pk[nk[i]]++;
	}
	for (int i = 0; i < nkmax + 1; i++)
	{
		pk[i] /= double(N);
	}
	return pk;
}
/********************************************************************************************************************************
** Function name:       Cc_NN
** Descriptions:        calculate cluster coefficient of net_nn
** input parameters:    N,net_nn
** output parameters:   avec
** Returned value:      avec
********************************************************************************************************************************/
double Cc_NN(const int N, int **net_nn)
{
	int *nebor;
	double *c;
	int m;
	double avec = 0;

	nebor = (int *)malloc(N * sizeof(int));  //neighbor
	c = (double *)malloc(N * sizeof(double));

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			nebor[j] = -9999;
		}
		int k = -1;
		for (int j = 0; j < N; j++)
		{
			if (net_nn[i][j] == 1)
			{
				k++;
				nebor[k] = j;
			}
		}
		if ((k + 1) <= 1)
		{
			c[i] = 0;
		}
		else
		{
			m = 0;
			for (int I = 0; I < k; I++)
			{
				for (int II = I + 1; II <= k; II++)
				{
					if (net_nn[nebor[I]][nebor[II]] == 1)
					{
						m++;
					}
				}
			}
			c[i] = (double)(2 * m) / (double)(k*(k + 1));
		}
		avec += c[i];
	}
	avec /= N;  //is N or N-1?

	free(nebor);
	free(c);
	return avec;
}
/********************************************************************************************************************************
** Function name:       Cck_NN
** Descriptions:        calculate cluster coefficient of net_nn
** input parameters:    nkmax,N,*nk,*pk,net_nn
** output parameters:   cck
** Returned value:      cck
********************************************************************************************************************************/
double *Cck_NN(const int nkmax, const int N, int *nk, double *pk, int **net_nn)  //cluster coefficient  //not checked
{
	int *nebor;
	double *c;
	double *cck;  //cc(k) distribution
	int m;

	nebor = (int *)malloc(N * sizeof(int));  //neighbor
	c = (double *)malloc(N * sizeof(double));
	cck = (double *)malloc((nkmax + 1) * sizeof(double));


	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			nebor[j] = -9999;
		}
		int k = -1;
		for (int j = 0; j < N; j++)
		{
			if (net_nn[i][j] == 1)
			{
				k++;
				nebor[k] = j;
			}
		}
		if ((k + 1) <= 1)
		{
			c[i] = 0;
		}
		else
		{
			m = 0;
			for (int I = 0; I < k; I++)
			{
				for (int II = I + 1; II <= k; II++)
				{
					if (net_nn[nebor[I]][nebor[II]] == 1)
					{
						m++;
					}
				}
			}
			c[i] = (double)(2 * m) / (double)(k*(k + 1));
		}
	}

	for (int i = 0; i < nkmax + 1; i++)  //initialization
	{
		cck[i] = 0;
	}
	for (int i = 0; i < N; i++)
	{
		cck[nk[i]] += c[i];

	}
	for (int i = 0; i < nkmax + 1; i++)
	{
		if (pk[i] != 0)
		{
			cck[i] /= (pk[i] * N);
		}
	}

	free(nebor);
	free(c);
	return cck;
}
/********************************************************************************************************************************
** Function name:       S_Avepath_NN
** Descriptions:        S_Avepath_NN
** input parameters:    s,N,net_nn
** output parameters:   s_avepath
** Returned value:      s_avepath
********************************************************************************************************************************/
double S_Avepath_NN(const int s, const int N, int **net_nn)
{
	int *u_color;
	int *u_d;
	int *u_pai;
	int *q;  //queue
	double s_avepath = 0;
	int tail = 0;
	int head = 0;

	u_color = (int *)malloc(N * sizeof(int));
	u_d = (int *)malloc(N * sizeof(int));
	u_pai = (int *)malloc(N * sizeof(int));
	q = (int *)malloc((N + 1) * sizeof(int));

	for (int i = 0; i < N; i++)  //initialization
	{
		u_color[i] = 0;  //white:0,gray:1,black:2
		u_d[i] = -9999;
		u_pai[i] = -9999;
		q[i] = -9999;
	}
	q[N] = -9999;

	u_color[s] = 1;  //gray
	u_d[s] = 0;
	u_pai[s] = -9999;

	q[head] = s;
	tail++;
	while (!(head == tail))
	{
		for (int i = 0; i < N; i++)
		{
			if ((net_nn[q[head]][i] == 1) && (u_color[i] == 0))
			{
				u_color[i] = 1;
				u_d[i] = u_d[q[head]] + 1;
				u_pai[i] = q[head];
				q[tail++] = i;
			}
		}
		u_color[q[head++]] = 2;
	}
	for (int i = 0; i < N; i++)
	{
		s_avepath += double(u_d[i]);
	}
	s_avepath /= double(N - 1);

	free(u_color);
	free(u_d);
	free(u_pai);
	free(q);

	return s_avepath;
}
/********************************************************************************************************************************
** Function name:       Is_Connected_NN
** Descriptions:        Is_Connected_NN
** input parameters:    N,net_nn
** output parameters:   connected: 1 or unconnected: 0
** Returned value:      1 or 0
********************************************************************************************************************************/
int Is_Connected_NN(const int N, int **net_nn)
{
	int *u_color;
	int *q;  //queue
	int tail = 0;
	int head = 0;

	u_color = (int *)malloc(N * sizeof(int));
	q = (int *)malloc((N + 1) * sizeof(int));

	for (int i = 0; i < N; i++)  //initialization
	{
		u_color[i] = 0;  //white:0,gray:1,black:2
		q[i] = -9999;
	}
	q[N] = -9999;
	u_color[0] = 1;  //gray,initial node=0

	q[head] = 0;
	tail++;
	while (!(head == tail))
	{
		for (int i = 0; i < N; i++)
		{
			if ((net_nn[q[head]][i] == 1) && (u_color[i] == 0))
			{
				u_color[i] = 1;
				q[tail++] = i;
			}
		}
		u_color[q[head++]] = 2;
	}
	free(u_color);
	free(q);
	if (tail == N)
	{
		return 1;  //connected
	}
	else
	{
		return 0;  //unconnected
	}
}
/********************************************************************************************************************************
** Function name:       Avepath_NN
** Descriptions:        Avepath_NN
** input parameters:    N,net_nn
** output parameters:   avepath
** Returned value:      avepath
********************************************************************************************************************************/
double Avepath_NN(const int N, int **net_nn)  //need "fun_s_avepath_nn"
{
	double sum = 0;
	for (int i = 0; i < N; i++)
	{
		sum += S_Avepath_NN(i, N, net_nn);
	}
	return sum / double(N);
}
///***********************************************End Of File*********************************************************************/

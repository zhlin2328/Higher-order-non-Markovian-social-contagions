/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           spread.cpp
** Created by:          linzhaohua
** Created date:        2019-11-26
** Version:             V1.0
** Descriptions:        spread.cpp
********************************************************************************************************************************/
#include "../headers/config.h"

void Update_Diffusion_Parm(DIFFUSION& Dif, LIST **net, TRILIST **netTri, double parm, vector<vector<double> >& rhoT, vector<int>& num, int& rhoT_Index, double tPre);
void Cal_AveStRho_Value_And_Write(double lamEff, double parm, vector<vector<double> >& rhoT, vector<vector<double> >& aveStRho_Parm);
void Rescale_RhoT(DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, int& rhoT_Index);
void Start_One_Step_Diffusion(LIST **net, TRILIST **netTri, DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, int& rhoT_Index, double tPre);
int Write_Into_RhoT(DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, double tNow);
void Init_Parm_Of_Dif(LIST **net, TRILIST **netTri, DIFFUSION& Dif);
void Init_EvtSort_And_Update_Pos(LIST **net, TRILIST **netTri, DIFFUSION& Dif);
void Push_Back_Elm_Into_EvtSort_And_Update_Pos(DIFFUSION& Dif, int srcNode, int desNode, double tau, NODE *work);
void Push_Back_Elm_Into_EvtSort_And_Update_Pos_2D(DIFFUSION& Dif, int srcNode, int desNode1, int desNode2, double tau, TRINODE *triWork);
void Init_Diffusion_Env(LIST **net, TRILIST **netTri, DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, int& rhoT_Index, double tPre);
void Recover(LIST **net, TRILIST **netTri, DIFFUSION& Dif);
void Infect(LIST **net, TRILIST **netTri, DIFFUSION& Dif, int is2D);
void Inc_Neighbor_I_Num(LIST **net, DIFFUSION& Dif, int srcNode);
void Dec_Neighbor_I_Num(LIST **net, DIFFUSION& Dif, int srcNode);
/********************************************************************************************************************************
** Function name:       Cal_AveSteadyRho_Of_Diff_Parm_Values
** Descriptions:        绝热过程的方式画迟滞回线，这个是NC文献simplex模型的异步更新程序，Gilbson异步更新
** input parameters:    net,aveStRho_Parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveSteadyRho_Of_Diff_Parm_Values(LIST **net, TRILIST **netTri, vector<vector<double> >& aveStRho_Parm)
{
	int rhoT_Index;			//record the last written index in (vector<vector<double> > rhoT), it is the index of rhoT[index]
	int index = 0;
	double tNow;
	double tPre = 0.0;
	double tTemp;
	int pIndex = 0;
	int flag = 0;			//flag = 0 means the program is running the first parameter now.
	DIFFUSION Dif;
	vector<double>::iterator it2;
	Dif.TOTAL_STEPS = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));

	vector<vector<double> > rhoT(2, vector<double>(Dif.TOTAL_STEPS));
	vector<int> num(Dif.TOTAL_STEPS, 0);			//record how many events happens in every rescaled time step

	aveStRho_Parm.resize(3);
#ifdef PARM_UP
	for (auto it = g_Spd.lamEff_Parm[1].begin(); it != g_Spd.lamEff_Parm[1].end(); it++)
#elif defined PARM_DOWN
	for (auto it = g_Spd.lamEff_Parm[1].end() - 1; it != g_Spd.lamEff_Parm[1].begin(); it--)
#else
#error "no defined PARM_UP or PARM_DOWN"
#endif // defined
	{
		if (flag == 0)		//flag = 0 means the program is running the first parameter now.
		{
#ifdef PARM_BETA_INF_1D
			g_Spd.BETA_INF_1D = *it;
#elif defined PARM_BETA_INF_2D
			g_Spd.BETA_INF_2D = *it;
#endif

#ifdef PARM_UP
			it2 = g_Spd.lamEff_Parm[0].begin();
#elif defined PARM_DOWN
			it2 = g_Spd.lamEff_Parm[0].end() - 1;
#endif // defined
			Init_Parm_Of_Dif(net, netTri, Dif);
			Init_Diffusion_Env(net, netTri, Dif, num, rhoT, rhoT_Index, tPre);
		}
		else
		{
			Update_Diffusion_Parm(Dif, net, netTri, (*it), rhoT, num, rhoT_Index, tPre);
		}
		printf("p = %f\n", *it);
		printf("lamEff = %f\n", (*it2));

		tNow = (Dif.evtSort[0].tau <= Dif.evtSort_2D[0].tau) ? Dif.evtSort[0].tau - tPre : Dif.evtSort_2D[0].tau - tPre;
		while (tNow < g_Spd.REAL_TIME_LEN)
		{
			tTemp = tNow;
			Start_One_Step_Diffusion(net, netTri, Dif, num, rhoT, rhoT_Index, tPre);
			tNow = (Dif.evtSort[0].tau <= Dif.evtSort_2D[0].tau) ? Dif.evtSort[0].tau : Dif.evtSort_2D[0].tau;
			if (tNow == ((TAU_INF)))
			{
				break;
			}
			tNow -= tPre;
		}

		pIndex++;
		tPre += tTemp;
		Rescale_RhoT(Dif, num, rhoT, rhoT_Index);
		//Output_rhoT_To_File(g_Spd.lamEff_Parm[0][index], rhoT);
		Cal_AveStRho_Value_And_Write((*it2), *it, rhoT, aveStRho_Parm);
		flag = 1;
#ifdef PARM_UP
		it2++;
#elif defined PARM_DOWN
		it2--;
#endif
	}
}
/********************************************************************************************************************************
** Function name:       Update_Diffusion_Parm
** Descriptions:        Update_Diffusion_Parm
** input parameters:    Dif,net,netTri,parm,rhoT,num,rhoT_Index,tPre
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Update_Diffusion_Parm(DIFFUSION& Dif, LIST **net, TRILIST **netTri, double parm, vector<vector<double> >& rhoT, vector<int>& num, int& rhoT_Index, double tPre)
{
#ifdef PARM_BETA_INF_1D
	g_Spd.BETA_INF_1D = parm;
#elif defined PARM_BETA_INF_2D
	g_Spd.BETA_INF_2D = parm;
#endif

	Free_2DDouble_Vector(2, rhoT);
	rhoT.resize(2, vector<double>(Dif.TOTAL_STEPS, 0.0));

	vector<int>().swap(num);
	num.resize(Dif.TOTAL_STEPS, 0);			//record how many events happens in every rescaled time step
	if (Dif.rhoCnt != 0)
	{
		rhoT[0][0] = 0.0;
		rhoT[1][0] = (1.0*Dif.rhoCnt) / (1.0*g_Net.N);
		num[0]++;
		rhoT_Index = 0;
	}
	else
	{
		Init_Diffusion_Env(net, netTri, Dif, num, rhoT, rhoT_Index, tPre);
	}
	//Output_Diffusion_Parm(p_Dif);
}
/********************************************************************************************************************************
** Function name:       Cal_AveStRho_Value_And_Write
** Descriptions:        Cal_AveStRho_Value_And_Write
** input parameters:    lamEff,parm,rhoT,aveStRho_Parm
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Cal_AveStRho_Value_And_Write(double lamEff, double parm, vector<vector<double> >& rhoT, vector<vector<double> >& aveStRho_Parm)
{
	int tFrom = Double_Into_Int((PCT_OF_STEP_BEGIN_OF_CAL_STEADY_RHO)*g_Spd.REAL_TIME_LEN);
	int stepTotal = Double_Into_Int(g_Spd.REAL_TIME_LEN / double(RESCALE_TIME_STEP));
	double stRho = 0.0;
	int num = 0;

	for (int i = tFrom; i < stepTotal; i++)
	{
		if (rhoT[0][i] != 0.0)
		{
			stRho += rhoT[1][i];
			num++;
		}
	}
	stRho = (num == 0) ? 0.0 : (stRho / double(num));
	aveStRho_Parm[0].push_back(lamEff);
	aveStRho_Parm[1].push_back(parm);
	aveStRho_Parm[2].push_back(stRho);
}
/********************************************************************************************************************************
** Function name:       Rescale_RhoT
** Descriptions:        得出以RESCALE_TIME_STEP为间隔的rhoT
** input parameters:    Dif,num,rhoT,rhoT_Index
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Rescale_RhoT(DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, int& rhoT_Index)
{
	for (int i = 0; i < Dif.TOTAL_STEPS; i++)
	{
		if (num[i] == 0)
		{
			rhoT[0][i] = 0.0;
			rhoT[1][i] = 0.0;
		}
		else
		{
			rhoT[0][i] /= num[i];
			rhoT[1][i] /= num[i];
		}
	}
	if (Dif.evtSort[0].tau == (TAU_INF) && rhoT_Index != Dif.TOTAL_STEPS - 1)   //tau = TAU_INF means: end of diffusion
	{
		for (int i = rhoT_Index + 1; i < Dif.TOTAL_STEPS; i++)
		{
			rhoT[0][i] = rhoT[0][rhoT_Index] + (i - rhoT_Index)*(RESCALE_TIME_STEP);
			rhoT[1][i] = rhoT[1][rhoT_Index];
		}
	}
}
/********************************************************************************************************************************
** Function name:       Start_One_Step_Diffusion
** Descriptions:        Start_One_Step_Diffusion
** input parameters:    net,netTri,Dif,num,rhoT,rhoT_Index,tPre
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Start_One_Step_Diffusion(LIST **net, TRILIST **netTri, DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, int& rhoT_Index, double tPre)
{
	int is2D;
	double tNow;
	double tNext;
	int srcNode = Dif.evtSort[0].srcNode;
	int srcState = Dif.state[srcNode];

	if (Dif.evtSort[0].tau <= Dif.evtSort_2D[0].tau)
	{
		tNow = Dif.evtSort[0].tau;
		is2D = 0;
		if (Dif.evtSort[0].srcNode != Dif.evtSort[0].desNode)
		{
			Infect(net, netTri, Dif, is2D);
		}
		else
		{
			Recover(net, netTri, Dif);
		}
	}
	else
	{
		tNow = Dif.evtSort_2D[0].tau;
		is2D = 1;
		Infect(net, netTri, Dif, is2D);
	}

	tNext = (Dif.evtSort[0].tau <= Dif.evtSort_2D[0].tau) ? Dif.evtSort[0].tau : Dif.evtSort_2D[0].tau;
	if (tNow != tNext)		//若同一时刻，则不记录
	{
		rhoT_Index = Write_Into_RhoT(Dif, num, rhoT, tNow - tPre);
	}
}
/********************************************************************************************************************************
** Function name:       Write_Into_RhoT
** Descriptions:        Write_Into_RhoT
** input parameters:    Dif,num,rhoT,tNow
** output parameters:   index
** Returned value:      rhoT's index
********************************************************************************************************************************/
int Write_Into_RhoT(DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, double tNow)
{
	int index = int(floor(tNow / (RESCALE_TIME_STEP)));

	rhoT[0][index] += tNow;
	rhoT[1][index] += double(Dif.rhoCnt) / double(g_Net.N);
	num[index]++;

	return index;
}
/********************************************************************************************************************************
** Function name:       Init_Parm_Of_Dif
** Descriptions:        Init_Parm_Of_Dif
** input parameters:    net,netTri,Dif
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_Parm_Of_Dif(LIST **net, TRILIST **netTri, DIFFUSION& Dif)
{
	Dif.rhoCnt = 0;
	Dif.state.resize(g_Net.N, 0);
	Dif.neighborI_Num.resize(g_Net.N, 0);				
	Init_EvtSort_And_Update_Pos(net, netTri, Dif);
}
/********************************************************************************************************************************
** Function name:       Init_EvtSort_And_Update_Pos
** Descriptions:        Init_EvtSort_And_Update_Pos,总共N+2M+3T个事件(M是网络中连边数，T是网络中三模体数)
** input parameters:    net,netTri,Dif
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_EvtSort_And_Update_Pos(LIST **net, TRILIST **netTri, DIFFUSION& Dif)
{
	NODE *work;
	TRINODE *triWork;

	Dif.evtSort_Size = 0;																	//1D simplex event
	for (int i = 0; i < g_Net.N; i++)
	{
		work = (*net)[i].p_head;
		Push_Back_Elm_Into_EvtSort_And_Update_Pos(Dif, i, i, TAU_INF, work);

		while (NULL != work->next)
		{
			work = work->next;
			Push_Back_Elm_Into_EvtSort_And_Update_Pos(Dif, i, work->desNode, TAU_INF, work);
		}
	}

	Dif.evtSort_2D_Size = 0;																//2D simplex event
	for (int i = 0; i < g_Net.N; i++)
	{
		triWork = (*netTri)[i].p_head;
		while (NULL != triWork->next)
		{
			triWork = triWork->next;
			Push_Back_Elm_Into_EvtSort_And_Update_Pos_2D(Dif, i, triWork->desNode1, triWork->desNode2, TAU_INF, triWork);
		}
	}
}
/********************************************************************************************************************************
** Function name:       Push_Back_Elm_Into_EvtSort_And_Update_Pos
** Descriptions:        Push_Back_Elm_Into_EvtSort_And_Update_Pos
** input parameters:    Dif,srcNode,desNode,tau,work
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Push_Back_Elm_Into_EvtSort_And_Update_Pos(DIFFUSION& Dif, int srcNode, int desNode, double tau, NODE *work)
{
	EVT oneEvt;

	oneEvt.srcNode = srcNode;
	oneEvt.desNode = desNode;
	oneEvt.tau = tau;
	oneEvt.posIn_Net = work;					//update pos in g_netAdjList

	Dif.evtSort.push_back(oneEvt);
	work->posIn_EvtSort = Dif.evtSort_Size;
	Dif.evtSort_Size++;
}
/********************************************************************************************************************************
** Function name:       Push_Back_Elm_Into_EvtSort_And_Update_Pos_2D
** Descriptions:        Push_Back_Elm_Into_EvtSort_And_Update_Pos_2D
** input parameters:    Dif,srcNode,desNode1,desNode2,tau,triWork
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Push_Back_Elm_Into_EvtSort_And_Update_Pos_2D(DIFFUSION& Dif, int srcNode, int desNode1, int desNode2, double tau, TRINODE *triWork)
{
	EVT2D oneEvt;

	oneEvt.srcNode = srcNode;
	oneEvt.desNode1 = desNode1;
	oneEvt.desNode2 = desNode2;
	oneEvt.tau = tau;
	oneEvt.posIn_Tri = triWork;					//update pos in g_netTriList

	Dif.evtSort_2D.push_back(oneEvt);
	triWork->posIn_EvtSort_2D = Dif.evtSort_2D_Size;
	Dif.evtSort_2D_Size++;
}   //ok
/********************************************************************************************************************************
** Function name:       Init_Diffusion_Env
** Descriptions:        Init_Diffusion_Env
** input parameters:    net,netTri,Dif,num,rhoT,rhoT_Index
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_Diffusion_Env(LIST **net, TRILIST **netTri, DIFFUSION& Dif, vector<int>& num, vector<vector<double> >& rhoT, int& rhoT_Index, double tPre)
{
	int node;
	int pos;
	int state1;
	int state2;
	NODE *work;
	TRINODE *triWork;
	TRINODE *triTemp;

	Dif.rhoCnt = g_Spd.INTIAL_RHO_NUM;
	rhoT[0][0] += 0.0;
	rhoT[1][0] += (1.0 * Dif.rhoCnt) / g_Net.N;
	num[0]++;
	rhoT_Index = 0;
	vector<int>().swap(Dif.seeds);
	for (int i = 0; i < g_Spd.INTIAL_RHO_NUM; i++)					//nodes for i states at first
	{
		do
		{
			node = g_Rd.distD(g_Rd.mt);
		} while (Dif.state[node] == 1);
		Dif.state[node] = 1;
		Dif.seeds.push_back(node);
		Inc_Neighbor_I_Num(net, Dif, node);
	}

	for (auto it = Dif.seeds.begin(); it != Dif.seeds.end(); it++)
	{
		pos = (*net)[*it].p_head->posIn_EvtSort;						//1D: allocate tau
		Dif.evtSort[pos].tau = tPre + g_Spd.BETA_REC*pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_REC);

		work = (*net)[*it].p_head->next;
		while (NULL != work)
		{
			pos = work->counterpart->posIn_EvtSort;
			Dif.evtSort[pos].tau = tPre + g_Spd.BETA_INF_1D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_1D);

			work = work->next;
		}

		triWork = (*netTri)[*it].p_head->next;							  //2D: allocate tau
		while (NULL != triWork)
		{
			state1 = Dif.state[triWork->desNode1];
			state2 = Dif.state[triWork->desNode2];

			if (state1 + state2 != 0)
			{
				if (state1 == 1 && state2 == 1)							//III
				{
					triTemp = Find_Tri_TriNK(netTri, (*it), triWork->desNode2, triWork->desNode1);
					pos = triTemp->posIn_EvtSort_2D;
					Dif.evtSort_2D[pos].tau = tPre + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);

					triTemp = Find_Tri_TriNK(netTri, (*it), triWork->desNode1, triWork->desNode2);
					pos = triTemp->posIn_EvtSort_2D;
					Dif.evtSort_2D[pos].tau = tPre + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
				}
				else if (state1 == 0)
				{
					triTemp = Find_Tri_TriNK(netTri, (*it), triWork->desNode2, triWork->desNode1);
					pos = triTemp->posIn_EvtSort_2D;
					Dif.evtSort_2D[pos].tau = tPre + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
				}
				else if (state2 == 0)
				{
					triTemp = Find_Tri_TriNK(netTri, (*it), triWork->desNode1, triWork->desNode2);
					pos = triTemp->posIn_EvtSort_2D;
					Dif.evtSort_2D[pos].tau = tPre + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
				}
			}
			triWork = triWork->next;
		}
	}
	Build_Min_Heap(Dif.evtSort, Dif.evtSort_Size);
	Build_Min_Heap_TRI(Dif.evtSort_2D, Dif.evtSort_2D_Size);
}
/********************************************************************************************************************************
** Function name:       Recover
** Descriptions:        Recover
** input parameters:    net,netTri,Dif
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Recover(LIST **net, TRILIST **netTri, DIFFUSION& Dif)
{
	int node;
	int state1;
	int state2;
	int stateSum;
	NODE *work;
	TRINODE *triWork;
	TRINODE *triTemp;
	int pos;
	double newTau;

	Dif.rhoCnt--;
	node = Dif.evtSort[0].srcNode;
	Dif.state[node] = 0;
	Dec_Neighbor_I_Num(net, Dif, node);

	Min_Heap_Change_Key(Dif.evtSort, 0, (TAU_INF), Dif.evtSort_Size);

	work = (*net)[node].p_head->next;												//1D
	while (NULL != work)
	{
		pos = work->counterpart->posIn_EvtSort;
		newTau = TAU_INF;
		Min_Heap_Change_Key(Dif.evtSort, pos, newTau, Dif.evtSort_Size);

		work = work->next;
	}

	triWork = (*netTri)[node].p_head->next;							  //2D: allocate tau
	while (NULL != triWork)
	{
		state1 = Dif.state[triWork->desNode1];
		state2 = Dif.state[triWork->desNode2];
		stateSum = state1 + state2;

		if (stateSum != 0)											//that is not SS
		{
			if (state1 == 1 && state2 == 1)							//II
			{
				triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode2, triWork->desNode1);
				pos = triTemp->posIn_EvtSort_2D;
				newTau = TAU_INF;
				Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);

				triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode1, triWork->desNode2);
				pos = triTemp->posIn_EvtSort_2D;
				newTau = TAU_INF;
				Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);
			}
			else if (state1 == 0)
			{
				triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode2, triWork->desNode1);
				pos = triTemp->posIn_EvtSort_2D;
				newTau = TAU_INF;
				Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);
			}
			else if (state2 == 0)
			{
				triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode1, triWork->desNode2);
				pos = triTemp->posIn_EvtSort_2D;
				newTau = TAU_INF;
				Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);
			}
		}
		triWork = triWork->next;
	}
}
/********************************************************************************************************************************
** Function name:       Infect_1D
** Descriptions:        Infect_1D
** input parameters:    net,netTri,Dif,is2D
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Infect(LIST **net, TRILIST **netTri, DIFFUSION& Dif, int is2D)
{
	int node;
	int state1;
	int state2;
	int stateSum;
	NODE *work;
	TRINODE *triWork;
	TRINODE *triTemp;
	int pos;
	double newTau;
	double tNow;

	if (is2D == 1)
	{											//2D
		tNow = Dif.evtSort_2D[0].tau;
		node = Dif.evtSort_2D[0].srcNode;
	}
	else
	{											//1D
		tNow = Dif.evtSort[0].tau;
		node = Dif.evtSort[0].srcNode;
	}

	if (Dif.state[node] == 0)
	{
		Dif.rhoCnt++;
		Dif.state[node] = 1;
		Inc_Neighbor_I_Num(net, Dif, node);

		if (is2D == 1)
		{											//2D
			newTau = tNow + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
			Min_Heap_Change_Key_TRI(Dif.evtSort_2D, 0, newTau, Dif.evtSort_2D_Size);
		}
		else
		{											//1D
			newTau = tNow + g_Spd.BETA_INF_1D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_1D);
			Min_Heap_Change_Key(Dif.evtSort, 0, newTau, Dif.evtSort_Size);
		}

		newTau = tNow + g_Spd.BETA_REC*pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_REC);
		pos = (*net)[node].p_head->posIn_EvtSort;
		Min_Heap_Change_Key(Dif.evtSort, pos, newTau, Dif.evtSort_Size);

		work = (*net)[node].p_head->next;
		while (NULL != work)
		{
			pos = work->counterpart->posIn_EvtSort;
			newTau = tNow + g_Spd.BETA_INF_1D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_1D);
			Min_Heap_Change_Key(Dif.evtSort, pos, newTau, Dif.evtSort_Size);

			work = work->next;
		}

		triWork = (*netTri)[node].p_head->next;							  //2D: allocate tau
		while (NULL != triWork)
		{
			state1 = Dif.state[triWork->desNode1];
			state2 = Dif.state[triWork->desNode2];
			stateSum = state1 + state2;
			if (stateSum != 0)											//that is not SS
			{
				if (state1 == 1 && state2 == 1)							//II
				{
					triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode2, triWork->desNode1);
					pos = triTemp->posIn_EvtSort_2D;
					newTau = tNow + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
					Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);

					triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode1, triWork->desNode2);
					pos = triTemp->posIn_EvtSort_2D;
					newTau = tNow + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
					Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);
				}
				else if (state1 == 0)
				{
					triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode2, triWork->desNode1);
					pos = triTemp->posIn_EvtSort_2D;
					newTau = tNow + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
					Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);
				}
				else if (state2 == 0)
				{
					triTemp = Find_Tri_TriNK(netTri, node, triWork->desNode1, triWork->desNode2);
					pos = triTemp->posIn_EvtSort_2D;
					newTau = tNow + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
					Min_Heap_Change_Key_TRI(Dif.evtSort_2D, pos, newTau, Dif.evtSort_2D_Size);
				}
			}
			triWork = triWork->next;
		}
	}
	else if (Dif.state[node] == 1)
	{
		if (is2D == 1)
		{											//2D
			newTau = tNow + g_Spd.BETA_INF_2D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_2D);
			Min_Heap_Change_Key_TRI(Dif.evtSort_2D, 0, newTau, Dif.evtSort_2D_Size);
		}
		else
		{											//1D
			newTau = tNow + g_Spd.BETA_INF_1D *pow(-1.0*log(g_Rd.distF(g_Rd.mt)), 1.0 / g_Spd.ALPHA_INF_1D);
			Min_Heap_Change_Key(Dif.evtSort, 0, newTau, Dif.evtSort_Size);
		}
	}
}
/********************************************************************************************************************************
** Function name:       Inc_Neighbor_I_Num
** Descriptions:        将node节点的邻居节点的neighbor_i_num都加1
** input parameters:    net,Dif,srcNode
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Inc_Neighbor_I_Num(LIST **net, DIFFUSION& Dif, int srcNode)
{
	NODE *work;
	int desNode;

	work = (*net)[srcNode].p_head->next;
	while (NULL != work)
	{
		desNode = work->desNode;
		Dif.neighborI_Num[desNode]++;

		work = work->next;
	}
}
/********************************************************************************************************************************
** Function name:       Dec_Neighbor_I_Num
** Descriptions:        将node节点的邻居节点的neighbor_i_num都减1
** input parameters:    net,Dif,srcNode
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Dec_Neighbor_I_Num(LIST **net, DIFFUSION& Dif, int srcNode)
{
	NODE *work;
	int desNode;

	work = (*net)[srcNode].p_head->next;
	while (NULL != work)
	{
		desNode = work->desNode;
		Dif.neighborI_Num[desNode]--;

		work = work->next;
	}
}
/***********************************************End Of File*********************************************************************/
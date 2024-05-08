#pragma once
/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           spread.h
** Created by:          linzhaohua
** Created date:        2019-11-26
** Version:             V1.0
** Descriptions:        spread.h
********************************************************************************************************************************/
#ifndef __SPREAD_H
#define __SPREAD_H

typedef struct Event
{
	int srcNode;
	int desNode;
	double tau;
	struct Node *posIn_Net;
}EVT;
typedef struct Event_2D
{
	int srcNode;
	int desNode1;
	int desNode2;
	double tau;
	struct TriNode *posIn_Tri;
}EVT2D;
typedef struct Diffusion
{
	int TOTAL_STEPS;
	int rhoCnt;
	vector<int> seeds;
	vector<int> state;
	vector<int> neighborI_Num;
	vector<EVT> evtSort;
	vector<EVT2D> evtSort_2D;
	int evtSort_Size;
	int evtSort_2D_Size;
}DIFFUSION;

void Cal_AveSteadyRho_Of_Diff_Parm_Values(LIST **net, TRILIST **netTri, vector<vector<double> >& aveStRho_Parm);
#endif //__SPREAD_H
/***********************************************End Of File*********************************************************************/
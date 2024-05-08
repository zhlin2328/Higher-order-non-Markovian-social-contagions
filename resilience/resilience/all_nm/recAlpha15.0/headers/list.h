#pragma once
/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           list.h
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        这里的LIST链表为双向链表结构，并且有counterpart, e.g. 可以 ij 指向 ji 的位置
********************************************************************************************************/
#ifndef __LIST_H
#define __LIST_H

//typedef int DATATYPE;
typedef struct Node {
	int desNode;				//数据域
	struct Node *before;		//指针域
	struct Node *next;			
	struct Node *counterpart;	

	int posIn_EvtSort;			//传播中会用到的变量,1D simplex: disease including: S->I (1D), I->S (1D)
}NODE;
typedef struct List				//双向链表
{
	int degree;
	NODE *p_head;				//表头指针
	NODE *p_tail;				//表尾指针
} LIST;

void Init_NK(LIST **net);
void Pop_Back_NK(LIST **net, int rowIndex);
void Push_Back_NK(LIST **net, int data, int rowIndex);
void Push_Back_And_Update_Counterpart_NK(LIST **net, int data, int rowIndex);
void Add_Double_Edge_NK(LIST **net, int edgeEnd, int edgeBegin);
void Delete_Double_Edge_NK(LIST **net, int edgeEnd, int edgeBegin);
void Delete_Edge_And_Update_Counterpart_NK(LIST **net, int edgeEnd, int edgeBegin);
void Set_Counterpart_NK(LIST **net);
NODE *Find_Edge_NK(LIST **net, int desNode, int srcNode);
int Is_In_List_NK(LIST **net, int desNode, int srcNode);
int Is_Degree_Equal_Length_NK(LIST **net, int node);
void Free2D_NK(LIST **net, int num);
void Output_NK(LIST **net, int num);
void Output_Net_Content_NK(LIST **net, int step);
void Init_DesNet_And_Copy_SrcNet_NK(LIST **srcNet, LIST **desNet);
#endif //__LIST_H
/*****************************************************End Of File***************************************/


#pragma once
/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           tri_list.h
** Created by:          linzhaohua
** Created date:        2019-11-26
** Version:             V1.0
** Descriptions:        这里的LIST链表为双向链表结构
********************************************************************************************************/
#ifndef __TRI_LIST_H
#define __TRI_LIST_H

#define NULL_VALUE_TRINK	-1

typedef struct TriNode {
	int desNode1;				//数据域
	int desNode2;				//数据域
	struct TriNode *before;		//指针域
	struct TriNode *next;

	int posIn_EvtSort_2D;			//传播中会用到的变量
}TRINODE;
typedef struct TriList			//单向链表
{
	int degree;
	TRINODE *p_head;			//表头指针
	TRINODE *p_tail;			//表尾指针
}TRILIST;

void Init_TriNK(TRILIST **netTri);
void Pop_Back_TriNK(TRILIST **netTri, int rowIndex);
void Push_Back_TriNK(TRILIST **netTri, int desNode1, int desNode2, int rowIndex);
void Add_Triple_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode);
void Delete_Triple_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode);
void Delete_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode);
TRINODE *Find_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode);
int Is_In_List_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode);
int Is_Degree_Equal_Length_TriNK(TRILIST **netTri, int srcNode);
void Free2D_TriNK(TRILIST **netTri, int num);
void Output_TriNK(TRILIST **netTri, char *name, int netSize);
void Output_Net_Content_TriNK(TRILIST **netTri, int step);
void Init_DesNet_And_Copy_SrcNet_TriNK(TRILIST **srcNet, TRILIST **desNet);

#endif //__TRI_LIST_H
/*****************************************************End Of File***************************************/


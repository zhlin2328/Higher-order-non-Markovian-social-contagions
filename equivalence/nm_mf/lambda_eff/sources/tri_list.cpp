/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           tri_list.cpp
** Created by:          linzhaohua
** Created date:        2019-11-26
** Version:             V1.0
** Descriptions:        tri_list.cpp,旧的g_netTriList改为(*netTri),&g_netTriList改为netTri
********************************************************************************************************/
#include "../headers/config.h"

static void Delete_Pointer_Of_Tri_TriNK(TRILIST **netTri, TRINODE *p_node, int edgeBegin);
/********************************************************************************************************************************
** Function name:       Init_TriNK
** Descriptions:        Init_TriNK
** input parameters:    netTri
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_TriNK(TRILIST **netTri)
{
	*netTri = (TRILIST *)malloc(g_Net.N * sizeof(TRILIST));
	for (int i = 0; i < g_Net.N; i++)
	{
		TRINODE *p_node = (TRINODE *)malloc(sizeof(TRINODE));

		p_node->desNode1 = NULL_VALUE_TRINK;
		p_node->desNode2 = NULL_VALUE_TRINK;
		p_node->before = NULL;
		p_node->next = NULL;

		(*netTri)[i].degree = 0;						 //netTri[i].degree用来存网络节点的度，也就是这一行链表的长度,head表示自己，head->next用来存首个节点,tail的地址为最后一个节点
		(*netTri)[i].p_head = p_node;
		(*netTri)[i].p_tail = (*netTri)[i].p_head;
	}
}
/********************************************************************************************************************************
** Function name:       Pop_Back_TriNK
** Descriptions:        Pop_Back_TriNK(只删除单向边)
** input parameters:    netTri,rowIndex
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Pop_Back_TriNK(TRILIST **netTri, int rowIndex)
{
	TRINODE *work = (*netTri)[rowIndex].p_head->next;
	TRINODE *temp = (*netTri)[rowIndex].p_head;

	if (NULL == work)
	{
		printf("Pop_Back: no node in this list\n");
		exit(-1);										//退出程序
	}
	while (NULL != work->next)							
	{
		temp = work;
		work = work->next;
	}
	temp->next = NULL;
	(*netTri)[rowIndex].p_tail = temp;
	free(work);

	work = NULL;										//这句话其实没什么作用，只是安全作用
	(*netTri)[rowIndex].degree -= 1;
}
/********************************************************************************************************************************
** Function name:       Push_Back_TriNK
** Descriptions:        Push_Back_TriNK(只增加单向边)
** input parameters:    netTri,desNode1,desNode2,rowIndex
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Push_Back_TriNK(TRILIST **netTri, int desNode1, int desNode2, int rowIndex)
{
	TRINODE *p_node = (TRINODE *)malloc(sizeof(TRINODE));

	p_node->desNode1 = desNode1;
	p_node->desNode2 = desNode2;
	p_node->before = (*netTri)[rowIndex].p_tail;
	p_node->next = NULL;

	(*netTri)[rowIndex].p_tail->next = p_node;
	(*netTri)[rowIndex].p_tail = p_node;
	(*netTri)[rowIndex].degree += 1;
}
/********************************************************************************************************************************
** Function name:       Add_Triple_Tri_TriNK
** Descriptions:        先判断是否已存在，之后再将模体在三个节点各自的链表中都加入
** input parameters:    netTri,desNode1,desNode2,srcNode
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Add_Triple_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode)
{
	int isMulti_Tri = Is_In_List_TriNK(netTri, desNode1, desNode2, srcNode);

	if (isMulti_Tri == 0)
	{
		Push_Back_TriNK(netTri, desNode1, desNode2, srcNode);
	}

	isMulti_Tri = Is_In_List_TriNK(netTri, desNode2, srcNode, desNode1);
	if (isMulti_Tri == 0)
	{
		Push_Back_TriNK(netTri, desNode2, srcNode, desNode1);
	}

	isMulti_Tri = Is_In_List_TriNK(netTri, desNode1, srcNode, desNode2);
	if (isMulti_Tri == 0)
	{
		Push_Back_TriNK(netTri, desNode1, srcNode, desNode2);
	}
}
/********************************************************************************************************************************
** Function name:       Delete_Triple_Tri_TriNK
** Descriptions:        Delete_Triple_Tri_TriNK，将三模体各自对应的都(全)删去（共删去3个）
** input parameters:    netTri,desNode1,desNode2,srcNode
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Delete_Triple_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode)
{
	Delete_Tri_TriNK(netTri, desNode1, desNode2, srcNode);
	cout << "1" << endl;
	Delete_Tri_TriNK(netTri, desNode2, srcNode, desNode1);
	cout << "2" << endl;
	Delete_Tri_TriNK(netTri, desNode1, srcNode, desNode2);
	cout << "3" << endl;
}
/********************************************************************************************************************************
** Function name:       Delete_Tri_TriNK
** Descriptions:        Delete_Tri_TriNK，只删除单向边
** input parameters:    netTri,desNode1,desNode2,srcNode
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Delete_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode)
{
	int flag = 0;
	TRINODE *curNode = (*netTri)[srcNode].p_head->next;

	while (NULL != curNode)
	{
		if ((curNode->desNode1 == desNode1 && curNode->desNode2 == desNode2) || (curNode->desNode1 == desNode2 && curNode->desNode2 == desNode1))
		{
			flag = 1;
			Delete_Pointer_Of_Tri_TriNK(netTri, curNode, srcNode);
			break;
		}
		curNode = curNode->next;
	}
	if (flag == 0 && NULL == curNode)
	{
		printf("Delete_Edge: no edges in this list or not found\n");
		//exit(-1);
	}
}
/********************************************************************************************************************************
** Function name:       Delete_Pointer_Of_Tri_TriNK
** Descriptions:        删除tri的具体操作（一般只在此cpp中使用），操作指针
** input parameters:    netTri,p_node,srcNode
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Delete_Pointer_Of_Tri_TriNK(TRILIST **netTri, TRINODE *p_node, int srcNode)
{
	if (NULL != p_node)
	{
		p_node->before->next = p_node->next;
		if (NULL != p_node->next)
		{
			p_node->next->before = p_node->before;
		}
		else
		{
			(*netTri)[srcNode].p_tail = p_node->before;
		}
		free(p_node);
		(*netTri)[srcNode].degree -= 1;
	}
}
/********************************************************************************************************************************
** Function name:       Find_Tri_TriNK
** Descriptions:        在srcNode节点内寻找tri: srcNode-desNode1-desNode2,找到则返回对应指针，否则返回NULL
** input parameters:    netTri,desNode1,desNode2,srcNode
** output parameters:   pointer（找到则返回对应指针，否则返回NULL）
** Returned value:      pointer
********************************************************************************************************************************/
TRINODE *Find_Tri_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode)
{
	TRINODE *work;

	work = (*netTri)[srcNode].p_head->next;
	while (NULL != work)
	{
		if ((work->desNode1 == desNode1 && work->desNode2 == desNode2) || (work->desNode1 == desNode2 && work->desNode2 == desNode1))
		{
			return work;
			break;
		}
		work = work->next;
	}
	return NULL;
}
/********************************************************************************************************************************
** Function name:       Is_In_List_TriNK
** Descriptions:        判断desNode1,desNode2是否在srcNode的tri链表里（yes：return 1，no：return 0）
** input parameters:    netTri,desNode1,desNode2,srcNode
** output parameters:   判断值
** Returned value:      1,0
********************************************************************************************************************************/
int Is_In_List_TriNK(TRILIST **netTri, int desNode1, int desNode2, int srcNode)
{
	TRINODE *work;

	work = (*netTri)[srcNode].p_head->next;
	while (NULL != work)
	{
		if ((work->desNode1 == desNode1 && work->desNode2 == desNode2) || (work->desNode1 == desNode2 && work->desNode2 == desNode1))
		{
			return 1;
			break;
		}
		work = work->next;
	}
	return 0;
}
/********************************************************************************************************************************
** Function name:       Is_Degree_Equal_Length_TriNK
** Descriptions:        判断节点node的degree是否计算正确
** input parameters:    netTri,srcNode
** output parameters:   yes：return 1，no：return 0
** Returned value:      1,0
********************************************************************************************************************************/
int Is_Degree_Equal_Length_TriNK(TRILIST **netTri, int srcNode)
{
	int cnt = 0;
	TRINODE *work = (*netTri)[srcNode].p_head->next;

	while (NULL != work)
	{
		cnt++;
		work = work->next;
	}
	if (cnt == (*netTri)[srcNode].degree)
	{
		return 1;
	}
	return 0;
}
/********************************************************************************************************************************
** Function name:       Free2D_TriNK
** Descriptions:        Free2D_TriNK
** input parameters:    netTri,num
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Free2D_TriNK(TRILIST **netTri, int num)
{
	TRINODE *work;

	for (int i = 0; i < num; i++)
	{
		work = (*netTri)[i].p_tail;
		while (work != (*netTri)[i].p_head)
		{
			work->before->next = NULL;
			(*netTri)[i].p_tail = work->before;
			free(work);
			work = (*netTri)[i].p_tail;
		}
		free((*netTri)[i].p_head);
	}
	free((*netTri));
	//netTri->p_head = NULL;
}
/********************************************************************************************************************************
** Function name:       Output_TriNK
** Descriptions:        输出网络节点的度，和对应的邻居节点
** input parameters:    netTri,netSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_TriNK(TRILIST **netTri, char *name, int netSize)
{
	FILE *fp;
	TRINODE *work;

	fp = fopen(name, "w");
	/*if (NULL == netTri->p_head)
	{
		printf("netTri is NULL\n");
		exit(-1);
	}*/
	//cout << endl;
	for (int i = 0; i < netSize; i++)
	{
		fprintf(fp, "%d,", (*netTri)[i].degree);
		//cout << (*netTri)[i].degree << " ";
		work = (*netTri)[i].p_head;
		while (NULL != work->next)
		{
			work = work->next;
			//cout << work->data << " ";
			fprintf(fp, "{%d,%d},", work->desNode1, work->desNode2);			//输出每个节点的三模体（三角形）的其他两个邻居节点
		}
		//cout << endl;
		fprintf(fp, "\n");
	}
	fclose(fp);
}
/********************************************************************************************************************************
** Function name:       Output_Net_Content_TriNK
** Descriptions:        输出netTri所有的成员变量特征，除了posIn_EvtSort这种传播中用到的成员变量
** input parameters:    netTri,step
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Net_Content_TriNK(TRILIST **netTri, int step)
{
	FILE *fp;
	char *name;
	TRINODE *work;

	name = (char *)malloc(300 * sizeof(char));
	sprintf(name, "net_step%d.csv", step);
	fp = fopen(name, "w");
	for (int i = 0; i < g_Net.N; i++)
	{
		work = (*netTri)[i].p_head->next;
		fprintf(fp, "%d(D:%d)\t", i, (*netTri)[i].degree);						//输出节点i和对应的 度
		while (NULL != work)
		{
			fprintf(fp, "{%d,%d},", work->desNode1, work->desNode2);			//输出三模体中剩下两个邻居
			work = work->next;
		}
		fprintf(fp, "\t\t\t\t\t{H:{%d,%d},T:{%d,%d}}\n",
			(*netTri)[i].p_head->desNode1,
			(*netTri)[i].p_head->desNode2,
			(*netTri)[i].p_tail->desNode1,
			(*netTri)[i].p_tail->desNode2);										//H：head，T：tail，输出对应的desnode1,desnode2值（head的desnode = NULL_VALUE_TRINK,即-1）
	}
	fclose(fp);
}
/********************************************************************************************************************************
** Function name:       Init_DesNet_And_Copy_SrcNet_TriNK
** Descriptions:        Init_DesNet_And_Copy_SrcNet_TriNK
** input parameters:    srcNet,desNet
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_DesNet_And_Copy_SrcNet_TriNK(TRILIST **srcNet, TRILIST **desNet)
{
	TRINODE *work;

	Init_TriNK(desNet);
	for (int i = 0; i < g_Net.N; i++)
	{
		work = (*srcNet)[i].p_head->next;
		while (NULL != work)
		{
			Push_Back_TriNK(desNet, work->desNode1, work->desNode2, i);
			work = work->next;
		}
	}
}
/*****************************************************End Of File***************************************/
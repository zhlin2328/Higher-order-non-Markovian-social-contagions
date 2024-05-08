/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           list.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        list.cpp
********************************************************************************************************/
#include "../headers/config.h"

static void Delete_Pointer_Of_Edge_NK(LIST **net, NODE *p_node, int edgeBegin);
/********************************************************************************************************************************
** Function name:       Init_NK
** Descriptions:        Init_NK
** input parameters:    net
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_NK(LIST **net)
{
	*net = (LIST *)malloc(g_Net.N * sizeof(LIST));
	for (int i = 0; i < g_Net.N; i++)
	{
		NODE *p_node = (NODE *)malloc(sizeof(NODE));

		p_node->desNode = i;
		p_node->before = NULL;
		p_node->next = NULL;
		p_node->counterpart = NULL;

		(*net)[i].degree = 0;				//net[i].degree用来存网络节点的度，也就是这一行链表的长度,head表示自己，head->next用来存首个节点,tail的地址为最后一个节点
		(*net)[i].p_head = p_node;
		(*net)[i].p_tail = (*net)[i].p_head;
	}
}
/********************************************************************************************************************************
** Function name:       Pop_Back_NK
** Descriptions:        Pop_Back_NK(只删除单向边),并更新counterpart值
** input parameters:    net,rowIndex
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Pop_Back_NK(LIST **net, int rowIndex)
{
	NODE *work = (*net)[rowIndex].p_head->next;
	NODE *temp = (*net)[rowIndex].p_head;
	NODE *temp2;

	if (NULL == work)
	{
		printf("Pop_Back: no node in this list\n");
		exit(-1);											//退出程序
	}
	while (NULL != work->next)
	{
		temp = work;
		work = work->next;
	}
	temp->next = NULL;
	(*net)[rowIndex].p_tail = temp;
	temp2 = Find_Edge_NK(net, rowIndex, work->desNode);		//找到counterpart，令其=NULL
	if (NULL != temp2)
	{
		temp2->counterpart = NULL;
	}
	free(work);

	work = NULL;											
	(*net)[rowIndex].degree -= 1;
}
/********************************************************************************************************************************
** Function name:       Push_Back_NK
** Descriptions:        Push_Back_NK(只增加单向边),没有更新counterpart值
** input parameters:    net,data,rowIndex
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Push_Back_NK(LIST **net, int data, int rowIndex)
{
	NODE *p_node = (NODE *)malloc(sizeof(NODE));

	p_node->desNode = data;
	p_node->before = (*net)[rowIndex].p_tail;
	p_node->next = NULL;
	p_node->counterpart = NULL;

	(*net)[rowIndex].p_tail->next = p_node;
	(*net)[rowIndex].p_tail = p_node;
	(*net)[rowIndex].degree += 1;
}
/********************************************************************************************************************************
** Function name:       Push_Back_And_Update_Counterpart_NK
** Descriptions:        Push_Back(只增加单向边),并且尝试更新counterpart值
** input parameters:    net,data,rowIndex
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Push_Back_And_Update_Counterpart_NK(LIST **net, int data, int rowIndex)
{
	NODE *p_node = (NODE *)malloc(sizeof(NODE));
	NODE *temp;

	p_node->desNode = data;
	p_node->before = (*net)[rowIndex].p_tail;
	p_node->next = NULL;
	p_node->counterpart = NULL;

	(*net)[rowIndex].p_tail->next = p_node;
	(*net)[rowIndex].p_tail = p_node;
	(*net)[rowIndex].degree += 1;

	temp = Find_Edge_NK(net, rowIndex, data);				//找到counterpart，则设置它的值
	if (NULL != temp)
	{
		temp->counterpart = p_node;
		p_node->counterpart = temp;
	}
}
/********************************************************************************************************************************
** Function name:       Add_Double_Edge_NK
** Descriptions:        先判断是否已存在，之后再将双向边都加入
** input parameters:    net,edgeEnd,edgeBegin
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Add_Double_Edge_NK(LIST **net, int edgeEnd, int edgeBegin)
{
	int isMulti_edge = Is_In_List_NK(net, edgeEnd, edgeBegin);

	if (isMulti_edge == 0)
	{
		Push_Back_NK(net, edgeEnd, edgeBegin);
	}
	isMulti_edge = Is_In_List_NK(net, edgeBegin, edgeEnd);
	if (isMulti_edge == 0)
	{
		Push_Back_And_Update_Counterpart_NK(net, edgeBegin, edgeEnd);
	}
}
/********************************************************************************************************************************
** Function name:       Delete_Double_Edge_NK
** Descriptions:        Delete_Double_Edge_NK，将双向边都删去
** input parameters:    net,edgeEnd,edgeBegin
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Delete_Double_Edge_NK(LIST **net, int edgeEnd, int edgeBegin)
{
	NODE *curNode = (*net)[edgeBegin].p_head->next;

	while (NULL != curNode)
	{
		if (curNode->desNode == edgeEnd)
		{
			Delete_Pointer_Of_Edge_NK(net, curNode->counterpart, edgeEnd);
			Delete_Pointer_Of_Edge_NK(net, curNode, edgeBegin);
			break;
		}
		curNode = curNode->next;
	}
	if (NULL == curNode)
	{
		printf("Delete_Edge: no edges in this list or not found\n");
		exit(-1);
	}
}
/********************************************************************************************************************************
** Function name:       Delete_Edge_And_Update_Counterpart_NK
** Descriptions:        Delete_Edge_And_Update_Counterpart_NK，只删除单向边，并尝试更新counterpart值
** input parameters:    net,edgeEnd,edgeBegin
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Delete_Edge_And_Update_Counterpart_NK(LIST **net, int edgeEnd, int edgeBegin)
{
	NODE *curNode = (*net)[edgeBegin].p_head->next;
	NODE *temp;

	while (NULL != curNode)
	{
		if (curNode->desNode == edgeEnd)
		{
			temp = Find_Edge_NK(net, edgeBegin, curNode->desNode);	//delete pointer of counterpart
			if (NULL != temp)
			{
				temp->counterpart = NULL;
			}
			Delete_Pointer_Of_Edge_NK(net, curNode, edgeBegin);
			break;
		}
		curNode = curNode->next;
	}
	if (NULL == curNode)
	{
		printf("Delete_Edge: no edges in this list or not found\n");
		exit(-1);
	}
}
/********************************************************************************************************************************
** Function name:       Delete_Pointer_Of_Edge_NK
** Descriptions:        删除边的具体操作（一般只在此cpp中使用），操作指针
** input parameters:    net,p_node,edgeBegin
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Delete_Pointer_Of_Edge_NK(LIST **net, NODE *p_node, int edgeBegin)
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
			(*net)[edgeBegin].p_tail = p_node->before;
		}
		free(p_node);
		(*net)[edgeBegin].degree -= 1;
	}
}
/********************************************************************************************************************************
** Function name:       Set_Counterpart_NK
** Descriptions:        遍历网络中所有的边，并尝试更新对应的counterpart指针值
** input parameters:    net
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Set_Counterpart_NK(LIST **net)
{
	NODE *work;
	NODE *p_counterpart;

	for (int i = 0; i < g_Net.N; i++)
	{
		work = (*net)[i].p_head->next;
		while (NULL != work)
		{
			if (NULL == work->counterpart)
			{
				p_counterpart = Find_Edge_NK(net, i, work->desNode);
				if (NULL != p_counterpart)
				{
					work->counterpart = p_counterpart;
					p_counterpart->counterpart = work;
				}
				else
				{
					cout << "Error: counterpart is NULL!" << endl;
				}
			}
			work = work->next;
		}
	}
}
/********************************************************************************************************************************
** Function name:       Find_Edge_NK
** Descriptions:        在srcNode节点内寻找边srcNode-desNode,找到则返回对应指针，否则返回NULL
** input parameters:    net,desNode,srcNode
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
NODE *Find_Edge_NK(LIST **net, int desNode, int srcNode)
{
	NODE *work;

	work = (*net)[srcNode].p_head->next;
	while (NULL != work)
	{
		if (work->desNode == desNode)
		{
			return work;
			break;
		}
		work = work->next;
	}
	return NULL;
}
/********************************************************************************************************************************
** Function name:       Is_In_List_NK
** Descriptions:        判断desNode是否在srcNode的链表里（yes：return 1，no：return 0）
** input parameters:    net,desNode,srcNode
** output parameters:   判断值
** Returned value:      1,0
********************************************************************************************************************************/
int Is_In_List_NK(LIST **net, int desNode, int srcNode)
{
	NODE *work;

	work = (*net)[srcNode].p_head->next;
	while (NULL != work)
	{
		if (work->desNode == desNode)
		{
			return 1;
			break;
		}
		work = work->next;
	}
	return 0;
}
/********************************************************************************************************************************
** Function name:       Is_Degree_Equal_Length_NK
** Descriptions:        判断节点node的degree是否计算正确
** input parameters:    net,node
** output parameters:   yes：return 1，no：return 0
** Returned value:      1,0
********************************************************************************************************************************/
int Is_Degree_Equal_Length_NK(LIST **net, int node)
{
	int cnt = 0;
	NODE *work = (*net)[node].p_head->next;

	while (NULL != work)
	{
		cnt++;
		work = work->next;
	}
	if (cnt == (*net)[node].degree)
	{
		return 1;
	}
	return 0;
}
/********************************************************************************************************************************
** Function name:       Free2D_NK
** Descriptions:        Free2D_NK
** input parameters:    net,num
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Free2D_NK(LIST **net, int num)
{
	NODE *work;

	for (int i = 0; i < num; i++)
	{
		work = (*net)[i].p_tail;
		while (work != (*net)[i].p_head)
		{
			work->before->next = NULL;
			(*net)[i].p_tail = work->before;
			free(work);
			work = (*net)[i].p_tail;
		}
		free((*net)[i].p_head);
	}
	free((*net));
	//net->p_head = NULL;
}
/********************************************************************************************************************************
** Function name:       Output_NK
** Descriptions:        输出网络节点的度，和对应的邻居节点
** input parameters:    net,num
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_NK(LIST **net, int num)
{
	FILE *fp;
	NODE *work;

	fp = fopen("net_nk.csv", "w");
	/*if (NULL == net->p_head)
	{
		printf("net is NULL\n");
		exit(-1);
	}*/
	//cout << endl;
	for (int i = 0; i < num; i++)
	{
		fprintf(fp, "%d,", (*net)[i].degree);
		//cout << (*net)[i].degree << " ";
		work = (*net)[i].p_head;
		while (NULL != work->next)
		{
			work = work->next;
			//cout << work->data << " ";
			fprintf(fp, "%d,", work->desNode);
		}
		//cout << endl;
		fprintf(fp, "\n");
	}
	fclose(fp);
}
/********************************************************************************************************************************
** Function name:       Output_Net_Content_NK
** Descriptions:        输出net所有的成员变量特征，除了posIn_EvtSort这种传播中用到的成员变量
** input parameters:    net,step
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Net_Content_NK(LIST **net, int step)
{
	FILE *fp;
	char *name;
	Node *work;

	name = (char *)malloc(300 * sizeof(char));
	sprintf(name, "net_s%d.csv", step);
	fp = fopen(name, "w");
	for (int i = 0; i < g_Net.N; i++)
	{
		work = (*net)[i].p_head->next;
		fprintf(fp, "%d(D:%d)\t", i, (*net)[i].degree);			//输出节点i和对应的 度
		while (NULL != work)
		{
			if (NULL != work->counterpart)
			{
				fprintf(fp, "%d{%d},",
					work->desNode, work->counterpart->desNode);	//输出邻居和counterpart的邻居（即节点i）
			}
			else
			{
				fprintf(fp, "%d{NULL},", work->desNode);		//若counterpart为NULL（有向网络中），输出邻居和counterpart=NULL
			}
			work = work->next;
		}
		fprintf(fp, "\t\t\t\t\t{H:%d,T:%d}\n",
			(*net)[i].p_head->desNode,
			(*net)[i].p_tail->desNode);							//H：head，T：tail，输出对应的desnode值（head的desnode = i）
	}
	fclose(fp);
}
/********************************************************************************************************************************
** Function name:       Init_DesNet_And_Copy_SrcNet_NK
** Descriptions:        Init_DesNet_And_Copy_SrcNet_NK
** input parameters:    srcNet,desNet
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Init_DesNet_And_Copy_SrcNet_NK(LIST **srcNet, LIST **desNet)
{
	NODE *work;

	Init_NK(desNet);
	for (int i = 0; i < g_Net.N; i++)
	{
		work = (*srcNet)[i].p_head->next;
		while (NULL != work)
		{
			Push_Back_NK(desNet, work->desNode, i);
			work = work->next;
		}
	}
	Set_Counterpart_NK(desNet);
}
/*****************************************************End Of File***************************************/
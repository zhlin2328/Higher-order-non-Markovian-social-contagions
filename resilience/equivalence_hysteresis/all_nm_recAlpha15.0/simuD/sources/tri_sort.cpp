/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           tri_sort.cpp
** Created by:          linzhaohua
** Created date:        2019-11-26
** Version:             V1.0
** Descriptions:        ���ڶ�����heapSizeΪ������Ĵ�С�������нڵ��±�Ϊ[0,heapSize-1],����[0, (heapSize-1)/2]Ϊ��Ҷ�ӽڵ㡣
						�����±�Ϊindex�Ľڵ㣬��left = 2*index + 1,right = left + 1,parent = (index - 1) / 2
********************************************************************************************************************************/
#include "../headers/config.h"
/********************************************************************************************************************************
** Function name:       Build_Min_Heap_TRI
** Descriptions:        ������С������
** input parameters:    evtSort_2D,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Build_Min_Heap_TRI(vector<EVT2D>& evtSort_2D, int heapSize)
{
	for (int i = (heapSize - 1) / 2; i >= 0; i--)
	{
		Min_Heapify_TRI(evtSort_2D, i, heapSize);
	}
}
/********************************************************************************************************************************
** Function name:       Min_Heapify_TRI
** Descriptions:        ������������indexλ�ý�����С������
** input parameters:    evtSort_2D,index: [0,heapSize-1],heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heapify_TRI(vector<EVT2D>& evtSort_2D, int index, int heapSize)
{
	int left;
	int right;
	int min;
	EVT2D eventTemp;

	left = 2 * index + 1;
	right = left + 1;
	if (left < heapSize && evtSort_2D[left].tau < evtSort_2D[index].tau)
	{
		min = left;
	}
	else
	{
		min = index;
	}
	if (right < heapSize && evtSort_2D[right].tau < evtSort_2D[min].tau)
	{
		min = right;
	}
	if (min != index)
	{
		eventTemp = evtSort_2D[index];
		evtSort_2D[index] = evtSort_2D[min];
		evtSort_2D[min] = eventTemp;

		Change_PosIn_EvtSort_Of_Net_TRI(evtSort_2D[index].posIn_Tri, index);
		Change_PosIn_EvtSort_Of_Net_TRI(evtSort_2D[min].posIn_Tri, min);

		Min_Heapify_TRI(evtSort_2D, min, heapSize);
	}
}
/********************************************************************************************************************************
** Function name:       Change_PosIn_EvtSort_Of_Net_TRI
** Descriptions:        ����������posIn_EvtSort_2D��ֵ
** input parameters:    p_node,newPos
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Change_PosIn_EvtSort_Of_Net_TRI(TRINODE *p_node, int newPos)
{
	p_node->posIn_EvtSort_2D = newPos;
}
/********************************************************************************************************************************
** Function name:       Min_Heap_Change_Key_TRI
** Descriptions:        ����С����indexλ�õ�ֵ��Ϊdata�����ҽ�����С�����򣬵õ����º����С��
** input parameters:    evtSort_2D,index: [0,heapSize-1],data,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heap_Change_Key_TRI(vector<EVT2D>& evtSort_2D, int index, double data, int heapSize)
{
	int parent = (index - 1) / 2;
	EVT2D eventTemp;

	if (data > evtSort_2D[index].tau)									//����С���У���dataֵ��oldֵ�������ƣ�����Min_Heapify��������������ƣ�
	{
		evtSort_2D[index].tau = data;
		Min_Heapify_TRI(evtSort_2D, index, heapSize);
	}
	else if (data < evtSort_2D[index].tau)								//��С�����ϸ�
	{
		evtSort_2D[index].tau = data;
		while (index > 0 && evtSort_2D[parent].tau > evtSort_2D[index].tau)
		{
			eventTemp = evtSort_2D[index];
			evtSort_2D[index] = evtSort_2D[parent];
			evtSort_2D[parent] = eventTemp;

			Change_PosIn_EvtSort_Of_Net_TRI(evtSort_2D[index].posIn_Tri, index);
			Change_PosIn_EvtSort_Of_Net_TRI(evtSort_2D[parent].posIn_Tri, parent);

			index = parent;
			parent = (index - 1) / 2;
		}
	}
}
/********************************************************************************************************************************
** Function name:       Min_Heap_Insert_TRI
** Descriptions:        ����С���в���evt���Ԫ��(�����󣬶Ѵ�С��heapSize��ΪheapSize+1)
** input parameters:    evtSort_2D,evt,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heap_Insert_TRI(vector<EVT2D>& evtSort_2D, EVT2D evt, int& heapSize)
{
	double data = evt.tau;

	evt.tau = TAU_INF;
	evtSort_2D.push_back(evt);
	evt.posIn_Tri->posIn_EvtSort_2D = heapSize;
	heapSize++;
	Min_Heap_Change_Key_TRI(evtSort_2D, heapSize - 1, data, heapSize);
}
/********************************************************************************************************************************
** Function name:       Min_Heap_Delete_TRI
** Descriptions:        ����С����ɾ��indexλ���ϵ�Ԫ��(�����󣬶Ѵ�С��heapSize��ΪheapSize-1)
** input parameters:    evtSort_2D,index,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heap_Delete_TRI(vector<EVT2D>& evtSort_2D, int index, int& heapSize)
{
	double data;
	double temp = evtSort_2D[index].tau;

	evtSort_2D[heapSize - 1].posIn_Tri->posIn_EvtSort_2D = index;
	evtSort_2D[index].posIn_Tri->posIn_EvtSort_2D = NOT_IN_EVTSORT_2D;

	evtSort_2D[index] = evtSort_2D[heapSize - 1];
	data = evtSort_2D[heapSize - 1].tau;
	evtSort_2D[index].tau = temp;

	heapSize--;
	evtSort_2D.pop_back();
	if (index != heapSize)
	{
		Min_Heap_Change_Key_TRI(evtSort_2D, index, data, heapSize);
	}
}
/********************************************************************************************************************************
** Function name:       Output_Min_Heap_Sort_TRI
** Descriptions:        �����С�������Ľ����0-(heapSize-1)���ɴ�С���г���,���µļ�evtSort2��ʾ
** input parameters:    evtSort,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Min_Heap_Sort_TRI(vector<EVT2D>& evtSort_2D, int heapSize)
{
	vector<EVT2D> evtSort2D_After;
	EVT2D temp;
	int newHeap_Size = heapSize;

	evtSort2D_After = evtSort_2D;
	//Build_Min_Heap(evtSort2, newHeap_Size);
	Output_Min_Heap_TRI(evtSort2D_After, heapSize, 0);
	for (int i = newHeap_Size - 1; i >= 1; i--)
	{
		temp = evtSort2D_After[0];
		evtSort2D_After[0] = evtSort2D_After[i];
		evtSort2D_After[i] = temp;
		newHeap_Size--;

		Min_Heapify_TRI(evtSort2D_After, 0, newHeap_Size);
	}
	Output_Min_Heap_TRI(evtSort2D_After, heapSize, 1);
}
/********************************************************************************************************************************
** Function name:       Output_Min_Heap_TRI
** Descriptions:        ֱ�������С�ѽṹ�ϵ�Ԫ�أ�������Output_Min_Heap_Sort�������Ľ����
** input parameters:    evtSort,heapSize,info
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Min_Heap_TRI(vector<EVT2D>& evtSort_2D, int heapSize, int info)
{
	FILE *fp;
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	sprintf(name, "heap_event_tau_i_j_info%d.csv", info);
	fp = fopen(name, "w");
	for (int i = 0; i < heapSize; i++)
	{
		fprintf(fp, "{%f,%d,%d,%d}\n", evtSort_2D[i].tau, evtSort_2D[i].srcNode, evtSort_2D[i].desNode1, evtSort_2D[i].desNode2);		//tau,i,j,k}��i,j,k����������ģ��
	}
	fclose(fp);
}
/***********************************************End Of File*********************************************************************/
/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           sort.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        ���ڶ�����heapSizeΪ������Ĵ�С�������нڵ��±�Ϊ[0,heapSize-1],����[0, (heapSize-1)/2]Ϊ��Ҷ�ӽڵ㡣
						�����±�Ϊindex�Ľڵ㣬��left = 2*index + 1,right = left + 1,parent = (index - 1) / 2
********************************************************************************************************************************/
#include "../headers/config.h"
/********************************************************************************************************************************
** Function name:       Build_Min_Heap
** Descriptions:        ������С������
** input parameters:    evtSort,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Build_Min_Heap(vector<EVT>& evtSort, int heapSize)
{
	for (int i = (heapSize - 1) / 2; i >= 0; i--)
	{
		Min_Heapify(evtSort, i, heapSize);
	}
}
/********************************************************************************************************************************
** Function name:       Min_Heapify
** Descriptions:        ������������indexλ�ý�����С������
** input parameters:    evtSort,index: [0,heapSize-1],heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heapify(vector<EVT>& evtSort, int index, int heapSize)
{
	int left;
	int right;
	int min;
	EVT eventTemp;

	left = 2 * index + 1;
	right = left + 1;
	if (left < heapSize && evtSort[left].tau < evtSort[index].tau)
	{
		min = left;
	}
	else
	{
		min = index;
	}
	if (right < heapSize && evtSort[right].tau < evtSort[min].tau)
	{
		min = right;
	}
	if (min != index)
	{
		eventTemp = evtSort[index];
		evtSort[index] = evtSort[min];
		evtSort[min] = eventTemp;

		Change_PosIn_EvtSort_Of_Net(evtSort[index].posIn_Net, index);
		Change_PosIn_EvtSort_Of_Net(evtSort[min].posIn_Net, min);

		Min_Heapify(evtSort, min, heapSize);
	}
}
/********************************************************************************************************************************
** Function name:       Change_PosIn_EvtSort_Of_Net
** Descriptions:        ����������posIn_EvtSort��ֵ
** input parameters:    p_node,eventFlag,newPos
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Change_PosIn_EvtSort_Of_Net(Node *p_node, int newPos)
{
	p_node->posIn_EvtSort = newPos;
}
/********************************************************************************************************************************
** Function name:       Min_Heap_Change_Key
** Descriptions:        ����С����indexλ�õ�ֵ��Ϊdata�����ҽ�����С�����򣬵õ����º����С��
** input parameters:    evtSort,index: [0,heapSize-1],data,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heap_Change_Key(vector<EVT>& evtSort, int index, double data, int heapSize)
{
	int parent = (index - 1) / 2;
	EVT eventTemp;

	if (data > evtSort[index].tau)									//����С���У���dataֵ��oldֵ�������ƣ�����Min_Heapify��������������ƣ�
	{
		evtSort[index].tau = data;
		Min_Heapify(evtSort, index, heapSize);
	}
	else if (data < evtSort[index].tau)								//��С�����ϸ�
	{
		evtSort[index].tau = data;
		while (index > 0 && evtSort[parent].tau > evtSort[index].tau)
		{
			eventTemp = evtSort[index];
			evtSort[index] = evtSort[parent];
			evtSort[parent] = eventTemp;

			Change_PosIn_EvtSort_Of_Net(evtSort[index].posIn_Net, index);
			Change_PosIn_EvtSort_Of_Net(evtSort[parent].posIn_Net, parent);

			index = parent;
			parent = (index - 1) / 2;
		}
	}
}
/********************************************************************************************************************************
** Function name:       Min_Heap_Insert
** Descriptions:        ����С���в���evt���Ԫ��(�����󣬶Ѵ�С��heapSize��ΪheapSize+1)
** input parameters:    evtSort,evt,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heap_Insert(vector<EVT>& evtSort, EVT evt, int& heapSize)
{
	double data = evt.tau;

	evt.tau = TAU_INF;
	evtSort.push_back(evt);
	evt.posIn_Net->posIn_EvtSort = heapSize;
	heapSize++;
	Min_Heap_Change_Key(evtSort, heapSize - 1, data, heapSize);
}
/********************************************************************************************************************************
** Function name:       Min_Heap_Delete
** Descriptions:        ����С����ɾ��indexλ���ϵ�Ԫ��(�����󣬶Ѵ�С��heapSize��ΪheapSize-1)
** input parameters:    evtSort,index,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heap_Delete(vector<EVT>& evtSort, int index, int& heapSize)
{
	double data;
	double temp = evtSort[index].tau;

	evtSort[heapSize - 1].posIn_Net->posIn_EvtSort = index;
	evtSort[index].posIn_Net->posIn_EvtSort = NOT_IN_EVTSORT;

	evtSort[index] = evtSort[heapSize - 1];
	data = evtSort[heapSize - 1].tau;
	evtSort[index].tau = temp;

	heapSize--;
	evtSort.pop_back();
	if (index != heapSize)
	{
		Min_Heap_Change_Key(evtSort, index, data, heapSize);
	}
}
/********************************************************************************************************************************
** Function name:       Output_Min_Heap_Sort
** Descriptions:        �����С�������Ľ����0-(heapSize-1)���ɴ�С���г���,���µļ�evtSort2��ʾ
** input parameters:    evtSort,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Min_Heap_Sort(vector<EVT>& evtSort, int heapSize)
{
	vector<EVT> evtSort2;
	EVT temp;
	int newHeap_Size = heapSize;

	evtSort2 = evtSort;
	//Build_Min_Heap(evtSort2, newHeap_Size);
	Output_Min_Heap(evtSort2, heapSize, 0);
	for (int i = newHeap_Size - 1; i >= 1; i--)
	{
		temp = evtSort2[0];
		evtSort2[0] = evtSort2[i];
		evtSort2[i] = temp;
		newHeap_Size--;

		Min_Heapify(evtSort2, 0, newHeap_Size);
	}
	Output_Min_Heap(evtSort2, heapSize, 1);
}
/********************************************************************************************************************************
** Function name:       Output_Min_Heap
** Descriptions:        ֱ�������С�ѽṹ�ϵ�Ԫ�أ�������Output_Min_Heap_Sort�������Ľ����
** input parameters:    evtSort,heapSize,info
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Output_Min_Heap(vector<EVT>& evtSort, int heapSize, int info)
{
	FILE *fp;
	char *name;

	name = (char *)malloc(NAME_SIZE * sizeof(char));
	sprintf(name, "heap_event_tau_i_j_info%d.csv", info);
	fp = fopen(name, "w");
	for (int i = 0; i < heapSize; i++)
	{
		fprintf(fp, "{%f,%d,%d}\n", evtSort[i].tau, evtSort[i].srcNode, evtSort[i].desNode);		//tau,i,j}
	}
	fclose(fp);
}
/***********************************************End Of File*********************************************************************/
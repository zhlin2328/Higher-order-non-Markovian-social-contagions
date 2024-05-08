/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           sort.cpp
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        对于堆排序：heapSize为堆排序的大小，数组中节点下标为[0,heapSize-1],其中[0, (heapSize-1)/2]为非叶子节点。
						对于下标为index的节点，其left = 2*index + 1,right = left + 1,parent = (index - 1) / 2
********************************************************************************************************************************/
#include "../headers/config.h"
/********************************************************************************************************************************
** Function name:       Build_Min_Heap
** Descriptions:        构建最小堆排序
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
** Descriptions:        单独对排序中index位置进行最小堆排序
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
** Descriptions:        更新网络中posIn_EvtSort的值
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
** Descriptions:        将最小堆中index位置的值改为data，并且进行最小堆排序，得到更新后的最小堆
** input parameters:    evtSort,index: [0,heapSize-1],data,heapSize
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Min_Heap_Change_Key(vector<EVT>& evtSort, int index, double data, int heapSize)
{
	int parent = (index - 1) / 2;
	EVT eventTemp;

	if (data > evtSort[index].tau)									//在最小堆中，若data值比old值大，则下移，调用Min_Heapify函数（变大，则下移）
	{
		evtSort[index].tau = data;
		Min_Heapify(evtSort, index, heapSize);
	}
	else if (data < evtSort[index].tau)								//变小，则上浮
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
** Descriptions:        在最小堆中插入evt这个元素(结束后，堆大小由heapSize变为heapSize+1)
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
** Descriptions:        在最小堆中删除index位置上的元素(结束后，堆大小由heapSize变为heapSize-1)
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
** Descriptions:        输出最小堆排序后的结果，0-(heapSize-1)，由大到小排列出来,用新的即evtSort2表示
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
** Descriptions:        直接输出最小堆结构上的元素（而不是Output_Min_Heap_Sort排序处理后的结果）
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
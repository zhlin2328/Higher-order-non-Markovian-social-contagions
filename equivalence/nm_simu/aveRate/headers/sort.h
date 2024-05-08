#pragma once
/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           sort.h
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        sort.h
********************************************************************************************************************************/
#ifndef __SORT_H
#define __SORT_H

#define NOT_IN_EVTSORT		-1

void Build_Min_Heap(vector<EVT>& evtSort, int heapSize);
void Min_Heapify(vector<EVT>& evtSort, int index, int heapSize);
void Change_PosIn_EvtSort_Of_Net(Node *p_node, int newPos);
void Min_Heap_Change_Key(vector<EVT>& evtSort, int index, double data, int heapSize);
void Min_Heap_Insert(vector<EVT>& evtSort, EVT evt, int& heapSize);
void Min_Heap_Delete(vector<EVT>& evtSort, int index, int& heapSize);
void Output_Min_Heap_Sort(vector<EVT>& evtSort, int heapSize);
void Output_Min_Heap(vector<EVT>& evtSort, int heapSize, int info);

#endif //__SORT_H
/***********************************************End Of File*********************************************************************/

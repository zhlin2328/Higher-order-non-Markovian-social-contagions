#pragma once
/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           tri_sort.h
** Created by:          linzhaohua
** Created date:        2019-11-26
** Version:             V1.0
** Descriptions:        tri_sort.h
********************************************************************************************************************************/
#ifndef __TRI_SORT_H
#define __TRI_SORT_H

#define NOT_IN_EVTSORT_2D		-1
void Build_Min_Heap_TRI(vector<EVT2D>& evtSort_2D, int heapSize);
void Min_Heapify_TRI(vector<EVT2D>& evtSort_2D, int index, int heapSize);
void Change_PosIn_EvtSort_Of_Net_TRI(TRINODE *p_node, int newPos);
void Min_Heap_Change_Key_TRI(vector<EVT2D>& evtSort_2D, int index, double data, int heapSize);
void Min_Heap_Insert_TRI(vector<EVT2D>& evtSort_2D, EVT2D evt, int& heapSize);
void Min_Heap_Delete_TRI(vector<EVT2D>& evtSort_2D, int index, int& heapSize);
void Output_Min_Heap_Sort_TRI(vector<EVT2D>& evtSort_2D, int heapSize);
void Output_Min_Heap_TRI(vector<EVT2D>& evtSort_2D, int heapSize, int info);

#endif //__TRI_SORT_H
/***********************************************End Of File*********************************************************************/

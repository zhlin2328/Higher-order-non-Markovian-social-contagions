/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           free_space.cpp
** Created by:          linzhaohua
** Created date:        2018-3-16
** Version:             V1.0
** Descriptions:        free_space.cpp
********************************************************************************************************/
#include "../headers/config.h"
/********************************************************************************************************************************
** Function name:       Free_2DInt_Vector
** Descriptions:        Free_2DInt_Vector
** input parameters:    num,vec
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Free_2DInt_Vector(int num, vector<vector<int> >& vec)
{
	for (int i = 0; i < num; i++)
	{
		vector<int>().swap(vec[i]);
	}
	vector<vector<int> >().swap(vec);
}
/********************************************************************************************************************************
** Function name:       Free_2DDouble_Vector
** Descriptions:        Free_2DDouble_Vector
** input parameters:    num,vec
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
void Free_2DDouble_Vector(int num, vector<vector<double> >& vec)
{
	for (int i = 0; i < num; i++)
	{
		vector<double>().swap(vec[i]);
	}
	vector<vector<double> >().swap(vec);
}
/*****************************************************End Of File***************************************/
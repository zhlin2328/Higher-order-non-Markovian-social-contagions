/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           rand.h
** Created by:          linzhaohua
** Created date:        2018-3-16
** Version:             V1.0
** Descriptions:        rand.h
********************************************************************************************************************************/
#ifndef __RAND_H
#define __RAND_H
typedef struct Rand
{
	std::random_device rd;											//seed is random
	std::mt19937 mt;												//rand()
	std::uniform_real_distribution<double> distF;					//double(rand())/double(RAND_MAX)  //dist_f(mt)
	std::uniform_int_distribution<int> distD;						//rand()%N  //dist_d(mt)
}RD;
extern RD g_Rd;

#endif //__RAND_H
/***********************************************End Of File*********************************************************************/

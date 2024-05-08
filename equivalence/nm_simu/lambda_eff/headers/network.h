/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           network.h
** Created by:          linzhaohua
** Created date:        2018-4-28
** Version:             V1.0
** Descriptions:        network.h
********************************************************************************************************/
#ifndef __NETWORK_H
#define __NETWORK_H
//#define ER_NETWORK
//#define WS_NETWORK
//#define BA_NETWORK
//#define COMPLETE_NETWORK
//#define UCM_NETWORK
//#define REGULAR_RANDOM_NETWORK
//#define REGULAR_NETWORK
#define	OTHER_NETWORK

struct ERNetwork
{
	int SERIAL;
	int N;
	double P; //edges connected probability
};
struct WSNetwork
{
	int SERIAL;
	int N;
	int HALF_DEGREE;
	double P;
};
struct BANetwork
{
	int SERIAL;
	int N;
	int M0;
	int M;
};
struct CmpltNetwork
{
	int SERIAL;
	int N;
};
struct UCMNetwork
{
	int SERIAL;
	int N;
	int KMIN;
	int KMAX;
	double GAMMA;
};
struct RRNNetwork
{
	int SERIAL;
	int N;
	double DEGREE;
};
struct REGNetwork
{
	int SERIAL;
	int N;
	double DEGREE;
};
struct Other_Network
{
	int SERIAL;
	int N;
};

#ifdef ER_NETWORK
extern struct ERNetwork g_Net; //声明全局变量

#elif defined WS_NETWORK
extern struct WSNetwork g_Net;

#elif defined BA_NETWORK
extern struct BANetwork g_Net;

#elif defined COMPLETE_NETWORK
extern struct CmpltNetwork g_Net;

#elif defined UCM_NETWORK
extern struct UCMNetwork g_Net;

#elif defined REGULAR_RANDOM_NETWORK
extern struct RRNNetwork g_Net;

#elif defined REGULAR_NETWORK
extern struct REGNetwork g_Net;

#elif defined OTHER_NETWORK
extern struct Other_Network g_Net;

#endif

#endif //__NETWORK_H
/*****************************************************End Of File***************************************/
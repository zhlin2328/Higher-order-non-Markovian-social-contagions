/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           network.cpp
** Created by:          linzhaohua
** Created date:        2018-4-28
** Version:             V1.0
** Descriptions:        network.cpp
********************************************************************************************************/
#include "../headers/config.h"

#ifdef ER_NETWORK
struct ERNetwork g_Net; //定义全局变量

#elif defined WS_NETWORK
struct WSNetwork g_Net;

#elif defined BA_NETWORK
struct BANetwork g_Net;

#elif defined COMPLETE_NETWORK
struct CmpltNetwork g_Net;

#elif defined UCM_NETWORK
struct UCMNetwork g_Net;

#elif defined REGULAR_RANDOM_NETWORK
struct RRNNetwork g_Net;

#elif defined REGULAR_NETWORK
struct REGNetwork g_Net;

#elif defined OTHER_NETWORK
struct Other_Network g_Net;

#else
#error "no defined network"
#endif
/***********************************************End Of File*********************************************************************/
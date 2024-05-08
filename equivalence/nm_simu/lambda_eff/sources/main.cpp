/********************************************************************************************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info--------------------------------------------------------------------------------------------------------
** File name:           main.cpp
** Created by:          linzhaohua
** Created date:        2018-3-16
** Version:             V1.0
** Descriptions:        main.cpp
********************************************************************************************************************************/
#include "../headers/config.h"
/********************************************************************************************************************************
** Function name:       main
** Descriptions:        main
** input parameters:    None
** output parameters:   None
** Returned value:      None
********************************************************************************************************************************/
int main(int argc, char **argv)
{
	clock_t startTime;
	clock_t endTime;

	startTime = clock();
	g_taskNum = atoi(argv[1]);
	Draw_Rho_Vs_AbscissaParm_Diagram1();
	endTime = clock();

	cout << "run time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s"
		<< "(" << (double)(endTime - startTime) / (CLOCKS_PER_SEC * 60) << "min)"
		<< "(" << (double)(endTime - startTime) / (CLOCKS_PER_SEC * 60 * 60) << "h)" << endl;

	system("pause");
	return 0;
}
/***********************************************End Of File*********************************************************************/
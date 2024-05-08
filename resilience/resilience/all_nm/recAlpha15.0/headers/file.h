/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           file.h
** Created by:          linzhaohua
** Created date:        2019-5-13
** Version:             V1.0
** Descriptions:        file.h
********************************************************************************************************/
//#pragma warning(disable:4996)  
#ifndef __FILE_H
#define __FILE_H

void Output_Network_Parm(void);
void Output_Spd_Parm(void);
void Output_Rd_Parm(void);
void Output_BetaC_Of_Diff_Rho0_To_File(int netIndex, vector<vector<double> >& betaC_Rho0);
void Output_AveSteadyRho_Parm_To_File(double rho0, vector<vector<double> >& aveStRho_Parm);
void Output_NotAveStRho_Parm_To_File(int netCnt, int diffusionCnt, vector<vector<double> >& rhoParm);
void Output_aveRho_t_To_File(double lamEff, vector<vector<double> >& aveRho_t);
void Output_rhoT_To_File(double lamEff, vector<vector<double> >& rhoT);
void Read_Network_File(LIST **net, char *file_name);
void Read_Net_TriList_File(TRILIST **netTri, char *file_name);

#endif //__FILE_H
/*****************************************************End Of File***************************************/

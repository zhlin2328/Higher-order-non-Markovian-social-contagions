#pragma once
/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           file.h
** Created by:          linzhaohua
** Created date:        2018-5-9
** Version:             V1.0
** Descriptions:        file.h
********************************************************************************************************/
#ifndef __FILE_H
#define __FILE_H

void Output_Network_Parm(void);
void Output_Fun_Parm(void);
void Output_RhoT_To_File(vector<vector<double> >& rhoT);
void Output_Rho_Parm_To_File(vector<vector<double> >& rhoParm, double rho0);
void Read_Network_File(LIST **net, char *file_name);
void Read_Net_TriList_File(TRILIST **netTri, char *file_name);
#endif // __FILE_H
/*****************************************************End Of File***************************************/
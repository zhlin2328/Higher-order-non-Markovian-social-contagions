/****************************************Copyright (c)*************************************************
**                      		East China Normal University
**                               http://www.ecnu.edu.cn/
**--------------File Info------------------------------------------------------------------------------
** File name:           property.h
** Created by:          linzhaohua
** Created date:        2018-11-7
** Version:             V1.0
** Descriptions:        property.h
********************************************************************************************************/
#ifndef __PROPERTY_H
#define __PROPERTY_H

int Nkmax_NK(const int N, LIST *net_nk);
double Avenk_NK(const int N, LIST *net_nk);
double Avenk_TriNK(const int N, TRILIST *netTri);
double *Pk_NK(const int N, LIST *net_nk);
double *Cupk_NK(const int N, LIST *net_nk);
double S_Avepath_NK(const int s, const int N, LIST *net_nk);	
double S_Hm_Avepath_NK(const int s, const int N, LIST *net_nk);
int Is_Connected_NK(const int N, LIST *net_nk);
double Avepath_NK(const int N, LIST *net_nk);
double Hm_Avepath_NK(const int N, LIST *net_nk);

int Nkmax_NN(const int N, int *nk);		
int *Nk_NN(const int N, int **net_nn);
double Avenk_NN(const int N, int *nk_nn);
double *Pk_NN(const int N, int *nk);
double Cc_NN(const int N, int **net_nn);
double *Cck_NN(const int nkmax, const int N, int *nk, double *pk, int **net_nn);
double S_Avepath_NN(const int s, const int N, int **net_nn);
int Is_Connected_NN(const int N, int **net_nn);
double Avepath_NN(const int N, int **net_nn);

#endif //__PROPERTY_H
/*****************************************************End Of File***************************************/
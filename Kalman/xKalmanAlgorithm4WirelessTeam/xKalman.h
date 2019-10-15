/**
 * @File    xKalman.h
 * @Date    Tuesday, Dec. 25, 2017 at 14:17:09 PM BJT
 * @Author  Washington Ruan
 * @Email   washingtonxr@live.com
 *
 * This file contains the implementation of the Kalman filter algorithm for sensor device's
 * module's main file.
 *
 * @bug No known bugs.
 **/
#ifndef XKALMAN_H
#define XKALMAN_H

#include "xKalmanCom.h"

#define xKalman_DLEN	4
#define xKalman_DALEN	16

typedef struct xKalman_Struct{

	/* Kalman Matrices */
	unsigned short nRows;	/* Each row. */
	unsigned short nCols;	/* Each Collumn. */
	float Ts;
	
	com_matrix_instance_f32 A;
	com_matrix_instance_f32 K;
	com_matrix_instance_f32 P;
	/* P Priority sign. */
	com_matrix_instance_f32 Ppriori;
	com_matrix_instance_f32 H;
	com_matrix_instance_f32 Ht;
	com_matrix_instance_f32 Q;
	com_matrix_instance_f32 R;
	/* Omega sign. */
	com_matrix_instance_f32 omega;
	com_matrix_instance_f32 Eye;
	com_matrix_instance_f32 q;
	/* Q Priority sign. */
	com_matrix_instance_f32 qPriori;
	com_matrix_instance_f32 qz;
	com_matrix_instance_f32 w;

	float qMod;
	float wMod;
	
	/* Kalman data vectors */
	float qData[xKalman_DLEN];
	float qPrioriData[xKalman_DLEN];
	float qzData[xKalman_DLEN];
	float wData[xKalman_DLEN-1];
	float AData[xKalman_DALEN];
	float KData[xKalman_DALEN];
	float PData[xKalman_DALEN];
	float PprioriData[xKalman_DALEN];
	float Pmod;  // Covariance matrix
	float HData[xKalman_DALEN];
	float HtData[xKalman_DALEN];
	float QData[xKalman_DALEN];
	float RData[xKalman_DALEN];
	float omegaData[xKalman_DALEN];
	float EyeData[xKalman_DALEN];

	unsigned char lastQuadrant;

}xKalman_t;

void xKalman_Vector2ModK(float *src, float *dst, unsigned char size);
void xKalman_reflashK(com_matrix_instance_f32 *src, com_matrix_instance_f32 *dst);
/**
 * Notice: The input data must be processed by least squares like NewtonGauss 
 *			before it can be used.
 * Status: Checked.
 */
void xKalman_Process(float *qNG, 
					float *qInit, 
					float *Pinit, 
					float *gyroMeasure, 
					float elapsedTime, 
					float *qDst);

int xKalman_Matrices_Init_Com(xKalman_t *k);

int xKalman_Init(xKalman_t *k);

#endif
/* End of this file. */
/**
 * @File    xKalman.c
 * @Date    Tuesday, Dec. 25, 2017 at 14:17:09 PM BJT
 * @Author  Washington Ruan
 * @Email   washingtonxr@live.com
 *
 * This file contains the implementation of the Kalman filter algorithm for sensor device's
 * module's main file.
 *
 * @bug No known bugs.
 **/

#include "xKalman.h"

void xKalman_Vector2ModK(float *src, float *dst, unsigned char size)
{
	unsigned char i;
	
	dst[0] = 0;
	
	for(i = 0; i < size; i++)
		dst[0] += (src[i]*src[i]);
	
	dst[0] = sqrtf(dst[0]);
}

void xKalman_reflashK(com_matrix_instance_f32 *src, com_matrix_instance_f32 *dst)
{
	unsigned char i;
	
	for(i = 0; i < (src->numCols*src->numRows); i++)
		dst->pData[i] = src->pData[i];
}

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
					float *qDst)
{
	
	float scale = 0;
	unsigned char count = 0;
	float PmodBest = 100;
	unsigned char i = 0;

	xKalman_t k;

	/* Auxiliary matrices */
	com_matrix_instance_f32 auxA_4_1;
	com_matrix_instance_f32 auxB_4_1;
	com_matrix_instance_f32 auxA_4_4;
	com_matrix_instance_f32 auxB_4_4;
	com_matrix_instance_f32 auxC_4_4;
	com_matrix_instance_f32 auxD_4_4;
	
	float auxAData_4_1[xKalman_DLEN];
	float auxBData_4_1[xKalman_DLEN];
	float auxAData_4_4[xKalman_DALEN];
	float auxBData_4_4[xKalman_DALEN];
	float auxCData_4_4[xKalman_DALEN];
	float auxDData_4_4[xKalman_DALEN];

	xKalman_Init(&k);

	matrix_init_f32(&auxA_4_1, xKalman_DLEN, 1, auxAData_4_1);
	matrix_init_f32(&auxB_4_1, xKalman_DLEN, 1, auxBData_4_1);
	matrix_init_f32(&auxA_4_4, xKalman_DLEN, xKalman_DLEN, auxAData_4_4);
	matrix_init_f32(&auxB_4_4, xKalman_DLEN, xKalman_DLEN, auxBData_4_4);
	matrix_init_f32(&auxC_4_4, xKalman_DLEN, xKalman_DLEN, auxCData_4_4);
	matrix_init_f32(&auxD_4_4, xKalman_DLEN, xKalman_DLEN, auxDData_4_4);

	/*------------------------------------------------------------
	 ********************* Update Measurements *******************
	 *------------------------------------------------------------
	Update measured (NewtonGauss) and initial quaternions */
	for(i = 0; i < xKalman_DLEN; i++){
		k.qzData[i] = qNG[i];
		k.qData[i] = qInit[i];
		qDst[i] = qInit[i];
	}
	for(i = 0; i < xKalman_DALEN; i++)
		k.PData[i] = Pinit[i];
	/* Update sampling rate */
	k.Ts = elapsedTime;

	/* Update angular velocity */
	for(i = 0; i< xKalman_DLEN - 1; i++)
		k.wData[i] = gyroMeasure[i];

	/* Updates Omega matrix
	 |          0,  wData[2],  -wData[1], wData[0],|
	 |-wData[2],           0,   wData[0], wData[1],|
	 | wData[1], -wData[0],            0, wData[2],|
	 |-wData[0], -wData[1],  -wData[2],          0 |
	*/

	k.omegaData[0] =  0;
	k.omegaData[1] =  k.wData[2];
	k.omegaData[2] = -k.wData[1];
	k.omegaData[3] =  k.wData[0];

	k.omegaData[4] = -k.wData[2];
	k.omegaData[5] =  0;
	k.omegaData[6] =  k.wData[0];
	k.omegaData[7] =  k.wData[1];

	k.omegaData[8] =  k.wData[1];
	k.omegaData[9] = -k.wData[0];
	k.omegaData[10] = 0;
	k.omegaData[11] = k.wData[2];

	k.omegaData[12] = -k.wData[0];
	k.omegaData[13] = -k.wData[1];
	k.omegaData[14] = -k.wData[2];
	k.omegaData[15] =  0;

	xKalman_Vector2ModK(k.wData, &k.wMod, xKalman_DLEN - 1);

	/*Prediction: time update
	 * Process Matrix update:
	 * o.A = eye(4) + 0.5*Ts*o.omega - 0.125*Ts^2*o.wMod*eye(4); */
	 scale = 0.125*k.Ts*k.Ts*k.wMod*k.wMod;
	 mat_scale_f32(&k.Eye, scale, &auxA_4_4);
	 scale = 0.5*k.Ts;
	 mat_scale_f32(&k.omega, scale, &auxB_4_4);
	 mat_sub_f32(&auxB_4_4, &auxA_4_4, &auxC_4_4);
	 mat_add_f32(&k.Eye, &auxC_4_4, &k.A);

	/* Quaternion prediction:
	 *  o.qPriori = o.A * q; */
	 mat_mult_f32(&k.A, &k.q, &k.qPriori);

	 /* Covariance Matrix prediction:
	  *  o.Ppriori = o.A * o.P * o.A' + o.Q; */
	 mat_mult_f32(&k.A, &k.P, &auxA_4_4);
	 mat_trans_f32(&k.A, &auxB_4_4);
	 mat_mult_f32(&auxA_4_4, &auxB_4_4, &auxC_4_4);
	 mat_add_f32(&auxC_4_4, &k.Q, &k.Ppriori);


	/*Estimation: measurement update
	 * Kalman Gain:
	 * o.K = o.Ppriori*o.H'/(o.H*o.Ppriori*o.H' + o.R); */
	 mat_mult_f32(&k.Ppriori, &k.Ht, &auxA_4_4);
	 mat_mult_f32(&k.H, &k.Ppriori, &auxB_4_4);
	 mat_mult_f32(&auxB_4_4, &k.Ht, &auxC_4_4);
	 mat_add_f32(&auxC_4_4, &k.R, &auxB_4_4);
	 xKalman_reflashK(&auxB_4_4, &auxD_4_4);
	 mat_inverse_f32(&auxD_4_4, &auxC_4_4);
	 mat_mult_f32(&auxA_4_4, &auxC_4_4, &k.K);

	 /* Quaternion estimation:
	  * o.q = o.qPriori + o.K*(o.z - (o.H*o.qPriori)); */
	 mat_mult_f32(&k.H, &k.qPriori, &auxA_4_1);
	 mat_sub_f32(&k.qz, &auxA_4_1, &auxB_4_1);
	 mat_mult_f32(&k.K, &auxB_4_1, &auxA_4_1);
	 mat_add_f32(&k.qPriori, &auxA_4_1, &k.q);

	 /* Quaternion Normalization:
	  * o.q = o.q/sqrt(o.q(1)^2+o.q(2)^2+o.q(3)^2+o.q(4)^2); */
	 xKalman_Vector2ModK(k.qData, &k.qMod, xKalman_DLEN);
	 
	 if(k.qMod > 0)
		 mat_scale_f32(&k.q, 1/k.qMod, &k.q);

	 /* Covariance Matrix Estimation:
	  * o.P = o.Ppriori*eye(4) - o.Ppriori*o.K*o.H; */
	 mat_mult_f32(&k.Ppriori, &k.Eye, &auxA_4_4);
	 mat_mult_f32(&k.Ppriori, &k.K, &auxB_4_4);
	 mat_mult_f32(&auxB_4_4, &k.H, &auxC_4_4);
	 mat_sub_f32(&auxA_4_4, &auxC_4_4, &k.P);

	 for(i = 0; i < xKalman_DLEN; i++)
		qDst[i] = k.qData[i];

	 for(i = 0; i < xKalman_DALEN; i++)
		Pinit[i] = k.PData[i];
}

int xKalman_Matrices_Init_Com(xKalman_t *k)
{
	/* Initialize Kalman Matrices */
	matrix_init_f32(&k->A, k->nRows, k->nCols, k->AData);
	matrix_init_f32(&k->K, k->nRows, k->nCols, k->KData);
	matrix_init_f32(&k->q, k->nRows, 1, k->qData);
	matrix_init_f32(&k->qPriori, k->nRows, 1, k->qPrioriData);
	matrix_init_f32(&k->P, k->nRows, k->nCols, k->PData); //
	matrix_init_f32(&k->Ppriori, k->nRows, k->nCols, k->PprioriData);
	matrix_init_f32(&k->H, k->nRows, k->nCols, k->HData);
	matrix_init_f32(&k->Ht, k->nRows, k->nCols, k->HtData); // H' is the same
	matrix_init_f32(&k->Q, k->nRows, k->nCols, k->QData);
	matrix_init_f32(&k->R, k->nRows, k->nCols, k->RData);
	matrix_init_f32(&k->Eye, k->nRows, k->nCols, k->EyeData);
	matrix_init_f32(&k->qz, k->nRows, 1, k->qzData);
	matrix_init_f32(&k->w, 3, 1, k->wData);
	matrix_init_f32(&k->omega, k->nRows, k->nCols, k->omegaData);
	
	return 0;
}

int xKalman_Init(xKalman_t *k)
{
	int ret;
	unsigned char i;
	
	/* Initializing to a 4 row, 4 column matrix according to theory. */
	k->nRows = xKalman_DLEN;
	k->nCols = xKalman_DLEN;
	
	/* Set mod. */
	k->wMod = 0;
	k->qMod = 0;
	k->Pmod = 100;

	/* Init all matrices. */
	for(i = 0; i < xKalman_DALEN; i++){
		k->AData[i] = 0;
		k->KData[i] = 0;
		k->HData[i] = 0;
		k->HtData[i] = 0;
		k->QData[i] = 0;
		k->RData[i] = 0;
		k->EyeData[i] = 0;
	}

	/* Load for starting. */
	/************************xKalman_DLEN************************
		HData  = [1 0 0 0
				  0 1 0 0 
				  0 0 1 0 
				  0 0 0 1 ]
		HtData = [1 0 0 0
				  0 1 0 0 
				  0 0 1 0 
				  0 0 0 1 ]
		QData  = [1e-6 0 0 0
				  0 1e-6 0 0 
				  0 0 1e-6 0 
				  0 0 0 1e-6 ]
		RData  = [1e-6 0 0 0
				  0 1e-6 0 0 
				  0 0 1e-6 0 
				  0 0 0 1e-6 ]
		EyeData= [1 0 0 0
				  0 1 0 0 
				  0 0 1 0 
				  0 0 0 1 ]
	************************************************************/
	k->HData[0] = 1;
	k->HData[5] = 1;
	k->HData[10] = 1;
	k->HData[15] = 1;

	k->HtData[0] = 1;
	k->HtData[5] = 1;
	k->HtData[10] = 1;
	k->HtData[15] = 1;

	k->QData[0] = 1e-6;
	k->QData[5] = 1e-6;
	k->QData[10] = 1e-6;
	k->QData[15] = 1e-6;

	k->RData[0] = 5e-5;
	k->RData[5] = 5e-5;
	k->RData[10] = 5e-5;
	k->RData[15] = 5e-5;

	k->EyeData[0] = 1;
	k->EyeData[5] = 1;
	k->EyeData[10] = 1;
	k->EyeData[15] = 1;

	/* Matrices initialize common function. */
	xKalman_Matrices_Init_Com(k);
	
	return 0;
}


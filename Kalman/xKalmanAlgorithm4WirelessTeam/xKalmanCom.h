/**
 * @File    xKalmanCom.h
 * @Date    Tuesday, Dec. 25, 2017 at 14:17:09 PM BJT
 * @Author  Washington Ruan
 * @Email   washingtonxr@live.com
 *
 * This file contains the implementation of the Kalman filter algorithm for sensor device's
 * module's main file.
 *
 * @bug No known bugs.
 **/
#ifndef XKALMANCOM_H
#define XKALMANCOM_H

#include <math.h>
#include <stdio.h>
#include <string.h>

/**
* @brief Instance structure for the floating-point matrix structure.
*/
typedef struct
{
	unsigned short numRows;     /**< number of rows of the matrix.     */
	unsigned short numCols;     /**< number of columns of the matrix.  */
	float *pData;				/**< points to the data of the matrix. */
}com_matrix_instance_f32;

/**
* @brief Error status returned by some functions in the library.
*/
typedef enum
{
  MATH_SUCCESS = 0,              /**< No error */
  MATH_ARGUMENT_ERROR = -1,      /**< One or more arguments are incorrect */
  MATH_LENGTH_ERROR = -2,        /**< Length of data buffer is incorrect */
  MATH_SIZE_MISMATCH = -3,       /**< Size of matrices is not compatible with the operation. */
  MATH_NANINF = -4,              /**< Not-a-number (NaN) or infinity is generated */
  MATH_SINGULAR = -5,            /**< Generated by matrix inversion if the input matrix is singular and cannot be inverted. */
  MATH_TEST_FAILURE = -6         /**< Test Failed  */
}mat_status;

/**   
 * @brief  Floating-point matrix initialization.   
 * @param[in,out] *S             points to an instance of the floating-point matrix structure.   
 * @param[in]     nRows          number of rows in the matrix.   
 * @param[in]     nColumns       number of columns in the matrix.   
 * @param[in]     *pData	   points to the matrix data array.   
 * @return        none   
 */
/* Floating-point matrix initialization. */
void matrix_init_f32(com_matrix_instance_f32 *S, 
						unsigned short nRows,
						unsigned short nColumns, 
						float * pData);
/**   
 * @brief Floating-point matrix scaling.   
 * @param[in]       *pSrc points to input matrix structure   
 * @param[in]       scale scale factor to be applied    
 * @param[out]      *pDst points to output matrix structure   
 * @return     		The function returns either <code>MATH_SIZE_MISMATCH</code>    
 * or <code>MATH_SUCCESS</code> based on the outcome of size checking.   
 *   
 */
mat_status mat_scale_f32(const com_matrix_instance_f32 * pSrc,
							float scale,
							com_matrix_instance_f32 * pDst);
/**   
 * @brief Floating-point matrix addition.   
 * @param[in]       *pSrcA points to the first input matrix structure   
 * @param[in]       *pSrcB points to the second input matrix structure   
 * @param[out]      *pDst points to output matrix structure   
 * @return     		The function returns either   
 * <code>MATH_SIZE_MISMATCH</code> or <code>MATH_SUCCESS</code> based on the outcome of size checking.   
 */
mat_status mat_add_f32(const com_matrix_instance_f32 * pSrcA,
							const com_matrix_instance_f32 * pSrcB,
							com_matrix_instance_f32 * pDst);
/**   
 * @brief Floating-point matrix subtraction   
 * @param[in]       *pSrcA points to the first input matrix structure   
 * @param[in]       *pSrcB points to the second input matrix structure   
 * @param[out]      *pDst points to output matrix structure   
 * @return     		The function returns either   
 * <code>MATH_SIZE_MISMATCH</code> or <code>MATH_SUCCESS</code> based on the outcome of size checking.   
 */
mat_status mat_sub_f32(const com_matrix_instance_f32 * pSrcA,
						const com_matrix_instance_f32 * pSrcB,
						com_matrix_instance_f32 * pDst);
/**   
  * @brief Floating-point matrix transpose.   
  * @param[in]  *pSrc points to the input matrix   
  * @param[out] *pDst points to the output matrix   
  * @return 	The function returns either  <code>MATH_SIZE_MISMATCH</code>   
  * or <code>MATH_SUCCESS</code> based on the outcome of size checking.   
  */
mat_status mat_trans_f32(const com_matrix_instance_f32 * pSrc, 
						com_matrix_instance_f32 * pDst);
/**   
 * @brief Floating-point matrix multiplication.   
 * @param[in]       *pSrcA points to the first input matrix structure   
 * @param[in]       *pSrcB points to the second input matrix structure   
 * @param[out]      *pDst points to output matrix structure   
 * @return     		The function returns either   
 * <code>MATH_SIZE_MISMATCH</code> or <code>MATH_SUCCESS</code> based on the outcome of size checking.   
 */
mat_status mat_mult_f32(const com_matrix_instance_f32 * pSrcA,
						const com_matrix_instance_f32 * pSrcB,
						com_matrix_instance_f32 * pDst);

/**
 * @brief Floating-point matrix inverse.
 * @param[in]       *pSrc points to input matrix structure
 * @param[out]      *pDst points to output matrix structure
 * @return     		The function returns
 * <code>MATH_SIZE_MISMATCH</code> if the input matrix is not square or if the size
 * of the output matrix does not match the size of the input matrix.
 * If the input matrix is found to be singular (non-invertible), then the function returns
 * <code>MATH_SINGULAR</code>.  Otherwise, the function returns <code>MATH_SUCCESS</code>.
 */
mat_status mat_inverse_f32(const com_matrix_instance_f32 * pSrc,
								com_matrix_instance_f32 * pDst);

#endif
/* End of this file. */
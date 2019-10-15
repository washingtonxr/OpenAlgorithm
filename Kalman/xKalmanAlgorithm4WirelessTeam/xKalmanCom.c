/**
 * @File    xKalmanCom.c
 * @Date    Tuesday, Dec. 25, 2017 at 14:17:09 PM BJT
 * @Author  Washington Ruan
 * @Email   washingtonxr@live.com
 *
 * This file contains the implementation of the Kalman filter algorithm for sensor device's
 * module's main file.
 *
 * @bug No known bugs.
 **/
 #include "xKalmanCom.h"
 
/**   
 * @brief  Floating-point matrix initialization.   
 * @param[in,out] *S             points to an instance of the floating-point matrix structure.   
 * @param[in]     nRows          number of rows in the matrix.   
 * @param[in]     nColumns       number of columns in the matrix.   
 * @param[in]     *pData	   points to the matrix data array.   
 * @return        none   
 */
void matrix_init_f32(com_matrix_instance_f32 *S, 
						unsigned short nRows,
						unsigned short nColumns, 
						float * pData)
{
	/* Assign Number of Rows */
	S->numRows = nRows;

	/* Assign Number of Columns */
	S->numCols = nColumns;

	/* Assign Data pointer */
	S->pData = pData;
}

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
						com_matrix_instance_f32 * pDst)
{
  float *pIn = pSrc->pData;                  /* input data matrix pointer */
  float *pOut = pDst->pData;                 /* output data matrix pointer */
  unsigned int numSamples;                           /* total number of elements in the matrix */
  unsigned int blkCnt;                               /* loop counters */
  mat_status status;                             /* status of matrix scaling     */

#ifdef MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if((pSrc->numRows != pDst->numRows) || (pSrc->numCols != pDst->numCols))
  {
    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef MATH_MATRIX_CHECK    */

  {
    /* Total number of samples in the input matrix */
    numSamples = (unsigned int) pSrc->numRows * pSrc->numCols;

#ifndef MATH_CM0

    /* Run the below code for Cortex-M4 and Cortex-M3 */

    /* Loop Unrolling */
    blkCnt = numSamples >> 2;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.   
     ** a second loop below computes the remaining 1 to 3 samples. */
    while(blkCnt > 0u)
    {
      /* C(m,n) = A(m,n) * scale */
      /* Scaling and results are stored in the destination buffer. */
      *pOut++ = (*pIn++) * scale;
      *pOut++ = (*pIn++) * scale;
      *pOut++ = (*pIn++) * scale;
      *pOut++ = (*pIn++) * scale;

      /* Decrement the numSamples loop counter */
      blkCnt--;
    }

    /* If the numSamples is not a multiple of 4, compute any remaining output samples here.   
     ** No loop unrolling is used. */
    blkCnt = numSamples % 0x4u;

#else

    /* Run the below code for Cortex-M0 */

    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;

#endif /* #ifndef MATH_CM0 */

    while(blkCnt > 0u)
    {
      /* C(m,n) = A(m,n) * scale */
      /* The results are stored in the destination buffer. */
      *pOut++ = (*pIn++) * scale;

      /* Decrement the loop counter */
      blkCnt--;
    }
    /* Set status as MATH_SUCCESS */
    status = MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

/**   
 * @brief Floating-point matrix subtraction   
 * @param[in]       *pSrcA points to the first input matrix structure   
 * @param[in]       *pSrcB points to the second input matrix structure   
 * @param[out]      *pDst points to output matrix structure   
 * @return     		The function returns either   
 * <code>MATH_SIZE_MISMATCH</code> or <code>MATH_SUCCESS</code> based on the outcome of size checking.   
 */
#define MATH_MATRIX_CHECK
mat_status mat_sub_f32(const com_matrix_instance_f32 * pSrcA,
						const com_matrix_instance_f32 * pSrcB,
						com_matrix_instance_f32 * pDst)
{
  float *pIn1 = pSrcA->pData;                /* input data matrix pointer A */
  float *pIn2 = pSrcB->pData;                /* input data matrix pointer B */
  float *pOut = pDst->pData;                 /* output data matrix pointer  */
  unsigned int numSamples;                           /* total number of elements in the matrix  */
  unsigned int blkCnt;                               /* loop counters */
  mat_status status;                             /* status of matrix subtraction */

#ifdef MATH_MATRIX_CHECK


  /* Check for matrix mismatch condition */
  if((pSrcA->numRows != pSrcB->numRows) ||
     (pSrcA->numCols != pSrcB->numCols) ||
     (pSrcA->numRows != pDst->numRows) || (pSrcA->numCols != pDst->numCols))
  {
    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef MATH_MATRIX_CHECK    */

  {
    /* Total number of samples in the input matrix */
    numSamples = (unsigned int) pSrcA->numRows * pSrcA->numCols;

#ifndef MATH_CM0

    /* Run the below code for Cortex-M4 and Cortex-M3 */

    /* Loop Unrolling */
    blkCnt = numSamples >> 2u;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.   
     ** a second loop below computes the remaining 1 to 3 samples. */
    while(blkCnt > 0u)
    {
      /* C(m,n) = A(m,n) - B(m,n) */
      /* Subtract and then store the results in the destination buffer. */
      *pOut++ = (*pIn1++) - (*pIn2++);
      *pOut++ = (*pIn1++) - (*pIn2++);
      *pOut++ = (*pIn1++) - (*pIn2++);
      *pOut++ = (*pIn1++) - (*pIn2++);

      /* Decrement the loop counter */
      blkCnt--;
    }

    /* If the numSamples is not a multiple of 4, compute any remaining output samples here.   
     ** No loop unrolling is used. */
    blkCnt = numSamples % 0x4u;

#else

    /* Run the below code for Cortex-M0 */

    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;

#endif /* #ifndef MATH_CM0 */

    while(blkCnt > 0u)
    {
      /* C(m,n) = A(m,n) - B(m,n) */
      /* Subtract and then store the results in the destination buffer. */
      *pOut++ = (*pIn1++) - (*pIn2++);

      /* Decrement the loop counter */
      blkCnt--;
    }
    /* Set status as MATH_SUCCESS */
    status = MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}
#undef MATH_MATRIX_CHECK

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
							com_matrix_instance_f32 * pDst)
{
  float *pIn1 = pSrcA->pData;                /* input data matrix pointer A  */
  float *pIn2 = pSrcB->pData;                /* input data matrix pointer B  */
  float *pOut = pDst->pData;                 /* output data matrix pointer   */
  unsigned int numSamples;                           /* total number of elements in the matrix  */
  unsigned int blkCnt;                               /* loop counters */
  mat_status status;                             /* status of matrix addition */

#ifdef MATH_MATRIX_CHECK


  /* Check for matrix mismatch condition */
  if((pSrcA->numRows != pSrcB->numRows) ||
     (pSrcA->numCols != pSrcB->numCols) ||
     (pSrcA->numRows != pDst->numRows) || (pSrcA->numCols != pDst->numCols))
  {
    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef MATH_MATRIX_CHECK    */

  {

    /* Total number of samples in the input matrix */
    numSamples = (unsigned int) pSrcA->numRows * pSrcA->numCols;

#ifndef MATH_CM0

    /* Run the below code for Cortex-M4 and Cortex-M3 */

    /* Loop unrolling */
    blkCnt = numSamples >> 2u;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.   
     ** a second loop below computes the remaining 1 to 3 samples. */
    while(blkCnt > 0u)
    {
      /* C(m,n) = A(m,n) + B(m,n) */
      /* Add and then store the results in the destination buffer. */
      *pOut++ = (*pIn1++) + (*pIn2++);
      *pOut++ = (*pIn1++) + (*pIn2++);
      *pOut++ = (*pIn1++) + (*pIn2++);
      *pOut++ = (*pIn1++) + (*pIn2++);

      /* Decrement the loop counter */
      blkCnt--;
    }

    /* If the numSamples is not a multiple of 4, compute any remaining output samples here.   
     ** No loop unrolling is used. */
    blkCnt = numSamples % 0x4u;

#else

    /* Run the below code for Cortex-M0 */

    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;

#endif /* #ifndef MATH_CM0 */

    while(blkCnt > 0u)
    {
      /* C(m,n) = A(m,n) + B(m,n) */
      /* Add and then store the results in the destination buffer. */
      *pOut++ = (*pIn1++) + (*pIn2++);

      /* Decrement the loop counter */
      blkCnt--;
    }
    /* set status as MATH_SUCCESS */
    status = MATH_SUCCESS;

  }

  /* Return to application */
  return (status);
}

/**   
  * @brief Floating-point matrix transpose.   
  * @param[in]  *pSrc points to the input matrix   
  * @param[out] *pDst points to the output matrix   
  * @return 	The function returns either  <code>MATH_SIZE_MISMATCH</code>   
  * or <code>MATH_SUCCESS</code> based on the outcome of size checking.   
  */
mat_status mat_trans_f32(const com_matrix_instance_f32 * pSrc, com_matrix_instance_f32 * pDst)
{
  float *pIn = pSrc->pData;                  /* input data matrix pointer */
  float *pOut = pDst->pData;                 /* output data matrix pointer */
  float *px;                                 /* Temporary output data matrix pointer */
  uint16_t nRows = pSrc->numRows;                /* number of rows */
  uint16_t nColumns = pSrc->numCols;             /* number of columns */

#ifndef MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */

  uint16_t blkCnt, i = 0u, row = nRows;          /* loop counters */
  mat_status status;                             /* status of matrix transpose  */

#ifdef MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
  {
    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef MATH_MATRIX_CHECK    */

  {
    /* Matrix transpose by exchanging the rows with columns */
    /* row loop     */
    do
    {
      /* Loop Unrolling */
      blkCnt = nColumns >> 2;

      /* The pointer px is set to starting address of the column being processed */
      px = pOut + i;

      /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.   
       ** a second loop below computes the remaining 1 to 3 samples. */
      while(blkCnt > 0u)        /* column loop */
      {
        /* Read and store the input element in the destination */
        *px = *pIn++;

        /* Update the pointer px to point to the next row of the transposed matrix */
        px += nRows;

        /* Read and store the input element in the destination */
        *px = *pIn++;

        /* Update the pointer px to point to the next row of the transposed matrix */
        px += nRows;

        /* Read and store the input element in the destination */
        *px = *pIn++;

        /* Update the pointer px to point to the next row of the transposed matrix */
        px += nRows;

        /* Read and store the input element in the destination */
        *px = *pIn++;

        /* Update the pointer px to point to the next row of the transposed matrix */
        px += nRows;

        /* Decrement the column loop counter */
        blkCnt--;
      }

      /* Perform matrix transpose for last 3 samples here. */
      blkCnt = nColumns % 0x4u;

      while(blkCnt > 0u)
      {
        /* Read and store the input element in the destination */
        *px = *pIn++;

        /* Update the pointer px to point to the next row of the transposed matrix */
        px += nRows;

        /* Decrement the column loop counter */
        blkCnt--;
      }

#else

  /* Run the below code for Cortex-M0 */

  uint16_t col, i = 0u, row = nRows;             /* loop counters */
  mat_status status;                             /* status of matrix transpose  */


#ifdef MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
  {
    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*      #ifdef MATH_MATRIX_CHECK    */

  {
    /* Matrix transpose by exchanging the rows with columns */
    /* row loop     */
    do
    {
      /* The pointer px is set to starting address of the column being processed */
      px = pOut + i;

      /* Initialize column loop counter */
      col = nColumns;

      while(col > 0u)
      {
        /* Read and store the input element in the destination */
        *px = *pIn++;

        /* Update the pointer px to point to the next row of the transposed matrix */
        px += nRows;

        /* Decrement the column loop counter */
        col--;
      }

#endif /* #ifndef MATH_CM0 */

      i++;

      /* Decrement the row loop counter */
      row--;

    } while(row > 0u);          /* row loop end  */

    /* Set status as MATH_SUCCESS */
    status = MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}
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
						com_matrix_instance_f32 * pDst)
{
  float *pIn1 = pSrcA->pData;                /* input data matrix pointer A */
  float *pIn2 = pSrcB->pData;                /* input data matrix pointer B */
  float *pInA = pSrcA->pData;                /* input data matrix pointer A  */
  float *pOut = pDst->pData;                 /* output data matrix pointer */
  float *px;                                 /* Temporary output data matrix pointer */
  float sum;                                 /* Accumulator */
  uint16_t numRowsA = pSrcA->numRows;            /* number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* number of columns of input matrix A */

#ifndef MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */

  uint16_t col, i = 0u, j, row = numRowsA, colCnt;      /* loop counters */
  mat_status status;                             /* status of matrix multiplication */

#ifdef MATH_MATRIX_CHECK


  /* Check for matrix mismatch condition */
  if((pSrcA->numCols != pSrcB->numRows) ||
     (pSrcA->numRows != pDst->numRows) || (pSrcB->numCols != pDst->numCols))
  {

    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*      #ifdef MATH_MATRIX_CHECK    */

  {
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of the row being processed */
      px = pOut + i;

      /* For every row wise process, the column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, the pIn2 pointer is set   
       ** to the starting address of the pSrcB data */
      pIn2 = pSrcB->pData;

      j = 0u;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0.0f;

        /* Initiate the pointer pIn1 to point to the starting address of the column being processed */
        pIn1 = pInA;

        /* Apply loop unrolling and compute 4 MACs simultaneously. */
        colCnt = numColsA >> 2;

        /* matrix multiplication        */
        while(colCnt > 0u)
        {
          /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
          sum += *pIn1++ * (*pIn2);
          pIn2 += numColsB;
          sum += *pIn1++ * (*pIn2);
          pIn2 += numColsB;
          sum += *pIn1++ * (*pIn2);
          pIn2 += numColsB;
          sum += *pIn1++ * (*pIn2);
          pIn2 += numColsB;

          /* Decrement the loop count */
          colCnt--;
        }

        /* If the columns of pSrcA is not a multiple of 4, compute any remaining MACs here.   
         ** No loop unrolling is used. */
        colCnt = numColsA % 0x4u;

        while(colCnt > 0u)
        {
          /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
          sum += *pIn1++ * (*pIn2);
          pIn2 += numColsB;

          /* Decrement the loop counter */
          colCnt--;
        }

        /* Store the result in the destination buffer */
        *px++ = sum;

        /* Update the pointer pIn2 to point to the  starting address of the next column */
        j++;
        pIn2 = pSrcB->pData + j;

        /* Decrement the column loop counter */
        col--;

      } while(col > 0u);

#else

  /* Run the below code for Cortex-M0 */

  float *pInB = pSrcB->pData;                /* input data matrix pointer B */
  uint16_t col, i = 0u, row = numRowsA, colCnt;  /* loop counters */
  mat_status status;                             /* status of matrix multiplication */

#ifdef MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if((pSrcA->numCols != pSrcB->numRows) ||
     (pSrcA->numRows != pDst->numRows) || (pSrcB->numCols != pDst->numCols))
  {

    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*      #ifdef MATH_MATRIX_CHECK    */

  {
    /* The following loop performs the dot-product of each row in pInA with each column in pInB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of the row being processed */
      px = pOut + i;

      /* For every row wise process, the column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, the pIn2 pointer is set    
       ** to the starting address of the pSrcB data */
      pIn2 = pSrcB->pData;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0.0f;

        /* Initialize the pointer pIn1 to point to the starting address of the row being processed */
        pIn1 = pInA;

        /* Matrix A columns number of MAC operations are to be performed */
        colCnt = numColsA;

        while(colCnt > 0u)
        {
          /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
          sum += *pIn1++ * (*pIn2);
          pIn2 += numColsB;

          /* Decrement the loop counter */
          colCnt--;
        }

        /* Store the result in the destination buffer */
        *px++ = sum;

        /* Decrement the column loop counter */
        col--;

        /* Update the pointer pIn2 to point to the  starting address of the next column */
        pIn2 = pInB + (numColsB - col);

      } while(col > 0u);

#endif /* #ifndef MATH_CM0 */

      /* Update the pointer pInA to point to the  starting address of the next row */
      i = i + numColsB;
      pInA = pInA + numColsA;

      /* Decrement the row loop counter */
      row--;

    } while(row > 0u);
    /* Set status as MATH_SUCCESS */
    status = MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}
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
								com_matrix_instance_f32 * pDst)
{
  float *pIn = pSrc->pData;                  /* input data matrix pointer */
  float *pOut = pDst->pData;                 /* output data matrix pointer */
  float *pInT1, *pInT2;                      /* Temporary input data matrix pointer */
  float *pInT3, *pInT4;                      /* Temporary output data matrix pointer */
  float *pPivotRowIn, *pPRT_in, *pPivotRowDst, *pPRT_pDst;  /* Temporary input and output data matrix pointer */
  unsigned int numRows = pSrc->numRows;              /* Number of rows in the matrix  */
  unsigned int numCols = pSrc->numCols;              /* Number of Cols in the matrix  */

#ifndef MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */

  float Xchg, in = 0.0f, in1;                /* Temporary input values  */
  unsigned int i, rowCnt, flag = 0u, j, loopCnt, k, l;      /* loop counters */
  mat_status status;                             /* status of matrix inverse */

#ifdef MATH_MATRIX_CHECK


  /* Check for matrix mismatch condition */
  if((pSrc->numRows != pSrc->numCols) || (pDst->numRows != pDst->numCols)
     || (pSrc->numRows != pDst->numRows))
  {
    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef MATH_MATRIX_CHECK    */

  {
    /* Working pointer for destination matrix */
    pInT2 = pOut;

    /* Loop over the number of rows */
    rowCnt = numRows;

    /* Making the destination matrix as identity matrix */
    while(rowCnt > 0u)
    {
      /* Writing all zeroes in lower triangle of the destination matrix */
      j = numRows - rowCnt;
      while(j > 0u)
      {
        *pInT2++ = 0.0f;
        j--;
      }

      /* Writing all ones in the diagonal of the destination matrix */
      *pInT2++ = 1.0f;

      /* Writing all zeroes in upper triangle of the destination matrix */
      j = rowCnt - 1u;
      while(j > 0u)
      {
        *pInT2++ = 0.0f;
        j--;
      }

      /* Decrement the loop counter */
      rowCnt--;
    }

    /* Loop over the number of columns of the input matrix.
       All the elements in each column are processed by the row operations */
    loopCnt = numCols;

    /* Index modifier to navigate through the columns */
    l = 0u;

    while(loopCnt > 0u)
    {
      /* Check if the pivot element is zero..
       * If it is zero then interchange the row with non zero row below.
       * If there is no non zero element to replace in the rows below,
       * then the matrix is Singular. */

      /* Working pointer for the input matrix that points
       * to the pivot element of the particular row  */
      pInT1 = pIn + (l * numCols);

      /* Working pointer for the destination matrix that points
       * to the pivot element of the particular row  */
      pInT3 = pOut + (l * numCols);

      /* Temporary variable to hold the pivot value */
      in = *pInT1;

      /* Destination pointer modifier */
      k = 1u;

      /* Check if the pivot element is zero */
      if(*pInT1 == 0.0f)
      {
        /* Loop over the number rows present below */
        i = numRows - (l + 1u);

        while(i > 0u)
        {
          /* Update the input and destination pointers */
          pInT2 = pInT1 + (numCols * l);
          pInT4 = pInT3 + (numCols * k);

          /* Check if there is a non zero pivot element to
           * replace in the rows below */
          if(*pInT2 != 0.0f)
          {
            /* Loop over number of columns
             * to the right of the pilot element */
            j = numCols - l;

            while(j > 0u)
            {
              /* Exchange the row elements of the input matrix */
              Xchg = *pInT2;
              *pInT2++ = *pInT1;
              *pInT1++ = Xchg;

              /* Decrement the loop counter */
              j--;
            }

            /* Loop over number of columns of the destination matrix */
            j = numCols;

            while(j > 0u)
            {
              /* Exchange the row elements of the destination matrix */
              Xchg = *pInT4;
              *pInT4++ = *pInT3;
              *pInT3++ = Xchg;

              /* Decrement the loop counter */
              j--;
            }

            /* Flag to indicate whether exchange is done or not */
            flag = 1u;

            /* Break after exchange is done */
            break;
          }

          /* Update the destination pointer modifier */
          k++;

          /* Decrement the loop counter */
          i--;
        }
      }

      /* Update the status if the matrix is singular */
      if((flag != 1u) && (in == 0.0f))
      {
        status = MATH_SINGULAR;

        break;
      }

      /* Points to the pivot row of input and destination matrices */
      pPivotRowIn = pIn + (l * numCols);
      pPivotRowDst = pOut + (l * numCols);

      /* Temporary pointers to the pivot row pointers */
      pInT1 = pPivotRowIn;
      pInT2 = pPivotRowDst;

      /* Pivot element of the row */
      in = *(pIn + (l * numCols));

      /* Loop over number of columns
       * to the right of the pilot element */
      j = (numCols - l);

      while(j > 0u)
      {
        /* Divide each element of the row of the input matrix
         * by the pivot element */
        in1 = *pInT1;
        *pInT1++ = in1 / in;

        /* Decrement the loop counter */
        j--;
      }

      /* Loop over number of columns of the destination matrix */
      j = numCols;

      while(j > 0u)
      {
        /* Divide each element of the row of the destination matrix
         * by the pivot element */
        in1 = *pInT2;
        *pInT2++ = in1 / in;

        /* Decrement the loop counter */
        j--;
      }

      /* Replace the rows with the sum of that row and a multiple of row i
       * so that each new element in column i above row i is zero.*/

      /* Temporary pointers for input and destination matrices */
      pInT1 = pIn;
      pInT2 = pOut;

      /* index used to check for pivot element */
      i = 0u;

      /* Loop over number of rows */
      /*  to be replaced by the sum of that row and a multiple of row i */
      k = numRows;

      while(k > 0u)
      {
        /* Check for the pivot element */
        if(i == l)
        {
          /* If the processing element is the pivot element,
             only the columns to the right are to be processed */
          pInT1 += numCols - l;

          pInT2 += numCols;
        }
        else
        {
          /* Element of the reference row */
          in = *pInT1;

          /* Working pointers for input and destination pivot rows */
          pPRT_in = pPivotRowIn;
          pPRT_pDst = pPivotRowDst;

          /* Loop over the number of columns to the right of the pivot element,
             to replace the elements in the input matrix */
          j = (numCols - l);

          while(j > 0u)
          {
            /* Replace the element by the sum of that row
               and a multiple of the reference row  */
            in1 = *pInT1;
            *pInT1++ = in1 - (in * *pPRT_in++);

            /* Decrement the loop counter */
            j--;
          }

          /* Loop over the number of columns to
             replace the elements in the destination matrix */
          j = numCols;

          while(j > 0u)
          {
            /* Replace the element by the sum of that row
               and a multiple of the reference row  */
            in1 = *pInT2;
            *pInT2++ = in1 - (in * *pPRT_pDst++);

            /* Decrement the loop counter */
            j--;
          }

        }

        /* Increment the temporary input pointer */
        pInT1 = pInT1 + l;

        /* Decrement the loop counter */
        k--;

        /* Increment the pivot index */
        i++;
      }

      /* Increment the input pointer */
      pIn++;

      /* Decrement the loop counter */
      loopCnt--;

      /* Increment the index modifier */
      l++;
    }


#else

  /* Run the below code for Cortex-M0 */

  float Xchg, in = 0.0f;                     /* Temporary input values  */
  unsigned int i, rowCnt, flag = 0u, j, loopCnt, k, l;      /* loop counters */
  mat_status status;                             /* status of matrix inverse */

#ifdef MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if((pSrc->numRows != pSrc->numCols) || (pDst->numRows != pDst->numCols)
     || (pSrc->numRows != pDst->numRows))
  {
    /* Set status as MATH_SIZE_MISMATCH */
    status = MATH_SIZE_MISMATCH;
  }
  else
#endif /*      #ifdef MATH_MATRIX_CHECK    */
  {
    /* Working pointer for destination matrix */
    pInT2 = pOut;

    /* Loop over the number of rows */
    rowCnt = numRows;

    /* Making the destination matrix as identity matrix */
    while(rowCnt > 0u)
    {
      /* Writing all zeroes in lower triangle of the destination matrix */
      j = numRows - rowCnt;
      while(j > 0u)
      {
        *pInT2++ = 0.0f;
        j--;
      }

      /* Writing all ones in the diagonal of the destination matrix */
      *pInT2++ = 1.0f;

      /* Writing all zeroes in upper triangle of the destination matrix */
      j = rowCnt - 1u;
      while(j > 0u)
      {
        *pInT2++ = 0.0f;
        j--;
      }

      /* Decrement the loop counter */
      rowCnt--;
    }

    /* Loop over the number of columns of the input matrix.
       All the elements in each column are processed by the row operations */
    loopCnt = numCols;

    /* Index modifier to navigate through the columns */
    l = 0u;
    //for(loopCnt = 0u; loopCnt < numCols; loopCnt++)
    while(loopCnt > 0u)
    {
      /* Check if the pivot element is zero..
       * If it is zero then interchange the row with non zero row below.
       * If there is no non zero element to replace in the rows below,
       * then the matrix is Singular. */

      /* Working pointer for the input matrix that points
       * to the pivot element of the particular row  */
      pInT1 = pIn + (l * numCols);

      /* Working pointer for the destination matrix that points
       * to the pivot element of the particular row  */
      pInT3 = pOut + (l * numCols);

      /* Temporary variable to hold the pivot value */
      in = *pInT1;

      /* Destination pointer modifier */
      k = 1u;

      /* Check if the pivot element is zero */
      if(*pInT1 == 0.0f)
      {
        /* Loop over the number rows present below */
        for (i = (l + 1u); i < numRows; i++)
        {
          /* Update the input and destination pointers */
          pInT2 = pInT1 + (numCols * l);
          pInT4 = pInT3 + (numCols * k);

          /* Check if there is a non zero pivot element to
           * replace in the rows below */
          if(*pInT2 != 0.0f)
          {
            /* Loop over number of columns
             * to the right of the pilot element */
            for (j = 0u; j < (numCols - l); j++)
            {
              /* Exchange the row elements of the input matrix */
              Xchg = *pInT2;
              *pInT2++ = *pInT1;
              *pInT1++ = Xchg;
            }

            for (j = 0u; j < numCols; j++)
            {
              Xchg = *pInT4;
              *pInT4++ = *pInT3;
              *pInT3++ = Xchg;
            }

            /* Flag to indicate whether exchange is done or not */
            flag = 1u;

            /* Break after exchange is done */
            break;
          }

          /* Update the destination pointer modifier */
          k++;
        }
      }

      /* Update the status if the matrix is singular */
      if((flag != 1u) && (in == 0.0f))
      {
        status = MATH_SINGULAR;

        break;
      }

      /* Points to the pivot row of input and destination matrices */
      pPivotRowIn = pIn + (l * numCols);
      pPivotRowDst = pOut + (l * numCols);

      /* Temporary pointers to the pivot row pointers */
      pInT1 = pPivotRowIn;
      pInT2 = pPivotRowDst;

      /* Pivot element of the row */
      in = *(pIn + (l * numCols));

      /* Loop over number of columns
       * to the right of the pilot element */
      for (j = 0u; j < (numCols - l); j++)
      {
        /* Divide each element of the row of the input matrix
         * by the pivot element */
        *pInT1++ = *pInT1 / in;
      }
      for (j = 0u; j < numCols; j++)
      {
        /* Divide each element of the row of the destination matrix
         * by the pivot element */
        *pInT2++ = *pInT2 / in;
      }

      /* Replace the rows with the sum of that row and a multiple of row i
       * so that each new element in column i above row i is zero.*/

      /* Temporary pointers for input and destination matrices */
      pInT1 = pIn;
      pInT2 = pOut;

      for (i = 0u; i < numRows; i++)
      {
        /* Check for the pivot element */
        if(i == l)
        {
          /* If the processing element is the pivot element,
             only the columns to the right are to be processed */
          pInT1 += numCols - l;
          pInT2 += numCols;
        }
        else
        {
          /* Element of the reference row */
          in = *pInT1;

          /* Working pointers for input and destination pivot rows */
          pPRT_in = pPivotRowIn;
          pPRT_pDst = pPivotRowDst;

          /* Loop over the number of columns to the right of the pivot element,
             to replace the elements in the input matrix */
          for (j = 0u; j < (numCols - l); j++)
          {
            /* Replace the element by the sum of that row
               and a multiple of the reference row  */
            *pInT1++ = *pInT1 - (in * *pPRT_in++);
          }
          /* Loop over the number of columns to
             replace the elements in the destination matrix */
          for (j = 0u; j < numCols; j++)
          {
            /* Replace the element by the sum of that row
               and a multiple of the reference row  */
            *pInT2++ = *pInT2 - (in * *pPRT_pDst++);
          }

        }
        /* Increment the temporary input pointer */
        pInT1 = pInT1 + l;
      }
      /* Increment the input pointer */
      pIn++;

      /* Decrement the loop counter */
      loopCnt--;
      /* Increment the index modifier */
      l++;
    }


#endif /* #ifndef MATH_CM0 */

    /* Set status as MATH_SUCCESS */
    status = MATH_SUCCESS;

    if((flag != 1u) && (in == 0.0f))
    {
      status = MATH_SINGULAR;
    }
  }
  /* Return to application */
  return (status);
}

/* End of this file. */
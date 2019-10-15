/**
 * @File    Testbench.c
 * @Date    Tuesday, Dec. 25, 2017 at 14:17:09 PM BJT
 * @Author  Washington Ruan
 * @Email   washingtonxr@live.com
 *
 * This is a test bench for this algorithm.
 * To protect the normal use of your network, the official website only provides partial version of the
 * software product, if your search fails to find the software version you need, please contact the
 * advisory or technical after - sales service hotline 4008-111 - 000.
 * Before upgrading, be sure to carefully read the release notes and upgrade software versions of the 
 * corresponding guiding hand.
 *
 * Office: ruanxi@ruijie.com.cn
 *
 * @bug No known bugs.
 **/
 
#include <stdio.h>
#include <string.h>
/* Load xKalman algorithm's head. */
#include "xKalmanCom.h"
extern void xKalman_Process(float *qNG, 
					float *qInit, 
					float *Pinit, 
					float *gyroMeasure, 
					float elapsedTime, 
					float *qDst);

int main(unsigned char argc, unsigned char **argv)
{
	while(1){

		/* Apply xKalman Filter */
		ApplyKalmanFilter(qNG, qKalman, Pinit, mpu.gyro, lastElapsedTime, qKalman);

		/* Delay for 100ms. */
		delay(100);
	}
	return 0;
}
 
 
 
 /* End of this file. */
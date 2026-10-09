/*
	Filename: fir.cpp
		FIR lab wirtten for WES/CSE237C class at UCSD.
		Match filter
	INPUT:
		x: signal (chirp)

	OUTPUT:
		y: filtered output

*/

#include "fir.h"
#include "ap_int.h"

void fir (
  data_t *y,
  data_t x
  )
{
        //ap_int<5>
	ap_int<5> c[N] = {10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
	#pragma HLS ARRAY_PARTITION variable = c dim = 1 block factor = 32
	// Write your code here
        static
                // data_t 
                ap_int<8> shift_reg[N];
		// acc_t acc;
                ap_int<16> acc;
                int i;

        acc = 0;
        TDL:
        for (i = N - 1; i > 0; i--){
                #pragma HLS PIPELINE II=1
                #pragma HLS unroll factor = 10
                // if (i == 0) {
                //         acc += x * c[0];
                //         shift_reg[0] = x;
                // } else {
                //         shift_reg[i] = shift_reg[i - 1];
                //         acc += shift_reg[i] * c[i];
                // }
                shift_reg[i] = shift_reg[i - 1]; // loop partition
                // shift_reg[i - 1] = shift_reg[i - 2];
                // shift_reg[i - 2] = shift_reg[i - 3];
                // shift_reg[i - 3] = shift_reg[i - 4];
                // shift_reg[i - 4] = shift_reg[i - 5]; // loop partition
                // shift_reg[i - 5] = shift_reg[i - 6];
                // shift_reg[i - 6] = shift_reg[i - 7];
                // shift_reg[i - 7] = shift_reg[i - 8];
        }
        // for (; i >= 0; i--){
        //         shift_reg[i] = shift_reg[i - 1];
        // }

        MAC:
        for (i = N - 1; i > 0; i--){     
                #pragma HLS PIPELINE II=1
                #pragma HLS unroll factor = 10           
                acc += shift_reg[i] * c[i];
                //  + shift_reg[i-1] * c[i-1] + shift_reg[i-2] * c[i-2] + shift_reg[i-3] * c[i-3]
                // + shift_reg[i-4] * c[i-4] + shift_reg[i-5] * c[i-5] + shift_reg[i-6] * c[i-6] + shift_reg[i-7] * c[i-7];
                // shift_reg[i] = shift_reg[i - 1]; // hoist
                // acc += shift_reg[i] * c[i];
        }
        // for (; i >= 0; i--) {
	//         acc += shift_reg[i] * c[i];
        // }

        acc += x * c[0]; // hoist
        shift_reg[0] = x;

        *y = acc;
}


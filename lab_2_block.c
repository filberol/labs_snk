#include "fir.h"

void fir (
  data_t *y,
  coef_t c[10],
  data_t x
  ) {

#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=y
#pragma HLS INTERFACE s_axilite port=c
#pragma HLS INTERFACE s_axilite port=x


  static data_t shift_reg[10];
  acc_t acc;
  data_t data;
  int i;
  
  acc=0;

  Shift_Accum_Loop: for (i=9;i>=0;i--)
{
#pragma HLS PIPELINE
	if (i==0) {
			shift_reg[0]=x;
     	data = x;
    } else {
			shift_reg[i]=shift_reg[i-1];
			data = shift_reg[i];
    }
    acc+=data*c[i];;       
  }
  *y=acc;
}

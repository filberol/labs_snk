#include <stdio.h>
#include "platform.h"
#include <xil_io.h>
#include <microblaze_sleep.h>

int read_int() {
    int num = 0;
    char ch = getchar();
    while (ch >= '0' && ch <= '9') {
        num = num * 10 + (ch - '0');
        putc(ch, stdout);
        ch = getchar();
    }
    print("\n");
    return num;
}

void put_int(int num) {
    char buffer[10];
    int i = 0;
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (i--) {
        putc(buffer[i], stdout);
    }
    print("\n");
}


int main()
{
    init_platform();
    print("Enter p (sliding window): ");
    int p = read_int();
    print("Enter n (number count): ");
    int n = read_int();

    int buffer[100];
    int mid_buffer[100];

    Xil_Out16(0x40000000, 0xAAAA);
    print("Write numbers:\n");
    // Read numbers and calculate sliding window
    for(int i = 0; i < n; i++) {
    	buffer[i] = read_int();
    	if (i > p - 2) {
    		// Не сохранился файл к сожалению( Вот тут нужно использовать встроенный блок
//    		int sum = 0;
//    		for (int j = 0; j < p; j++) {
//    			sum = sum + buffer[i - j];
//    		}
//    		int med = sum / p;
//    		put_int(med);
//    		mid_buffer[i - p + 1] = med;
    	}
    }
    //Output numbers
    for (int k = 0; k < n - p + 1; k++) {
    	Xil_Out16(0x40000000, mid_buffer[k]);
    	MB_Sleep(1000);
    }
    Xil_Out16(0x40000000, 0xAAAA);


    cleanup_platform();
    return 0;
}

// Base address of the FIR block (from your register map image)
#define FIR_BASE_ADDR    0x44A10000
#define FIR_CTRL         (FIR_BASE_ADDR + 0x00)
#define FIR_GIER         (FIR_BASE_ADDR + 0x04)
#define FIR_IP_IER       (FIR_BASE_ADDR + 0x08)
#define FIR_IP_ISR       (FIR_BASE_ADDR + 0x0C)
#define FIR_Y_ADDR       (FIR_BASE_ADDR + 0x10)
#define FIR_MEMORY_C     (FIR_BASE_ADDR + 0x40)
#define FIR_X_ADDR       (FIR_BASE_ADDR + 0x80)

#define TIMER_BASE_ADDR  XPAR_AXI_TIMER_0_BASEADDR
#define TIMER_COUNTER_0  0

#define WRITE_REG(addr, data)   Xil_Out32((addr), (data))
#define READ_REG(addr)          Xil_In32((addr))

void fir(
  int* y,
  int c[10],
  int x
  ) {

  static int shift_reg[10];
  int acc;
  int data;
  int i;

  acc=0;

  Shift_Accum_Loop: for (i=9;i>=0;i--)
{
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
  return;
}

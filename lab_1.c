#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "microblaze_sleep.h"

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
    		int sum = 0;
    		for (int j = 0; j < p; j++) {
    			sum = sum + buffer[i - j];
    		}
    		int med = sum / p;
    		put_int(med);
    		mid_buffer[i - p + 1] = med;
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

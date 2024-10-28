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

    int buffer[100] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int mid_buffer[1];

    // Read numbers and calculate sliding window
    for(int i = 3; i < 11; i++) {
    	Xil_Out16(0x40000000, 0xFFFF);
		int sum = 0;
		for (int j = 0; j < i; j++) {
			sum = sum + buffer[j];
		}
		int med = sum / i;
		mid_buffer[0] = med;
		Xil_Out16(0x40000000, 0xAAAA);
    }

    cleanup_platform();
    return 0;
}

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "microblaze_sleep.h"

#include <stdio.h>
#include <stdint.h>
#include "xparameters.h" // For base address definitions
#include "xil_io.h"      // For Xil_In32 and Xil_Out32

// Base address of the FIR IP block
#define FIR_BASE_ADDR 0x44A10000

// Register offsets
#define CTRL_OFFSET         0x00
#define GIER_OFFSET         0x04
#define IP_IER_OFFSET       0x08
#define IP_ISR_OFFSET       0x0C
#define Y_OFFSET            0x10
#define Y_CTRL_OFFSET       0x14
#define MEMORY_C_OFFSET     0x40
#define X_OFFSET            0x80

// Control signals in the CTRL register
#define AP_START_MASK       0x01
#define AP_DONE_MASK        0x02
#define AP_IDLE_MASK        0x04
#define AP_READY_MASK       0x08

void write_register(uint32_t offset, uint32_t value) {
    Xil_Out32(FIR_BASE_ADDR + offset, value);
}

uint32_t read_register(uint32_t offset) {
    return Xil_In32(FIR_BASE_ADDR + offset);
}

void fir_ip_process(uint32_t input_data, uint32_t coefficient) {
    write_register(X_OFFSET, input_data);
    write_register(MEMORY_C_OFFSET, coefficient);
    write_register(CTRL_OFFSET, AP_START_MASK);
    while (!(read_register(CTRL_OFFSET) & AP_DONE_MASK)) {}

    uint32_t result = read_register(Y_OFFSET);

    // Clear AP_DONE by writing back to CTRL register
    write_register(CTRL_OFFSET, 0x0);
}


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

    int buffer[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int coeffs[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int mid_buffer;

    Xil_Out16(0x40000000, 0xFFFF);

	int sum = 0;
	for (int j = 0; j < 10; j++) {
		sum = sum + buffer[j];
	}
	int med = sum / 10;
	mid_buffer = med;

	Xil_Out16(0x40000000, 0xAAAA);

	fir_ip_process(0x1234, 0x1);

    cleanup_platform();
    return 0;
}

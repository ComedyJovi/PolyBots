#include "drivers/mss_uart/mss_uart.h"
#include "inttypes.h"

#define CONTROLLER_DATA_REG ((uint32_t *) FPGA_FABRIC_BASE)

int main(void)
{
	uint32_t controller_buff; //holds all 32 bits of controller data

	uint8_t y_axis; //all 8 bits hold data about y_axis data from controller
	uint8_t x_axis; //all 8 bits hold data about x_axis data from controller
	uint8_t up; //1 or 0
	uint8_t down; //1 or 0
	uint8_t left; //1 or 0
	uint8_t right; //1 or 0

	uint32_t y_mask = 0xFF000000;
	uint32_t x_mask = 0x00FF0000;
	uint32_t up_mask = 0x00000010;
	uint32_t down_mask = 0x00000020;
	uint32_t left_mask = 0x00000040;
	uint32_t right_mask = 0x00000080;

	while( 1 )
	{
		controller_buff = *CONTROLLER_DATA_REG;

		y_axis = (uint8_t)((controller_buff & y_mask) >> 24);
		x_axis = (uint8_t)((controller_buff & x_mask) >> 16);
		up = (uint8_t)((controller_buff & up_mask) >> 4);
		down = (uint8_t)((controller_buff & down_mask) >> 5);
		left = (uint8_t)((controller_buff & left_mask) >> 6);
		right =(uint8_t)((controller_buff & right_mask) >> 7);

		//printf("Controller Data: %" PRIu32 "\n",controller_buff);

	}

	return 0;
}

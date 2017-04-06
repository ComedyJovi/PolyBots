#include "drivers/mss_uart/mss_uart.h"
#include "inttypes.h"

#define CONTROLLER_DATA_REG ((uint8_t *) FPGA_FABRIC_BASE)

int main(void)
{
	while( 1 )
	{
		uint32_t tx_buff;

		tx_buff = *CONTROLLER_DATA_REG;

		printf("Controller Data: %" PRIu32 "\n",tx_buff);

	}

	return 0;
}

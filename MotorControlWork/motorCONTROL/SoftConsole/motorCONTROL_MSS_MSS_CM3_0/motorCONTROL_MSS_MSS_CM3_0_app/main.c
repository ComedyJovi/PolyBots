#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

int * MOTOR_INPUTS = (int*) 0x40050004;
int * LEFT_MOTOR = (int*) 0x40050008;
int * RIGHT_MOTOR = (int*) 0x4005000C;
int * ENCODERS = (int*) 0x40050010;

const int PERIOD = 400000;
const int MAX_PWM = 400000;
/*const double Kp = 0.5;
const double Ki = 0.05;
const double Kd = 10.0;*/
const double Kp = 0.1;
const double Ki = 0.0001;
const double Kd = 4.5;

const int buff_size = 8;

int right_wheel_direction = 0;
int left_wheel_direction = 0;
int mode = 1;
float left_duty_cycle = 0;
float right_duty_cycle = 0;
float prev_right_duty_cycle = 0;
float prev_left_duty_cycle = 0;

/*void process_rx_data(uint8_t rx_buff[buff_size], uint8_t rx_size)
{
	prev_right_duty_cycle = right_duty_cycle;
	prev_left_duty_cycle = left_duty_cycle;

	right_duty_cycle = rx_buff[1];
	right_wheel_direction = rx_buff[2];
	left_duty_cycle = rx_buff[3];
	left_wheel_direction = rx_buff[4];
	mode = rx_buff[5];
}*/

void uart1_rx_handler( mss_uart_instance_t * this_uart )
{
	uint8_t rx_buff[8] = {0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	uint8_t rx_size = MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );
	//process_rx_data(rx_buff, rx_size );

	prev_right_duty_cycle = right_duty_cycle;
	prev_left_duty_cycle = left_duty_cycle;

	right_duty_cycle = rx_buff[1];
	right_wheel_direction = rx_buff[2];
	left_duty_cycle = rx_buff[3];
	left_wheel_direction = rx_buff[4];
	mode = rx_buff[5];
}

int main()
{
	int prev_left_encoder_val = 0;
	int prev_right_encoder_val = 0;
	int left_wheel_tot = 0;
	int right_wheel_tot = 0;

	int left_encoder_val = 0;
	int right_encoder_val = 0;
	int left_rotating = 0;
	int right_rotating = 0;

	float previous_error = 0;
	float current_error = 0;
	float total_error = 0;

	float correction_duty = 0.0;

	float percent_diff = 0.0;

	int reset_value = 1000;

	int LEFT_PWM = 0;
	int RIGHT_PWM = 0;

	int begin = 1;


	uint32_t counter = 0;
	uint32_t gpio_inputs = 0;
	uint32_t count[8] = {0};
	int max = 0;
	int max_index = 0;

	MSS_GPIO_init();
	MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_INOUT_MODE);
	MSS_GPIO_config(MSS_GPIO_1, MSS_GPIO_INOUT_MODE);
	MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_INOUT_MODE);
	MSS_GPIO_config(MSS_GPIO_3, MSS_GPIO_INOUT_MODE);
	MSS_GPIO_config(MSS_GPIO_4, MSS_GPIO_INOUT_MODE);
	MSS_GPIO_config(MSS_GPIO_5, MSS_GPIO_INOUT_MODE);
	MSS_GPIO_config(MSS_GPIO_6, MSS_GPIO_INOUT_MODE);
	MSS_GPIO_config(MSS_GPIO_7, MSS_GPIO_INOUT_MODE);

	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler, MSS_UART_FIFO_EIGHT_BYTES );



	while( 1 )
	{
		// Get command from control module


			/*if(right_duty_cycle < 0.2)
				right_duty_cycle = 0.0;
			if(left_duty_cycle < 0.2)
				right_duty_cycle = 0.0;*/

		if(right_duty_cycle > 1) {
			right_duty_cycle = right_duty_cycle/100.0;
		}
		if(left_duty_cycle > 1){
			left_duty_cycle = left_duty_cycle/100.0;
		}


		if (left_duty_cycle == right_duty_cycle)
		{
			if(prev_left_duty_cycle != prev_right_duty_cycle)
			{
				//if(prev_left_duty_cycle != left_duty_cycle)
					left_wheel_tot = 0;
				//if(prev_right_duty_cycle != left_duty_cycle)
					right_wheel_tot = 0;
			}
		}
		else
		{
			if(prev_left_duty_cycle != left_duty_cycle)
				left_wheel_tot = 0;
			if(prev_right_duty_cycle != right_duty_cycle)
				right_wheel_tot = 0;
		}

		if(mode == 1)
		{
			prev_left_encoder_val = left_encoder_val;
			prev_right_encoder_val = right_encoder_val;

			if(*ENCODERS == 1 || *ENCODERS == 3)
				left_encoder_val = 1;
			else
				left_encoder_val = 0;

			if(*ENCODERS == 2 || *ENCODERS == 3)
				right_encoder_val = 1;
			else
				right_encoder_val = 0;

			// increment totals only when encoder is rotating
			if((prev_left_encoder_val == 0 && left_encoder_val == 1) || (prev_left_encoder_val == 1 && left_encoder_val == 0))
			{
				left_wheel_tot += left_encoder_val;
				left_rotating = 1;
			}
			else
			{
				left_rotating = 0;
			}

			if((prev_right_encoder_val == 0 && right_encoder_val == 1) || (prev_right_encoder_val == 1 && right_encoder_val == 0))
			{
				right_wheel_tot += right_encoder_val;
				right_rotating = 1;
			}
			else
			{
				right_rotating = 0;
			}

			percent_diff = left_duty_cycle/right_duty_cycle;
			previous_error = current_error;
			current_error = right_wheel_tot*(percent_diff) - left_wheel_tot;

			// only accumulate error when wheels are rotating
			if(left_rotating == 1 || right_rotating == 1)
				total_error += current_error;

			// PID calculations
			correction_duty = Kp*current_error/20.0 + Ki*total_error/20.0 + Kd*(current_error-previous_error)/20.0;

			RIGHT_PWM = PERIOD*right_duty_cycle;

			if(begin == 1)
			{
				LEFT_PWM = RIGHT_PWM*(percent_diff);
				begin = 0;
				left_wheel_tot = 0;
				right_wheel_tot = 0;
				previous_error = 0;
				total_error = 0;
				current_error = 0;
			}
			else
			{
				//LEFT_PWM = (*LEFT_MOTOR)*(1 + correction_duty);
				LEFT_PWM = PERIOD*left_duty_cycle;
			}

			/*previous_error = current_error;
			current_error = right_wheel_tot*percent_diff - left_wheel_tot;

			// only accumulate error when wheels are rotating
			if(left_rotating == 1 || right_rotating == 1)
				total_error += current_error;

			// PID calculations
			correction_duty = Kp*current_error/20.0 + Ki*total_error/20.0 + Kd*(current_error-previous_error)/20.0;

			percent_diff = 1 - (right_duty_cycle - left_duty_cycle);
			RIGHT_PWM = PERIOD*right_duty_cycle;

			if(begin == 1)
			{
				LEFT_PWM = RIGHT_PWM*(percent_diff);
				begin = 0;
				left_wheel_tot = 0;
				right_wheel_tot = 0;
				previous_error = 0;
				total_error = 0;
				current_error = 0;
			}
			else
				LEFT_PWM = (*LEFT_MOTOR)*(1 + correction_duty);*/

			if(LEFT_PWM > MAX_PWM)
				LEFT_PWM = MAX_PWM;
			else if(LEFT_PWM < 0)
				LEFT_PWM = 0;

			if(right_wheel_direction == 1)
			{
				*RIGHT_MOTOR = 0;
				*LEFT_MOTOR = 0;
				*MOTOR_INPUTS = 0x5;
			}
			else if(right_wheel_direction == 2)
			{
				*RIGHT_MOTOR = 0;
				*LEFT_MOTOR = 0;
				*MOTOR_INPUTS = 0xa;
			}
			else if(right_wheel_direction == 0)
			{
				left_wheel_tot = 0;
				right_wheel_tot = 0;
				previous_error = 0;
				total_error = 0;
				current_error = 0;

				LEFT_PWM = 0;
				RIGHT_PWM = 0;
				*MOTOR_INPUTS = 0x0;
			}
			else
			{
				left_wheel_tot = 0;
				right_wheel_tot = 0;
				previous_error = 0;
				total_error = 0;
				current_error = 0;

				LEFT_PWM = 0;
				RIGHT_PWM = 0;
				*MOTOR_INPUTS = 0x0;
			}

			*RIGHT_MOTOR = RIGHT_PWM;
			*LEFT_MOTOR = LEFT_PWM;

			if(LEFT_PWM == 0 && RIGHT_PWM == 0)
			{
				begin = 1;
			}

			/*printf("Left motor pulse-width = %d\r\n" , *LEFT_MOTOR);
			printf("Right motor pulse-width = %d\r\n" , *RIGHT_MOTOR);
			printf("right_wheel_direction: %d\r\n", right_wheel_direction);
			printf("mode: %d\r\n\n", mode);*/

			if(left_wheel_tot >= reset_value && right_wheel_tot >= reset_value)
			{
				left_wheel_tot -= reset_value;
				right_wheel_tot -= reset_value;
			}
		}
		else if (mode == 0)
		{
			printf("mode: %d\r\n\n", mode);
			*MOTOR_INPUTS = 0xA;
			//while(mode == 0)
			{
				while(counter <= 500)
				{
					MSS_GPIO_drive_inout(MSS_GPIO_0, MSS_GPIO_DRIVE_HIGH);
					MSS_GPIO_drive_inout(MSS_GPIO_1, MSS_GPIO_DRIVE_HIGH);
					MSS_GPIO_drive_inout(MSS_GPIO_2, MSS_GPIO_DRIVE_HIGH);
					MSS_GPIO_drive_inout(MSS_GPIO_3, MSS_GPIO_DRIVE_HIGH);
					MSS_GPIO_drive_inout(MSS_GPIO_4, MSS_GPIO_DRIVE_HIGH);
					MSS_GPIO_drive_inout(MSS_GPIO_5, MSS_GPIO_DRIVE_HIGH);
					MSS_GPIO_drive_inout(MSS_GPIO_6, MSS_GPIO_DRIVE_HIGH);
					MSS_GPIO_drive_inout(MSS_GPIO_7, MSS_GPIO_DRIVE_HIGH);
					counter = counter + 1;
				}

				MSS_GPIO_drive_inout(MSS_GPIO_0, MSS_GPIO_HIGH_Z);
				MSS_GPIO_drive_inout(MSS_GPIO_1, MSS_GPIO_HIGH_Z);
				MSS_GPIO_drive_inout(MSS_GPIO_2, MSS_GPIO_HIGH_Z);
				MSS_GPIO_drive_inout(MSS_GPIO_3, MSS_GPIO_HIGH_Z);
				MSS_GPIO_drive_inout(MSS_GPIO_4, MSS_GPIO_HIGH_Z);
				MSS_GPIO_drive_inout(MSS_GPIO_5, MSS_GPIO_HIGH_Z);
				MSS_GPIO_drive_inout(MSS_GPIO_6, MSS_GPIO_HIGH_Z);
				MSS_GPIO_drive_inout(MSS_GPIO_7, MSS_GPIO_HIGH_Z);

				while(counter <= 10000)
				{
					gpio_inputs = MSS_GPIO_get_inputs();
					if (gpio_inputs & 0x1) count[0] = count[0] + 1;
					if (gpio_inputs & 0x2) count[1] = count[1] + 1;
					if (gpio_inputs & 0x4) count[2] = count[2] + 1;
					if (gpio_inputs & 0x8) count[3] = count[3] + 1;
					if (gpio_inputs & 0x10) count[4] = count[4] + 1;
					if (gpio_inputs & 0x20) count[5] = count[5] + 1;
					if (gpio_inputs & 0x40) count[6] = count[6] + 1;
					if (gpio_inputs & 0x80) count[7] = count[7] + 1;
					counter = counter + 1;
				}

				if(right_wheel_direction == 1)
				{
					if(1500<count[4])
					{
						if((480<count[3]) && (count[3] < 600)&& (480<count[5]) && (count[5] < 600))
						{
							*LEFT_MOTOR = 0.25 * PERIOD;
							*RIGHT_MOTOR = 0.25 * PERIOD;
						}
						else if(count[3] > count[5])
						{
							*LEFT_MOTOR = 0.27 * PERIOD;
							*RIGHT_MOTOR = 0.3 * PERIOD;
						}
						else
						{
							*LEFT_MOTOR = 0.3 * PERIOD;
							*RIGHT_MOTOR = 0.27 * PERIOD;
						}

					}
					else if((1000<count[4]) && (count[4] <1500)) //slightly left
					{
						if(count[3] > count[5])
						{
							*LEFT_MOTOR = 0.24 * PERIOD;
							*RIGHT_MOTOR = 0.3 * PERIOD;
						}
						else
						{
							*LEFT_MOTOR = 0.3 * PERIOD;
							*RIGHT_MOTOR = 0.24 * PERIOD;
						}
					}
					else
					{
						int i = 0;
						max = count[0];
						max_index = 0;
						for(i = 1; i < 7; i++)
						{
							if(count[i] > max)
							{
								max = count[i];
								max_index = i;
							}
						}
						if (max < 500)
						{
							*LEFT_MOTOR = 0;
							*RIGHT_MOTOR = 0;
						}
						else
						{
							if(max_index == 3)
							{
								*LEFT_MOTOR = 0.21 * PERIOD;
								*RIGHT_MOTOR = 0.3 * PERIOD;
							}
							if(max_index == 2)
							{
								*LEFT_MOTOR = 0.17 * PERIOD;
								*RIGHT_MOTOR = 0.3 * PERIOD;
							}
							if(max_index == 1)
							{
								*LEFT_MOTOR = 0.13 * PERIOD;
								*RIGHT_MOTOR = 0.3 * PERIOD;
							}
							if(max_index == 0)
							{
								*LEFT_MOTOR = 0.09 * PERIOD;
								*RIGHT_MOTOR = 0.3 * PERIOD;
							}
							if(max_index == 5)
							{
								*LEFT_MOTOR = 0.3 * PERIOD;
								*RIGHT_MOTOR = 0.21 * PERIOD;
							}
							if(max_index == 6)
							{
								*LEFT_MOTOR = 0.3 * PERIOD;
								*RIGHT_MOTOR = 0.16 * PERIOD;
							}
							if(max_index == 7)
							{
								*LEFT_MOTOR = 0.3 * PERIOD;
								*RIGHT_MOTOR = 0.10 * PERIOD;
							}
						}
					}
				}
				else
				{
					*LEFT_MOTOR = 0;
					*RIGHT_MOTOR = 0;
				}

				printf("count0: %u\r\n", (unsigned)count[0]);
				printf("count1: %u\r\n", (unsigned)count[1]);
				printf("count2: %u\r\n", (unsigned)count[2]);
				printf("count3: %u\r\n", (unsigned)count[3]);
				printf("count4: %u\r\n", (unsigned)count[4]);
				printf("count5: %u\r\n", (unsigned)count[5]);
				printf("count6: %u\r\n", (unsigned)count[6]);
				printf("count7: %u\r\n\n", (unsigned)count[7]);

				count[0] = 0;
				count[1] = 0;
				count[2] = 0;
				count[3] = 0;
				count[4] = 0;
				count[5] = 0;
				count[6] = 0;
				count[7] = 0;
				counter = 0;

				/*if(MSS_UART_get_rx(&g_mss_uart1, rx_buff, sizeof(rx_buff)))
				{

					right_duty_cycle = rx_buff[1]/100.0;
					right_wheel_direction = rx_buff[2];
					left_duty_cycle = rx_buff[3]/100.0;
					left_wheel_direction = rx_buff[4];
					mode = rx_buff[5];
				}*/
			}
		}
		else
		{
			*RIGHT_MOTOR = 0;
			*LEFT_MOTOR = 0;
		}
	}
}

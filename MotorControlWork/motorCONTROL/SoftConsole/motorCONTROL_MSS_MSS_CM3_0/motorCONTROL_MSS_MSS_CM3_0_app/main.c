#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

int * MOTOR_INPUTS = (int*) 0x40050004;
int * LEFT_MOTOR = (int*) 0x40050008;
int * RIGHT_MOTOR = (int*) 0x4005000C;
int * ENCODERS = (int*) 0x40050010;

const int PERIOD = 5000;
const int MAX_PWM = 500000;
const int LINE_PERIOD = 1100000;
/*const double fwd_Kp = 1.0;
const double fwd_Ki = 0.5;
const double fwd_Kd = 7.0;
const double rev_Kp = 1.0;
const double rev_Ki = 0.5;
const double rev_Kd = 20.0;*/
const int fwd_Kp = 6;
const int fwd_Ki = 0;
const int fwd_Kd = 0;
const int rev_Kp = 2;
const int rev_Ki = 0;
const int rev_Kd = 0;
//kpid = 2,0,0;2.365,0,0.0248;5,0,19.5;
/*const double Kp = 0.1;
const double Ki = 0.0001;
const double Kd = 4.5;*/

const int buff_size = 8;
int LEFT_DUTY = 0;
int RIGHT_DUTY = 0;

int right_wheel_direction = 0;
int left_wheel_direction = 0;
int mode = 1;
/*float left_duty_cycle = 0;
float right_duty_cycle = 0;
float prev_right_duty_cycle = 0;
float prev_left_duty_cycle = 0;*/
uint8_t rx_buff[8] = {0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t x_buff[8];
//int i = 0;

void process_rx_data(uint8_t rx_buff[buff_size], uint8_t rx_size)
{
//	int i = 0;
//	for(i=0; i < 8; i++){
//			x_buff[i] = rx_buff[i];
//	}
	/*x_buff[1] = rx_buff[1];
	x_buff[2] = rx_buff[2];
	x_buff[3] = rx_buff[3];
	x_buff[4] = rx_buff[4];
	x_buff[5] = rx_buff[5];

	int j = 0;
	for(j=3;j>0;j--)
	{
		if(i>1)
		{
			x_buff[5*j+1] = x_buff[5*(j)+1];
			x_buff[5*j+2] = x_buff[5*(j)+2];
			x_buff[5*j+3] = x_buff[5*(j)+3];
			x_buff[5*j+4] = x_buff[5*(j)+4];
			x_buff[5*j+5] = x_buff[5*(j)+5];
		}
	}*/



	RIGHT_DUTY = rx_buff[1];
	right_wheel_direction = rx_buff[2];
	LEFT_DUTY = rx_buff[3];
	left_wheel_direction = rx_buff[4];
	mode = rx_buff[5];
}

void uart1_rx_handler( mss_uart_instance_t * this_uart )
{
	uint8_t rx_size = MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );
	process_rx_data(rx_buff, rx_size );
}

int main()
{
	int prev_left_encoder_val = 0;
	int prev_right_encoder_val = 0;
	int left_encoder_val = 0;
	int right_encoder_val = 0;
	int left_rotating = 0;
	int right_rotating = 0;
	int left_wheel_tot = 0;
	int right_wheel_tot = 0;

	int previous_error = 0;
	int current_error = 0;
	int total_error = 0;

	int left_duty_cycle = 0;
	int right_duty_cycle = 0;
	int prev_right_duty_cycle = 0;
	int prev_left_duty_cycle = 0;
	int correction_duty = 0;

	//int percent_diff = 0.0;

	int LEFT_PWM = 0;
	int RIGHT_PWM = 0;

	int begin = 1;
	//int reset_value = 1000;

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

//	while(1)
//	{
//		*LEFT_MOTOR = PERIOD*(RIGHT_DUTY/100.0);
//		*RIGHT_MOTOR = PERIOD*(LEFT_DUTY/100.0);
//
//	}
	while( 1 )
	{
		// Get command from control module
		//if (x_buff[1] != 0) printf("x_buff: %d\r\n", x_buff[1]);
/*
		printf("x_buff[0] = %d\r\n", rx_buff[0]);
		printf("x_buff[1] = %d\r\n", rx_buff[1]);
		printf("x_buff[2] = %d\r\n", rx_buff[2]);
		printf("x_buff[3] = %d\r\n", rx_buff[3]);
		printf("x_buff[4] = %d\r\n", rx_buff[4]);
		printf("x_buff[5] = %d\r\n", rx_buff[5]);
		printf("x_buff[6] = %d\r\n", rx_buff[6]);
		printf("x_buff[7] = %d\r\n", rx_buff[7]);
		printf("mode: %d\r\n\n", mode);
*/
		// right_duty_cycle = RIGHT_DUTY/100.0;
		// left_duty_cycle = LEFT_DUTY/100.0;
		right_duty_cycle = RIGHT_DUTY;
		left_duty_cycle = LEFT_DUTY;


		if(mode == 1)
		{
			if(left_duty_cycle == right_duty_cycle)
			{
				if (prev_left_duty_cycle != prev_right_duty_cycle)
				{
					left_wheel_tot = 0;
					right_wheel_tot = 0;
				}

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

				if(RIGHT_DUTY == 0)
				{
					correction_duty = 0.0;
				}
				else
				{
					//percent_diff = (left_duty_cycle*100)/right_duty_cycle;
					previous_error = current_error;
					current_error = right_wheel_tot - left_wheel_tot;

					// only accumulate error when wheels are rotating
					if(left_rotating == 1 || right_rotating == 1)
						total_error += current_error;

					// PID calculations
					if(right_wheel_direction == 1)
						correction_duty = fwd_Kp*current_error + fwd_Ki*total_error + fwd_Kd*(current_error-previous_error);
					else if(right_wheel_direction == 2)
						correction_duty = rev_Kp*current_error + rev_Ki*total_error + rev_Kd*(current_error-previous_error);
				}

				if(begin == 1)
				{
					// if(RIGHT_PWM == 0)
					// 	LEFT_PWM = PERIOD*left_duty_cycle;
					// else
					// 	LEFT_PWM = RIGHT_PWM*(1.25);
					LEFT_PWM = PERIOD*left_duty_cycle;
					begin = 0;
					left_wheel_tot = 0;
					right_wheel_tot = 0;
					previous_error = 0;
					total_error = 0;
					current_error = 0;
				}
				else
				{
					//LEFT_PWM = PERIOD*(correction_duty);
					LEFT_PWM = ((*LEFT_MOTOR)/PERIOD + correction_duty)*PERIOD;
					//LEFT_PWM = RIGHT_PWM*(percent_diff*1.25);
				}
			}
			else
			{
				LEFT_PWM = PERIOD*left_duty_cycle;
			}

			RIGHT_PWM = PERIOD*right_duty_cycle;
			//LEFT_PWM = PERIOD*left_duty_cycle;

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

//			printf("left_pwm = %d\r\n", LEFT_PWM);
//			printf("right_pwm = %d\r\n", RIGHT_PWM);
			// *LEFT_MOTOR = PERIOD*(LEFT_DUTY/100.0);
			// *RIGHT_MOTOR = PERIOD*(RIGHT_DUTY/100.0);

			if(LEFT_PWM == 0 && RIGHT_PWM == 0)
			{
				begin = 1;
			}

			prev_right_duty_cycle = right_duty_cycle;
			prev_left_duty_cycle = left_duty_cycle;
			//printf("Left motor pulse-width = %d\r\n" , *LEFT_MOTOR);
			//printf("Right motor pulse-width = %d\r\n" , *RIGHT_MOTOR);
			//printf("right_wheel_direction: %d\r\n", right_wheel_direction);
//			printf("left total: %d\r\n", left_wheel_tot);
//			printf("right total: %d\r\n\n", right_wheel_tot);

			/*if(left_wheel_tot >= reset_value && right_wheel_tot >= reset_value)
			{
				left_wheel_tot -= reset_value;
				right_wheel_tot -= reset_value;
			}*/
		}
		else if (mode == 0)
		{

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
							*LEFT_MOTOR = 0.23 * LINE_PERIOD;
							*RIGHT_MOTOR = 0.23 * LINE_PERIOD;
						}
						else if(count[3] > count[5])
						{
							*LEFT_MOTOR = 0.21 * LINE_PERIOD;
							*RIGHT_MOTOR = 0.23 * LINE_PERIOD;
						}
						else
						{
							*LEFT_MOTOR = 0.23 * LINE_PERIOD;
							*RIGHT_MOTOR = 0.21 * LINE_PERIOD;
						}

					}
					else if((1000<count[4]) && (count[4] <1500)) //slightly left
					{
						if(count[3] > count[5])
						{
							*LEFT_MOTOR = 0.19 * LINE_PERIOD;
							*RIGHT_MOTOR = 0.23 * LINE_PERIOD;
						}
						else
						{
							*LEFT_MOTOR = 0.23 * LINE_PERIOD;
							*RIGHT_MOTOR = 0.19 * LINE_PERIOD;
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
								*LEFT_MOTOR = 0.19 * LINE_PERIOD;
								*RIGHT_MOTOR = 0.3 * LINE_PERIOD;
							}
							if(max_index == 2)
							{
								*LEFT_MOTOR = 0.15 * LINE_PERIOD;
								*RIGHT_MOTOR = 0.35 * LINE_PERIOD;
							}
							if(max_index == 1)
							{
								*LEFT_MOTOR = 0.11 * LINE_PERIOD;
								*RIGHT_MOTOR = 0.35 * LINE_PERIOD;
							}
							if(max_index == 0)
							{
								*LEFT_MOTOR = 0.07 * LINE_PERIOD;
								*RIGHT_MOTOR = 0.35 * LINE_PERIOD;
							}
							if(max_index == 5)
							{
								*LEFT_MOTOR = 0.3 * LINE_PERIOD;
								*RIGHT_MOTOR = 0.16 * LINE_PERIOD;
							}
							if(max_index == 6)
							{
								*LEFT_MOTOR = 0.35 * LINE_PERIOD;
								*RIGHT_MOTOR = 0.12 * LINE_PERIOD;
							}
							if(max_index == 7)
							{
								*LEFT_MOTOR = 0.35 * LINE_PERIOD;
								*RIGHT_MOTOR = 0.07* LINE_PERIOD;
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
//		i++;
//		if(i==5)
//			i = 0;
	}
}

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "pixy.h"
#include "IRcontrol.h"


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

void get_motor_command(int * rpwm, int * lpwm, int * dir){

	/*
	unsigned int pixy_mag, pixy_dir;

	//return if data is none existant
	if(!pixy_x_err( &pixy_mag, &pixy_dir)){ return; }

	//printf("%x, %x\n\r", pixy_mag, pixy_dir);
	pixy_mag /= 3; // scale error
	if(pixy_mag > 10)pixy_mag = 10;

	rpwm[0] = 500000*0.1*pixy_mag;
	lpwm[0] = rpwm[0];
	dir[0] = pixy_dir;

	//switch on the intended direction
	switch (right_wheel_direction){
		case 1: //forward
			//respond more to when the leader is leading
			//and less for when it is lagging
			break;
		case 2: //backward
			break;
		case 0: //halt - no direction
		default:

			break;
	}
	*/
	*dir = 2;
	int ir_dir = 0, ir_value = 0;

	ir_value = ir_read(&ir_dir);

	int temp = 350000;
//	printf("ir_value: %x\n\r", ir_value);

	//if turning left
	if(ir_dir == 1)
	{
		*rpwm =*rpwm + ir_value * IR_CORRECTION;
		*rpwm = temp + (temp * 0.3 * ir_value);
		*lpwm = temp - (temp * 0.3 * ir_value);

//		*rpwm = 300000;
//		*lpwm = 0;
	}
	//if turning right
	else if(ir_dir == 2)
	{
		*lpwm = *lpwm + ir_value * IR_CORRECTION;
		*lpwm = temp + (temp * 0.3 * ir_value);
		*rpwm = temp - (temp * 0.3 * ir_value) ;
//
//		*lpwm = 300000;
//		*rpwm = 0;
	}
	else{
		*lpwm = 0;//temp;
		*rpwm = 0;//temp;
	}

	if (*rpwm > 500000){*rpwm = 500000;}
	if (*lpwm > 500000){*lpwm = 500000;}


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



	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler, MSS_UART_FIFO_EIGHT_BYTES );


	//pixy variables
	//unsigned int pixy_mag, pixy_dir;
	int pixy_rmotor_pwm = 0;
	int pixy_lmotor_pwm = 0;
	int pixy_motor_dir = 0;

	//pixy init functions
	MSS_I2C_init(&g_mss_i2c1 , PIXY_I2C_DEFAULT_ADDR, MSS_I2C_PCLK_DIV_256 );
	start_hardware_cont_timer();
	init_ideal_pixy_dots();


	//init IR sensors
	init_ir_control();


//	while(1)
//	{
//		*MOTOR_INPUTS = 0xa;
//		*RIGHT_MOTOR = 400000;//speed 0 - 500000
//		*LEFT_MOTOR = 400000;//speed 0 - 500000
//
//	}
	while( 1 )
	{
		// Get command from control module


			/*if(right_duty_cycle < 0.2)
				right_duty_cycle = 0.0;
			if(left_duty_cycle < 0.2)
				right_duty_cycle = 0.0;*/


//		process_pixy_i2c();
		get_motor_command(&pixy_rmotor_pwm, &pixy_lmotor_pwm, &pixy_motor_dir);

//		printf("%x\n\r", pixy_lmotor_pwm);



		// pixy cam bypass controller

					//direction 0x5->forward  | 0xa->backward | 0x0->none
					switch(pixy_motor_dir){
					case 1:
						*MOTOR_INPUTS = 0xa;
						break;
					case 2:
						*MOTOR_INPUTS = 0x5;
						break;
					case 0:
					default:
						*MOTOR_INPUTS = 0x0;
						break;
					}

					//printf("l: %x, r: %x\n\r", pixy_lmotor_pwm, pixy_rmotor_pwm);

					*RIGHT_MOTOR = pixy_rmotor_pwm;//speed 0 - 500000
					*LEFT_MOTOR = pixy_lmotor_pwm;//speed 0 - 500000

					//For testing, bypasses other instructions
		//			*MOTOR_INPUTS = 0xa;
		//			*RIGHT_MOTOR = 400000;//speed 0 - 500000
		//			*LEFT_MOTOR = 400000;//speed 0 - 500000




//		if(right_duty_cycle > 1) {
//			right_duty_cycle = right_duty_cycle/100.0;
//		}
//		if(left_duty_cycle > 1){
//			left_duty_cycle = left_duty_cycle/100.0;
//		}
//
//
//		if (left_duty_cycle == right_duty_cycle)
//		{
//			if(prev_left_duty_cycle != prev_right_duty_cycle)
//			{
//				//if(prev_left_duty_cycle != left_duty_cycle)
//					left_wheel_tot = 0;
//				//if(prev_right_duty_cycle != left_duty_cycle)
//					right_wheel_tot = 0;
//			}
//		}
//		else
//		{ // TODO: check if necessary
//			if(prev_left_duty_cycle != left_duty_cycle)
//				left_wheel_tot = 0;
//			if(prev_right_duty_cycle != right_duty_cycle)
//				right_wheel_tot = 0;
//		}
//
//		if(mode == 1)
//		{
//			prev_left_encoder_val = left_encoder_val;
//			prev_right_encoder_val = right_encoder_val;
//
//			if(*ENCODERS == 1 || *ENCODERS == 3)
//				left_encoder_val = 1;
//			else
//				left_encoder_val = 0;
//
//			if(*ENCODERS == 2 || *ENCODERS == 3)
//				right_encoder_val = 1;
//			else
//				right_encoder_val = 0;
//
//			// increment totals only when encoder is rotating
//			if((prev_left_encoder_val == 0 && left_encoder_val == 1) || (prev_left_encoder_val == 1 && left_encoder_val == 0))
//			{
//				left_wheel_tot += left_encoder_val;
//				left_rotating = 1;
//			}
//			else
//			{
//				left_rotating = 0;
//			}
//
//			if((prev_right_encoder_val == 0 && right_encoder_val == 1) || (prev_right_encoder_val == 1 && right_encoder_val == 0))
//			{
//				right_wheel_tot += right_encoder_val;
//				right_rotating = 1;
//			}
//			else
//			{
//				right_rotating = 0;
//			}
//
//
//			// PID starts
//			percent_diff = left_duty_cycle/right_duty_cycle;
//			previous_error = current_error;
//			current_error = right_wheel_tot*(percent_diff) - left_wheel_tot;
//
//			// only accumulate error when wheels are rotating
//			if(left_rotating == 1 || right_rotating == 1)
//				total_error += current_error;
//
//			// PID calculations
//			correction_duty = Kp*current_error/20.0 + Ki*total_error/20.0 + Kd*(current_error-previous_error)/20.0;
//
//			RIGHT_PWM = PERIOD*right_duty_cycle;
//
//			if(begin == 1)
//			{
//				LEFT_PWM = RIGHT_PWM*(percent_diff);
//				begin = 0;
//				left_wheel_tot = 0;
//				right_wheel_tot = 0;
//				previous_error = 0;
//				total_error = 0;
//				current_error = 0;
//			}
//			else
//			{
//				//LEFT_PWM = (*LEFT_MOTOR)*(1 + correction_duty);
//				LEFT_PWM = PERIOD*left_duty_cycle;
//			}
//
//			/*previous_error = current_error;
//			current_error = right_wheel_tot*percent_diff - left_wheel_tot;
//
//			// only accumulate error when wheels are rotating
//			if(left_rotating == 1 || right_rotating == 1)
//				total_error += current_error;
//
//			// PID calculations
//			correction_duty = Kp*current_error/20.0 + Ki*total_error/20.0 + Kd*(current_error-previous_error)/20.0;
//
//			percent_diff = 1 - (right_duty_cycle - left_duty_cycle);
//			RIGHT_PWM = PERIOD*right_duty_cycle;
//
//			if(begin == 1)
//			{
//				LEFT_PWM = RIGHT_PWM*(percent_diff);
//				begin = 0;
//				left_wheel_tot = 0;
//				right_wheel_tot = 0;
//				previous_error = 0;
//				total_error = 0;
//				current_error = 0;
//			}
//			else
//				LEFT_PWM = (*LEFT_MOTOR)*(1 + correction_duty);*/
//
//			if(LEFT_PWM > MAX_PWM)
//				LEFT_PWM = MAX_PWM;
//			else if(LEFT_PWM < 0)
//				LEFT_PWM = 0;
//
//
//			if(right_wheel_direction == 1)
//			{// move forward
//				*RIGHT_MOTOR = 0;
//				*LEFT_MOTOR = 0;
//				*MOTOR_INPUTS = 0x5;
//			}
//			else if(right_wheel_direction == 2)
//			{// move backward
//				*RIGHT_MOTOR = 0;
//				*LEFT_MOTOR = 0;
//				*MOTOR_INPUTS = 0xa;
//			}
//			else if(right_wheel_direction == 0)
//			{// stay still
//				left_wheel_tot = 0;
//				right_wheel_tot = 0;
//				previous_error = 0;
//				total_error = 0;
//				current_error = 0;
//
//				LEFT_PWM = 0;
//				RIGHT_PWM = 0;
//				*MOTOR_INPUTS = 0x0;
//			}
//			else
//			{// stay still
//				left_wheel_tot = 0;
//				right_wheel_tot = 0;
//				previous_error = 0;
//				total_error = 0;
//				current_error = 0;
//
//				LEFT_PWM = 0;
//				RIGHT_PWM = 0;
//				*MOTOR_INPUTS = 0x0;
//			}
//
//			//sends pwm to hardware
//			//takes values 0 - 500000
//			*RIGHT_MOTOR = RIGHT_PWM;
//			*LEFT_MOTOR = LEFT_PWM;
//
//
//			if(LEFT_PWM == 0 && RIGHT_PWM == 0)
//			{
//				begin = 1;
//			}
//
//			/*printf("Left motor pulse-width = %d\r\n" , *LEFT_MOTOR);
//			printf("Right motor pulse-width = %d\r\n" , *RIGHT_MOTOR);
//			printf("right_wheel_direction: %d\r\n", right_wheel_direction);
//			printf("mode: %d\r\n\n", mode);*/
//
//			if(left_wheel_tot >= reset_value && right_wheel_tot >= reset_value)
//			{
//				left_wheel_tot -= reset_value;
//				right_wheel_tot -= reset_value;
//			}
//		}
//		else if (mode == 0)
//		{}

	}
}

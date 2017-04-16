/*
 * IRcontrol.h
 *
 *  Created on: Apr 15, 2017
 *      Author: baldeeb
 */

#ifndef IRCONTROL_H_
#define IRCONTROL_H_


#include "drivers/mss_ace/mss_ace.h"

#define IR_DIVIDER 100
#define IR_CORRECTION  20

#define IR_TOLERANCE (250 / IR_DIVIDER)
#define IR_LOWER_LIMIT (1500 / IR_DIVIDER)

//the number of averaged values
#define IR_SAMPLE_COUNT 10
#define IR_ERROR_SAMPLE_COUNT  5

//#define IDEAL_RIGHT_IR
//#define IDEAL_LEFT_IR


ace_channel_handle_t adc_handler_f;
ace_channel_handle_t adc_handler_b;

int sample_index;
int ir_front_samples[IR_SAMPLE_COUNT];
int ir_back_samples[IR_SAMPLE_COUNT];

int ir_dir;

int ir_front_sum;
int ir_back_sum;

int ir_front_ave;
int ir_back_ave;


int ir_error_samples[IR_ERROR_SAMPLE_COUNT];
int ir_error_index;
int ir_error_sum;
int ir_error;

/* *
 * Initialize IR sensors and respective ADC
 * */
void init_ir_control( void );

/* *
 * reads IR sensors and determines the difference in values
 * returns the difference and indicates direction
 * dir= 0, moving foward, dir = 1, turning left, dir = 2, turning right
 * */
int ir_read(int * dir);


#endif /* IRCONTROL_H_ */

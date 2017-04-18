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
#define IR_ERROR_DIVIDER 1 // currently not in use

#define IR_TOLERANCE (200 / IR_DIVIDER)//
#define IR_LOWER_LIMIT (1500 / IR_DIVIDER)

//the number of averaged values
#define IR_SAMPLE_COUNT 10 //NUMBER OF SAMPLED READS
#define IR_ERROR_SAMPLE_COUNT  5 //NUMBER OF SAMPLED ERROR VALUES

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

void ir_sample( void );

void ir_update_error( void );

#endif /* IRCONTROL_H_ */

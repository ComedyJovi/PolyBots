/*
 * IRcontrol.c
 *
 *  Created on: Apr 15, 2017
 *      Author: baldeeb
 */

#include "IRcontrol.h"

void init_ir_control(void){
	int i;

	ACE_init();

	/* DAC initialization */
	ACE_configure_sdd(
		SDD1_OUT,
		SDD_8_BITS,
		SDD_VOLTAGE_MODE | SDD_RETURN_TO_ZERO,
		INDIVIDUAL_UPDATE
	);
	ACE_enable_sdd(SDD1_OUT);

    /* handler for ADC channel */
    adc_handler_f = ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_0");
    adc_handler_b = ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_1");

    for(i = 0; i < IR_SAMPLE_COUNT; i++){
    	ir_front_samples[i] = 0;
    	ir_back_samples[i] = 0;
    }
    sample_index = 0;
}

//update ir_samples and average result
void ir_process_data( void ){

	int i;
	int temp_f, temp_b;
    uint16_t adc_data_f = 0;
    uint16_t adc_data_b = 0;

	adc_data_f = ACE_get_ppe_sample(adc_handler_f) / IR_DIVIDER;
	adc_data_b = ACE_get_ppe_sample(adc_handler_b) / IR_DIVIDER;

	printf("adc_data_f: %i\r\n", adc_data_f);
	printf("adc_data_b: %i\r\n\n", adc_data_b);

	ir_front_sum -= ir_front_samples[sample_index];
	ir_back_sum -= ir_back_samples[sample_index];

	ir_front_samples[sample_index] = adc_data_f;
	ir_back_samples[sample_index] = adc_data_b;

	ir_front_sum += ir_front_samples[sample_index];
	ir_back_sum += ir_back_samples[sample_index];
	++sample_index;
	if(sample_index == IR_SAMPLE_COUNT)
	{
		sample_index = 0;
	}

	ir_front_ave = ir_front_sum / IR_SAMPLE_COUNT;
	ir_back_ave = ir_back_sum / IR_SAMPLE_COUNT;



//	printf("adc_data_f: %i\r\n", ir_front_ave);
//	printf("adc_data_b: %i\r\n\n", ir_back_ave);
}


//check IR sensor and test if the master bot is turing. Find the turning direction and difference in the reading of 2 IR_sensors
int ir_read(int * dir)
{
	ir_process_data();
	int temp_error;
	if(ir_front_ave < IR_LOWER_LIMIT || ir_back_ave < IR_LOWER_LIMIT){
		*dir = 0;
	    return 0;
	}

	//turning right, dir = 2
	if((ir_front_ave > ir_back_ave) && ((ir_front_ave - ir_back_ave) > IR_TOLERANCE))
	{
		*dir = 2;
		temp_error = ir_front_ave - ir_back_ave;
		ir_error_sum -= ir_error_samples[ir_error_index];
		ir_error_samples[ir_error_index] = temp_error;
		ir_error_sum += temp_error;
		ir_error = ir_error_sum / IR_ERROR_SAMPLE_COUNT;
		++ir_error_index;
		if(ir_error_index == IR_ERROR_SAMPLE_COUNT)
		{
			ir_error_index = 0;
		}
		return ir_error;
	}
	//turning left, dir = 1
	else if((ir_front_ave < ir_back_ave) && ((ir_back_ave - ir_front_ave) > IR_TOLERANCE))
	{
		*dir = 1;
		temp_error = ir_back_ave - ir_front_ave;
		ir_error_sum -= ir_error_samples[ir_error_index];
		ir_error_samples[ir_error_index] = temp_error;
		ir_error_sum += temp_error;
		ir_error = ir_error_sum / IR_ERROR_SAMPLE_COUNT;
		++ir_error_index;
		if(ir_error_index == IR_ERROR_SAMPLE_COUNT)
		{
			ir_error_index = 0;
		}
		return ir_error;
	}
	//return 0 if it's not turning
	*dir = 0;
    return 0;
}

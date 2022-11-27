/*
 * automatic_fsm.c
 *
 *  Created on: Nov 19, 2022
 *      Author: acer
 */

#include "automatic_fsm.h"

extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart2;

uint8_t buffer_byte;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;


int status1 = INIT;
uint8_t cmd_flag = INIT;
//uint8_t cmd_data[MAX_CMD_SIZE];
//uint8_t cmd_data_index = 0;
int ADC_value = 0;

void cmd_parser_fsm(){
	switch(status1){
		case INIT:
			if (buffer_byte == '!') status1 = READING;
			else status1 = INIT;
			break;
		case READING:
			if (buffer_byte == 'R') status1 = RECEIVE_R;
			else if (buffer_byte == 'O') status1 = RECEIVE_O;
			else status1 = INIT;
			break;
		case RECEIVE_R:
			if (buffer_byte == 'S') status1 = RECEIVE_S;
			else status1 = INIT;
			break;
		case RECEIVE_S:
			if (buffer_byte == 'T') status1 = RECEIVE_T;
			else status1 = INIT;
			break;
		case RECEIVE_T:
			if (buffer_byte == '#') {cmd_flag = RST; status1=INIT; setTimer1(1);}
			else status1 = INIT;
			break;
		case RECEIVE_O:
			if (buffer_byte == 'K') status1 = RECEIVE_K;
			else status1 = INIT;
			break;
		case RECEIVE_K:
			if (buffer_byte == '#') {cmd_flag = OK; status1=INIT;}
			else status1 = INIT;
			break;
		default:
			break;
	}
}

void uart_comms_fsm(){
	char str[50];
	switch(cmd_flag){
		case RST:
			if (timer1_flag == 1){
				ADC_value = HAL_ADC_GetValue(&hadc1);
				HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n",ADC_value), 500);
				setTimer1(300);
			}
		    break;
		case OK:
			ADC_value = -1;
			cmd_flag = INIT;
			break;
		default:
			break;
	}
}

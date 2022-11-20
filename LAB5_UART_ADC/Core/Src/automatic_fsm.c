/*
 * automatic_fsm.c
 *
 *  Created on: Nov 19, 2022
 *      Author: acer
 */

#include "automatic_fsm.h"

extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart2;

int status1 = INIT;

int status2 = INIT;

uint8_t cmd_data[MAX_CMD_SIZE];

uint8_t cmd_data_index = 0;

uint8_t cmd_flag = INIT;

uint32_t ADC_value = 0;

char str[50];

int isCmdEqualTo0(uint8_t *buffer, int size){
	int flag = 0;
	for (int i=0; i<size; i++){
		if (buffer[i] != 0) flag = 0;
		else flag = 1;
	}
	return flag;
}

int isCmdEqualToRST(uint8_t *buffer, int size){
	int flag = 0;
	for (int i=0; i<size; i++){
		if (buffer[i] == '!' && buffer[i+1] == 'R' && buffer[i+2] == 'S' && buffer[i+3] == 'T' && buffer[i+4] == '#')
			flag = 1;
	}
	return flag;
}

int isCmdEqualToOK(uint8_t *buffer, int size){
	int flag = 0;
	for (int i=0; i<size; i++){
		if (buffer[i] == '!' && buffer[i+1] == 'O' && buffer[i+2] == 'K' && buffer[i+3] == '!')
			flag = 1;
	}
	return flag;
}

void cmd_parser_fsm(uint8_t *buffer_byte, int size){
	switch(status1){
		case INIT:
			if (buffer_byte == STARTING_BYTE) status1 = READING;
			if (buffer_byte == ENDING_BYTE) status1 = STOP;
			break;
		case READING:

			break;
		case STOP:

			break;
		default:
			break;
	}
}

void uart_comms_fsm(){
	switch(cmd_flag){
		case READ:
		    ADC_value = HAL_ADC_GetValue(&hadc1);
		    HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%ld#\r\n",ADC_value), 100);
		    break;
		case END:
			ADC_value = 0;
			HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%ld#\r\n",ADC_value), 100);
			break;
		default:
			break;
	}
}

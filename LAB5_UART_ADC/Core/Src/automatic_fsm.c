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

uint8_t cmd_data[MAX_CMD_SIZE];

uint8_t cmd_data_index = 0;

uint8_t cmd_flag = INIT;

uint32_t ADC_value = 0;

char str[50];

int isCmdEqualToRST(uint8_t cmd[]){
	int flag = 0;
	if (cmd[0] == 'R' && cmd[1] == 'S' && cmd[2] == 'T')
		flag = 1;
	return flag;
}

int isCmdEqualToOK(uint8_t cmd[]){
	int flag = 0;
	if (cmd[0] == 'O' && cmd[1] == 'K')
		flag = 1;
	return flag;
}

void cmd_parser_fsm(uint8_t buffer_byte){
	switch(status1){
		case INIT:
			if (buffer_byte == STARTING_BYTE) status1 = READING;
			break;
		case READING:
			cmd_data[cmd_data_index] = buffer_byte;
			cmd_data_index++;
			if (cmd_data_index > MAX_CMD_SIZE) cmd_data_index = 0;
			if (buffer_byte == ENDING_BYTE) status1 = STOP;
			break;
		case STOP:
			status1 = INIT;
			break;
		default:
			break;
	}
}

void uart_comms_fsm(){
	switch(cmd_flag){
		case INIT:
			if (isCmdEqualToRST(cmd_data) == 1) cmd_flag = READING;
			if (isCmdEqualToOK(cmd_data) == 1) cmd_flag = STOP;
		case READING:
		    ADC_value = HAL_ADC_GetValue(&hadc1);
		    HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%ld#\r\n",ADC_value), 100);
		    if (isCmdEqualToOK(cmd_data) == 1) cmd_flag = STOP;
		    break;
		case STOP:
			ADC_value = 0;
			HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%ld#\r\n",ADC_value), 100);
			break;
		default:
			break;
	}
}

/*
 * automatic_fsm.c
 *
 *  Created on: Nov 19, 2022
 *      Author: acer
 */

#include "automatic_fsm.h"

extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart2;

uint8_t cmd_flag = INIT;

int status = INIT;

uint32_t ADC_value = 0;

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
	if (buffer[0] == '!' && buffer[1] == 'R' && buffer[2] == 'S' && buffer[3] == 'T' && buffer[4] == '#')
		flag = 1;
	else
		flag = 0;
	return flag;
}

int isCmdEqualToOK(uint8_t *buffer, int size){
	int flag = 0;
	if (buffer[0] == '!' && buffer[1] == 'O' && buffer[2] == 'K' && buffer[3] == '#')
		flag = 1;
	else
		flag = 0;
	return flag;
}

void cmd_parser_fsm(uint8_t *buffer, int size){
	switch(status){
		case INIT:
			if (isCmdEqualToRST(buffer, size) == 1) status = READ;
			else if (isCmdEqualToOK(buffer, size) == 1) status = END;
			else status = INIT;
			break;
		case READ:
			cmd_flag = READ;
			if (isCmdEqualToOK(buffer, size) == 1) status = END;
			else status = INIT;
			break;
		case END:
			cmd_flag = END;
			if (isCmdEqualToRST(buffer, size) == 1) status = READ;
			else status = INIT;
			break;
		default:
			break;
	}
}

void uart_comms_fsm(){
	char str[50];
	switch(cmd_flag){
		case READ:
		    ADC_value = HAL_ADC_GetValue(&hadc1);
		    HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "%ld\r\n",ADC_value), 100);
		    break;
		case END:
			return;
		default:
			break;
	}
}

/*
 * automatic_fsm.h
 *
 *  Created on: Nov 19, 2022
 *      Author: acer
 */

#ifndef INC_AUTOMATIC_FSM_H_
#define INC_AUTOMATIC_FSM_H_

#include "main.h"
#include <stdio.h>

#define INIT 0

#define READING 1

#define STOP 2

#define STARTING_BYTE '!'

#define ENDING_BYTE '#'

#define MAX_CMD_SIZE 3

int isCmdEqualTo0(uint8_t *buffer, int size);

int isCmdEqualToRST(uint8_t *buffer, int size);

int isCmdEqualToOK(uint8_t *buffer, int size);

void cmd_parser_fsm(uint8_t *buffer_byte, int size);

void uart_comms_fsm();

#endif /* INC_AUTOMATIC_FSM_H_ */

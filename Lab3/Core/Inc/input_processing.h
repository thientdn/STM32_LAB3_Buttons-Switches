/*
 * input_processing.h
 *
 *  Created on: Oct 27, 2026
 *      Author: ASUS
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"

#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4

extern int status;

// Thời gian Hướng 1 (SN)
extern int red1_time;
extern int amber1_time;
extern int green1_time;

// Thời gian Hướng 2 (EW)
extern int red2_time;
extern int amber2_time;
extern int green2_time;

// Biến chỉnh sửa tạm thời
extern int temp_red1;
extern int temp_amber;
extern int temp_green1;

void reset_mode_1(void);
void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */

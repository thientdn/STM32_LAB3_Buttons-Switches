/*
 * input_reading.h
 *
 *  Created on: Oct 27, 2026
 *      Author: ASUS
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "main.h"

//khai báo các hàm giao tiếp với nút bấm
void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);

#endif /* INC_INPUT_READING_H_ */

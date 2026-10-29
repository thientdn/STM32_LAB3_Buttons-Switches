/*
 * timer.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ASUS
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

// Chu kỳ ngắt phần cứng của Timer2 (ms)
#define TIMER_CYCLE 10

// Cờ báo hết giờ cho 2 Software Timers
extern int timer1_flag; // Dùng cho chớp LED 2Hz
extern int timer2_flag; // Dùng cho đếm lùi 1s (1000ms)

void setTimer1(int duration_ms);
void setTimer2(int duration_ms);
void timerRun(void);

#endif /* INC_TIMER_H_ */

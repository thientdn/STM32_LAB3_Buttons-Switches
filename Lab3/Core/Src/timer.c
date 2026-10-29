/*
 * timer.c
 *
 *  Created on: Oct 27, 2026
 *      Author: ASUS
 */
#include "timer.h"
#include "input_reading.h"
#include "led_display.h"

// biến đếm phầm mềm nội bộ và cờ ngắt
int timer1_counter = 0;
int timer1_flag = 0;

int timer2_counter = 0;
int timer2_flag = 0;

// thiết lập thời gian cho software timer 1&2
void setTimer1(int duration_ms) {
    timer1_counter = duration_ms / TIMER_CYCLE;
    timer1_flag = 0;
}

void setTimer2(int duration_ms) {
    timer2_counter = duration_ms / TIMER_CYCLE;
    timer2_flag = 0;
}


void timerRun(void) {
	// xử lý timer 1
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter <= 0) {
            timer1_flag = 1;
        }
    }
    // xử lý timer 2
    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter <= 0) {
            timer2_flag = 1;
        }
    }
}


// hàm callback ngắt định kỳ của timer phần cứng
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        button_reading(); // Quét nút bấm chống dội
        timerRun();       // Cập nhật các bộ đếm phần mềm
        scan7SEG();       // Quét LED 7 đoạn
    }
}

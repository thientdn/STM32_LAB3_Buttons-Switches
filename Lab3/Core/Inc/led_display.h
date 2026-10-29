/*
 * led_display.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ASUS
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

// biến lưu giá trị cần hiển thị trên 4 LED 7 Segment
extern int mode_display_value; // hiển thị số mode hiện tại (hướng Bắc - Nam)
extern int time_display_value; // hiển thị giá trị đèn đang được chỉnh ở hiện tại (hướng Đông - Tây)

// hàm hiển thị LED 7 segment
void display7SEG(int num);
void update7SEG(int index);
void scan7SEG(void);

// hàm điều khiển nhóm LED giao thông (led red, yellow & green)
void clearAllLEDs(void);
void toggleRedLEDs(void);
void toggleYellowLEDs(void);
void toggleGreenLEDs(void);

// hàm cài đặt trạng thái ở mode 1 để chạy tự động
void setLedRed1Green2(void);
void setLedRed1Amber2(void);
void setLedGreen1Red2(void);
void setLedAmber1Red2(void);

#endif /* INC_LED_DISPLAY_H_ */

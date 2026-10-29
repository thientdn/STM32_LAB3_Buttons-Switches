/*
 * led_display.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ASUS
 */

#include "led_display.h"

int mode_display_value = 1;
int time_display_value = 1;

// mã Segment đại diện cho số từ 0 đến 9
static const uint8_t segment_code[10] = {
    0b1000000, /* 0 */
    0b1111001, /* 1 */
    0b0100100, /* 2 */
    0b0110000, /* 3 */
    0b0011001, /* 4 */
    0b0010010, /* 5 */
    0b0000010, /* 6 */
    0b1111000, /* 7 */
    0b0000000, /* 8 */
    0b0010000  /* 9 */
};


// ghi mã của 1 chữ số cần hiển thị ra bus segment PA8-PA14 (a-g)
void display7SEG(int num) {
    // Ép biên an toàn từ 0 đến 9
    if (num < 0) num = 0;
    if (num > 9) num = 9;

    uint8_t code = segment_code[num];
    for (uint8_t i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 << i, (code >> i) & 0x01);
    }
}


// kích nguồn chop trans pnp cho từng led 7 segment
void update7SEG(int index) {
	// tắt tất cả led 7 segment để tránh lem nét trước khi chuyển đổi số
    HAL_GPIO_WritePin(GPIOB, EN1_Pin | EN2_Pin | EN3_Pin | EN4_Pin, GPIO_PIN_SET);

    switch (index) {
        case 0: // hàng chục của hướng Bắc - Nam
            display7SEG(mode_display_value / 10);
            HAL_GPIO_WritePin(GPIOB, EN1_Pin, GPIO_PIN_RESET);
            break;
        case 1: // hàng đơn vị của hướng Bắc - Nam
            display7SEG(mode_display_value % 10);
            HAL_GPIO_WritePin(GPIOB, EN2_Pin, GPIO_PIN_RESET);
            break;
        case 2: // hàng chục của hướng Đông - Tây
            display7SEG(time_display_value / 10);
            HAL_GPIO_WritePin(GPIOB, EN3_Pin, GPIO_PIN_RESET);
            break;
        case 3: // hàng đơn vị của hướng Đông - Tây
            display7SEG(time_display_value % 10);
            HAL_GPIO_WritePin(GPIOB, EN4_Pin, GPIO_PIN_RESET);
            break;
    }
}

// quét luân phiên 4 led 7 segment
void scan7SEG(void) {
    static uint8_t seg_index = 0;
    static uint16_t scan_counter = 0;
    scan_counter++;
    // mỗi lần chuyển led cần 120 ms => 4 led 7 segment = 480 ms tương đương f ~ 2Hz
    // để mô phỏng không lag còn thực tế có thể bỏ phần scan_counter thì khi này f = 25Hz phù hợp cho mắt người hơn
    if (scan_counter >= 12) {
		scan_counter = 0;
		update7SEG(seg_index);
		seg_index = (seg_index + 1) % 4;
	}
}


// các hàm bật, tắt, chớp đèn giao thông
void clearAllLEDs(void) {
    HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin | LED_YELLOW1_Pin | LED_GREEN1_Pin |
                             LED_RED2_Pin | LED_YELLOW2_Pin | LED_GREEN2_Pin, GPIO_PIN_RESET);
}

void toggleRedLEDs(void) {
    HAL_GPIO_WritePin(GPIOA, LED_YELLOW1_Pin | LED_GREEN1_Pin |
                             LED_YELLOW2_Pin | LED_GREEN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_TogglePin(GPIOA, LED_RED1_Pin | LED_RED2_Pin);
}

void toggleYellowLEDs(void) {
    HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin | LED_GREEN1_Pin |
                             LED_RED2_Pin | LED_GREEN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_TogglePin(GPIOA, LED_YELLOW1_Pin | LED_YELLOW2_Pin);
}

void toggleGreenLEDs(void) {
    HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin | LED_YELLOW1_Pin |
                             LED_RED2_Pin | LED_YELLOW2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_TogglePin(GPIOA, LED_GREEN1_Pin | LED_GREEN2_Pin);
}

void setLedRed1Green2(void) {
    clearAllLEDs();
    HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin | LED_GREEN2_Pin, GPIO_PIN_SET);
}

void setLedRed1Amber2(void) {
    clearAllLEDs();
    HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin | LED_YELLOW2_Pin, GPIO_PIN_SET);
}

void setLedGreen1Red2(void) {
    clearAllLEDs();
    HAL_GPIO_WritePin(GPIOA, LED_GREEN1_Pin | LED_RED2_Pin, GPIO_PIN_SET);
}

void setLedAmber1Red2(void) {
    clearAllLEDs();
    HAL_GPIO_WritePin(GPIOA, LED_YELLOW1_Pin | LED_RED2_Pin, GPIO_PIN_SET);
}

#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

// khởi tạo ban đầu là ở mode 1 chạy bình thường
int status = MODE_1;

// Hướng 1 (Bắc - Nam / Axis 1)
int green1_time = 13;
int amber1_time = 2;
int red1_time   = 15;

// Hướng 2 (Đông - Tây / Axis 2)
int green2_time = 13;
int amber2_time = 2;
int red2_time   = 15;

// Biến hiệu chỉnh tạm thời ở Mode 2, 3, 4
int temp_red1   = 15;
int temp_amber  = 2;
int temp_green1 = 13;

// Sub-FSM Trạng thái chạy tự động Mode 1
enum SubState {
    AUTO_RED1_GREEN2,
    AUTO_RED1_AMBER2,
    AUTO_GREEN1_RED2,
    AUTO_AMBER1_RED2
};
static enum SubState sub_status = AUTO_RED1_GREEN2;

// Biến đếm ngược thời gian thực tế cho 2 hướng
static int count_road1 = 15;
static int count_road2 = 13;

// FSM Nút bấm chống dội
enum ButtonState { BTN_RELEASED, BTN_PRESSED };
static enum ButtonState btn1_state = BTN_RELEASED;
static enum ButtonState btn2_state = BTN_RELEASED;
static enum ButtonState btn3_state = BTN_RELEASED;

// Khởi chạy lại MODE 1 sau khi chỉnh xong các thông số
void reset_mode_1(void) {
    // Tính toán thời gian Đỏ theo quy tắc chéo bất đối xứng
    red1_time = green2_time + amber2_time;
    red2_time = green1_time + amber1_time;

    sub_status = AUTO_RED1_GREEN2;
    count_road1 = red1_time;
    count_road2 = green2_time;
    setLedRed1Green2();
    setTimer2(1000);
}

// Sub-FSM chạy tự động chu kỳ giao thông ở MODE 1
void fsm_for_mode1_auto(void) {
    if (timer2_flag == 1) {
        timer2_flag = 0;
        setTimer2(1000);

        count_road1--;
        count_road2--;

        switch (sub_status) {
            case AUTO_RED1_GREEN2:
                setLedRed1Green2();
                if (count_road2 <= 0) {
                    sub_status = AUTO_RED1_AMBER2;
                    count_road2 = amber2_time;
                    setLedRed1Amber2();
                }
                break;

            case AUTO_RED1_AMBER2:
                setLedRed1Amber2();
                if (count_road2 <= 0) {
                    sub_status = AUTO_GREEN1_RED2;
                    count_road1 = green1_time;
                    count_road2 = red2_time;
                    setLedGreen1Red2();
                }
                break;

            case AUTO_GREEN1_RED2:
                setLedGreen1Red2();
                if (count_road1 <= 0) {
                    sub_status = AUTO_AMBER1_RED2;
                    count_road1 = amber1_time;
                    setLedAmber1Red2();
                }
                break;

            case AUTO_AMBER1_RED2:
                setLedAmber1Red2();
                if (count_road1 <= 0) {
                    sub_status = AUTO_RED1_GREEN2;
                    count_road1 = red1_time;
                    count_road2 = green2_time;
                    setLedRed1Green2();
                }
                break;
        }
    }
}

// Tăng biến tạm thời theo từng Mode (1 - 99)
static void increment_temp_value(void) {
    if (status == MODE_2) {
        temp_red1++;
        if (temp_red1 > 99) temp_red1 = 1;
    } else if (status == MODE_3) {
        temp_amber++;
        if (temp_amber > 99) temp_amber = 1;
    } else if (status == MODE_4) {
        temp_green1++;
        if (temp_green1 > 99) temp_green1 = 1;
    }
}

// cập nhật giá trị led 7 segment và chớp led 2hz
void fsm_for_led_blinking(void) {
    switch (status) {
        case MODE_1: // MODE 1: EN1-EN2 hiển thị đếm Hướng 1, EN3-EN4 hiển thị đếm Hướng 2
            mode_display_value = count_road1;
            time_display_value = count_road2;
            fsm_for_mode1_auto();
            break;
        case MODE_2: // MODE 2: EN1-EN2 hiển thị Mode 02, EN3-EN4 hiển thị temp_red1
            mode_display_value = 2;
            time_display_value = temp_red1;
            break;
        case MODE_3: // MODE 3: EN1-EN2 hiển thị Mode 03, EN3-EN4 hiển thị temp_amber
            mode_display_value = 3;
            time_display_value = temp_amber;
            break;
        case MODE_4: // MODE 4: EN1-EN2 hiển thị Mode 04, EN3-EN4 hiển thị temp_green1
            mode_display_value = 4;
            time_display_value = temp_green1;
            break;
    }
    // xử lý đèn chớp 2Hz (kích hoạt mỗi 250ms qua timer1_flag)
    if (timer1_flag == 1) {
        timer1_flag = 0;
        setTimer1(250);

        switch (status) {
            case MODE_1: // không nháy đèn
                break;
            case MODE_2: // chớp các LED Đỏ ở Mode 2
                toggleRedLEDs();
                break;
            case MODE_3: // chớp các LED Vàng ở Mode 3
                toggleYellowLEDs();
                break;
            case MODE_4: // chớp các LED Xanh ở Mode 4
                toggleGreenLEDs();
                break;
        }
    }
}

// FSM xử lý nút bấm chính cho toàn hệ thống
void fsm_for_input_processing(void) {
	// hiển thị led 7 segment và led blinking
    fsm_for_led_blinking();

    // 1. NÚT 1: CHUYỂN MODE (MODE 1 -> 2 -> 3 -> 4 -> 1)
    switch (btn1_state) {
		case BTN_RELEASED:
			if (is_button_pressed(0)) {
				btn1_state = BTN_PRESSED;
				status++;
				if (status > MODE_4) status = MODE_1;
				clearAllLEDs();

				if (status == MODE_1) {
					reset_mode_1();
				} else if (status == MODE_2) {
					temp_red1 = red1_time;
				} else if (status == MODE_3) {
					temp_amber = amber1_time;
				} else if (status == MODE_4) {
					temp_green1 = green1_time; // Nạp thời gian Green 1 vào temp
				}
			}
			break;

		case BTN_PRESSED:
			if (!is_button_pressed(0)) {
				btn1_state = BTN_RELEASED;
			}
			break;
	}

    // 2. NÚT 2: TĂNG GIÁ TRỊ TẠM THỜI (Hỗ trợ Nhấn giữ >1s tăng mỗi 0.5s)
    switch (btn2_state) {
		case BTN_RELEASED:
			if (is_button_pressed(1)) {
				btn2_state = BTN_PRESSED;
				increment_temp_value();
			}
			break;

		case BTN_PRESSED:
			if (is_button_pressed(1)) {
				if (is_button_pressed_1s(1)) {
					static uint16_t auto_inc_counter = 0;
					auto_inc_counter++;
					// 50 nhịp ngắt 10ms = 500ms = 500ms tự động tăng 1 lần
					if (auto_inc_counter >= (500 / TIMER_CYCLE)) {
						auto_inc_counter = 0;
						increment_temp_value();
					}
				}
			} else {
				btn2_state = BTN_RELEASED;
			}
			break;
	}

    // 3. NÚT 3: LƯU GIÁ TRỊ VÀ TỰ ĐỘNG TÍNH RÀNG BUỘC BẤT ĐỐI XỨNG
    switch (btn3_state) {
            case BTN_RELEASED:
                if (is_button_pressed(2)) {
                    btn3_state = BTN_PRESSED;

                    if (status == MODE_2) {
                        red1_time = temp_red1;
                        if (red1_time > amber2_time) {
                            green2_time = red1_time - amber2_time;
                        } else {
                            green2_time = 1;
                            red1_time = green2_time + amber2_time;
                            temp_red1 = red1_time;
                        }
                    } else if (status == MODE_3) {
                        amber1_time = temp_amber;
                        amber2_time = temp_amber;
                        red1_time = green2_time + amber2_time;
                        red2_time = green1_time + amber1_time;
                    } else if (status == MODE_4) {
                        // Lưu thời gian Green 1
                        green1_time = temp_green1;
                        // Tự động tính Red 2 theo công thức bất đối xứng
                        red2_time = green1_time + amber1_time;
                    }
                }
                break;

            case BTN_PRESSED:
                if (!is_button_pressed(2)) {
                    btn3_state = BTN_RELEASED;
                }
                break;
        }
}

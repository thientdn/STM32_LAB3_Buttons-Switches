# STM32 Asymmetric Traffic Light Controller System (Lab 3)

An embedded real-time traffic light control system developed for the **STM32F103C6** microcontroller (ARM Cortex-M3 core). This project implements a **Hierarchical Finite State Machine (HFSM)** architecture featuring non-blocking execution, hardware-independent modular software timers, 2-stage sampling button debouncing, time-multiplexed 7-segment displays, and an **asymmetric cross-axis traffic timing model**.

---

## 📌 Features & Highlights

- **4-Mode Operational State Machine**:
  - **MODE 1 (Normal Mode)**: Automatic asymmetric traffic cycle countdown execution across North-South (Axis 1) and East-West (Axis 2) approaches.
  - **MODE 2 (Modify N-S Red Duration)**: Adjusts $T_{\text{Red1}}$ ($1\text{--}99\text{s}$) with $2\text{ Hz}$ blinking Red LEDs.
  - **MODE 3 (Modify Shared Amber Duration)**: Adjusts shared clearance interval $T_{\text{Amber}}$ ($1\text{--}99\text{s}$) with $2\text{ Hz}$ blinking Yellow LEDs.
  - **MODE 4 (Modify N-S Green Duration)**: Adjusts $T_{\text{Green1}}$ ($1\text{--}99\text{s}$) with $2\text{ Hz}$ blinking Green LEDs.
- **Asymmetric Timing Model**: Supports different traffic density profiles for primary vs. secondary thoroughfares ($T_{\text{Green1}} \neq T_{\text{Green2}}$ and $T_{\text{Red1}} \neq T_{\text{Red2}}$) while maintaining strict collision safety rules.
- **Hardware-Independent Software Timers**: All high-level periodic tasks scale dynamically using a centralized `TIMER_CYCLE` ($10\text{ ms}$) hardware base interrupt.
- **Robust Button Debouncing & Long-Press Auto-Increment**:
  - Non-blocking 2-stage sampling algorithm ($20\text{ ms}$ threshold).
  - Holding Button 2 down for $> 1\text{s}$ triggers an auto-increment feature stepping values every $0.25\text{s}$ ($250\text{ ms}$).
- **Optimized 7-Segment Display Multiplexing**: Time-multiplexed driver for Common-Anode displays calibrated for CPU-friendly execution in Proteus simulations ($25\text{ Hz}$ refresh rate).

---

## 📐 Mathematical Timing Constraints

To prevent orthogonal traffic collisions at the 4-way intersection, all timing parameters satisfy the cross-axis conservation laws:

$$T_{\text{Red1}} = T_{\text{Green2}} + T_{\text{Amber}}$$
$$T_{\text{Red2}} = T_{\text{Green1}} + T_{\text{Amber}}$$

Where:
- $T_{\text{Amber1}} = T_{\text{Amber2}} = T_{\text{Amber}}$ is the shared clearance warning interval.
- Total Intersection Cycle Duration: $C = T_{\text{Green1}} + T_{\text{Green2}} + 2 \times T_{\text{Amber}}$.

---

## 🛠️ Hardware Specifications & Pin Mapping

| Functional Group | Net Label | STM32 Pin | GPIO Mode / Description |
| :--- | :--- | :--- | :--- |
| **Push Buttons** | `BUTTON_1` | `PB0` | GPIO_Input (Pull-Up) - Mode Select |
| | `BUTTON_2` | `PB1` | GPIO_Input (Pull-Up) - Value Increment |
| | `BUTTON_3` | `PB2` | GPIO_Input (Pull-Up) - Save / Set Parameter |
| **Traffic LEDs** | `LED_RED1` | `PA2` | GPIO Output - N-S Red LEDs |
| | `LED_YELLOW1` | `PA3` | GPIO Output - N-S Yellow LEDs |
| | `LED_GREEN1` | `PA4` | GPIO Output - N-S Green LEDs |
| | `LED_RED2` | `PA5` | GPIO Output - E-W Red LEDs |
| | `LED_YELLOW2` | `PA6` | GPIO Output - E-W Yellow LEDs |
| | `LED_GREEN2` | `PA7` | GPIO Output - E-W Green LEDs |
| **7-Seg Bus** | `A - G` | `PA8 - PA14` | GPIO Output - Shared Segment Data Bus ($a-g$) |
| **Digit Enables** | `EN1` | `PB4` | GPIO Output - Digit 1 (N-S Tens / Mode Tens) |
| | `EN2` | `PB5` | GPIO Output - Digit 2 (N-S Units / Mode Units) |
| | `EN3` | `PB6` | GPIO Output - Digit 3 (E-W Tens / Time Tens) |
| | `EN4` | `PB7` | GPIO Output - Digit 4 (E-W Units / Time Units) |

---

## 📁 Project Architecture

```text
Core/
├── Inc/
│   ├── main.h                 # HAL definitions and GPIO pin assignments
│   ├── timer.h                # Software timer module header
│   ├── input_reading.h        # Button debouncing header
│   ├── led_display.h          # 7-Segment and LED control header
│   └── input_processing.h     # FSM & state machine declarations
└── Src/
    ├── main.c                 # System entry point & TIM2 interrupt callback
    ├── timer.c                # Modular software timers implementation
    ├── input_reading.c        # 2-stage sampling button debouncer
    ├── led_display.c          # Display multiplexing & LED drivers
    └── input_processing.c     # Main FSM and Sub-FSM state transitions
```

---

## 🚀 How to Build & Simulate

### 1. STM32CubeIDE Setup
1. Open **STM32CubeIDE** and import the project folder.
2. Ensure Timer 2 (`TIM2`) is configured with:
   - **Prescaler**: `7999`
   - **Counter Period (ARR)**: `9` (Generates a $10\text{ ms}$ interrupt at $8\text{ MHz}$ internal clock).
   - **NVIC**: `TIM2 global interrupt` enabled.
3. Build the project (`Ctrl + B`) to generate the target `.elf` or `.hex` file inside the `Debug/` directory.

### 2. Proteus Simulation
1. Open the Proteus schematic file (`Lab3_Schematic.pdsprj`).
2. Double-click the `STM32F103C6` microcontroller block.
3. Set the **Program File** property to the compiled `Lab3.hex` or `Lab3.elf` file.
4. Click **Play** to run the simulation.

---

## 👨‍💻 Author Info

- **Student Name**: Cao Thiên
- **Student ID**: 2453192
- **Class**: CC02
- **Course**: Microcontrollers (Semester 261)
- **Faculty**: Computer Science and Engineering
- **Institution**: Ho Chi Minh City University of Technology (HCMUT)
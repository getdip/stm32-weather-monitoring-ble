# 🌦️ Real-Time Weather Monitoring System (STM32 | Bare-Metal | BLE Controlled)

A fully bare-metal embedded system built on STM32 (ARM Cortex-M4) for real-time weather monitoring with BLE-based command control.  
This project demonstrates low-level driver development, interrupt-driven communication, and structured embedded system design.

---

## 🚀 Features

- 📡 Real-time monitoring of:
  - Temperature
  - Pressure
  - Humidity
  - Date & Time

- 🔵 BLE-based wireless control (HM-10)
- 🔁 Interrupt-driven UART communication (TX/RX)
- 🔄 Circular buffer-based command parsing
- 🧠 Finite State Machine (FSM)-based application logic
- 🖥️ LCD display for local visualization

---

## 🛠️ Hardware Used

- STM32F407 (ARM Cortex-M4)
- BME280 (Temperature, Pressure, Humidity sensor)
- DS1307 (RTC with battery backup)
- HM-10 BLE Module
- 16x2 LCD
- External Power Supply (12V → 3.3V)

---

## 📁 Project Structure
/
├── inc/
│ ├── command_parser.h
│ ├── format_utils.h
│ ├── bme280.h
│ ├── ds1307.h
│ ├── hm10.h
│ ├── lcd.h
│ ├── stm32f407xx.h
│ ├── stm32f407xx_i2c_driver.h
│ ├── stm32f407xx_usart_driver.h
│ ├── stm32f407xx_gpio_driver.h
│ ├── stm32f407xx_rcc_driver.h
│
├── src/
│ ├── application/
│ │ ├── app.c
│ │ ├── command_parser.c
│ │ ├── format_utils.c
│ │ ├── syscalls.c
│ │ ├── sysmem.c
│ │
│ ├── bsp/
│ │ ├── bme280.c
│ │ ├── ds1307.c
│ │ ├── hm10.c
│ │ ├── lcd.c
│ │
│ ├── drivers/
│ ├── i2c_driver.c
│ ├── stm32f407xx_gpio_driver.c
│ ├── stm32f407xx_rcc_driver.c
│ ├── stm32f407xx_usart_driver.c
│
├── linker/
│ ├── STM32F407VGTX_FLASH.ld
│ ├── STM32F407VGTX_RAM.ld
│
├── startup/
│ ├── startup_stm32f407vgtx.s
│
├── .gitignore
├── README.md


## ⚙️ System Architecture

The project follows a **layered architecture**:

- **Application Layer (`/src/application`)**
  - Core logic, state machine, command parsing
  - Includes system call interface (`syscalls.c`, `sysmem.c`)

- **BSP Layer (`/src/bsp`)**
  - Sensor and peripheral abstraction (BME280, DS1307, HM-10, LCD)

- **Driver Layer (`/src/drivers`)**
  - Register-level peripheral drivers (GPIO, I2C, USART, RCC)

---

## 🔧 Low-Level Driver Implementation

All peripheral drivers are implemented **from scratch (no HAL)**:

- RCC (Clock configuration)
- GPIO (Input/Output + Interrupt support)
- USART (Interrupt-based TX/RX)
- I2C (Master communication for sensors)

---

## 🔄 Communication Design

### UART (Interrupt-Driven)
- RX handled using **RXNE interrupt**
- TX handled using **TXE + TC interrupts**
- Ensures **non-blocking communication**

### Circular Buffer
- Uses `read_head` and `write_head`
- Enables continuous and asynchronous command parsing

---

## 📲 BLE Command Interface

Commands are received via BLE (HM-10) using `#` as delimiter.

### Supported Commands

| Command | Description |
|--------|------------|
| `ON#` | Turn ON device (STATE_ACTIVE) |
| `OFF#` | Turn OFF device (STATE_IDLE) |
| `stream#` | Start data streaming |
| `stop#` | Stop streaming |
| `settime:hh:mm:ss AM/PM` | Set RTC time |
| `setdate:dd/mm/yy day` | Set RTC date |

- Invalid formats for date and time commands, revert to default values
- Command parser supports continuous input stream

---

## 🧠 Application Flow

1. System initializes peripherals and sensors
2. SysTick configured for 1 ms tick
3. NVIC configured for USART interrupts
4. Enters **superloop architecture**
5. Interrupts handle UART communication
6. FSM manages system states and behavior

---

## 🔁 State Machine

- **STATE_IDLE**
  - Default state
  - Device OFF
  - Static display

- **STATE_ACTIVE**
  - Displays real-time sensor data

- **STATE_STREAMING**
  - Enables streaming over BLE

- **STATE_ERROR**
    - At receive buffer overflows OR UART Overrun error

---

## 🌡️ Sensor Implementation

### DS1307 (RTC)
- Battery-backed timekeeping
- Uses BCD format
- Runs independently after initialization

### BME280
- Configured in **forced mode**
- Steps:
  - Calibration register read
  - Raw ADC acquisition
  - Compensation formula implementation (datasheet-based)

---

## 📤 System I/O (Newlib Integration)

- `syscalls.c` → Redirects standard I/O (e.g., `printf`) through the `_write()` system call. Instead of using UART, debug output is transmitted via the **ITM (Instrumentation Trace Macrocell)** unit of the Cortex-M4, enabling high-speed, low-overhead tracing through the SWD (Serial Wire Debugger) interface. 
- `sysmem.c` → Manages heap allocation (`_sbrk`) for dynamic memory (Not used in the application)

---

## 📺 Output

- LCD displays:
  - Date & Time
  - Temperature, Pressure, Humidity

- BLE interface:
  - Receives commands from mobile
  - Streams real-time sensor data

---

## 🧪 Key Learnings

- Interrupt-driven embedded system design
- TXE vs TC handling in UART communication
- Circular buffer implementation
- Sensor interfacing using datasheets
- FSM-based application control
- Bare-metal memory and I/O handling (newlib syscalls)

---

## 🔧 Build & Flash

### Option 1: STM32CubeIDE (Recommended)
1. Open STM32CubeIDE
2. Import project:
   - File → Import → Existing Projects into Workspace
3. Build the project
4. Flash using ST-Link

### Option 2: Manual Setup
1. Create a new STM32 project
2. Add source and header files
3. Configure include paths and linker script
4. Build and flash

## 🤝 Contributions

Suggestions and improvements are welcome!

---

## 📬 Contact

Feel free to connect if you're working on similar embedded systems or have feedback.
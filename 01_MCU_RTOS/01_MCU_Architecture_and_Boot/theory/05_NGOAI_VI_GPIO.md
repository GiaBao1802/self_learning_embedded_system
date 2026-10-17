# 05 — Peripheral, GPIO/pin mux và điện

## 1. Peripheral là state machine phần cứng

GPIO, UART, SPI, I2C, timer, PWM, ADC và watchdog không phải “hàm thư viện”. Chúng là khối phần cứng có register cấu hình, status, data và interrupt. Driver là lớp phần mềm tạo giao dịch register đúng thứ tự và quản lý lỗi/timing/ownership. Trước khi dùng peripheral, xác định clock/reset, pin routing, register map, IRQ/DMA, mode và electrical constraints.

## 2. Ba lớp của một chân ESP32

```text
GPIO output latch/peripheral signal → GPIO Matrix → IO MUX → pad → mạch ngoài
                                               input path ngược lại → GPIO_IN
```

GPIO18 có IO MUX `0x3FF49070`. `MCU_SEL` tại bit 12–14 chọn function 2 cho GPIO matrix; `FUN_PU` bit 8 và `FUN_PD` bit 7 là pull nội; `FUN_IE` bit 9 bật input path. `GPIO_FUNC18_OUT_SEL_CFG_REG` ở `0x3FF44578`: bit 0–8 chọn output signal; giá trị **256** chọn GPIO software output, bit 10 chọn output enable từ GPIO enable register (theo ESP-IDF HAL). `GPIO_ENABLE_W1TS_REG` bật output driver; `GPIO_OUT_W1TS/W1TC_REG` set/clear latch.

Với GPIO input, nên cấu hình input enable/pull ở IO MUX, tắt output enable, rồi đọc `GPIO_IN_REG`. Với open-drain, còn phải hiểu pull-up và mức 1 có thể chỉ là nhả đường, không chủ động cấp mức cao. Bài lab chỉ cho phép GPIO18, 19, 21, 22, 23 để tránh pin strapping/Flash/input-only trong phạm vi học. Với board khác, đối chiếu schematic trước khi dùng.

## 3. Điện và giới hạn mô phỏng

`1` trên GPIO là mức logic của pad theo nguồn cấp/chế độ, không phải nguồn dòng vô hạn. LED cần điện trở hạn dòng. Input floating có thể dao động; pull-up/down tạo trạng thái mặc định nhưng giá trị pull nội không thay thế thiết kế điện chính xác. Kiểm tra voltage absolute maximum, input thresholds, output current và power-up state trong datasheet. Wokwi mô phỏng kết nối/logic, còn dòng, EMI, signal integrity và timing vật lý cần board thật cùng thiết bị đo.

## 4. Mẫu đọc datasheet cho mọi peripheral

1. Xác định chip/revision/board/module.
2. Tìm block diagram và đường clock/reset/bus.
3. Tìm base address/register fields, reset value, RO/RW/WO và side effect.
4. Tìm pin mapping, electrical/timing constraints, interrupt/DMA.
5. Viết sequence init → use → error/recovery → deinit và test case tương ứng.

Áp dụng mẫu này sang UART/SPI/I2C/timer tại [`04_Timers_PWM_UART_SPI_I2C`](../../04_Timers_PWM_UART_SPI_I2C/README.md).

**Nguồn:** [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) §2, §4.8, §4.10, §5; [TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf) Chapter 6; [ESP-IDF GPIO API](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html).

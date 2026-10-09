# 04 — Timer, PWM, UART, SPI và I2C

Học peripheral theo góc nhìn kiến trúc: nguồn clock, timing, cấu hình, giao thức, buffering, timeout, lỗi bus và interface của driver.

## Thứ tự học

1. Hardware timer và tick; phân biệt delay tương đối với chu kỳ được lập lịch.
2. PWM: tần số, duty cycle, độ phân giải và giới hạn đầu ra.
3. UART: baud rate, frame, timeout, ring buffer và giao thức console.
4. I2C: địa chỉ, pull-up, ACK/NACK, clock stretching và phục hồi bus.
5. SPI: mode, clock, chip select, throughput và ownership của bus.

## Đầu ra và bài đọc

- Roadmap: tuần 3, 5–6; chi tiết tài liệu theo SoC sẽ được ghi cùng từng lab.
- [ESP-IDF Peripheral API Reference](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/index.html)
- Đọc chương tương ứng trong ESP32 TRM và datasheet từ trang tài liệu ở [`01_MCU_Architecture_and_Boot/README.md`](../01_MCU_Architecture_and_Boot/README.md); không áp dụng register map của ESP32 cho chip khác.
- Đầu ra: sensor/console demo, timing budget, interface spec và bảng xử lý timeout/lỗi.

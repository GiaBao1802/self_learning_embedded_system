# 03 — GPIO và pin mux

Học điện và logic ở ranh giới giữa firmware với chân chip: input/output, pull-up/down, open-drain, pin mux, GPIO matrix, active-high/low và giới hạn điện. Tuần 1 có bài GPIO LED đầu tiên trong [`01_MCU_Architecture_and_Boot/Week_01`](../01_MCU_Architecture_and_Boot/Week_01/README.md); các tuần sau quay lại GPIO input, button, debounce và interrupt.

## Đầu ra cần đạt

- Vẽ sơ đồ từ output latch đến pad và tải; phân biệt mức logic với điện áp/dòng cho phép.
- Chọn GPIO sau khi kiểm tra datasheet, pin strapping, chức năng alternate và đường mạch trên board.
- Cấu hình GPIO qua API driver, kiểm tra error path và đồng bộ source với wiring trong `diagram.json`.
- Mô tả rõ giới hạn của Wokwi so với datasheet và kiểm thử trên phần cứng.

## Tài liệu đọc

- ESP32 Datasheet: mục 2.2–2.3 (pin và I/O), 4.8.1 (GPIO), 4.10 (pin configuration), 5 (electrical characteristics).
- ESP32 TRM: Chapter 6, “IO MUX and GPIO Matrix”. Đọc phần overview trước, chỉ xem register khi cần.
- [ESP-IDF GPIO API](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html)
- [Wokwi ESP32 guide](https://docs.wokwi.com/guides/esp32)

Datasheet/TRM: xem liên kết tại [`01_MCU_Architecture_and_Boot/README.md`](../01_MCU_Architecture_and_Boot/README.md).

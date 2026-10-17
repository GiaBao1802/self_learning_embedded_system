# 02 — Thanh ghi, memory map và truy cập bus

## 1. Ba loại register không được trộn lẫn

- **CPU architectural register:** PC, stack pointer, register tổng dụng; instruction CPU thao tác trực tiếp.
- **Peripheral register:** vùng MMIO của GPIO/UART/timer; CPU dùng load/store qua địa chỉ để cấu hình hoặc đọc trạng thái.
- **C variable:** dữ liệu trong RAM; có thể được compiler giữ tạm trong CPU register nếu quy tắc ngôn ngữ cho phép.

Trong lab, `*(volatile uint32_t *)0x3FF44008 = mask` là một lần ghi tới vùng MMIO GPIO của **ESP32 gốc**. Đây không phải địa chỉ hợp lệ chung cho mọi MCU. Address map nằm trong TRM; số offset chỉ có ý nghĩa khi cộng đúng base.

## 2. Đọc register table theo thứ tự

Với mỗi register, ghi vào bảng: tên; base + offset; độ rộng; quyền truy cập; reset value; ý nghĩa từng bit; side effect khi đọc/ghi; clock/reset dependency; owner; yêu cầu đồng bộ. `RO` không ghi, `WO` không đọc để “xác nhận”, `W1TS` viết bit 1 để set, `W1TC` viết bit 1 để clear. Giá trị 0 ở bit khác **không** tác động tới chúng.

Ví dụ ESP32 GPIO (TRM Chapter 6):

| Register | Địa chỉ | Ý nghĩa |
|---|---:|---|
| `GPIO_OUT_REG` | `0x3FF44004` | mức output latch GPIO0–31, R/W |
| `GPIO_OUT_W1TS_REG` | `0x3FF44008` | ghi 1 để set bit output, WO |
| `GPIO_OUT_W1TC_REG` | `0x3FF4400C` | ghi 1 để clear bit output, WO |
| `GPIO_ENABLE_W1TS_REG` | `0x3FF44024` | ghi 1 để bật output driver, WO |
| `GPIO_ENABLE_W1TC_REG` | `0x3FF44028` | ghi 1 để tắt output driver, WO |
| `GPIO_IN_REG` | `0x3FF4403C` | đọc trạng thái input GPIO0–31, RO |
| `GPIO_PIN18_REG` | `0x3FF440D0` | cấu hình pad/interrupt riêng GPIO18 |
| `GPIO_FUNC18_OUT_SEL_CFG_REG` | `0x3FF44578` | chọn tín hiệu output và output-enable cho GPIO18 |
| `IO_MUX_GPIO18_REG` | `0x3FF49070` | function/pull/input-enable của pad GPIO18 |

Base GPIO là `0x3FF44000`, IO MUX là `0x3FF49000`; kiểm tra lại trong `components/soc/esp32/register/soc/reg_base.h`, `gpio_reg.h` và `io_mux_reg.h` của ESP-IDF 6.1. TRM là nguồn phần cứng, header là cách SDK biểu diễn nguồn đó. Nếu hai nguồn khác nhau, dừng và kiểm tra đúng chip/revision/SDK.

## 3. Vì sao dùng W1TS/W1TC

Với register R/W chung, `reg |= mask` tạo chuỗi read → modify → write. Core khác hoặc ISR có thể sửa một bit khác ở giữa; lần write cuối làm mất cập nhật. Register W1TS/W1TC cho phép ghi trực tiếp mask và chỉ tác động các bit 1, phù hợp GPIO output/enable. Đây là tính nguyên tử của **thao tác từng bit trong register này theo mô tả phần cứng**, không phải lời hứa mọi thao tác driver hay mọi bus đều atomic.

Không viết `GPIO_OUT_W1TS_REG |= mask`: đây là WO; cú pháp `|=` sẽ đọc trước. Chỉ ghi `= mask`.

## 4. `volatile`, alignment và thứ tự

Con trỏ MMIO phải có kiểu và độ rộng đúng, ví dụ `volatile uint32_t *`. Địa chỉ 32-bit cần alignment 4 byte. `volatile` bảo toàn truy cập MMIO khỏi bị tối ưu bỏ/gộp theo nghĩa ngôn ngữ C; nó không thay thế lock, atomic cho state chia sẻ, hay cache maintenance cho DMA. Nếu nhiều task cùng sở hữu configuration register R/W, đặt owner rõ và bảo vệ chuỗi read-modify-write. Tránh ghi reserved bits; giữ nguyên giá trị ngoài mask.

## 5. Cách đối chiếu tài liệu

TRM Chapter 6 mô tả `GPIO_OUT_W1TS`, `GPIO_ENABLE_W1TS`, `GPIO_IN`, `GPIO_FUNC18_OUT_SEL_CFG` và IO MUX GPIO18. Header `gpio_reg.h` tính `GPIO_FUNC18_OUT_SEL_CFG_REG = base + 0x578`. `1U << 18` là mask bit GPIO18 trong bank 0. Phần tự tra và chứng minh được đưa vào bài tập sau lab.

**Nguồn:** [ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf), [ESP-IDF GPIO API](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html).

# Hồ sơ V-model mẫu — Driver GPIO MMIO ESP32

Đây là một ví dụ nhỏ nhưng đủ đường truy vết. Bắt đầu từ yêu cầu có thể đo/kiểm, rồi thiết kế, code, test và lưu kết quả. V-model không có nghĩa chờ code xong mới nghĩ test: định nghĩa test cùng lúc với requirement và design.

## 1. Scope và giả định

SoC: ESP32 gốc, target `esp32` trong ESP-IDF 6.1. Mạch Wokwi: GPIO18 → 1 kΩ → LED → GND. Thư viện chỉ sở hữu GPIO18/19/21/22/23 do ứng dụng cấp quyền; không dùng cùng pin với ESP-IDF GPIO driver/peripheral khác. Clock/boot/scheduler do ESP-IDF quản lý. Chưa có board thật nên mục điện và worst-case timing để trạng thái **chưa xác nhận trên hardware**.

## 2. Yêu cầu và acceptance criteria

| ID | Yêu cầu | Tiêu chí chấp nhận |
|---|---|---|
| SYS-GPIO-01 | LED GPIO18 đổi mức mỗi lần ứng dụng yêu cầu | Mô phỏng thấy LED sáng/tắt và log tương ứng; board thật cần kiểm tra pad |
| SW-GPIO-01 | Chỉ cho phép GPIO18/19/21/22/23 | Pin khác trả `INVALID_PIN`, không ghi MMIO |
| SW-GPIO-02 | Cấu hình output không tạo xung do latch chưa khởi tạo | Latch initial level được ghi trước output enable; glitch vật lý cần đo riêng |
| SW-GPIO-03 | Ghi một pin không đổi bit pin khác | Dùng W1TS/W1TC với mask một bit; host test xác nhận địa chỉ/mask |
| SW-GPIO-04 | Input có none/up/down và trả mức pad | IO MUX pull/input bit đúng; đọc `GPIO_IN`, null pointer bị từ chối |
| SW-GPIO-05 | Deinit đưa pad về input không pull, latch low | `ENABLE_W1TC` → `OUT_W1TC` → IO MUX update |

## 3. Kiến trúc và detailed design

```text
app_main → mmio_gpio public API → register sequence → mmio_port_read32/write32
                                                     ├─ ESP32 volatile MMIO (target)
                                                     └─ fake register bank (host test)
```

`mmio_gpio.c` chứa address/bit/sequence. `mmio_port_esp32.c` là điểm chạm phần cứng. Host test thay file port bằng fake để quan sát địa chỉ, dữ liệu và thứ tự ghi. IO MUX và FUNC_OUT là register R/W nên dùng read-modify-write để giữ bit khác; W1TS/W1TC dùng ghi trực tiếp. Caller serialize việc cấu hình pin, vì thư viện không cung cấp lock. `write` chỉ hợp lệ sau `config_output`; `read` chỉ hợp lệ sau `config_input` hoặc khi input path đã bật theo thiết kế.

## 4. Traceability matrix

| Requirement | Thiết kế/code | Verification | Evidence hiện có / cần có |
|---|---|---|---|
| SYS-GPIO-01 | `main.c`, `diagram.json` | IT-01 Wokwi LED/log; AT-01 board thật | Build đã đạt; Wokwi: người học ghi log/screenshot; board thật: pending |
| SW-GPIO-01 | `io_mux_address()`, validate trước MMIO | UT-01 pin 20/6 không ghi | Test source có sẵn; thực thi host pending |
| SW-GPIO-02 | `config_output()` tắt output, đặt OUT trước khi bật lại ENABLE | UT-02 thứ tự ghi; HT-01 đo glitch | Test source có sẵn; thực thi host/HT-01 pending |
| SW-GPIO-03 | `write()` dùng W1TS/W1TC | UT-03 mask/địa chỉ, IT-01 | Test source có sẵn; host/Wokwi pending |
| SW-GPIO-04 | `config_input()`, `read()` | UT-04 pull/input/read/null; IT-02 button | Test source có sẵn; host pending; Wokwi button là bài tập 3 |
| SW-GPIO-05 | `deinit()` | UT-05 sequence/mask | Test source có sẵn; thực thi host pending |

UT = unit test host fake MMIO; IT = integration test trong mô phỏng; HT = hardware measurement; AT = acceptance test. Test dùng mock chứng minh **lệnh ghi mà phần mềm định tạo**, không chứng minh electrical behavior của pad.

## 5. Test procedure và review gate

1. **Review requirement:** ID duy nhất; input, output, điều kiện lỗi, giới hạn chip và pin rõ; test được định nghĩa trước code.
2. **Review design:** address/offset/bit đối chiếu TRM; pin owner và race policy; boot-safe output state; lỗi/timeout.
3. **Unit:** compile native với warning-as-error; chạy fake MMIO và lưu log PASS/FAIL. Bổ sung boundary/negative cases khi API đổi.
4. **Integration:** `idf.py build`, Wokwi LED/log. Khi thêm button, dùng kịch bản bấm/thả/bounce.
5. **System/hardware:** nạp lên board ESP32 đúng module; đo pad, voltage/current, startup glitch, timing min/mean/max và reset types. Các mục này chưa được Wokwi xác nhận.
6. **Release:** traceability không còn yêu cầu chưa có test hoặc deviation; lưu commit hash, ESP-IDF version, chip revision, board/wiring, tool version, log và kết luận.

## 6. Các lỗi cố ý cần phát hiện trong review

- Dùng register base của ESP32-C3/S3 cho ESP32 gốc.
- Đọc WO W1TS/W1TC bằng cú pháp `|=`.
- Bật output trước khi đặt latch an toàn.
- Hai module cùng cấu hình một pin.
- Cho rằng `volatile` đã giải quyết race trên RMW register.
- Ghi “đã test hardware” trong khi chỉ mới build hoặc mô phỏng.

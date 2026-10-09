# Bài tập và quiz — Tuần 1 MCU/GPIO

Hãy làm phần bài tập trước khi đọc đáp án. Với mỗi thay đổi code, ghi expected result, sau đó mô phỏng và so sánh actual result.

## Bài tập thực hành

### Bài 1 — Thay đổi GPIO

Đổi `LED_GPIO` từ GPIO18 sang GPIO19 và cập nhật `diagram.json`. Build lại, chạy simulator. Ghi rõ thay đổi nào cần đồng bộ giữa source và wiring.

**Đáp án:** đổi macro `LED_GPIO` trong `main.c`, đổi endpoint `esp:18` thành `esp:19` trong connection của LED, build lại và restart/reload simulator. Nếu chỉ sửa source hoặc chỉ sửa wiring, LED sẽ không hoạt động như mong đợi.

### Bài 2 — Thay đổi blink pattern

Sửa code để LED sáng 200 ms, tắt 800 ms. Không thay driver configuration.

**Đáp án tham khảo:** thay vòng lặp thành hai lần `gpio_set_level(LED_GPIO, 1/0)` và delay riêng 200/800 ms. Ghi chú: đây là relative delay; log và overhead làm chu kỳ thực không chính xác tuyệt đối.

### Bài 3 — Hai GPIO output

Thêm LED xanh trên GPIO19. Tạo cấu hình riêng cho GPIO19 hoặc gom cả hai pin vào `pin_bit_mask`; tạo pattern luân phiên: LED đỏ sáng khi LED xanh tắt, rồi đổi trạng thái.

**Đáp án:** mỗi pin phải được kết nối riêng và cấu hình là output. Có thể dùng một `gpio_config_t` có mask `(1ULL << GPIO_NUM_18) | (1ULL << GPIO_NUM_19)` nếu mode/pull/interrupt giống nhau. Ghi mức logic từng LED để tránh hai LED cùng sáng.

### Bài 4 — Pull resistor và input

Thêm button active-low trên GPIO4: một đầu button nối GPIO4, đầu kia GND. Cấu hình GPIO4 input + pull-up; in serial khi button được nhấn.

**Đáp án:** khi nhả button, pull-up đưa input lên logic 1; khi nhấn, button nối pin xuống GND nên input đọc 0. Active-low nghĩa là mức 0 là asserted/pressed. Không bật pull-down đồng thời với pull-up.

### Bài 5 — Lỗi init

Bỏ `ESP_ERROR_CHECK` và bỏ qua giá trị trả về của `gpio_config()`. Vì sao đây là design xấu? Thêm error path có log và dừng firmware nếu init thất bại.

**Đáp án:** nếu init thất bại, loop tiếp tục điều khiển pin chưa được config, trong khi log có thể làm người debug tưởng init thành công. Check `esp_err_t`; log GPIO/error code và dừng hoặc chuyển sang state fault rõ ràng. `ESP_ERROR_CHECK` là fail-fast shorthand.

### Bài 6 — Nhìn qua GPIO driver

Lần theo declarations của `gpio_config_t`, `gpio_config()` và `gpio_set_level()` trong ESP-IDF API. Ghi lại: field nào cấu hình pin, field nào chọn mode, API nào đặt output level, error type là gì.

**Đáp án:** `pin_bit_mask` chọn nhóm GPIO; `mode` chọn input/output; `pull_up_en`/`pull_down_en` bật pull; `intr_type` chọn edge/level interrupt; `gpio_config()` apply configuration; `gpio_set_level()` set output 0/1; API trả `esp_err_t`.

## Câu hỏi lý thuyết và đáp án

### 1. MCU khác board development kit thế nào?

**Đáp án:** MCU là chip/SoC; development board là bo mạch gồm MCU, nguồn, clock/reset circuitry, USB/debug interface và các đầu pin/linh kiện phụ.

### 2. Peripheral là gì? Cho ba ví dụ.

**Đáp án:** Khối phần cứng bên trong MCU thực hiện chức năng ngoài CPU, ví dụ GPIO, timer, UART, SPI, I2C, ADC, DMA.

### 3. Memory map là gì?

**Đáp án:** Bảng phân bổ các địa chỉ CPU có thể truy cập cho Flash, SRAM và các peripheral/register region. Firmware/linker và chip manual phải thống nhất với memory map.

### 4. Pin pad có phải luôn là GPIO không?

**Đáp án:** Không. Một pad có thể được mux sang GPIO hoặc alternate function như UART/SPI/PWM/ADC. Phải chọn đúng pin mux và kiểm tra board routing.

### 5. GPIO mode output nghĩa là gì?

**Đáp án:** Pad output driver được điều khiển bởi output latch/peripheral; code có thể đặt mức logic. Nó không có nghĩa pin cấp đủ dòng cho bất kỳ tải nào.

### 6. Tại sao LED cần điện trở?

**Đáp án:** LED cần current limit; điện trở hạn dòng và bảo vệ LED/GPIO. Tính giá trị theo voltage supply, LED forward voltage và dòng cho phép; datasheet board/chip là tham chiếu.

### 7. `pin_bit_mask` biểu diễn gì?

**Đáp án:** Mỗi bit ứng với một GPIO number; bit 1 trong mask chọn pin được config. Vì field là 64-bit, sample dùng `1ULL << GPIO_NUM_18`.

### 8. `gpio_config()` khác `gpio_set_level()` thế nào?

**Đáp án:** `gpio_config()` setup mode/pull/interrupt cho pin; `gpio_set_level()` đặt output level sau khi pin đã được cấu hình output.

### 9. `ESP_ERROR_CHECK()` xử lý điều gì?

**Đáp án:** Kiểm tra giá trị `esp_err_t`; nếu lỗi, ghi log và abort thay vì âm thầm chạy tiếp với hardware state không rõ ràng.

### 10. Wokwi có xác nhận dòng điện/analog/EMI của board thật không?

**Đáp án:** Không. Simulator tốt cho logic firmware, wiring mô phỏng và quan sát output. Electrical limits, analog accuracy, EMI, silicon errata và timing vật lý cần datasheet/measurement và hardware validation.

### 11. Vì sao `vTaskDelay(500 ms)` không đảm bảo LED toggle mỗi chính xác 500 ms?

**Đáp án:** Delay yield task theo tick; thời gian code trước delay, scheduler và tick resolution làm chu kỳ quan sát khác 500 ms. Dùng hardware timer hoặc periodic scheduling API và measurement để có timing yêu cầu rõ ràng.

### 12. `app_main()` có phải là hardware reset vector không?

**Đáp án:** Không. Boot ROM/bootloader/startup framework xử lý reset và runtime initialization; ESP-IDF gọi `app_main()` sau đó. `app_main()` chạy như một task trong ESP-IDF FreeRTOS integration.

### 13. Tại sao sample dùng GPIO driver thay vì ghi địa chỉ register trực tiếp?

**Đáp án:** Driver đóng gói register fields, pin configuration và chip-specific details; app code dễ đọc hơn và ít phụ thuộc register encoding. Register-level learning vẫn cần để hiểu hardware, nhưng ghi tay cần đúng reference manual, barriers/access semantics và errata.

### 14. Điểm khác nhau giữa software simulation và hardware test?

**Đáp án:** Simulation có thể xác nhận logic, state transition, driver interaction được model và regression nhanh. Hardware test xác nhận silicon, electrical conditions, board wiring, timing vật lý, power và peripheral behavior thực.

## Quiz tự chấm

Không xem đáp án: vẽ boot flow; vẽ mạch GPIO; mô tả 4 field của `gpio_config_t`; giải thích active-high/active-low; nói rõ giới hạn Wokwi. Đạt 8/10 equivalent (không bỏ trống mục nào) trước khi sang tuần 2.

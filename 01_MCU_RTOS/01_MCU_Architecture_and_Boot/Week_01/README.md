# Tuần 1 — Luồng thực thi MCU và GPIO output

> Đây là bài GPIO **qua API ESP-IDF** được giữ để so sánh. Nếu học theo lộ trình Senior MCU, bắt đầu từ [module lý thuyết kiến trúc](../README.md), hoàn thành quiz và [driver thanh ghi](../register_gpio/README.md) trước khi dùng bài này.

Tuần này đi thẳng vào MCU/peripheral, không ôn lại C. Kết quả cuối tuần là bạn build được firmware ESP-IDF, mô phỏng ESP32 DevKitC V4 trong Wokwi, điều khiển LED ngoài qua GPIO và giải thích được luồng từ `app_main()` đến chân GPIO.

## Board và project ảo

- Board: **ESP32 DevKitC V4 / ESP32**.
- Simulator: Wokwi.
- Source/build: project ESP-IDF nằm trong `esp32_gpio/`; Wokwi nạp artifact đã build từ `build/flasher_args.json`.
- LED: GPIO18 → điện trở 1 kΩ → anode LED; cathode LED → GND.

Wokwi browser editor cho phép mở project ESP32 mới: https://wokwi.com/projects/new/esp32. Để chạy đúng source ESP-IDF bên dưới, cài ESP-IDF và Wokwi for VS Code, build local rồi chọn **Wokwi: Start Simulator**. Tài liệu Wokwi yêu cầu build trước và firmware path ESP-IDF trỏ đến `build/flasher_args.json`; board trong diagram phải khớp target chip.

## Tuần 1 học gì, đọc ở đâu?

Tuần 1 không phải chỉ là “chạy LED”. Phần chính là hiểu MCU architecture và boot flow; LED là bài thực hành để nối khái niệm với peripheral thật. Học theo thứ tự dưới đây, không cần đọc hết tài liệu dài:

1. **ESP32 Datasheet** — mục 1 (tổng quan), mục 2.2–2.3 (pin), mục 3 (boot mode), mục 4.1 (CPU và memory), 4.2 (clock), 4.8.1 (GPIO), 4.10 (pin configuration). Đọc để biết chip có gì và giới hạn/chức năng của các chân.
2. **ESP-IDF Application Startup Flow** — đọc phần high-level overview, “First Stage (ROM) Bootloader”, “Second Stage Bootloader” và “Running the Main Task”. Mục tiêu là tự vẽ luồng reset → bootloader → app startup → FreeRTOS main task → `app_main()`.
3. **ESP32 Technical Reference Manual (TRM)** — đọc Chapter 3 “System and Memory” để hiểu memory map ở mức hệ thống; đọc Chapter 6 “IO MUX and GPIO Matrix” để hiểu GPIO/pin mux. Chỉ xem khái niệm và sơ đồ; chưa cần học thuộc register.
4. **ESP-IDF GPIO API** — tra `gpio_config_t`, `gpio_config()` và `gpio_set_level()` khi đọc code trong `esp32_gpio/main/main.c`.

Tài liệu chính thức:

- [ESP32 Series Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [ESP-IDF: Application Startup Flow](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html)
- [ESP32 Technical Reference Manual (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [ESP-IDF: GPIO API](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html)
- [Wokwi: ESP-IDF simulation](https://docs.wokwi.com/vscode/esp-idf)

Ghi chú về phạm vi: datasheet/TRM ở trên dành cho SoC **ESP32** trên board mô phỏng này. Khi đổi sang ESP32-S3/C3 hoặc MCU khác, phải dùng đúng datasheet/TRM của chip đó; không lấy memory map hay pin limits của ESP32 áp sang chip khác.

## Lý thuyết

### 1. MCU là một hệ thống trên chip

MCU gồm CPU core, Flash, SRAM, clock/reset logic, interrupt controller và peripheral. CPU chạy instruction; peripheral thực hiện I/O/timing/giao tiếp; memory map gán địa chỉ cho các vùng RAM, Flash và peripheral registers. Mỗi dòng chip có memory map, clock tree và pin mux riêng: tra datasheet/reference manual thay vì đoán theo tên pin.

### 2. Pin không tự động là GPIO

Một pad có thể có nhiều chức năng: GPIO, UART, SPI, I2C, PWM, ADC... GPIO driver/pin mux chọn chức năng. GPIO output có data latch/direction/enable logic và pad driver. GPIO input đọc mức logic của pad. Board pin label, SoC GPIO number và function signal là ba khái niệm có thể khác nhau.

### 3. Logic 0/1 và electrical limits

GPIO là mức logic, không phải nguồn cấp tải tùy ý. GPIO có giới hạn voltage/dòng; LED cần điện trở hạn dòng. Wokwi mô phỏng logic và một số hành vi linh kiện, nhưng không thay thế datasheet hay đo dòng thật. Trong project này, LED được nối qua điện trở 1 kΩ để mạch có current limit.

### 4. Driver/HAL và memory-mapped register

Trong code project, `gpio_config()` cấu hình mode/pull/interrupt và `gpio_set_level()` đổi mức logic. Đây là API driver; driver phía dưới thao tác hardware registers và pin mux. Học theo hai lớp: hiểu register-level idea, rồi dùng API driver của chip để code app. Không viết địa chỉ register ESP32 bằng phỏng đoán.

### 5. Reset đến `app_main()`

Sau reset, chip chạy boot ROM/bootloader, nạp app image từ Flash, startup code khởi tạo runtime và gọi entry point của framework. ESP-IDF sau đó gọi `app_main()`. Trong ESP-IDF, `app_main()` chạy như một FreeRTOS task; tuần này ta chưa thiết kế task/scheduler, chỉ dùng task hiện tại để thực hiện GPIO blink.

### 6. Config object và error handling

`gpio_config_t` mô tả nhóm pin, mode, pull và interrupt. `ESP_ERROR_CHECK()` dùng để fail-fast nếu API trả error: khi init hardware thất bại, tiếp tục chạy với trạng thái không rõ ràng sẽ nguy hiểm hơn dừng lại có log.

Trong sample, `vTaskDelay()` yield task hiện tại trong một số tick. Khoảng thời gian giữa hai lần đổi LED còn bao gồm thời gian chạy code/log; đây là bài blink để học GPIO, chưa phải periodic timing chính xác. Tuần 3 sẽ học hardware timer và jitter.

## Kế hoạch học 6 buổi (90 phút/buổi)

### Buổi 1 — MCU architecture và datasheet (90 phút)

- 25 phút: học mục 1 và vẽ CPU/Flash/RAM/peripheral/clock/reset.
- 25 phút: mở Wokwi ESP32 board, tìm GPIO18, GND, 3V3; phân biệt SoC pin với board pin label.
- 25 phút: đọc README project và lần theo `app_main()` → `gpio_config()` → `gpio_set_level()`.
- 15 phút: tự vẽ boot flow từ reset đến app.

**Bài nộp:** một sơ đồ khối MCU và 5 dòng mô tả boot flow.

### Buổi 2 — GPIO, pin mux và mạch LED (90 phút)

- 25 phút: học mục 2–4; vẽ đường GPIO output latch → pad → resistor → LED → GND.
- 20 phút: đọc `diagram.json`; xác nhận GPIO18, anode/cathode và resistor.
- 25 phút: dự đoán LED sáng/tắt theo mức GPIO và chu kỳ.
- 20 phút: tra datasheet thông số current/voltage của GPIO và LED (không lấy Wokwi làm thông số điện).

**Bài nộp:** pin map và ghi chú electrical limit cần kiểm tra khi chuyển sang board thật.

### Buổi 3 — ESP-IDF build và Wokwi (90 phút)

- 20 phút: cài ESP-IDF theo official setup nếu chưa có.
- 20 phút: mở project folder `esp32_gpio` trong VS Code có ESP-IDF extension.
- 25 phút: build `idf.py build`; đọc summary và artifact được tạo.
- 25 phút: mở Wokwi simulator và xem serial log/LED.

Lệnh chính:

```powershell
idf.py set-target esp32
idf.py build
```

Trong VS Code: **F1 → Wokwi: Start Simulator**. Project đã có `wokwi.toml` và `diagram.json`.

**Bài nộp:** log build thành công và mô tả artifact ELF/flash images.

### Buổi 4 — Sửa firmware có chủ đích (90 phút)

- Đổi `LED_GPIO` từ 18 sang một GPIO khác được board và simulator hỗ trợ; cập nhật mạch.
- Đổi chu kỳ 500 ms thành 100 ms, sau đó thành 1 s; ghi nhận thay đổi quan sát.
- Đổi output pattern thành 3 blink nhanh + 1 blink dài.
- Nếu `gpio_config()` lỗi, xác định `ESP_ERROR_CHECK` làm gì và log nào cần giữ lại.

**Bài nộp:** 3 thay đổi nhỏ, mỗi thay đổi có một ghi chú expected vs observed.

### Buổi 5 — Hiện tượng và giới hạn mô phỏng (90 phút)

- So sánh LED output với UART log: serial log có thể làm thay đổi thời điểm quan sát.
- Tìm pin có chức năng alternate; đọc pin mux và peripheral ownership trong tài liệu chip.
- Liệt kê những gì simulator không xác nhận: điện áp/dòng thật, EMI, silicon errata, timing vật lý, hardware wiring.
- Thêm một assertion/validation cho GPIO pin configuration trước khi gọi driver.

**Bài nộp:** bảng 2 cột: simulator có thể xác nhận / phải xác nhận trên board thật.

### Buổi 6 — Review và quiz (90 phút)

- 30 phút: làm bài tập trong `exercises/EXERCISES_AND_ANSWERS.md` mà chưa xem đáp án.
- 20 phút: giải thích lại GPIO config struct từng field.
- 20 phút: demo firmware và trình bày boot-to-GPIO flow trong 5 phút.
- 20 phút: làm quiz, đánh dấu chủ đề cần học lại.

## Code project

Mở file `esp32_gpio/main/main.c`. Firmware cấu hình GPIO18 output, đổi mức logic mỗi 500 ms và ghi log. `app_main()` là entry của ứng dụng ESP-IDF; driver GPIO cấu hình hardware và báo lỗi qua `esp_err_t`. Tuần này mục tiêu là MCU/peripheral flow, không phải học lại syntax C.

## Tiêu chí hoàn thành

- Build project ESP-IDF thành công và mô phỏng LED toggle trong Wokwi.
- Chỉ ra được MCU, board pin, SoC GPIO, LED, resistor, GND trên diagram.
- Giải thích được mode/pull/intr_type/pin_bit_mask và `gpio_set_level()`.
- Vẽ được boot flow và nói rõ `app_main()` là FreeRTOS task trong ESP-IDF.
- Trả lời đúng ít nhất 8/10 câu quiz; đọc lại phần sai trước khi sang tuần 2.

# 01 — CPU, SoC và kiến trúc hệ thống

## Mục tiêu

Sau bài này, tự vẽ được đường đi của một lệnh đọc GPIO: CPU phát địa chỉ → bus/interconnect → GPIO peripheral → thanh ghi → dữ liệu về CPU. Giải thích được điều gì do CPU thực hiện và điều gì do phần cứng ngoại vi thực hiện.

## 1. MCU khác CPU, SoC, module và board

- **CPU core** giải mã/thực thi lệnh, giữ PC và các register kiến trúc, phản ứng với exception/interrupt.
- **MCU/SoC** gồm CPU, ROM/SRAM, clock/reset, bus, interrupt controller, peripheral và thường có bộ điều khiển Flash. ESP32 còn có radio Wi‑Fi/Bluetooth.
- **Module** là SoC cùng Flash, thạch anh, antenna và linh kiện cần thiết trên một module đóng gói.
- **Development board** đặt module/SoC cùng nguồn, USB-UART/debug, nút reset/boot và header. `GPIO18` là số tín hiệu của SoC; chữ in trên board chỉ là nhãn vị trí chân.

Đọc **ESP32 Datasheet §1 và §4.1** để xác định block diagram và các khối dùng trong lab GPIO.

## 2. CPU thực thi thế nào

Một vòng khái niệm: lấy instruction theo PC → giải mã → đọc toán hạng → thực hiện → ghi kết quả → cập nhật PC. Pipeline cho phép các bước của nhiều instruction chồng lên nhau; điều này không làm mỗi lệnh luôn mất đúng một chu kỳ. Branch, cache miss, truy cập peripheral hoặc interrupt có thể thay đổi độ trễ.

Các trạng thái tối thiểu cần biết: PC, register tổng dụng, stack pointer, trạng thái interrupt/privilege. C function gọi nhau làm stack tăng/giảm; stack overflow có thể phá dữ liệu khác. `volatile` buộc compiler thực hiện truy cập MMIO theo biểu thức C, nhưng **không** tạo mutex, không bảo đảm thao tác read-modify-write là nguyên tử giữa hai core, và không thay thế memory barrier khi kiến trúc yêu cầu.

ESP32 gốc dùng một hoặc hai Xtensa LX6 core tùy biến thể chip. Trong ESP-IDF trên ESP32 hai core, PRO CPU và APP CPU cùng truy cập nhiều tài nguyên; mỗi core có bối cảnh interrupt riêng. Vì vậy driver dùng thanh ghi chung cần xét đồng thời task/core/ISR.

## 3. Instruction/data, Flash/RAM và cache

ESP32 có ROM dùng boot/chức năng nền, SRAM nội, Flash SPI ngoài chip hoặc trong module, và các vùng địa chỉ cho instruction/data/peripheral. Firmware không đơn giản là “chạy trực tiếp mọi thứ trong Flash”: một số đoạn được map qua cache, một số được copy vào IRAM/DRAM lúc startup. Khi cache Flash tạm thời không sẵn sàng, code/data cần cho ISR quan trọng phải ở vùng bộ nhớ phù hợp.

Phân biệt:

- `.text`, `.rodata`: code/hằng; vị trí thực tế phụ thuộc linker và memory type.
- `.data`: biến tĩnh có giá trị khởi tạo, phải có giá trị đúng khi ứng dụng bắt đầu.
- `.bss`: biến tĩnh khởi tạo về 0.
- **heap**: cấp phát runtime; **stack**: khung gọi hàm và dữ liệu tự động của từng task.
- **MMIO**: địa chỉ CPU trông như bộ nhớ nhưng thao tác tới peripheral, không phải RAM thường.

Đọc **ESP32 TRM Chapter 3** và [ESP-IDF Memory Types](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/memory-types.html). Sau đó chạy `idf.py size` và đối chiếu số liệu với map file của project.

## 4. Bus và chủ thể giao dịch

CPU và DMA có thể là **bus master**; RAM/peripheral là target. Một thao tác load/store chứa địa chỉ, kiểu truy cập và dữ liệu. Interconnect giải mã địa chỉ rồi chuyển giao dịch đến đúng target. Một bus chậm hoặc đang bị tranh chấp làm thời gian truy cập biến thiên; vì thế `GPIO_OUT_W1TS_REG = mask` không đồng nghĩa pin đổi trạng thái ngay trong cùng CPU cycle.

Hai luồng cần phân biệt:

1. CPU ghi GPIO: CPU → interconnect → GPIO register → output latch → GPIO matrix/IO MUX → pad.
2. DMA nhận dữ liệu: peripheral → DMA/bus → RAM → interrupt/status → CPU xử lý buffer.

## 5. Phân biệt interrupt, exception và task

**Interrupt** thường bắt nguồn từ sự kiện phần cứng bất đồng bộ; **exception** có thể xuất hiện do lỗi/trap khi CPU xử lý instruction; **task** là thực thể phần mềm do scheduler chuyển đổi. ISR chạy trong interrupt context với quy tắc API riêng, không phải “một task ưu tiên cao”. Phần sau giải thích interrupt matrix và handoff sang task.

## Điểm cần nắm

`app_main()` do runtime gọi sau boot; CPU truy cập GPIO bằng load/store tới peripheral register; `volatile` không loại bỏ race giữa hai core; cache và bus có thể khiến thời gian truy cập biến thiên. Phần trắc nghiệm chỉ làm sau khi hoàn thành toàn bộ sáu bài lý thuyết.

**Nguồn:** [Datasheet ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf), [ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf), [ESP-IDF startup](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html).

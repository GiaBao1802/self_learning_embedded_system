# 03 — Clock, reset, boot và linker

## 1. Clock tree

Nguồn clock (oscillator/thạch anh) đi qua PLL/divider/mux rồi cấp cho CPU, bus và từng peripheral. Tần số CPU không nhất thiết bằng tần số APB/peripheral. Register cấu hình peripheral không có nghĩa peripheral hoạt động nếu clock của nó bị gate. Thay đổi clock có thể đổi baud rate, timer tick, PWM và thời gian đáp ứng; driver cần biết clock source thực tế hoặc dùng API clock của SoC.

Đọc **ESP32 Datasheet §4.2** và **TRM Chapter 7 “Reset and Clock”**. Cây clock mức hệ thống là source → PLL/divider → CPU/APB → GPIO/UART/timer; khi thiết kế sau này phải ghi cạnh nào đã xác nhận trong tài liệu và cạnh nào còn là giả định. Không suy thời gian thực từ vòng `for` rỗng: compiler, cache, bus và scheduler làm kết quả thay đổi.

## 2. Reset domain và trạng thái sau reset

Power-on reset, watchdog reset, software reset và wakeup từ sleep có thể khác phạm vi. Một số register có reset value xác định, một số có giá trị `x`/không được đảm bảo; driver phải cấu hình trạng thái cần dùng thay vì ngầm tin kết quả từ phiên trước. Đọc reset reason trước khi kết luận lỗi khởi động. Với GPIO có tải ngoài, xem trạng thái pad từ lúc cấp nguồn đến khi firmware cấu hình: boot strapping và điện áp tạm thời có thể quan trọng trên board thật.

## 3. Boot flow ESP32 + ESP-IDF

```text
Reset → ROM bootloader → second-stage bootloader → partition table
      → chọn app image → app startup/CRT → FreeRTOS scheduler + main task
      → app_main()
```

ROM code có sẵn trong chip. Second-stage bootloader từ flash cấu hình phần cần thiết, đọc partition table rồi chọn app. App startup chuẩn bị C runtime, memory/clock/system services và tạo main task. `app_main()` **không** là reset vector. Theo tài liệu ESP-IDF, bootloader mặc định ở flash offset `0x1000`, partition table thường ở `0x8000` và app factory có thể bắt đầu `0x10000`; đây là offset flash theo cấu hình dự án, không phải “địa chỉ thanh ghi GPIO”.

Đọc [Application Startup Flow](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html), [Bootloader](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/bootloader.html), [Partition Tables](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html).

## 4. Build artifacts và linker

Source → compiler tạo object → linker ghép section/symbol theo linker script → ELF cho debug/analysis → image `.bin` để flash. `idf.py build` còn tạo bootloader, partition table và `build/flasher_args.json` để Wokwi biết nạp từng image vào offset nào. ELF có symbol/debug information; BIN là byte image để nạp. Linker map giúp kiểm tra địa chỉ/size của `.text`, `.data`, `.bss`, IRAM/DRAM; `idf.py size` là điểm khởi đầu.

## 5. Điểm cần nắm

Tần số CPU không tự quyết định UART baud; phải xem clock source/divider. Driver cấu hình trạng thái cần dùng sau reset. Mỗi bước boot có chủ thể khác nhau: ROM, bootloader, app startup và scheduler. Flash offset `0x1000` khác địa chỉ MMIO `0x3FF44008` cả về không gian lẫn ý nghĩa.

**Nguồn:** [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf), [ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf), [ESP-IDF build](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/start-project.html).

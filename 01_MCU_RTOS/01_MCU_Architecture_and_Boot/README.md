# 01 — Kiến trúc MCU và quy trình khởi động

Đây là thư mục học phần đầu tiên, không phải thư mục trống. Bắt đầu tại [`Week_01/README.md`](Week_01/README.md): bài học giải thích MCU/SoC, CPU–memory–peripheral, reset/boot đến `app_main()`, và GPIO LED đầu tiên. Mã ESP-IDF và mạch Wokwi nằm ngay trong `Week_01/esp32_gpio/`.

## Đọc theo thứ tự

1. **ESP32 Series Datasheet** — đọc mục 1 (tổng quan), 2 (pin), 3 (boot configuration), 4.1 (CPU và memory), 4.2 (clock), 4.8.1 (GPIO) và 4.10 (pin configuration). Đây là tài liệu chính để hiểu đúng chip ESP32; không cần đọc hết datasheet trong tuần 1.
2. **ESP32 Technical Reference Manual (TRM)** — đọc Chapter 3 “System and Memory” để xem address map; đọc Chapter 6 “IO MUX and GPIO Matrix” để hiểu đường từ GPIO đến pad và peripheral. Chỉ tra phần cần thiết, chưa cần học thuộc register.
3. **ESP-IDF Application Startup Flow** — đọc phần tổng quan, ROM bootloader, second-stage bootloader và “Running the Main Task” để nối boot flow đến `app_main()`.
4. **ESP-IDF GPIO API** — tra `gpio_config_t`, `gpio_config()` và `gpio_set_level()` khi làm lab.

## Tài liệu chính thức

- [ESP32 Series Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [ESP32 Technical Reference Manual (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [ESP-IDF: Application Startup Flow](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html)
- [ESP-IDF: Bootloader](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/bootloader.html)
- [ESP-IDF: GPIO API](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html)
- [Wokwi: Mô phỏng ESP-IDF](https://docs.wokwi.com/vscode/esp-idf)

## Sau tuần 1, bạn cần giải thích được

- MCU/SoC, board phát triển và module ESP32 khác nhau thế nào.
- CPU, ROM, SRAM, Flash, peripheral và memory map có vai trò gì.
- Đường khởi động mức cao: reset/ROM → bootloader → app startup → FreeRTOS main task → `app_main()`.
- GPIO number của chip khác tên chân in trên board ra sao; pin mux chọn chức năng nào.
- Vì sao GPIO chỉ là tín hiệu logic, không phải nguồn cấp tải tùy ý; LED cần điện trở hạn dòng.

## Các bài tiếp theo trong chủ đề

- Tuần 8: memory map, stack/heap, startup và linker.
- Tuần 9: DMA/buffer ownership (liên kết với [`05_Interrupts_DMA_and_Concurrency`](../05_Interrupts_DMA_and_Concurrency/README.md)).
- Dùng [`09_Projects`](../09_Projects/README.md) để tích hợp thành một sản phẩm có yêu cầu và bằng chứng kiểm thử.

# 07 — FreeRTOS hoặc Zephyr

FreeRTOS là đường học chính trên ESP32/ESP-IDF trong roadmap hiện tại. Sau khi nắm task và synchronization, học Zephyr để hiểu hệ thống cấu hình, devicetree, driver model, build bằng west và portability.

## Lựa chọn lộ trình

- Tuần 10–20: FreeRTOS với ESP-IDF; hoàn thành sensor monitor trước.
- Tuần 21: chuyển một sample nhỏ sang Zephyr trên board/simulator được hỗ trợ.
- Không cố dịch toàn bộ project cùng lúc: ghi abstraction, phần phụ thuộc RTOS và trade-off.

## Tài liệu đọc

- [ESP-IDF FreeRTOS](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html)
- [Zephyr Project documentation](https://docs.zephyrproject.org/latest/)
- [Zephyr: Build and Flash](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html)
- [Zephyr: Device tree](https://docs.zephyrproject.org/latest/build/dts/index.html)

Đầu ra: một bảng so sánh task/thread, queue, timer, driver binding, cấu hình và quy trình build giữa hai hệ.

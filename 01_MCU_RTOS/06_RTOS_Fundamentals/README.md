# 06 — Nền tảng RTOS

Học scheduler, task lifecycle, priority, context switch, tick, blocking, latency/jitter, queue, semaphore, mutex, critical section, timer và watchdog. Bắt đầu bằng mô hình và phân tích timing trước khi thêm nhiều task vào firmware.

## Lộ trình

- Tuần 10: task, scheduler, state, tick, context switch.
- Tuần 11–12: task FreeRTOS, priority, stack, periodic scheduling, latency/jitter/deadline.
- Tuần 13–17: queue, synchronization, ISR-to-task, event/timer, memory budget và watchdog.
- Tuần 20: tích hợp thành sensor monitor.

## Tài liệu đọc

- [ESP-IDF FreeRTOS](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html)
- [FreeRTOS Kernel Book](https://www.freertos.org/Documentation/RTOS_book.html) — đọc các chương về task, queue, interrupt management và software timers.
- [FreeRTOS documentation](https://www.freertos.org/Documentation/RTOS_book.html)

Mỗi lab phải ghi priority, stack budget, blocking behavior, dữ liệu chia sẻ và phương án xử lý lỗi.

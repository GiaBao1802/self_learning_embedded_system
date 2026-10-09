# 05 — Interrupt, DMA và concurrency

Học cách sự kiện phần cứng đi vào CPU, quy tắc ISR, interrupt priority, handoff sang task, DMA, cache/buffer ownership và race condition. Mục tiêu là thiết kế luồng dữ liệu có giới hạn thời gian và ownership rõ.

## Đầu ra cần đạt

- Vẽ đường interrupt từ peripheral đến ISR và từ ISR sang task.
- Phân biệt công việc an toàn trong ISR với xử lý cần deferred context.
- Định nghĩa ai sở hữu buffer trước/trong/sau DMA; xử lý lifetime, alignment và cache khi SoC yêu cầu.
- Phân tích race, deadlock, priority inversion và queue backpressure.

## Tài liệu đọc

- [ESP-IDF Interrupt Allocation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/intr_alloc.html)
- [ESP-IDF FreeRTOS SMP considerations](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html)
- [ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf): Chapter 8 “Interrupt Matrix”; đọc chapter của peripheral cụ thể khi cần.
- Lộ trình: tuần 4, 9, 14–15.

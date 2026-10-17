# 09 — Bài tập sau lý thuyết, quiz và lab thanh ghi

Làm theo thứ tự. Mỗi bài nộp: requirement ID, thiết kế ngắn, diff/source, test case, kết quả thực tế, giới hạn bằng chứng. Mẫu traceability có trong [`register_gpio/docs/VMODEL.md`](../register_gpio/docs/VMODEL.md).

## Bài 1 — Truy vết một lần ghi GPIO

Từ `mmio_gpio_write(18, true)`, ghi chính xác giá trị mask và địa chỉ được ghi; vẽ CPU → bus → GPIO output latch → GPIO Matrix → IO MUX → pad → LED. Dẫn mục TRM xác nhận từng register. Kiểm tra host test và log/Wokwi. Giải thích vì sao `GPIO_OUT_W1TS` không cần đọc trước.

## Bài 2 — Thêm LED thứ hai

Chọn GPIO19, thêm LED/điện trở trong `diagram.json`; init 18 và 19, luân phiên đỏ/xanh. Viết requirement cho trạng thái khởi tạo an toàn và hai LED không cùng sáng. Unit test giả MMIO phải kiểm tra hai bit độc lập; Wokwi kiểm tra hành vi trên mạch mô phỏng.

## Bài 3 — Nút input active-low

Nối GPIO21 → button → GND, dùng pull-up. Đọc `GPIO_IN` và giải thích pressed=0/released=1. Bổ sung state machine debounce ở task, không delay trong ISR. Test host cho pull-up, bit đọc, null pointer; Wokwi cho bấm/thả và bounce logic. Ghi rõ pull nội và debounce ảo không chứng minh chất lượng điện trên board.

## Bài 4 — Review register ownership và lỗi

Cố ý gửi pin 20, pin 6 và pull enum sai; chứng minh driver không ghi MMIO. Tạo hai caller muốn cấu hình GPIO18 khác nhau; thiết kế ownership/serialization policy. Nêu vì sao `volatile` không giải quyết race trên IO MUX read-modify-write. Viết ADR: tiếp tục dùng direct MMIO hay chuyển sang ESP-IDF GPIO driver trong sản phẩm thật.

## Bài 5 — Boot và timing

Đọc startup guide, trích 5 bước boot flow bằng lời của bạn; lấy `idf.py size`, ELF và partition table; liên hệ artifact với boot. Thay chu kỳ blink từ 500 thành 100 ms; đo chu kỳ Wokwi ở mức quan sát và nêu vì sao chưa đủ cho worst-case hardware timing. Nếu có board thật, dùng logic analyzer đo 1000 cạnh, ghi min/mean/max và điều kiện clock/build.

## Bài 6 — Mở rộng driver một peripheral khác

Chọn GPTimer hoặc UART. Trước khi code, nộp register map, clock source, reset/enable sequence, pin routing, IRQ/DMA, error cases, owner và test matrix. Chỉ sau design review mới cài đặt. Với ESP32/ESP-IDF đang chạy, tránh ghi register đang do subsystem của IDF sở hữu (ví dụ UART console); chọn peripheral chưa bị dùng hoặc một môi trường thí nghiệm cô lập.

## Điều kiện qua module

- Quiz ≥26/30, tự giải thích được tất cả câu sai.
- Host test MMIO pass; build ESP-IDF pass; Wokwi LED hoạt động.
- Bài 1–5 có requirement ↔ test ↔ evidence; Bài 6 có thiết kế được review.
- Nêu được phần nào cần board thật: power-up state, điện áp/dòng, glitch, interrupt latency, bus contention, worst-case timing và EMI.

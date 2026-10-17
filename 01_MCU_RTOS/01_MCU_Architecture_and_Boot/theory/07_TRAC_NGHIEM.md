# 07 — Trắc nghiệm kiến trúc MCU (làm sau lý thuyết 01–06)

Làm **không xem đáp án**. Mỗi câu chọn một phương án đúng nhất. Mục tiêu ≥ **26/30**; sau khi chấm, tự viết bằng chứng từ datasheet/TRM cho tất cả câu sai. Không dùng quiz thay cho giải thích miệng/sơ đồ.

1. Khối nào giải mã và thực thi instruction? A. GPIO Matrix · B. CPU core · C. Flash · D. PLL
2. MMIO là gì? A. Biến local C · B. Địa chỉ RAM cache · C. Địa chỉ CPU trỏ đến thanh ghi peripheral · D. Địa chỉ linker symbol
3. `volatile` trên con trỏ MMIO bảo đảm điều nào? A. Không có race giữa core · B. Atomic RMW · C. Compiler thực hiện truy cập MMIO theo biểu thức · D. Cache luôn coherent với DMA
4. `GPIO_OUT_W1TS_REG = 1U << 18` làm gì? A. Clear mọi bit khác · B. Set riêng bit GPIO18 · C. Đọc input18 · D. Bật clock
5. Vì sao tránh `W1TS_REG |= mask`? A. Chỉ tốn thời gian · B. W1TS là write-only, `|=` đọc trước · C. Bit mask quá rộng · D. Cấm cú pháp C
6. Giá trị `0x3FF44008` trong lab là gì? A. Offset Flash · B. Địa chỉ register GPIO absolute của ESP32 gốc · C. UART baud · D. Interrupt level
7. `GPIO_ENABLE_W1TS` khác `GPIO_OUT_W1TS` ở đâu? A. Chọn output driver vs mức latch · B. Một cái là RAM · C. Không khác · D. Chọn input pull
8. `GPIO_FUNC18_OUT_SEL_CFG` chọn 256 có ý nghĩa gì trong lab? A. Input pull · B. Clock divider · C. Software GPIO output signal · D. Interrupt core
9. `IO_MUX_GPIO18.MCU_SEL=2` dùng để? A. Chọn GPIO function/matrix · B. Set LED high · C. Set IRQ priority · D. Reset SoC
10. Vì sao pin GPIO18 của ESP32 không đồng nhất với “chân thứ 18” trên board? A. Board pin label/pad/SoC GPIO là các hệ đánh số khác nhau · B. Vì ROM · C. Vì compiler · D. Vì FreeRTOS
11. CPU đọc `GPIO_IN_REG` được dữ liệu từ đâu? A. Output latch thuần · B. Input path/pad của GPIO · C. Partition table · D. ELF symbol
12. Nếu cấu hình pad input nhưng không có pull và nút đang hở, vấn đề gì? A. Chắc chắn mức 0 · B. Có thể floating · C. GPIO tự tắt · D. Bootloader lỗi
13. Boot flow ESP-IDF đúng nhất? A. `app_main → ROM` · B. ROM → second-stage bootloader → app startup → main task → `app_main` · C. CPU → Wokwi → ELF · D. ISR → ROM
14. Bootloader ESP32 thường đọc bảng gì để chọn app? A. IRQ vector của C · B. Partition table · C. Stack watermark · D. GPIO matrix
15. Offset flash `0x1000` khác địa chỉ MMIO vì? A. Offset mô tả vị trí image trong flash, MMIO ánh xạ register vào không gian địa chỉ CPU · B. Cùng nghĩa · C. Offset là UART · D. MMIO là text section
16. `.bss` chứa gì về mặt khởi tạo? A. Code ROM · B. Dữ liệu tĩnh zero-initialized · C. ISR vector thuần · D. Flash offset
17. Vì sao cùng một function có execution time biến thiên? A. Chỉ do C syntax · B. Cache, bus contention, interrupt và scheduler · C. Do tên file · D. Không thể biến thiên
18. UART baud thay đổi khi CPU clock thay đổi? A. Luôn có · B. Không bao giờ · C. Tùy nguồn clock/divider của UART và driver · D. Tùy tên task
19. Interrupt matrix ESP32 dùng để? A. Gán nguồn interrupt đến input interrupt CPU · B. Quản lý heap · C. Lưu firmware · D. Đọc ADC
20. ISR khác task ở đâu? A. ISR là task priority 0 · B. ISR chạy trong interrupt context, có quy tắc API/timing riêng · C. Task không chạy trên CPU · D. ISR chỉ là macro
21. Quên xóa cờ interrupt có thể gây? A. Interrupt storm/lặp ISR · B. ELF nhỏ hơn · C. Tăng điện trở · D. Xóa Flash
22. Quy tắc tốt cho ISR nút bấm? A. `vTaskDelay` ngay trong ISR · B. Log liên tục và debounce 1 giây trong ISR · C. Ghi nhận event, notify task, xử lý debounce ngoài ISR · D. Không cần xóa status
23. Khi DMA sở hữu buffer RX, CPU nên? A. Sửa tùy ý · B. Chỉ truy cập theo ownership/coherency contract · C. Gọi `free` · D. Đổi GPIO
24. `volatile bool ready` có thể thay queue cho frame UART đa byte không? A. Luôn có · B. Không, thiếu ownership/lifetime và đồng bộ nội dung frame · C. Chỉ trên dual-core · D. Nếu LED đang tắt
25. `vTaskDelay(500 ms)` bảo đảm cạnh LED chính xác mỗi 500 ms? A. Có · B. Không, còn tick/scheduler/execution time · C. Có nếu `volatile` · D. Chỉ khi dùng SPI
26. Vì sao driver đặt latch low trước khi enable output pad? A. Giảm khả năng glitch ở đầu ra · B. Để tăng CPU clock · C. Để reset Flash · D. Để tắt UART
27. `gpio_reg.h` của ESP32-S3 có dùng trực tiếp thay ESP32 được không? A. Có, cùng thương hiệu · B. Không, cần đúng target/register map · C. Có nếu Wokwi chạy · D. Chỉ cần đổi tên macro
28. Unit test fake MMIO chứng minh được gì? A. Điện áp thực ở pad · B. Logic lựa chọn địa chỉ/mask/thứ tự ghi ở mức phần mềm · C. EMI · D. Worst-case hardware timing
29. Bằng chứng phù hợp nhất cho yêu cầu “LED không glitch > 1 µs lúc boot” là? A. Chỉ host test · B. Chỉ đọc code · C. Phép đo trên board thật bằng logic analyzer/oscilloscope theo điều kiện boot · D. Chỉ Wokwi screenshot
30. Trong V-model, detailed driver design thường đối ứng với? A. Unit/component test · B. Market analysis · C. Chỉ release note · D. Board purchase

Chuyển sang [`08_DAP_AN.md`](08_DAP_AN.md) sau khi đã nộp câu trả lời và lý do cho ít nhất 10 câu khó nhất.

# 08 — Đáp án và lý do

| Câu | Đúng | Lý do ngắn |
|---:|:---:|---|
| 1 | B | CPU core thực thi instruction; GPIO/PLL là peripheral/hệ clock. |
| 2 | C | MMIO ánh xạ peripheral register vào không gian địa chỉ CPU. |
| 3 | C | `volatile` ràng buộc compiler với truy cập, không thay lock/atomic/cache management. |
| 4 | B | W1TS chỉ set các bit có giá trị 1 trong mask. |
| 5 | B | `|=` là read-modify-write; WO không được đọc để lấy trạng thái. |
| 6 | B | TRM ESP32 Chapter 6: GPIO base + `0x08`. |
| 7 | A | ENABLE điều khiển output driver; OUT giữ giá trị logic của latch. |
| 8 | C | 256 chọn đường software GPIO output trong GPIO Matrix. |
| 9 | A | IO MUX chọn GPIO function (2) để đường GPIO Matrix ra pad. |
| 10 | A | Số GPIO là tín hiệu SoC; vị trí vật lý/nhãn board khác. |
| 11 | B | IN báo trạng thái trên input path; không đồng nghĩa đọc output latch. |
| 12 | B | Input không được kéo có thể floating và đọc không ổn định. |
| 13 | B | Startup guide mô tả ROM → stage 2 → app startup → main task. |
| 14 | B | Bootloader tra partition table/factory/OTA để chọn image. |
| 15 | A | Flash offset và MMIO CPU address là hai không gian/ý nghĩa khác. |
| 16 | B | `.bss` là dữ liệu tĩnh khởi tạo 0 trước khi app chạy. |
| 17 | B | Cache/bus/interrupt/scheduler tạo biến thiên thời gian. |
| 18 | C | Cần xem clock tree và UART source/divider thực tế. |
| 19 | A | Interrupt matrix nối nguồn interrupt với input CPU. |
| 20 | B | ISR ở interrupt context, giới hạn API và thời gian thực thi. |
| 21 | A | Cờ còn set có thể kích hoạt ISR liên tục. |
| 22 | C | ISR báo event ngắn; task xử lý debounce và việc dài. |
| 23 | B | Buffer phải theo ownership và cache/coherency contract. |
| 24 | B | Một bool không mô tả payload, ownership hay lúc buffer được dùng lại. |
| 25 | B | Delay theo tick và thời gian xử lý/scheduling tạo sai số/jitter. |
| 26 | A | Output latch ở mức an toàn trước khi pad được enable. |
| 27 | B | Register map và pin/clock khác theo SoC; dùng đúng target. |
| 28 | B | Fake MMIO xác nhận hành vi phần mềm, không kiểm chứng silicon. |
| 29 | C | Glitch/timing vật lý cần đo tại pad trên board thật. |
| 30 | A | Detailed design đối ứng unit/component verification trong V-model. |

**Nguồn để giải thích sâu:** [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf), [ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf) Chapter 3/6/7/8, [ESP-IDF startup](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html), [ESP-IDF interrupts](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/intr_alloc.html).

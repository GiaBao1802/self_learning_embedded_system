# Lộ trình MCU và RTOS từ A đến Z

## Kết quả cuối cùng

Sau lộ trình này, bạn có thể đọc datasheet/reference manual, giải thích luồng boot và memory map, viết firmware C có module, dùng peripheral/interrupt, tạo ứng dụng RTOS an toàn hơn, debug lỗi timing/concurrency, và trình bày kiến trúc bằng sơ đồ + ADR + kết quả test.

Thời lượng gợi ý: **24 tuần, 8–12 giờ/tuần**. Nếu chỉ có 4–6 giờ/tuần, kéo mỗi tuần học thành 2 tuần. Ưu tiên làm lab và project; đọc tài liệu phải dẫn đến một đầu ra cụ thể.

## Roadmap 24 tuần

| Tuần | Chủ đề | Đầu ra bắt buộc |
|---|---|---|
| 1 | MCU architecture, boot/memory overview, GPIO output, ESP-IDF build flow | Build và mô phỏng GPIO LED trên ESP32; xem `Week_01` |
| 2 | GPIO input, pull-up/down, button, debounce và state machine | Button đổi trạng thái LED; giải thích active-low và bounce |
| 3 | Clock tree, hardware timer, periodic tick, PWM | Timer task-free tạo chu kỳ; đo period và duty cycle |
| 4 | Interrupt controller, GPIO interrupt, priority và ISR rules | ISR ngắn gửi event; foreground xử lý event |
| 5 | UART, baud rate, serial framing, timeout, ring buffer | UART command console với `help/status/start/stop` |
| 6 | I2C và SPI, bus timing, addressing, chip select, error recovery | Đọc sensor giả lập; chọn bus và định nghĩa driver interface |
| 7 | ADC, sampling, quantization, filtering, PWM output | Đọc analog input giả lập và điều khiển PWM |
| 8 | Core/registers, address map, stack/heap/static/Flash, startup/linker | Vẽ boot flow và phân tích ELF/map/memory regions |
| 9 | DMA, cache/buffer ownership, producer-consumer | UART RX/DMA design và phân tích buffer lifetime |
| 10 | RTOS overview: task, scheduler, state, tick, context switch | Vẽ state diagram và giải thích preemption/blocking |
| 11 | FreeRTOS task lifecycle, create/delete, priority, stack | Tách app thành acquisition, processing, logger tasks |
| 12 | Periodic task, delay-until, latency/jitter/deadline | Bảng response time/jitter của các task |
| 13 | Queue, message passing, ownership, backpressure | Truyền sample sensor; xử lý queue full và stale data |
| 14 | Semaphore, mutex, critical section, race và deadlock | Phân tích priority inversion và sửa design |
| 15 | ISR-to-task notification và interrupt-safe API | Button/UART ISR báo task; không làm việc dài trong ISR |
| 16 | Software timer, event group/flags, state machine | Điều khiển mode `IDLE/RUN/FAULT` bằng event |
| 17 | Memory budget, static allocation, stack watermark, watchdog | Bảng RAM/stack budget; fault/watchdog strategy |
| 18 | Testing: host unit test, mock/fake, integration, simulator limits | Test business logic và driver boundaries có hệ thống |
| 19 | Debugging/tracing: logs, assertions, core dump, race hunt | Bug report có reproduction, root cause, regression case |
| 20 | FreeRTOS architecture and integration | Project sensor monitor đã tách task và giao tiếp |
| 21 | Zephyr: west, Kconfig, devicetree, drivers, workqueue | Build/run sample trên board simulator được hỗ trợ |
| 22 | Safety/reliability: watchdog, fail-safe, timeout, recovery | Fault matrix và behavior khi sensor/queue/UART lỗi |
| 23 | System integration, requirements, interfaces, timing analysis | Architecture review và trade-off analysis |
| 24 | Capstone và architecture review | Source, tests, diagrams, ADR, timing/RAM budget, demo và retrospective |

## Công việc lặp lại mỗi tuần

1. Đọc/ghi notes tối đa 2–3 giờ, sau đó làm lab.
2. Mỗi lab cần có: mục tiêu, input/output, assumptions, lỗi có thể xảy ra, kết quả.
3. Build với warning bật (`-Wall -Wextra -Wpedantic`); không bỏ qua warning mà không hiểu.
4. Ghi lại một sơ đồ nhỏ hoặc ADR khi có quyết định về interface, timing, memory hay concurrency.
5. Cuối tuần tự giải thích lại chủ đề trong 5 phút và làm lại bài tập không xem đáp án.

## Capstone: sensor monitor có RTOS

Thiết kế ứng dụng có 3–4 task: `sensor_task` đọc theo chu kỳ; `processing_task` lọc/kiểm tra ngưỡng; `logger_task` gửi UART; `command_task` nhận lệnh. Dữ liệu đi qua queue, lỗi sensor tạo event `FAULT`, watchdog phát hiện task treo. Xác định chu kỳ, deadline, queue depth, stack/RAM budget, timeout, retry và hành vi khi queue đầy.

Hồ sơ bàn giao: requirements ngắn, context/component diagram, sequence diagram, interface spec, 3 ADR, source code, unit/integration tests, log demo, bảng timing/RAM và danh sách rủi ro/giới hạn của simulator.

## Toolchain và simulator

### Bắt đầu ngay: ESP32 + ESP-IDF + Wokwi

Vì bạn đã vững C, bỏ qua phần ôn tập C. Tuần 1 bắt đầu trực tiếp từ GPIO và MCU execution flow. Dùng Wokwi ESP32 DevKitC V4 để xem LED/pins và serial; build source bằng ESP-IDF (`idf.py build`), rồi mô phỏng qua Wokwi for VS Code/CLI. Wokwi nạp artifact đã build; phần compile vẫn do toolchain ESP-IDF thực hiện.

- Wokwi: https://wokwi.com/
- ESP32 board/simulator và FreeRTOS examples: https://docs.wokwi.com/guides/esp32
- ESP-IDF build/flash/monitor: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html
- Wokwi ESP-IDF workflow: https://docs.wokwi.com/vscode/esp-idf
- ESP-IDF GPIO API: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html

### Nâng cấp cho Zephyr: Renode

Renode là simulator cài trên máy, không phải web IDE. Zephyr có board `stm32h7_renode_reference_board` và runner `west simulate --runner=renode`; dùng ở tuần 23–24 để build ELF, chạy firmware và automation test trên platform được hỗ trợ.

- Board/reference: https://docs.zephyrproject.org/latest/boards/antmicro/stm32h7_renode_reference_board/doc/index.html
- West build/simulate: https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html

## Cách đánh giá tiến độ

- **Foundation:** giải thích được khái niệm, làm bài lab có input/output và xử lý boundary.
- **Practitioner:** tích hợp subsystem, có test, log, timing/memory measurement và xử lý lỗi.
- **Architect-ready:** đề xuất được phương án, định lượng trade-off, kiểm soát interface/risk và bảo vệ quyết định bằng ADR/diagram/test evidence.

## Ghi chú về simulator

Simulator rất tốt để học firmware flow, scheduling, giao tiếp và test logic. Kết quả mô phỏng không thay thế xác nhận trên silicon về analog, power, EMI, silicon errata, timing vật lý hay driver của board đích. Khi có board, cần chạy lại các test quan trọng trên hardware thật.

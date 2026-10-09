# Lo trinh MCU va RTOS tu A den Z

## Ket qua cuoi cung

Sau lo trinh nay, ban co the doc datasheet/reference manual, giai thich luong boot va memory map, viet firmware C co module, dung peripheral/interrupt, tao ung dung RTOS an toan hon, debug loi timing/concurrency, va trinh bay kien truc bang so do + ADR + ket qua test.

Thoi luong goi y: **24 tuan, 8-12 gio/tuan**. Neu chi co 4-6 gio/tuan, keo moi tuan hoc thanh 2 tuan. Uu tien lam lab va project; doc tai lieu phai dan den mot dau ra cu the.

## Roadmap 24 tuan

| Tuan | Chu de | Dau ra bat buoc |
|---|---|---|
| 1 | MCU architecture, boot/memory overview, GPIO output, ESP-IDF build flow | Build va mo phong GPIO LED tren ESP32; xem `Week_01` |
| 2 | GPIO input, pull-up/down, button, debounce va state machine | Button doi trang thai LED; giai thich active-low va bounce |
| 3 | Clock tree, hardware timer, periodic tick, PWM | Timer task-free tao chu ky; do period va duty cycle |
| 4 | Interrupt controller, GPIO interrupt, priority va ISR rules | ISR ngan gui event; foreground xu ly event |
| 5 | UART, baud rate, serial framing, timeout, ring buffer | UART command console voi `help/status/start/stop` |
| 6 | I2C va SPI, bus timing, addressing, chip select, error recovery | Doc sensor gia lap; chon bus va dinh nghia driver interface |
| 7 | ADC, sampling, quantization, filtering, PWM output | Doc analog input gia lap va dieu khien PWM |
| 8 | Core/registers, address map, stack/heap/static/Flash, startup/linker | Ve boot flow va phan tich ELF/map/memory regions |
| 9 | DMA, cache/buffer ownership, producer-consumer | UART RX/DMA design va phan tich buffer lifetime |
| 10 | RTOS overview: task, scheduler, state, tick, context switch | Ve state diagram va giai thich preemption/blocking |
| 11 | FreeRTOS task lifecycle, create/delete, priority, stack | Tach app thanh acquisition, processing, logger tasks |
| 12 | Periodic task, delay-until, latency/jitter/deadline | Bang response time/jitter cua cac task |
| 13 | Queue, message passing, ownership, backpressure | Truyen sample sensor; xu ly queue full va stale data |
| 14 | Semaphore, mutex, critical section, race va deadlock | Phan tich priority inversion va sua design |
| 15 | ISR-to-task notification va interrupt-safe API | Button/UART ISR bao task; khong lam cong viec dai trong ISR |
| 16 | Software timer, event group/flags, state machine | Dieu khien mode `IDLE/RUN/FAULT` bang event |
| 17 | Memory budget, static allocation, stack watermark, watchdog | Bang RAM/stack budget; fault/watchdog strategy |
| 18 | Testing: host unit test, mock/fake, integration, simulator limits | Test business logic va driver boundaries co he thong |
| 19 | Debugging/tracing: logs, assertions, core dump, race hunt | Bug report co reproduction, root cause, regression case |
| 20 | FreeRTOS architecture and integration | Project sensor monitor da tach task va giao tiep |
| 21 | Zephyr: west, Kconfig, devicetree, drivers, workqueue | Build/run sample tren board simulator duoc ho tro |
| 22 | Safety/reliability: watchdog, fail-safe, timeout, recovery | Fault matrix va behavior khi sensor/queue/UART loi |
| 23 | System integration, requirements, interfaces, timing analysis | Architecture review va trade-off analysis |
| 24 | Capstone va architecture review | Source, tests, diagrams, ADR, timing/RAM budget, demo va retrospective |

## Cong viec lap lai moi tuan

1. Doc/ghi notes toi da 2-3 gio, sau do lam lab.
2. Moi lab can co: muc tieu, input/output, assumptions, loi co the xay ra, ket qua.
3. Build voi warning bat (`-Wall -Wextra -Wpedantic`); khong bo qua warning ma khong hieu.
4. Ghi lai mot so do nho hoac ADR khi co quyet dinh ve interface, timing, memory hay concurrency.
5. Cuoi tuan tu giai thich lai chu de trong 5 phut va lam lai bai tap khong xem dap an.

## Capstone: sensor monitor co RTOS

Thiet ke ung dung co 3-4 task: `sensor_task` doc theo chu ky; `processing_task` loc/kiem tra nguong; `logger_task` gui UART; `command_task` nhan lenh. Du lieu di qua queue, loi sensor tao event `FAULT`, watchdog phat hien task treo. Xac dinh chu ky, deadline, queue depth, stack/RAM budget, timeout, retry va hanh vi khi queue day.

Ho so ban giao: requirements ngan, context/component diagram, sequence diagram, interface spec, 3 ADR, source code, unit/integration tests, log demo, bang timing/RAM, va danh sach rui ro/con han che cua simulator.

## Toolchain va simulator

### Bat dau ngay: ESP32 + ESP-IDF + Wokwi

Vi ban da vung C, bo qua phan on tap C. Tuan 1 bat dau truc tiep tu GPIO va MCU execution flow. Dung Wokwi ESP32 DevKitC V4 de xem LED/pins va serial; build source bang ESP-IDF (`idf.py build`), roi mo phong qua Wokwi for VS Code/CLI. Wokwi nap artifact da build; phan compile van do toolchain ESP-IDF thuc hien.

- Wokwi: https://wokwi.com/
- ESP32 board/simulator va FreeRTOS examples: https://docs.wokwi.com/guides/esp32
- ESP-IDF build/flash/monitor: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html
- Wokwi ESP-IDF workflow: https://docs.wokwi.com/vscode/esp-idf
- ESP-IDF GPIO API: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html

### Nang cap cho Zephyr: Renode

Renode la simulator cai tren may, khong phai web IDE. Zephyr co board `stm32h7_renode_reference_board` va runner `west simulate --runner=renode`; dung o tuan 23-24 de build ELF, chay firmware va automation test tren platform duoc ho tro.

- Board/reference: https://docs.zephyrproject.org/latest/boards/antmicro/stm32h7_renode_reference_board/doc/index.html
- West build/simulate: https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html

## Cach danh gia tien do

- **Foundation:** giai thich duoc khai niem, lam bai lab co input/output va xu ly boundary.
- **Practitioner:** tich hop subsystem, co test, log, timing/memory measurement va xu ly loi.
- **Architect-ready:** de xuat duoc phuong an, dinh luong trade-off, kiem soat interface/risk, va bao ve quyet dinh bang ADR/diagram/test evidence.

## Ghi chu ve simulator

Simulator rat tot de hoc firmware flow, scheduling, giao tiep va test logic. Ket qua mo phong khong thay the xac nhan tren silicon ve analog, power, EMI, silicon errata, timing vat ly, hay driver cua board dich. Khi co board, can chay lai cac test quan trong tren hardware that.

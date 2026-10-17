# Lộ trình 40 tuần hướng tới năng lực Senior MCU Engineer theo V-model

Giả định bạn đã thành thạo C. Thời lượng tham khảo **40 tuần × 10–15 giờ/tuần**; đây là lộ trình xây năng lực và portfolio, không phải cam kết chức danh senior chỉ sau 40 tuần. Senior còn cần nhiều lần xử lý sự cố, review thiết kế và đo trên sản phẩm thật. Mỗi module đi theo thứ tự: **lý thuyết đầy đủ → quiz/design review → driver thanh ghi → bài tập/lab → test/evidence**. V-model chạy xuyên suốt từ tuần đầu.

Lộ trình 40 tuần này lấy ESP32 làm nền. Để học đúng các chip từ ảnh tuyển dụng, đi tiếp [module STM32/NXP/SPC58/TC3xx](../10_MCU_Families_and_Portability/README.md) và [36 tuần mở rộng](../10_MCU_Families_and_Portability/07_LICH_MO_RONG_36_TUAN.md), không trộn địa chỉ/register của các họ chip.

## Giai đoạn 1 — Kiến trúc MCU và đọc tài liệu (tuần 1–6)

| Tuần | Học và làm | Bằng chứng/verification |
|---:|---|---|
| 1 | CPU/SoC, pipeline, memory types, stack/heap, bus master/target; đọc Datasheet/TRM | Sơ đồ CPU–bus–memory–peripheral, 10 câu giải thích miệng |
| 2 | MMIO, register access types, bit fields, atomic set/clear, `volatile`, ownership | Register map GPIO18 có địa chỉ/offset/field/source |
| 3 | Clock tree, reset domain, boot ROM/stage2/app startup, linker/ELF/partition | Boot sequence, memory/clock diagram, `idf.py size` report |
| 4 | Interrupt matrix, ISR, DMA, cache/coherency, bus contention | ISR/DMA state diagram và risk review |
| 5 | Hoàn tất lý thuyết 01–06, quiz ≥85%, review driver requirements/design | Quiz có giải thích câu sai, requirement và test plan trước code |
| 6 | Viết GPIO MMIO library, host fake MMIO test, Wokwi và bài tập 1–5 | Build, test log, demo, traceability; hardware gap liệt kê |

Tài liệu và lab cụ thể: [`01_MCU_Architecture_and_Boot`](../01_MCU_Architecture_and_Boot/README.md).

## Giai đoạn 2 — Peripheral và real-time nền tảng (tuần 7–14)

| Tuần | Học và làm | Bằng chứng/verification |
|---:|---|---|
| 7 | GPIO input, electrical limits, debounce state machine | Button driver + truth table + host/Wokwi test |
| 8 | Timer/counter, prescaler, capture/compare, clock source | Timer spec, period/jitter measurement plan |
| 9 | PWM/LEDC, frequency–resolution trade-off | PWM driver/API + waveform measurement |
| 10 | UART framing, baud error, FIFO/ring buffer, timeout | Console protocol, parser boundary tests |
| 11 | SPI modes, CS timing, full-duplex, DMA | Device driver contract và logic-analyzer trace |
| 12 | I2C addressing/ACK/NACK, pull-up, timeout, bus recovery | Sensor driver + fault injection |
| 13 | ADC sampling/quantization/calibration/noise | Sampling plan và error budget |
| 14 | Peripheral architecture review | Interface spec, driver tests, chọn bus có trade-off |

## Giai đoạn 3 — Interrupt, DMA và RTOS (tuần 15–24)

| Tuần | Học và làm | Bằng chứng/verification |
|---:|---|---|
| 15 | Interrupt priority, sharing, affinity, ISR constraints | Event flow/latency budget, ISR code review |
| 16 | DMA descriptors, buffer lifetime, cache/alignment | Ownership state machine và corruption tests |
| 17 | FreeRTOS task states, scheduler, context switch | Task diagram, priority table, stack budget |
| 18 | Periodic scheduling, deadlines, jitter và measurement | Timing trace min/mean/max, missed deadline policy |
| 19 | Queue/notification/event group, backpressure | Queue depth rationale và overflow/fault tests |
| 20 | Mutex, race, deadlock, priority inversion | Concurrency review, lock order, stress test |
| 21 | Watchdog, reset reason, fault containment | Fault matrix và recovery test |
| 22 | Memory allocation policy, fragmentation, stack watermark | Static/RAM budget report |
| 23 | Dual-core ownership/affinity, cache/Flash constraints | Cross-core race scenario và mitigation |
| 24 | RTOS integrated sensor monitor | Requirements, ADR, tests, measured timing/memory |

## Giai đoạn 4 — Reliability, security và verification (tuần 25–34)

| Tuần | Học và làm | Bằng chứng/verification |
|---:|---|---|
| 25 | Bootloader, partitions, firmware update, rollback | Update/rollback state model và failure cases |
| 26 | Secure boot/Flash encryption/keys ở mức thiết kế | Threat model, key ownership, non-destructive test plan |
| 27 | Brownout, power/sleep, startup/shutdown sequencing | Power-state diagram và bench measurement |
| 28 | Logging, assertions, panic/core dump | Root-cause report từ fault injected |
| 29 | Host unit test/mock register, boundary/error cases | Unit report và traceability coverage |
| 30 | Target test/pytest-embedded, integration test | Test script, firmware/board matrix |
| 31 | Static analysis/compiler warnings/format/UB review | Baseline findings + disposition |
| 32 | GDB/OpenOCD/JTAG, trace/logic analyzer | Debug transcript, timing evidence |
| 33 | Fault injection: stuck sensor, lost IRQ, queue full, reboot | Fault campaign report với expected/actual |
| 34 | CI build/test/size, artifact retention | Pipeline, reproducible build notes, release gate |

## Giai đoạn 5 — Architecture và capstone (tuần 35–40)

| Tuần | Học và làm | Bằng chứng/verification |
|---:|---|---|
| 35 | System requirement, hazards, measurable quality attributes | SRS có ID/acceptance criteria |
| 36 | Component/interface architecture, state/sequence/deployment | SAD, interface contract, ADR |
| 37 | Capstone vertical slice: sensor → process → event → output | Component/unit tests + Wokwi demo |
| 38 | Reliability and timing integration | Load/fault tests, RAM/CPU/timing budget |
| 39 | Hardware verification and design review | Board test report; nếu chưa có board, ghi pending rõ |
| 40 | Release package và independent review | Requirement ↔ design ↔ code ↔ test matrix, risks/deviations, demo |

## V-model bắt buộc tại mỗi cột mốc

| Bên trái: tạo và review | Bên phải: xác nhận |
|---|---|
| User/system needs | Acceptance/system test trên board thật |
| Software requirements | Integration test + fault cases |
| Architecture/interface | Component tests + architecture review |
| Detailed design/register sequence | Unit tests mock MMIO, static analysis |
| Implementation | Build, warning gate, code review |

Mỗi requirement có ID, điều kiện hoạt động, error case và acceptance criteria; mỗi test case trỏ về requirement; mọi kết quả ghi commit, target, tool version và môi trường. Không kết luận timing/điện/EMI từ Wokwi. Theo [quy trình V-model và template](VMODEL_PROCESS.md), xem mẫu đã điền ở [`register_gpio/docs/VMODEL.md`](../01_MCU_Architecture_and_Boot/register_gpio/docs/VMODEL.md). Danh sách tool và cách tự kiểm tra ở [`TOOLING_AND_SELF_TEST.md`](TOOLING_AND_SELF_TEST.md).

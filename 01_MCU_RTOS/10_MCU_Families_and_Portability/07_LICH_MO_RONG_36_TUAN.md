# Lịch mở rộng 36 tuần sau nền tảng MCU 40 tuần

Đây là lịch **tham khảo**, không ép học song song bốn họ chip. Đi theo thứ tự lý thuyết → quiz/design gate → code → test của [quy trình A–Z](01_COMMON_A_TO_Z_VMODEL.md). Nếu chưa có board hoặc tài liệu hãng cần tài khoản, ghi pending và tiếp tục host/design work; **không tự đánh dấu target test PASS**.

| Tuần tổng | Target | Mốc đầu ra |
|---:|---|---|
| 41–42 | [STM32H753](02_STM32H753_ARM.md) | Core/memory/cache/bus, clock/reset/boot/linker, register notebook |
| 43–44 | STM32H753 | Interrupt/DMA/peripherals → quiz → SRS/SAD/test plan review |
| 45–46 | STM32H753 | GPIO register driver, timer/UART/state machine, host tests |
| 47–48 | STM32H753 | Board integration, DMA/cache, timing, fault/release gate |
| 49–50 | [S32K344](03_NXP_S32K344_ARM.md) | Exact part/board, automotive Cortex-M7, boot/clock/watchdog |
| 51–52 | S32K344 | GPIO/interrupt/FlexCAN theory → quiz → V-model design gate |
| 53–54 | S32K344 | S32DS/RTD, register driver, timer/event/host+target tests |
| 55–56 | S32K344 | CAN peer/error cases, reset/fault, traceability review |
| 57–58 | [SPC584B](04_ST_SPC584B_POWER.md) | Power Architecture, board/errata, boot/clock/bus |
| 59–60 | SPC584B | Registers/peripherals/interrupt theory → quiz → design gate |
| 61–62 | SPC584B | SPC5Studio, GPIO/timer/serial/CAN, host+target tests |
| 63–64 | SPC584B | Watchdog/fault/timing, debug evidence, release review |
| 65–66 | [TC375](05_INFINEON_TC375_TRICORE.md) | TriCore/multicore/memory/interconnect, board/startup |
| 67–68 | TC375 | Registers/interrupt/DMA/safety theory → quiz → design gate |
| 69–70 | TC375 | ADS/iLLD, GPIO/timer/event/host+target tests |
| 71–72 | TC375 | Core ownership, fault campaign, debugger intrusion, review |
| 73–74 | [Capstone](06_CROSS_CHIP_CAPSTONE.md) | Cùng SRS/API/test intent trên ≥2 chip, hoàn thiện 4 bản nếu đủ board |
| 75–76 | Capstone | Independent review, regression, requirement–test matrix, portfolio |

**Ưu tiên khi thời gian ít:** ESP32 → STM32H753 → S32K344 → chọn TC375 hoặc SPC584B theo JD thực tế. Trường hợp chỉ có ESP32/Wokwi, có thể học lý thuyết/host test của họ khác, nhưng phần target debug, CAN bus điện, watchdog thực và timing phải để pending. TRACE32 không là gate bắt buộc; phương pháp debug/trace là gate, công cụ cụ thể theo hardware/license của dự án.

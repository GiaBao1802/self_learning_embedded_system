# Một quy trình A–Z áp dụng lại cho từng họ MCU

Mỗi chip là một **lần đi hết V-model**, không phải chỉ đổi IDE rồi chạy blink. Giữ requirement ID và hành vi cấp ứng dụng giống nhau để có thể so sánh, nhưng mỗi chip có bản thiết kế phần cứng riêng. Có thể triển khai thành 8–12 tuần/chip sau nền tảng ESP32, 8–12 giờ/tuần; nhiều tuần cần board thật. Mọi mục chưa thực hiện phải ghi **pending**, không điền PASS.

## Bên trái V: hiểu và thiết kế trước khi code

| Gate | Việc phải hiểu/thiết kế | Đầu ra bắt buộc | Điều kiện qua gate |
|---|---|---|---|
| A. Chốt exact part | Part number, board revision, silicon revision, SDK/compiler version, schematic, errata, debug probe | `TARGET.md` và BOM tối thiểu | Không dùng tài liệu nhầm family/revision |
| B. Đọc hệ thống | Core ISA, exception/privilege, memory/cache/MPU, bus master/target, reset/clock/boot, watchdog | Block diagram có nguồn tài liệu và giả định | Giải thích từ reset đến `main`/task đầu |
| C. Đọc register | Base/offset/bit/reset/access/side effect/clock gate/pin mux; đối chiếu header với manual | Register notebook (không chỉ copy macro) | Mọi địa chỉ đều có nguồn và part áp dụng |
| D. Viết system needs | Một sensor/button → xử lý → LED/UART/CAN; period/deadline, lỗi, recovery, startup safe state | SRS ID `SYS-*`, acceptance criteria định lượng | Có happy, boundary và fault case |
| E. Phân rã kiến trúc | BSP → MMIO/SDK driver → service → application; ownership core/ISR/task; interface và error code | SAD, state/sequence diagram, ADR, memory/timing budgets | Review phát hiện resource conflict/race |
| F. Thiết kế chi tiết | Pin/clock/interrupt/DMA/buffer/ISR-to-task, startup/shutdown, register sequence và timeout | Driver contract, register sequence, test cases liên kết ID | Không có “magic address” chưa kiểm chứng |

## Đáy V: cài đặt có kiểm soát

| Gate | Việc làm | Bằng chứng |
|---|---|---|
| G. Build tối thiểu | Startup/linker, clock, GPIO, log; warning-as-error nếu toolchain cho phép | Reproducible build command, ELF/map/size, tool versions |
| H. Driver | Viết **một** driver nhỏ ở mức register hoặc low-level SDK để hiểu phần cứng; sau đó so sánh với vendor driver | Source, code review, API docs, register readback/trace; không cùng lúc để hai driver sở hữu một peripheral |

## Bên phải V: verify/validate và release

| Gate | Test/đánh giá | Evidence và giới hạn |
|---|---|---|
| I. Unit + static | Host test bằng fake MMIO cho validation, mask, order, timeout; compiler/static checks | PASS/FAIL log; fake MMIO **không** chứng minh silicon |
| J. Component + integration | Chạy target: GPIO/timer/UART/CAN; ISR, queue, DMA, reset; test lỗi và xử lý recovery | Serial/debug trace, logic analyzer/CAN log, board/revision |
| K. System + acceptance | Đo period/jitter/latency, RAM/Flash/stack; injection mất IRQ, bus timeout, sensor stuck, reset | Expected/actual, min/max, acceptance test và deviation; power/EMI cần thiết bị chuyên dụng |
| L. Review/release | Requirement ↔ design ↔ code ↔ test ↔ result, errata disposition, known gaps, reproducible build | Traceability matrix và release note; independent review nếu có thể |

## Bộ test tối thiểu lặp lại ở mọi chip

1. **UT-01 register:** pin/channel sai không ghi MMIO; mask đúng; reserved bits giữ nguyên; RMW không dùng cho W1C/W1S.
2. **UT-02 state machine:** tất cả transition hợp lệ/sai, timeout và recovery; bounded queue/ring buffer không tràn.
3. **IT-01 startup:** reset → safe state → clock/pin init → peripheral ready; thử các loại reset được hỗ trợ.
4. **IT-02 real-time:** interrupt → task, jitter/deadline, queue full, watchdog; tách thời gian debug-attached và free-running.
5. **IT-03 bus:** UART/CAN timeout, frame lỗi, không có ACK/peer, bus-off nếu phần cứng và transceiver hỗ trợ; không nối trực tiếp MCU pin vào bus xe.
6. **AT-01:** demo cùng đặc tả và cùng traceability cho bốn họ; ghi rõ test nào chỉ là mô phỏng, test nào cần board.

## Tool theo mức chứng minh

| Câu hỏi | Tool đề xuất | Không thể kết luận từ tool đó |
|---|---|---|
| Logic/state/register transaction đúng? | Host GCC/Clang, fake MMIO, unit test, static analyzer | Điện/timing của pad thật |
| Linker/Flash/RAM đúng? | ELF/map/`size`/`objdump`, IDE vendor | Chạy được trên silicon |
| Firmware chạy và register đổi đúng? | Debug probe vendor, GDB/IDE; TRACE32 nếu có license/probe phù hợp | Worst-case timing khi debugger đang halt |
| Bus/timing có đạt? | Logic analyzer, oscilloscope, CAN/LIN analyzer, power supply | Functional safety certification |
| Tài liệu và truy vết đủ? | Git, Markdown, Doxygen cho API, CI/issue tracker | Chất lượng thiết kế chỉ bằng số trang tài liệu |

Áp dụng thêm [template V-model chung](../00_Roadmap/VMODEL_PROCESS.md) và [capstone đa chip](06_CROSS_CHIP_CAPSTONE.md). Khi đổi chip, chỉ tái sử dụng **requirement/test intent và phần logic thuần C**; cập nhật `TARGET.md`, register notebook, BSP, clock/pin/interrupt mapping, linker, errata và test target.

# Chặng 4 — Infineon AURIX TC375 / KIT_A2G_TC375_LITE (TriCore)

Ảnh ghi “Infineon TC3XX”. Chọn [KIT_A2G_TC375_LITE](https://www.infineon.com/evaluation-board/KIT-A2G-TC375-LITE) làm target học tập. TC3xx là họ đa lõi TriCore; **không** phải Arm Cortex-M, không mang địa chỉ/thanh ghi từ S32K hay ESP32 sang. Board này có miniWiggler và DAP connector theo Infineon; kiểm tra đúng phiên bản kit/manual trước khi cấp nguồn.

## Đọc tài liệu trước mọi quiz và code

1. [TC3xx user manuals và architecture volumes](https://documentation.infineon.com/aurixtc3xx/docs/qmd1702366622648): Part 1/2, kiến trúc TriCore, **appendix riêng TC37x/TC375** và errata silicon. Manual họ là superset; đừng coi feature nào cũng có trên exact part.
2. [TC375 lite kit](https://www.infineon.com/evaluation-board/KIT-A2G-TC375-LITE) và [kit user manual](https://www.infineon.com/assets/row/public/documents/10/44/infineon-aurix-tc375-lite-kit-usermanual-en.pdf): nguồn, pin, LED, transceiver, miniWiggler, connector; ghi board revision.
3. [TC3xx overview](https://documentation.infineon.com/aurixtc3xx/docs/tjo1702364009277): core, local/shared memory, FPI/SRI interconnect, interrupt, DMA. Đọc [startup/initialization note](https://documentation.infineon.com/aurixtc3xx/docs/nyb1710229964455) cho boot firmware, user startup, cache và core bring-up.
4. Theo user manual: CPU context save area/interrupt/trap, reset/clock/watchdog, PORT/pin mux, service request routing, timer/serial/CAN, DMA, safety mechanisms. **Tách phần mô tả kiến trúc với giá trị register thực tế của TC375 revision.**

**Register notebook:** PORT pad/output/input, clock/reset dependency, service request/interrupt priority/CPU routing, watchdog, một timer, UART/CAN, DMA/buffer, error/fault status. Mỗi địa chỉ/bit cần manual + appendix/errata; không hardcode từ TC33x hoặc TC39x example.

Sau phần lý thuyết, đọc [bản đồ kiến trúc](08_SO_SANH_KIEN_TRUC.md), làm câu **25–32** của [quiz](09_QUIZ_DA_CHIP.md), đối chiếu [đáp án](10_DAP_AN_QUIZ_DA_CHIP.md); rồi mới code.

Ở tuần 5–7, đi theo [bản đồ iLLD và AURIX code examples](11_THU_VIEN_SDK_MA_MAU.md); kiểm `Cpu0_Main.c` metadata của từng example trước khi dùng, đặc biệt exact device/board.

## Lịch 8 tuần

| Tuần | Học/làm theo thứ tự | Bằng chứng |
|---:|---|---|
| 1 | Exact chip/kit, TriCore core, context/exception và memory local/shared | Architecture + board/revision manifest |
| 2 | FPI/SRI/bus arbitration, boot/startup, cache, clock/reset/watchdog | Boot/core bring-up diagram + timing risks |
| 3 | PORT/pin mux, service requests/interrupt, DMA, timer/serial/CAN | Register notebook và ownership table |
| 4 | SRS/SAD/ADR, multicore/ISR ownership, fault/verification plan; quiz ≥85% sau lý thuyết | Gate A–F [quy trình chung](01_COMMON_A_TO_Z_VMODEL.md) |
| 5 | [AURIX Development Studio](https://documentation.infineon.com/aurixtc3xx/docs/hqk1751612176436) + iLLD build mẫu; inspect startup/linker/map; driver GPIO nhỏ | Build/ELF/map, fake-MMIO unit test, code review |
| 6 | Timer/interrupt→event; multi-core shared-state synchronization và bounded queues | Target logs, race/priority test, core mapping |
| 7 | CAN/UART + DMA hoặc timer subsystem; fault injection, watchdog/reset | Bus/timing trace, fault result; no vehicle bus |
| 8 | Test chạy tự do vs debug-attached, errata disposition, V-model closure | Matrix requirement–test–result và gap report |

## Acceptance và debug chuyên sâu

- `TC3-REQ-01`: safe output state sau reset; mọi core/application stage chỉ sử dụng memory/peripheral đã init.
- `TC3-REQ-02`: một resource có owner rõ, shared-state được đồng bộ; test contention/fault, không dùng `volatile` làm mutex.
- `TC3-REQ-03`: ISR/service request routing đến core dự kiến; latency và priority đo trên target theo workload đã định.
- `TC3-REQ-04`: watchdog, reset, bus/CAN faults có response định nghĩa; **debugger có thể ảnh hưởng test an toàn**, phải ghi điều kiện đo.
- `TC3-REQ-05`: địa chỉ/bit/feature được kiểm theo exact device appendix và errata; mọi requirement có bằng chứng hoặc pending.

Infineon công bố [AURIX Development Studio](https://documentation.infineon.com/aurixtc3xx/docs/hqk1751612176436) với compiler/debugger/iLLD; dùng miniWiggler trên kit để bắt đầu. [Lauterbach TRACE32 TriCore](https://www.lauterbach.com/supported-platforms/architectures/tricore) là nhánh chuyên sâu: attach/reset, multicore halt/run, register/memory, trace, trigger và tác động của breakpoint đến timing. Không cần mua TRACE32 để hoàn thành các bước nền tảng; nếu có, ghi version/probe/license và so sánh với kết quả chạy tự do.

**Câu hỏi trước code:** context save của TriCore khác ARM stacking ra sao? Interrupt được route tới CPU nào? Memory nào là local/shared? Nếu debugger tạm dừng watchdog, test watchdog có còn giá trị không? Trích manual/trace để trả lời.

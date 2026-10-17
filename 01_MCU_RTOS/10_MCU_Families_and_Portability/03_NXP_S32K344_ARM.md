# Chặng 2 — NXP S32K344 / S32K3X4EVB-T172 (Arm Cortex-M7 ô tô)

**Đích:** chuyển nền ARM MCU sang một dòng automotive khác hãng. Board mẫu [S32K3X4EVB-T172](https://www.nxp.com/design/design-center/development-boards-and-designs/S32K3X4EVB-T172) dùng S32K344; board phức tạp/đắt hơn Nucleo nên không cần mua ngay. NXP liệt kê S32 Design Studio, RTD, debugger và tài liệu board trên trang đó. **S32K3 reference manual có thể yêu cầu tài khoản NXP**; hãy lấy hợp lệ từ hãng, không dùng bản mirror sai revision.

## Đọc trước khi làm trắc nghiệm và code

1. [S32K3 product/documentation](https://www.nxp.com/products/S32K3): datasheet S32K3xx, reference manual `S32K3XXRM`, errata đúng mask set, safety/security manuals nếu quyền truy cập cho phép. [S32K3xx datasheet](https://www.nxp.com/docs/en/data-sheet/S32K3xx.pdf) cung cấp package/pin/electrical limits.
2. [Board T172](https://www.nxp.com/design/design-center/development-boards-and-designs/S32K3X4EVB-T172): hardware user manual, schematic, debug interface, LED/button/CAN transceiver và nguồn. Board này không phải ESP32/Wokwi; không hứa mô phỏng đầy đủ trên web.
3. Đọc Cortex-M7 exception/NVIC/MPU/cache, memory and bus, boot/clock/reset, watchdog, pin routing, interrupt/DMA, UART/timer, FlexCAN và transceiver. So sánh với STM32H753: cùng core family **không** có cùng peripheral register hoặc clock tree.
4. [S32 Design Studio/RTD](https://www.nxp.com/products/S32K3): đọc generated startup/linker, clock/pin configuration và low-level driver source; phân biệt RTD, application code và safety/security package.

**Register notebook bắt buộc:** pin mux + GPIO output/input, clock gate/root, interrupt routing, watchdog service, một timer, UART, FlexCAN status/error/interrupt, DMA nếu dùng. Ghi offset/bit/reset/access từ **RM đúng revision**; không đưa địa chỉ “mẫu” không xác minh vào source. Với CAN, ghi rõ CAN controller ≠ transceiver ≠ termination ≠ bus topology.

Sau phần lý thuyết, đọc [bản đồ kiến trúc](08_SO_SANH_KIEN_TRUC.md), làm câu **9–16** của [quiz](09_QUIZ_DA_CHIP.md), đối chiếu [đáp án](10_DAP_AN_QUIZ_DA_CHIP.md); rồi mới code.

## Lịch 8 tuần

| Tuần | Học/làm theo thứ tự | Bằng chứng |
|---:|---|---|
| 1 | Datasheet/board, core/memory/bus và sơ đồ nguồn–debug–CAN | `TARGET.md`, block diagram, pin/electrical notes |
| 2 | Boot/clock/reset/watchdog/linker, silicon revision/errata | Boot flow, clock diagram, risk register |
| 3 | Register map pin/GPIO, interrupt, timer/UART, FlexCAN, DMA | Register notebook có trích chapter/section |
| 4 | Requirements + design + test plan; quiz sau toàn bộ lý thuyết (≥85%) | SRS/SAD/ADR, fault matrix, Gate A–F |
| 5 | S32 Design Studio build minimal app; GPIO driver mức register hoặc RTD low-level, so sánh | ELF/map/size, code review, host fake-MMIO test |
| 6 | Timer/interrupt→event, bounded queue/UART log, watchdog | Target logs + timing/timeout test |
| 7 | FlexCAN: TX/RX với **hai node hoặc CAN tester**; error/timeout/bus-off/recovery theo phần cứng | CAN trace, transceiver wiring, expected/actual |
| 8 | Fault injection, memory/timing budget, target regression, V-model closure | Traceability + errata/deviation report |

## Test cụ thể

- `NXP-REQ-01`: sau reset, output vào trạng thái an toàn trước khi application bắt đầu phát event; thử reset thông thường và watchdog nếu được hỗ trợ.
- `NXP-REQ-02`: input tạo event không mất/nhân đôi ngoài đặc tả; đo worst observed latency trong điều kiện tải đã định.
- `NXP-REQ-03`: CAN packet có ID/payload/rate theo SRS; khi peer mất hoặc bus lỗi phải timeout/report/recover theo chính sách. **Không đấu board trực tiếp vào mạng xe thật** chỉ để học.
- `NXP-REQ-04`: pin sai, buffer sai, queue full, timeout và reset không làm ghi bừa register hoặc treo vô hạn.
- `NXP-REQ-05`: mọi requirement có unit/integration/target test hoặc ghi pending; safety hardware của chip **không** tự làm ứng dụng đạt ASIL.

**Tool:** S32 Design Studio + S32 Debugger/compatible probe theo board revision, NXP RTD, host compiler + fake MMIO, CAN analyzer/USB-CAN + transceiver đúng mức điện, logic analyzer. Một số tài liệu/download cần tài khoản/điều kiện hãng. TRACE32 là tùy chọn khi có probe/license. Giữ bản toolchain và RTD trong hồ sơ vì thay version có thể làm generated code khác.

**Câu hỏi trước code:** CPU Cortex-M7 giống STM32 thì vì sao source MMIO không portable? Lockstep nghĩa gì đối với test application? CAN controller báo TX thành công khi nào? Watchdog dưới debugger có hành vi giống chạy tự do không? Trả lời dựa trên tài liệu NXP và kết quả target.

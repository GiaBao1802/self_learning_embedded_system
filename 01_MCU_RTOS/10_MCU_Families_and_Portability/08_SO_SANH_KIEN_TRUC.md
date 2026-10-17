# Bản đồ lý thuyết để chuyển giữa các họ MCU

Học phần này sau [ESP32 nền tảng](../01_MCU_Architecture_and_Boot/README.md) và trước bài quiz đa chip. Đây là **bản đồ so sánh**, không thay datasheet/RM/errata của exact part. Bảng cố ý không chứa “địa chỉ mẫu”: địa chỉ đúng phụ thuộc chip/revision và phải được tự tra trong register notebook.

## 1. CPU và bối cảnh thực thi

| Họ | Điểm phải học mới khi chuyển từ ESP32 |
|---|---|
| STM32H753 Cortex-M7 | Vector table, exception entry/exit, NVIC priority grouping, SysTick, MPU, cache, Thumb và memory ordering; hiểu ISR không phải task. Đọc [PM0253/RM0433](https://www.st.com/en/microcontrollers-microprocessors/stm32h743-753/documentation.html). |
| S32K344 Cortex-M7 | Cùng ISA/core family nhưng boot, peripheral, clock, reset và register map của NXP. Xem cấu hình lockstep theo part/board, watchdog và automotive fault handling trong [NXP S32K3](https://www.nxp.com/products/S32K3). |
| SPC584B e200z4 | Power Architecture Book E/VLE, register/exception model và startup khác ARM; không dùng CMSIS/NVIC API. Đọc [RM0004/UM0438/RM0449](https://www.st.com/en/microcontrollers-microprocessors/spc58-4b-line-mcus/documentation.html). |
| TC375 TriCore | Context save area, interrupt/trap, nhiều CPU, local/shared memory và core-specific resource ownership; xem [TC3xx architecture/user manuals](https://documentation.infineon.com/aurixtc3xx/docs/qmd1702366622648). |

**Bài hiểu:** Với cùng event button, vẽ đường từ pad→interrupt routing→CPU/ISR→event queue→task/service trên từng chip. Chỉ ghi tên block/field khi đã kiểm trong manual đúng part.

## 2. Bộ nhớ, bus và DMA

Memory map là hợp đồng phần cứng, không là “RAM chung”. CPU load/store qua bus/interconnect đến Flash/RAM/MMIO; DMA có thể là master khác và truy cập buffer khi CPU/cache còn giữ bản cũ. Trên STM32H7 và S32K3 Cortex-M7 phải đặc biệt kiểm tra **vùng RAM có thể được DMA truy cập**, alignment và cache maintenance theo RM/SDK. TC3xx có local/shared memory và interconnect [FPI/SRI](https://documentation.infineon.com/aurixtc3xx/docs/tjo1702364009277), nên ownership và bus contention phức tạp hơn một core. Với SPC584B, đọc sơ đồ bus/memory của RM0449 trước khi chọn buffer và timing assumption.

Trên mọi chip, ghi vào notebook: CPU nào/engine nào là bus master, vùng RAM/Flash/MMIO nào là target, path của peripheral, có cache/buffer/barrier hay không, timeout/error mode, ai sở hữu vùng nhớ. **`volatile` không đồng bộ cache DMA và không khóa RMW.**

## 3. Clock, reset, boot, watchdog

Sơ đồ phổ quát: nguồn dao động → PLL/divider/mux → CPU/bus/peripheral. Tần số CPU không bằng mặc định tần số timer/UART/CAN. Reset có nhiều domain và watchdog có thể chịu tác động của debugger. Với mỗi chip phải vẽ `reset → ROM/boot firmware → startup/linker init → clock/memory/peripheral init → application`, và ghi điểm nào do vendor SDK làm. [STM32 H7](https://www.st.com/en/microcontrollers-microprocessors/stm32h743-753/documentation.html), [S32K3](https://www.nxp.com/products/S32K3), [SPC58 4B](https://www.st.com/en/microcontrollers-microprocessors/spc58-4b-line-mcus/documentation.html) và [TC3xx startup](https://documentation.infineon.com/aurixtc3xx/docs/nyb1710229964455) có tài liệu khác nhau; không dùng một boot flow chi tiết cho tất cả.

## 4. Register và pin/peripheral

Một driver GPIO phải đi qua: clock enable → pin mux/route → direction/open-drain/pull → initial output latch → output enable → set/clear/read → deinit. **Thứ tự an toàn cụ thể** còn tùy hardware reset state và board circuit. R/W register dùng RMW có lock/ownership; W1S/W1C/WO chỉ ghi mask, không dùng `|=`. Readback có thể chứng minh register write nhưng không chứng minh pad voltage. Pin GPIO nối CAN bus luôn cần controller + transceiver + termination/peer thích hợp.

Khác biệt quan trọng: STM32 dùng RCC/GPIO/EXTI/NVIC theo RM0433; S32K3 có register/peripheral riêng của NXP dù cùng Cortex-M7; SPC58 là Power Architecture và peripheral riêng; TC3xx có PORT + interrupt service routing/core ownership riêng. So sánh **chức năng** trước rồi mới tìm register tương ứng, không tìm tên register giống nhau bằng trực giác.

## 5. RTOS, architecture và safety

ISR chỉ thu sự kiện tối thiểu; service/task xử lý dài, queue có capacity/backpressure, timeout là requirement. Một module driver có owner rõ cho pin/peripheral/DMA buffer; một interface tốt tách application logic khỏi register. Cùng một FSM `INIT/IDLE/RUN/FAULT/RECOVER` có thể port, nhưng WCET/timing/memory budget phải đo lại mỗi target. Automotive safety hardware hỗ trợ thiết kế, **không chứng nhận** sản phẩm chỉ vì có trên chip. V-model yêu cầu requirement/test evidence cả happy path lẫn fault/recovery, và đánh dấu pending khi chưa có board.

## 6. Debugger là một phần của môi trường đo

ST-LINK, NXP debugger, PLS và miniWiggler là đường bắt đầu theo kit. TRACE32 có thể dùng cho Power Architecture và TriCore khi có probe/license phù hợp, đặc biệt multicore/trace. Nhưng breakpoint/halt có thể đổi timing, watchdog và trạng thái hệ thống: chạy lại acceptance test ở chế độ không gắn debugger và ghi cả hai điều kiện. Doxygen tạo API docs, **không** tự sinh SRS/SAD/test evidence.

Sau khi đọc bản đồ này **và tài liệu riêng của chip**, làm [quiz 32 câu](09_QUIZ_DA_CHIP.md), rồi xem [đáp án giải thích](10_DAP_AN_QUIZ_DA_CHIP.md). Các câu hỏi là gate khái niệm; bản thân chúng không thay register notebook và design review.

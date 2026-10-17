# Chặng 3 — ST SPC584B70E1 / SPC584B-DIS (Power Architecture)

Ảnh tuyển dụng ghi “ST SPC58”, một **họ**, không phải một part number. Chọn [SPC584B-DIS](https://www.st.com/en/evaluation-tools/spc584b-dis.html) với SPC584B70E1/e200z4 làm target mẫu vì ST công bố board, debugger tích hợp và [RM0449 cho SPC58 4B](https://www.st.com/en/microcontrollers-microprocessors/spc58-4b-line-mcus/documentation.html). Các SPC58 C/E/H khác không được coi là tương thích địa chỉ hay safety setup. Đây là **Power Architecture**, không phải STM32/Arm.

## Lý thuyết phải học trước quiz/code

1. [SPC58 4B documentation](https://www.st.com/en/microcontrollers-microprocessors/spc58-4b-line-mcus/documentation.html): datasheet DS11701, reference manual RM0449, Book E programming manual RM0004, VLE manual UM0438, errata đúng JTAG ID/revision; ghi version và ngày tải.
2. [SPC584B-DIS](https://www.st.com/en/evaluation-tools/spc584b-dis.html): board user manual UM2461, schematic, LED/button, debugger PLS tích hợp, mức điện và jumper cấp nguồn. Không sao chép pin mapping từ SPC582B hay SPC58 C line.
3. Core e200z4, register/exception/interrupt, VLE, memory/Flash/RAM, bus, reset/boot, clock domains, watchdog, pin mux/GPIO, timer, serial, CAN, fault control. Đọc startup/linker của [SPC5Studio](https://www.st.com/en/development-tools/spc5-studio.html) để nối lý thuyết với `main`.
4. Ghi register notebook cho đường LED **từ clock đến pad** và đường button **từ pad đến interrupt/controller đến ISR**. Với CAN, xác định controller và transceiver trên board bằng schematic. Không bịa địa chỉ cố định từ ví dụ chip khác.

Sau phần lý thuyết, đọc [bản đồ kiến trúc](08_SO_SANH_KIEN_TRUC.md), làm câu **17–24** của [quiz](09_QUIZ_DA_CHIP.md), đối chiếu [đáp án](10_DAP_AN_QUIZ_DA_CHIP.md); rồi mới code.

## Lịch 8 tuần

| Tuần | Học/làm theo thứ tự | Bằng chứng |
|---:|---|---|
| 1 | Datasheet/board/core e200z4, ISA/exception, memory/bus | Part/revision manifest, kiến trúc so với ARM |
| 2 | Boot/reset/clock/watchdog/linker/Flash và errata | Boot sequence, clock tree, reset reason table |
| 3 | GPIO/pin mux/interrupt/timer/UART/CAN; register access types | Register notebook có nguồn từng field |
| 4 | SRS, architecture, state machine và test plan; quiz sau lý thuyết ≥85% | Gate A–F [quy trình chung](01_COMMON_A_TO_Z_VMODEL.md) |
| 5 | SPC5Studio build mẫu, inspect startup/map; driver GPIO mức register/low-level; Doxygen API | Build/ELF/map, host MMIO unit tests |
| 6 | Timer/event/serial, interrupt latency và fault/timeout handling | Target debug log, timing measurement |
| 7 | CAN với peer/tester phù hợp, watchdog/reset và mất nguồn có kiểm soát | Bus trace, reset/fault matrix, không test trên xe |
| 8 | Regression, errata disposition, traceability và review độc lập | V-model package, pending items ghi rõ |

## Acceptance và tool

- `SPC-REQ-01`: boot vào safe output state, không tạo cạnh ngoài ý muốn sau reset được thử; đo pad trên board.
- `SPC-REQ-02`: driver GPIO reject pin không thuộc board mapping, set/clear không phá chân khác; fake MMIO kiểm transaction, target kiểm pad.
- `SPC-REQ-03`: interrupt button→event có latency nằm trong budget bạn đặt; xử lý bounce và event overflow.
- `SPC-REQ-04`: CAN/serial báo lỗi và recovery theo SRS; không kết luận bus-pass khi chỉ tự loopback nội bộ.
- `SPC-REQ-05`: watchdog/fault test được chạy không gắn debugger **nếu debugger làm thay đổi hành vi watchdog**; ghi rõ điều kiện đo.

[SPC5Studio](https://www.st.com/en/development-tools/spc5-studio.html) là đường build ban đầu; board có debugger PLS tích hợp. [TRACE32 hỗ trợ Power Architecture/SPC5](https://www.lauterbach.com/supported-platforms/architectures/powerpc) nhưng cần probe/license thích hợp, là mục nâng cao chứ không phải điều kiện bắt đầu. Dùng oscilloscope/logic analyzer, CAN interface, nguồn bench cho các test vật lý. Chỉ dùng cấu hình cấp nguồn/jumper đúng board manual.

**Câu hỏi trước code:** vì sao cùng ST nhưng STM32 và SPC58 không dùng chung startup/register map? VLE ảnh hưởng disassembly/debug ra sao? Register watchdog/fault nào không được tùy tiện ghi? Reset có xóa mọi RAM/peripheral state không? Tìm đáp án trong RM/board manual/errata.

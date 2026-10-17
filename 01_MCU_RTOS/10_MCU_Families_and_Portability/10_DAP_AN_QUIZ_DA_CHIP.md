# Đáp án quiz đa chip và lý do

Chỉ mở sau khi làm [đề](09_QUIZ_DA_CHIP.md). Mỗi họ cần ≥7/8; mọi câu sai phải ghi lại nguồn datasheet/RM/board manual/errata đúng revision. Các đáp án dưới đây là **nguyên tắc**; với câu hỏi register địa chỉ cụ thể, tự tra tài liệu exact part.

## STM32H753

1. **B** — cùng Cortex-M7 không có nghĩa cùng register/clock/startup của hai hãng.
2. **A** — `BSRR` hỗ trợ set/reset bit GPIO; phải tra access/semantics ở RM0433.
3. **B** — DMA và CPU/cache cần vùng RAM phù hợp, alignment, ownership và maintenance khi thiết kế yêu cầu.
4. **A** — NVIC quản lý interrupt priority; scheduler quản lý task priority.
5. **A** — baud phụ thuộc clock source/divider/peripheral, không chỉ CPU frequency.
6. **A** — schematic/polarity/jumper quyết định đường từ pad ra LED của board.
7. **B** — reset handler/startup chuẩn bị runtime trước `main`.
8. **C** — waveform trên board chạy tự do dưới workload xác định có bằng chứng timing phù hợp hơn build/halt.

Tra lại [STM32H743/753 docs](https://www.st.com/en/microcontrollers-microprocessors/stm32h743-753/documentation.html), [NUCLEO-H753ZI](https://www.st.com/en/evaluation-tools/nucleo-h753zi.html).

## NXP S32K344

9. **B** — tài liệu và errata phải đúng part/revision và được truy cập hợp lệ.
10. **B** — safety capability của silicon không tự chứng minh safety case của application/system.
11. **B** — CAN controller cần transceiver, wiring, termination và peer/tester tùy setup.
12. **B** — tái sử dụng contract/test intent; phần hardware mapping phải viết lại.
13. **B** — errata thường gắn mask/revision; không xét có thể dẫn đến driver sai.
14. **B** — version/config/diff/regression giúp tái hiện kết quả khi generated code thay đổi.
15. **A** — host fake chỉ chứng minh transaction logic, đo pad/reset mới xác nhận target behavior.
16. **B** — timeout/recovery phải có trong SRS và test, không chờ vô hạn.

Tra lại [S32K3 product/docs](https://www.nxp.com/products/S32K3), [S32K3X4EVB-T172](https://www.nxp.com/design/design-center/development-boards-and-designs/S32K3X4EVB-T172).

## ST SPC584B

17. **B** — board SPC584B-DIS dùng e200z4 Power Architecture.
18. **A** — VLE là phần instruction encoding cần hiểu cho build/disassembly/debug.
19. **B** — RM0449/Book E/VLE/board manual/errata đúng họ SPC58 4B.
20. **A** — board có debugger PLS tích hợp; TRACE32 là tùy chọn.
21. **B** — loopback không kiểm tra toàn bộ bus điện/transceiver/peer.
22. **B** — ST có hai hệ chip khác nhau; STM32 HAL không là SPC58 driver.
23. **A** — halt/debug có thể thay timing/watchdog, nên lưu điều kiện và chạy free-running.
24. **B** — logic FSM có thể tái sử dụng, nhưng BSP/MMIO/clock/interrupt/target test khác.

Tra lại [SPC58 4B docs](https://www.st.com/en/microcontrollers-microprocessors/spc58-4b-line-mcus/documentation.html), [SPC584B-DIS](https://www.st.com/en/evaluation-tools/spc584b-dis.html).

## Infineon TC375

25. **C** — TC375 thuộc AURIX TriCore.
26. **B** — multicore/shared memory đòi hỏi ownership/synchronization/visibility rõ.
27. **B** — context save/interrupt/trap nằm trong kiến trúc/manual TC3xx.
28. **B** — service request routing tới core/priority dự kiến và latency phải được chứng minh.
29. **A** — kit có miniWiggler; ADS là đường học ban đầu.
30. **B** — manual superset cần device appendix/errata đúng exact part/revision.
31. **B** — `volatile` không thay thế atomic/lock/cache policy giữa core.
32. **B** — TRACE32 chuyên sâu và phụ thuộc probe/license, không là điều kiện để bắt đầu.

Tra lại [TC3xx manuals](https://documentation.infineon.com/aurixtc3xx/docs/qmd1702366622648), [TC375 lite kit](https://www.infineon.com/evaluation-board/KIT-A2G-TC375-LITE), [AURIX Development Studio](https://documentation.infineon.com/aurixtc3xx/docs/hqk1751612176436).

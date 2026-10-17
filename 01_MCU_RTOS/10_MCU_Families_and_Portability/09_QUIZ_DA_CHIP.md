# Quiz đa chip — làm sau lý thuyết, trước driver

Mỗi họ có 8 câu, chọn **một** đáp án. Đạt ít nhất **7/8 cho họ đang học** rồi giải thích mọi câu sai bằng manual của exact part. Không tra đáp án trước; [đáp án và lý do](10_DAP_AN_QUIZ_DA_CHIP.md) ở tệp riêng. Quiz này kiểm khái niệm; địa chỉ/bit thực tế vẫn phải tra register notebook.

## STM32H753 (câu 1–8)

1. STM32H753 và S32K344 cùng Cortex-M7. Kết luận nào đúng? A. GPIO register map giống nhau. B. ISA/core có điểm chung nhưng peripheral map khác. C. Cùng linker script. D. Cùng clock tree.
2. Muốn set một pin mà không RMW cả output register, trước hết tra register nào trong RM0433? A. `BSRR`. B. `ODR |=`. C. `IDR`. D. `MODER`.
3. DMA ghi buffer còn CPU có D-cache. Việc nào cần xem xét? A. Chỉ thêm `volatile`. B. Vùng DMA-accessible, alignment/cache maintenance/ownership. C. Tắt warning. D. Tăng task priority.
4. NVIC priority và RTOS task priority là gì? A. Hai cơ chế khác nhau. B. Cùng một số. C. ISR là task. D. NVIC chỉ điều khiển UART.
5. UART baud sai sau khi đổi CPU clock. Việc đầu tiên? A. Tra clock source/divider của UART. B. Thêm delay bận. C. Đổi queue depth. D. Bỏ watchdog.
6. LED Nucleo không sáng dù register GPIO ghi đúng. Tài liệu cần kiểm tra trước? A. Board schematic, pin/LED polarity/jumper. B. RAM size. C. C++ compiler. D. TRACE32 license.
7. Code nào thường chạy trước `main()`? A. UART task. B. Reset handler/startup, khởi tạo runtime. C. Doxygen. D. CAN peer.
8. Muốn đo worst-case latency, phương án nào đáng tin hơn? A. Đọc log khi CPU đang halt. B. Suy từ số lệnh C. C. Đo waveform trên board chạy tự do, workload định nghĩa. D. Chỉ compile.

## NXP S32K344 (câu 9–16)

9. Khi reference manual S32K3 yêu cầu tài khoản hãng, nên làm gì? A. Dùng PDF mirror bất kỳ. B. Lấy tài liệu hợp lệ từ NXP, ghi revision, để pending phần không có quyền. C. Đoán địa chỉ từ STM32. D. Bỏ errata.
10. “Có hardware lockstep/safety feature” cho phép kết luận gì? A. Ứng dụng đã đạt ASIL D. B. Đây là capability của chip/board cần cấu hình và kiểm chứng theo sản phẩm. C. Unit test không cần. D. Watchdog thừa.
11. CAN controller tự nó có thể nối thẳng MCU pin vào mạng CAN ô tô không? A. Có. B. Không, cần transceiver/bus wiring và kiểm termination/peer. C. Chỉ cần UART. D. Chỉ cần Wi-Fi.
12. Port driver GPIO từ STM32 sang S32K344: phần nào tái sử dụng an toàn nhất? A. Địa chỉ MMIO. B. Requirement, API intent và unit test logic; BSP/register phải viết lại. C. Clock registers. D. Startup assembly.
13. Errata phải khớp với gì? A. Tên IDE. B. Exact silicon mask/revision/part. C. Tên file source. D. GPIO18 ESP32.
14. Generated code của RTD thay version. Việc gì cần lưu? A. Chỉ screenshot IDE. B. SDK/RTD/tool version, config, diff và regression result. C. Không cần gì. D. Chỉ tên project.
15. “Safe output after reset” được chứng minh mạnh nhất bằng gì? A. Target reset/power-cycle + đo pad và log init. B. Host fake-MMIO test duy nhất. C. Doxygen output. D. Build log duy nhất.
16. CAN không có ACK/peer và TX timeout. Driver nên làm gì? A. Chờ vô hạn. B. Báo lỗi/timeout/recovery theo SRS và lưu trace. C. Ghi thanh ghi ngẫu nhiên. D. Xóa test.

## ST SPC584B (câu 17–24)

17. SPC584B dùng core nào theo board ST? A. Cortex-M7. B. e200z4 Power Architecture. C. Xtensa LX6. D. RISC-V.
18. VLE là điều gì cần xét khi học SPC58? A. Dạng mã lệnh/decoder ảnh hưởng assembly/disassembly/debug. B. Tên GPIO register. C. RTOS task. D. CAN transceiver.
19. Bộ tài liệu nào hợp với SPC584B70E1? A. STM32 RM0433. B. SPC58 4B RM0449, Book E/VLE, board manual/errata. C. ESP32 TRM. D. TC3xx appendix.
20. Để bắt đầu debug board SPC584B-DIS, trước tiên có thể dùng gì? A. Debugger PLS tích hợp theo board manual. B. Bắt buộc mua TRACE32. C. ESP-IDF monitor. D. Web browser.
21. CAN loopback nội bộ pass chứng minh điều gì? A. Toàn bộ bus điện đã pass. B. Một phần logic controller; peer/transceiver/bus lỗi còn phải test riêng. C. ECU đạt ASIL. D. Không cần CAN tester.
22. Có thể dùng STM32 HAL GPIO trực tiếp cho SPC584B không? A. Có vì cùng hãng ST. B. Không, khác ISA/peripheral/SDK. C. Có nếu đổi pin. D. Có nếu bật VLE.
23. Debugger có thể ảnh hưởng watchdog/timing. Cách xử lý? A. Ghi điều kiện debug và chạy lại test critical ở free-running. B. Luôn coi kết quả halt là worst case. C. Xóa watchdog. D. Bỏ requirement.
24. Mang application FSM từ STM32 sang SPC58: điều gì vẫn phải làm lại? A. Mọi requirement. B. BSP/register/clock/interrupt mapping và target tests. C. Tên state. D. Giải thích lỗi cho người dùng.

## Infineon TC375 (câu 25–32)

25. TC375 dùng ISA/core family nào? A. Cortex-M7. B. e200z4. C. TriCore. D. Xtensa.
26. Với nhiều core/local/shared memory, cần xác định gì? A. Chỉ một biến `volatile`. B. Ownership, memory visibility, synchronization và bus contention. C. Chỉ màu LED. D. Doxygen theme.
27. Context save area/interrupt/trap của TC3xx nên học ở đâu? A. RM0433 STM32. B. TC3xx architecture/user manuals. C. ESP32 GPIO header. D. CAN cable.
28. Interrupt/service request phải kiểm điều gì? A. Chỉ text log. B. Routing/priority/core đích và latency trên target. C. Chỉ task name. D. Chỉ pin voltage.
29. Đường debug ban đầu của TC375 lite kit? A. MiniWiggler/ADS phù hợp board. B. Bắt buộc TRACE32. C. ST-LINK. D. ESP-IDF JTAG.
30. User manual TC3xx mô tả family superset. Khi dùng TC375 cần làm gì? A. Coi mọi feature đều có. B. Xác minh device-specific appendix, part/revision/errata. C. Copy từ TC33x. D. Bỏ register notebook.
31. `volatile` trên biến chia sẻ giữa core có tác dụng gì? A. Đủ để loại bỏ mọi race. B. Không thay thế synchronization/cache policy/atomic cần thiết. C. Tự tạo mutex. D. Tự route interrupt.
32. TRACE32 cần thiết thế nào cho học TC375? A. Bắt buộc trước blink. B. Tùy chọn nâng cao khi có probe/license, miniWiggler/ADS dùng bắt đầu. C. Thay reference manual. D. Chứng nhận safety tự động.

# Thư viện/SDK và mã mẫu chính hãng để học MCU sâu hơn

Tệp này là **bản đồ đọc source**, không phải danh sách thư viện để copy nguyên vào sản phẩm. Ưu tiên tài liệu chip (datasheet → reference manual → errata) khi kiểm địa chỉ/bit/reset/side effect; SDK cho biết hãng *triển khai* abstraction và sequence thế nào. Mã mẫu thường tối ưu cho việc minh họa, có thể bỏ qua timeout, watchdog, safety case hoặc đo điện. Trước khi dùng, ghi **part/board revision, SDK version/commit, compiler, license, ví dụ dùng target nào**, rồi kiểm với phần cứng của bạn.

## 1. Tầng thư viện cần phân biệt

| Tầng | Vai trò | Có thể học gì | Không được suy ra |
|---|---|---|---|
| Device header/SVD | Tên register, base/offset/field, IRQ number | Đối chiếu notebook với RM/errata | Header luôn đúng với mọi silicon revision |
| Startup/BSP | Vector/startup/linker, board pins/clock/debug | Luồng reset→`main`, pin LED/button, memory placement | Board khác dùng cùng wiring |
| LL/iLLD/IP driver | Ghi register, init sequence, polling/ISR/DMA | Order và mask, clock/pin dependency | API luôn ổn định hoặc safe-by-default |
| HAL/RTD/MCAL | API cấp cao + cấu hình/generated code | Contract, validation, lỗi, portability giới hạn | Trộn HAL với raw MMIO trên cùng peripheral là an toàn |
| Example/demo | Một kịch bản build/run | Toolchain, wiring, logs, test entry point | Demo là production-ready hay đã kiểm worst-case/safety |
| Test framework | Assert/mock/CI/target harness | Boundary/fault/regression evidence | Host PASS chứng minh electrical/timing của silicon |

## 2. ESP32 gốc — ESP-IDF (bạn đang có board ảo)

| Nguồn chính hãng | Đọc chính xác phần nào | Việc tự làm |
|---|---|---|
| [ESP-IDF source](https://github.com/espressif/esp-idf) | Đúng checkout **v6.1 đang cài**: `components/soc/esp32/register/soc/`, `components/esp_hal_gpio/esp32/`, `components/esp_driver_gpio/` (tên/vị trí có thể đổi giữa release). | Từ `gpio_set_level()` lần ngược đến HAL/LL và `GPIO_OUT_W1TS/W1TC`; lập bảng 5 thao tác với driver `register_gpio` của mình. |
| [ESP32 GPIO LL source](https://github.com/espressif/esp-idf/blob/master/components/esp_hal_gpio/esp32/include/hal/gpio_ll.h) | IO MUX, GPIO matrix, output enable, set/clear, interrupt status. | Giải thích vì sao `OUT_SEL=SIG_GPIO_OUT_IDX`, vì sao W1TC không đọc-modify-write. **File trên `master` chỉ để định vị; dùng source v6.1 local để so sánh chính xác.** |
| [Ví dụ GPIO chính hãng](https://github.com/espressif/esp-idf/blob/master/examples/peripherals/gpio/generic_gpio/README.md) | GPIO output/input, interrupt, wiring output→input, `menuconfig`, log. | Build `set-target esp32`, thay một input bằng button Wokwi; thêm requirement, expected log, test debounce. |
| [ESP-IDF pytest + Unity](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/contribute/esp-idf-tests-with-pytest.html) | Target tests, multi-DUT và test selection. | Giữ host fake-MMIO test cho transaction, thêm test trên ESP32 thật khi có board. |

**Cảnh báo:** Espressif đánh dấu `gpio_ll.h` là **HAL nội bộ, không phải public application API**. Đọc để hiểu phần cứng; ứng dụng thông thường dùng public GPIO driver. Driver MMIO tự viết là bài học có owner riêng, không dùng cùng chân/peripheral với ESP-IDF GPIO driver. Địa chỉ trong lab chỉ đúng ESP32 gốc.

## 3. ST STM32H753 — STM32CubeH7 + CMSIS

| Nguồn chính hãng | Đọc chính xác phần nào | Việc tự làm |
|---|---|---|
| [STM32CubeH7 package](https://github.com/STMicroelectronics/STM32CubeH7) | `Drivers/CMSIS/`, `Drivers/STM32H7xx_HAL_Driver/`, BSP và `Projects/`. Clone với submodules/version tương thích nếu thật sự build. | Lập cây phụ thuộc CMSIS Core → CMSIS Device → HAL/LL → BSP/app; ghi version, kiểm release notes. |
| [STM32H7 HAL/LL component](https://github.com/STMicroelectronics/stm32h7xx-hal-driver) | Đọc GPIO/RCC/EXTI/TIM/UART LL và HAL: điều kiện clock, validation, callback/error handling. | Với LED, lần từ `HAL_GPIO_WritePin()`/LL đến `BSRR`, so sánh với raw-register driver và tìm những trường hợp API làm thêm. |
| [Arm CMSIS-Core](https://github.com/ARM-software/CMSIS_6) | NVIC, exception, SysTick, intrinsic/barrier, core register API. | Vẽ đường EXTI→NVIC→ISR→task; phân biệt IRQ priority với task priority. CMSIS chỉ chuẩn hóa phần **core**, không làm GPIO hai hãng giống nhau. |
| [NUCLEO-H753ZI note trong STM32CubeH7](https://github.com/STMicroelectronics/STM32CubeH7/blob/master/Projects/NUCLEO-H753ZI/readme.txt) | ST ghi ví dụ NUCLEO-H743ZI tương thích với H753ZI; board H753 vẫn cần schematic riêng cho pin/revision. | Chọn một ví dụ GPIO/EXTI, build nguyên bản, sau đó đổi pin theo schematic của board mình và đo. |

**Bài bắt buộc:** GPIO output + button EXTI + timer/UART. Có register notebook (`RCC`, `MODER`, `PUPDR`, `BSRR`, `EXTI`, `NVIC`) với RM0433; host test kiểm mask/order; target test đo pad/interrupt. Khi học DMA, xem cache/buffer ownership và vùng RAM phù hợp; không giải bằng `volatile` đơn thuần. Một số middleware trong repo STM32CubeH7 không có vì điều khoản click-through, nên không cam kết clone GitHub là toàn bộ package.

## 4. NXP S32K344 — S32K3 RTD, không coi MCUXpresso SDK là thay thế

| Nguồn chính hãng | Đọc chính xác phần nào | Việc tự làm |
|---|---|---|
| [NXP Real-Time Drivers (RTD)](https://www.nxp.com/design/design-center/software/automotive-software/real-time-drivers-rtd%3AAUTOMOTIVE-RTD) và [RTD product brief](https://www.nxp.com/docs/en/product-brief/RTD-S32K3-PB.pdf) | RTD cho AUTOSAR/non-AUTOSAR; xem download trong S32K3 Standard Software, release notes và quyền truy cập. | Ghi version S32DS + RTD + board; mở source/config/generated files của **bản đã tải hợp lệ**. |
| [NXP RTD training](https://www.nxp.com/design/design-center/software/automotive-software/real-time-drivers-rtd%3AAUTOMOTIVE-RTD) | Pins/clocks, timer, LPUART/LIN, FlexCAN; một số khóa/tệp cần đăng nhập. | Trước khi code, giải thích config tool tạo gì, file nào có thể chỉnh tay, init order nằm ở đâu. |
| [NXP S32K3 product/RM](https://www.nxp.com/products/S32K3) | Reference manual `S32K3XXRM`, datasheet, silicon-mask errata. | Đối chiếu `Siul2_Port_Ip`/GPIO/clock/FlexCAN code với register/bit và board schematic. Không lấy địa chỉ từ STM32. |
| [NXP S32K344 FlexCAN RTD example](https://community.nxp.com/t5/S32K-Knowledge-Base/Example-S32K344-FlexCAN-Ip-TX-RX-EnhanceRXFIFO-DMA-test-S32DS3-5/ta-p/2015832) | Ví dụ do NXP TechSupport đăng: TX/RX, Enhanced RXFIFO, DMA, có ghi board/RTD/compiler cụ thể. | Chỉ dùng để học flow; **board/version Q172 trong ví dụ khác T172 của lộ trình**. Port pin/transceiver/config, test hai node hoặc CAN tester. |

**Bài bắt buộc:** chọn một RTD pin/clock example tương ứng S32K344, build không sửa; lần theo call config→IP driver→register; sau đó viết driver GPIO nhỏ và fake-MMIO test. Với CAN, nhiều ví dụ mặc định là **internal loopback**, không chứng minh transceiver/bus. [NXP TechSupport giải thích trường hợp T172](https://community.nxp.com/t5/S32K/Can-communication-in-s32k3X4-evb-t172-rev-b2/td-p/1976029) cần kiểm mode và transceiver, có peer/tester. Ghi exact RTD/IDE version; không hứa example cũ mở được bằng version mới.

## 5. ST SPC584B — SPC5Studio, RLA/HAL/examples trong IDE

| Nguồn chính hãng | Đọc chính xác phần nào | Việc tự làm |
|---|---|---|
| [SPC5-STUDIO](https://www.st.com/en/development-tools/spc5-studio.html) | Framework cho SPC5 Power Architecture, code generation, BSP/startup/driver components và examples. | Tạo project **SPC584B70E1/SPC584B-DIS** đúng target, build ví dụ LED gốc rồi lưu config/tool version. |
| [SPC584B-DIS product](https://www.st.com/en/evaluation-tools/spc584b-dis.html) và [UM2461](https://www.st.com/resource/en/user_manual/um2461-spc584bdis--discovery-board-stmicroelectronics.pdf) | Board wiring, jumper nguồn, debugger PLS, ví dụ firmware đi kèm SPC5Studio. | Đối chiếu pin LED/button/COM với schematic, không suy từ BSP của SPC582B/SPC58EC. |
| [SPC58 4B documentation](https://www.st.com/en/microcontrollers-microprocessors/spc58-4b-line-mcus/documentation.html) | RM0449, datasheet, Book E/VLE, errata; đối chiếu peripheral sequence với generated source. | Notebook clock→SIUL2/pad→interrupt/timer/CAN, ghi field/access/reset và errata. |

**Không có một kho GitHub mở tương đương STM32CubeH7 được ST nêu cho SPC584B:** lấy RLA/HAL và example qua SPC5Studio/Marketplace chính hãng theo quyền dùng. [ST mô tả example wizard và RLA/HAL](https://www.st.com/resource/en/product_presentation/spc5_family_overview.pdf). Không copy SDK/driver binaries của ST vào repo học tập nếu license không cho. Bài tối thiểu là build example, lần từ API GPIO đến generated config và register trong RM0449, rồi host fake-register + target pad test.

## 6. Infineon TC375 — iLLD + AURIX code examples

| Nguồn chính hãng | Đọc chính xác phần nào | Việc tự làm |
|---|---|---|
| [AURIX Development Studio](https://documentation.infineon.com/aurixtc3xx/docs/hqk1751612176436) | Compiler/multicore debugger/iLLD tích hợp; xem release notes và help local. | Build một project board TC375 trước khi sửa source; lưu ADS/iLLD/kit revision. |
| [Infineon AURIX code examples](https://github.com/Infineon/AURIX_code_examples) | Lọc đúng `KIT_A2G_TC375_LITE` và theo thứ tự GPIO/port → ERU interrupt → STM/GTM timer → ASCLIN UART → CAN → DMA/watchdog. Repo có metadata, README/PDF tutorial và code dựa trên iLLD. | Với mỗi example, đọc `Cpu0_Main.c` metadata **device và board đều phải khớp**; trace iLLD call→register trong TC3xx UM + device appendix. |
| [iLLD documentation guidance](https://community.infineon.com/t5/Knowledge-Base-Articles/AURIX-MCU-Where-to-find-documentation-for-Infineon-low-level-drivers-iLLD/ta-p/351148) và [iLLD training](https://www.infineon.com/dgdl/Infineon-AURIX_TC3xx_Infineon_Low_Level_Driver-Training-v01_00-EN.pdf?fileId=5546d46272e49d2a0172eb2bcb14737f) | Device-dependent API, help link trong ADS, source/header. | Đối chiếu interrupt service request, CPU đích, memory/core ownership, watchdog behavior. |

**Bài bắt buộc:** chọn một example đúng kit, vẽ call graph `core0_main → init → iLLD → PORT/SRC/clock`; biến thành driver/API contract và host test cho validation/sequence; đo thật ISR latency/fault ở target. Một số example học tập tắt watchdog: **không xem đó là chính sách production**. Vì repo có tên example/board tương tự nhau, kiểm lại metadata exact chip/step trước khi build; không lấy iLLD TC33x hoặc TC4x cho TC375.

## 7. Thư viện dùng chung cho test, RTOS và tài liệu

| Nguồn | Dùng ở giai đoạn nào | Bài tự kiểm |
|---|---|---|
| [Unity](https://github.com/ThrowTheSwitch/Unity) | Host unit C; có thể dùng trên target tùy SDK | Viết test mask/sequence/error, so với `assert` đang có trong lab ESP32. |
| [Ceedling](https://github.com/ThrowTheSwitch/Ceedling) | Khi số unit test/mocks tăng; chạy bằng compiler **native**, không phải compiler target rồi chạy `.exe` | Tách fake MMIO, test driver + FSM; xuất report/coverage nếu cấu hình. |
| [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) | Khi học scheduler/queue/notification/port; **dùng port/vendor integration đúng target** | Đọc `tasks.c`, `queue.c`, port layer và đo stack/jitter/queue-full trên target. |
| [Doxygen](https://www.doxygen.nl/manual/starting.html) | Sau khi có API contract | Generate API docs; vẫn viết riêng SRS/SAD/ADR/test matrix. |
| [ST Standard C sensor drivers](https://github.com/STMicroelectronics/STMems_Standard_C_drivers) | Tùy chọn khi học portable sensor driver trên nhiều MCU | Xem `platform_read/write` làm ranh giới bus; **ví dụ SPC584B-DIS có chỗ cần tự điền SPI**, không coi là demo chạy sẵn. |

## 8. Công thức đọc **một** library đến nơi đến chốn

1. **Chọn scope:** chỉ một API và một peripheral (ví dụ GPIO output), không đọc toàn SDK tuần đầu.
2. **Khóa version:** part, board, silicon, RM/errata, SDK tag/commit, compiler, build flags, license.
3. **Đọc contract:** prototype, pre/postconditions, error/status, ISR-safe, timeout, thread/core ownership.
4. **Lần đường gọi:** application → BSP/config → HAL/RTD/iLLD → LL/IP → register write/read; vẽ call graph và init sequence.
5. **Đối chiếu hardware:** mỗi base/offset/bit/access/side effect với RM và board schematic; ghi lý do cho RMW, W1C/W1S và barrier/cache.
6. **Chạy sample gốc:** build/flash nếu có board; lưu log, map/size và setup. Nếu không có board, đánh dấu integration **pending**.
7. **Tự viết phiên bản nhỏ:** driver register không chiếm cùng peripheral với vendor driver đồng thời; fake-MMIO unit test positive/negative/sequence.
8. **So sánh và review:** vendor library xử lý gì mà bản tự viết chưa có (clock, lock, power, error, errata)? Tạo ADR chọn dùng vendor driver hay custom driver cho use case.
9. **V-model evidence:** requirement ID → library version/config/source path → test case → PASS/FAIL/pending; rerun khi nâng version.

**Không tự động tải/commit tất cả thư viện**: chúng lớn, thay đổi theo release và một số cần quyền truy cập. Lưu link chính hãng, version/commit và notes tự viết trong Git; khi làm project thực tế mới pin dependency theo license và SBOM.

# Học nhiều họ MCU từ A đến Z, cùng một quy trình V-model

Học xong [nền tảng ESP32 và driver thanh ghi](../01_MCU_Architecture_and_Boot/README.md) rồi mới chuyển họ chip. **Không bê địa chỉ thanh ghi ESP32 sang chip khác.** Mỗi họ có ISA, memory map, clock/reset, interrupt, peripheral, startup, errata và toolchain riêng. Mục tiêu là dùng cùng phương pháp đọc tài liệu → thiết kế → code → kiểm thử, chứ không giả định firmware hoặc HAL có thể đổi chip bằng một `#define`.

## Thứ tự khuyến nghị

| Chặng | Chip/board mẫu | Tại sao học | Khi nào chuyển tiếp |
|---|---|---|---|
| 0 | ESP32 DevKitC / Wokwi hiện có | Tập đọc TRM, MMIO, fake-register test | Hoàn thành quiz, lab GPIO, hồ sơ V-model |
| 1 | [ST STM32H753ZI / NUCLEO-H753ZI](02_STM32H753_ARM.md) | Cortex-M7, NVIC, cache/DMA, SWD; đường sang ARM MCU | Driver GPIO + timer + UART có unit/target test |
| 2 | [NXP S32K344 / S32K3X4EVB-T172](03_NXP_S32K344_ARM.md) | ARM Cortex-M7 trong MCU ô tô; CAN FD, watchdog, RTD | Có CAN fault cases, traceability, target evidence |
| 3 | [ST SPC584B70E1 / SPC584B-DIS](04_ST_SPC584B_POWER.md) | Power Architecture e200z4, nền automotive khác ARM | Boot/register/interrupt/CAN lab + log debug thật |
| 4 | [Infineon AURIX TC375 / KIT_A2G_TC375_LITE](05_INFINEON_TC375_TRICORE.md) | TriCore đa lõi, bus/interconnect, safety mechanisms | Timing/core-ownership/fault campaign có bằng chứng |

Không cần mua đồng thời bốn board. Hãy tận dụng ESP32 + host unit test trước; board khác chỉ mua khi cần xác nhận thật về điện, CAN, timing, startup, watchdog hoặc debug. **Simulator/host test không thay thế silicon.** Tình trạng sản phẩm, giá, quyền tải SDK và giấy phép debug có thể thay đổi; kiểm tra trang hãng trước khi mua/cài.

Lựa chọn mô phỏng miễn phí đã kiểm tra cho giai đoạn đầu: Wokwi cho ESP32; [Renode có mô hình Nucleo-H753ZI](https://github.com/renode/renode/releases) khi sang STM32H753, nhưng phải kiểm mỗi peripheral cần học. Với **đúng** S32K344/SPC584B/TC375, lộ trình chưa mặc định một simulator miễn phí mô phỏng đầy đủ; tiếp tục host fake-register test và ghi target verification là PENDING cho đến khi có board/mô hình được xác minh. Xem [bảng tool theo từng mốc](../00_Roadmap/TOOLING_AND_SELF_TEST.md).

## Các tệp trong module

1. [Lọc kỹ năng trong ảnh tuyển dụng](00_SKILL_FILTER_FROM_JOB.md): học gì ngay, gì tùy vị trí, gì chưa cần.
2. [Quy trình A–Z chung theo V-model](01_COMMON_A_TO_Z_VMODEL.md): 12 gate cho **từng chip**, đầu ra và bằng chứng.
3. Bốn hướng dẫn riêng theo các link ở bảng trên: tài liệu hãng, thứ tự đọc, toolchain, lab và gate.
4. [Bản đồ so sánh kiến trúc](08_SO_SANH_KIEN_TRUC.md) → [quiz 32 câu](09_QUIZ_DA_CHIP.md) → [đáp án](10_DAP_AN_QUIZ_DA_CHIP.md), chỉ làm quiz của họ chip sau khi đọc tài liệu của họ đó.
5. [Capstone cùng một đặc tả trên nhiều chip](06_CROSS_CHIP_CAPSTONE.md): so sánh portability và hồ sơ review.
6. [Thư viện/SDK và mã mẫu chính hãng](11_THU_VIEN_SDK_MA_MAU.md): đọc source theo tầng, biết ví dụ nào đúng board, version/quyền truy cập và cách tự kiểm.

## Cách đi không bị ngợp

Nền tảng ESP32 → STM32H753 → S32K344 → chọn SPC584B hoặc TC375 theo vị trí tuyển dụng → hoàn thành họ còn lại nếu cần. Sau mỗi họ, nộp một **bộ hồ sơ**: SRS có ID, kiến trúc, register notebook, driver/API contract, unit/target test, lỗi/errata, timing/memory report, traceability và quyết định còn pending. Hoàn thành checklist không tự động đồng nghĩa đạt chức danh senior; kỹ năng đó cần thêm kinh nghiệm sản phẩm, review và xử lý lỗi thật.

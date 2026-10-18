# 01 — Kiến trúc MCU, thanh ghi và boot (ESP32)

Đích của học phần là tự đọc datasheet/TRM để thiết kế, cài đặt và kiểm thử một driver thanh ghi nhỏ; giải thích được CPU, bộ nhớ, bus, clock, interrupt, peripheral và luồng boot. Thời lượng hợp lý: **4–6 tuần × 8–12 giờ/tuần**. Đây là nền tảng để đi tiếp RTOS; không có phần ôn cú pháp C.

**Bắt đầu tại [Bài 01 — Kiến trúc MCU và các thanh ghi cần thiết](theory/01_CPU_SOC_KIEN_TRUC.md):** học block diagram, ba loại thanh ghi, memory map và bảng địa chỉ/bit GPIO18 trước khi sang các bài lý thuyết khác. Bài này chưa yêu cầu viết code.

## Đi theo đúng thứ tự

1. **Lý thuyết:** đọc lần lượt toàn bộ mục 01–06 dưới đây. Mỗi mục có khái niệm, điều phải giải thích được và chỗ cần tra trong tài liệu chip.
2. **Trắc nghiệm:** làm [`07_TRAC_NGHIEM.md`](theory/07_TRAC_NGHIEM.md) trước; sau đó tự chấm bằng [`08_DAP_AN.md`](theory/08_DAP_AN.md). Đạt ít nhất 85% và giải thích được câu sai.
3. **Driver thanh ghi:** đọc [`register_gpio/README.md`](register_gpio/README.md), lần theo register map, build thư viện GPIO có input/output, pull, đọc mức, set/clear và deinit. Chạy Wokwi cùng bài test MMIO trên host.
4. **Bài tập:** làm [`09_BAI_TAP.md`](theory/09_BAI_TAP.md), nộp bằng chứng theo V-model.

| Thứ tự | Nội dung | Đầu ra bắt buộc |
|---|---|---|
| 01 | [Kiến trúc MCU và các thanh ghi cần thiết](theory/01_CPU_SOC_KIEN_TRUC.md) | Sơ đồ CPU–bus–memory–GPIO–pad; sổ tay thanh ghi GPIO18 gồm địa chỉ, bit và ngữ nghĩa ghi |
| 02 | [Thanh ghi, memory map và bus](theory/02_THANH_GHI_MEMORY_BUS.md) | Register map GPIO18, mô tả R/W, RO, WO, W1TS/W1TC |
| 03 | [Clock, reset, boot và linker](theory/03_CLOCK_RESET_BOOT.md) | Boot flow và cây clock mức hệ thống, giải thích ELF/Flash |
| 04 | [Interrupt, ISR, DMA](theory/04_NGAT_DMA.md) | Sơ đồ peripheral → interrupt matrix → CPU → ISR → task |
| 05 | [Peripheral, GPIO và điện](theory/05_NGOAI_VI_GPIO.md) | Pin map, tín hiệu GPIO đến pad, giới hạn điện |
| 06 | [Timing, RTOS và kiến trúc driver](theory/06_TIMING_RTOS_DRIVER.md) | Timing/error/ownership contract cho driver |
| 07–08 | [Đề trắc nghiệm](theory/07_TRAC_NGHIEM.md) và [đáp án](theory/08_DAP_AN.md) | ≥85%, giải thích bằng chứng trong TRM |
| Lab | [Thư viện GPIO qua thanh ghi](register_gpio/README.md) | Build ESP-IDF, chạy Wokwi, host test, review register writes |
| Cuối | [Bài tập và hồ sơ V-model](theory/09_BAI_TAP.md) | Requirements → design → code → test → traceability |

## Tài liệu gốc để tra cứu

- [ESP32 Series Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf): pin, boot modes, CPU/memory, clock, electrical characteristics.
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf): Chapter 3 (System and Memory), 6 (IO MUX/GPIO Matrix), 7 (Reset and Clock), 8 (Interrupt Matrix); các chương peripheral tương ứng.
- [ESP-IDF Application Startup Flow](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html): reset đến `app_main()`.
- [ESP-IDF Interrupt Allocation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/intr_alloc.html).
- [ESP-IDF FreeRTOS](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html).

**Phạm vi chính xác:** tất cả địa chỉ trong lab là của ESP32 gốc (Xtensa LX6), không áp dụng cho ESP32-S3/C3 hay MCU khác. ESP-IDF vẫn cung cấp boot, scheduler và build; chỉ GPIO được điều khiển trực tiếp bằng register trong lab. Với sản phẩm thực tế, cân nhắc HAL/driver chính thức để tránh tranh chấp sở hữu peripheral và lỗi portability.

## Bài đã có trước

[`Week_01/README.md`](Week_01/README.md) là bài GPIO qua API ESP-IDF/Wokwi đã có. Hãy dùng nó **sau** lab thanh ghi để so sánh abstraction và đối chiếu output của hai cách điều khiển cùng một mạch.

Khi muốn đọc source thư viện hãng để hiểu driver bên dưới API, dùng [bản đồ ESP-IDF GPIO/LL, ví dụ chính hãng và bài lần theo register](../10_MCU_Families_and_Portability/11_THU_VIEN_SDK_MA_MAU.md). Đọc source đúng bản ESP-IDF v6.1 đã cài, không lấy file `master` làm bằng chứng cho version local.

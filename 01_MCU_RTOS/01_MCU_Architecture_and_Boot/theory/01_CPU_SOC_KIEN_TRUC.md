# Bài 01 — Kiến trúc MCU và các thanh ghi cần thiết

Đây là bài **đầu tiên** của nhánh MCU. Học lý thuyết và đọc tài liệu gốc trước; chưa cần làm trắc nghiệm hay viết driver. Ví dụ xuyên suốt là **ESP32 gốc (Xtensa LX6), GPIO18**, LED nối GPIO18 → điện trở 1 kΩ → anode LED, cathode → GND. Các địa chỉ dưới đây dành riêng cho ESP32 gốc và đã đối chiếu với header của **ESP-IDF v6.1**; không chép sang ESP32-S3/C3 hoặc họ chip khác.

## Sau bài này bạn phải làm được gì?

1. Tự vẽ đường đi CPU → bus → GPIO → GPIO matrix/IO MUX → pad → LED và giải thích vai trò của từng khối.
2. Phân biệt thanh ghi CPU, thanh ghi ngoại vi ánh xạ bộ nhớ (MMIO), ô RAM và vị trí firmware trong Flash.
3. Từ base address + offset tìm địa chỉ thanh ghi, xác định bit cần đọc/ghi và hiệu ứng của thao tác đó.
4. Nêu đúng thứ tự cấu hình GPIO18, lý do dùng `W1TS/W1TC`, và giới hạn của việc đọc lại output latch.
5. Biết tra datasheet, TRM, header SoC, source driver hãng và ghi một “sổ tay thanh ghi” có thể kiểm chứng.

## 1. MCU thực chất gồm những gì?

**CPU core** thực thi lệnh; **MCU/SoC** ghép CPU với ROM, RAM, bus, interrupt controller, clock/reset và các ngoại vi. **Module** thêm Flash, thạch anh, antenna và linh kiện phụ; **board** thêm nguồn, USB-UART, nút reset/boot và đầu nối. Vì vậy `GPIO18` là tín hiệu của SoC, còn vị trí chân trên board phải tra sơ đồ board.

```text
           SPI Flash ── controller/cache/MMU ──┐
                                                │
 PRO CPU / APP CPU ── bus/interconnect ── ROM, SRAM
          │                   │
          │                   ├── GPIO registers ── GPIO matrix ── IO MUX ── pad GPIO18 ── LED
          │                   └── UART, timer, SPI, I2C, DMA, ...
          └── interrupt controller              clock/reset cấp nhịp, đặt lại các khối
```

ESP32 hai core có PRO CPU và APP CPU. CPU thực hiện load/store; bus giải mã địa chỉ rồi chuyển giao dịch đến RAM hay ngoại vi. Khi ghi GPIO, phần cứng GPIO giữ giá trị output latch, matrix chọn nguồn tín hiệu, IO MUX chọn chức năng chân, pad lái mức điện áp ra mạch LED. Một lệnh ghi thanh ghi **không** chứng minh pin đã đổi điện áp đúng lúc đó; muốn kiểm chứng pin cần LED/logic analyzer/oscilloscope hoặc mô hình mô phỏng phù hợp.

Đọc [ESP32 datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) phần tổng quan/block diagram, rồi [ESP32 Technical Reference Manual (TRM)](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf) phần system/memory map và GPIO/IO MUX. Tên chương có thể thay đổi theo bản PDF; tìm bằng tên khối thay vì chỉ dựa vào số chương.

## 2. Ba loại “thanh ghi” thường bị lẫn

| Loại | Ví dụ | Ai định nghĩa? | Ý nghĩa |
| --- | --- | --- | --- |
| Thanh ghi kiến trúc CPU | PC, `A0`–`A15`, `PS`, `EXCCAUSE`, `EXCVADDR` của Xtensa | Kiến trúc CPU | PC chỉ lệnh kế tiếp; nhóm A chứa toán hạng/trạng thái gọi hàm; PS chứa trạng thái CPU/interrupt; cặp EXC giúp chẩn đoán exception. Không dùng để bật GPIO. |
| Thanh ghi ngoại vi MMIO | `GPIO_OUT_W1TS_REG`, `GPIO_ENABLE_REG`, `IO_MUX_GPIO18_REG` | Nhà sản xuất SoC | Có địa chỉ trong không gian CPU, nhưng đọc/ghi là giao dịch phần cứng với GPIO/IO MUX, **không** phải ô RAM. |
| Biến trong RAM | `counter`, stack task, `buffer` | Chương trình/linker/runtime | Dữ liệu ứng dụng. Địa chỉ RAM không đồng nghĩa địa chỉ peripheral dù cùng cú pháp con trỏ C. |

CPU có pipeline và cache; branch, cache miss, bus contention, interrupt làm độ trễ thay đổi. `volatile` giúp compiler không loại bỏ/gộp truy cập MMIO theo cách của biến thường, nhưng không biến thao tác đọc–sửa–ghi thành nguyên tử, không tạo lock giữa hai core và không tự giải quyết vấn đề thứ tự bộ nhớ của toàn hệ thống.

ESP32 có ROM và SRAM nội; firmware thường nằm trong SPI Flash ngoài SoC/module, có phần được ánh xạ qua cache và phần được nạp vào IRAM/DRAM khi boot. Phân biệt:

- **IROM/DROM**: vùng instruction/data đọc từ Flash qua ánh xạ/cache; **IRAM/DRAM**: RAM nội cho lệnh/dữ liệu; **ROM**: mã có sẵn trong chip.
- `.data` có giá trị khởi tạo; `.bss` được xóa về 0; stack lưu ngữ cảnh gọi hàm/task, heap là vùng cấp phát động. Đừng đồng nhất tên section với một địa chỉ cố định: xem linker map của build cụ thể.
- **MMIO** là vùng địa chỉ của ngoại vi, không nằm trong heap/stack. Flash offset của image dùng khi nạp chương trình cũng **không phải** địa chỉ thanh ghi GPIO.

Đọc [ESP-IDF Memory Types](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/memory-types.html) và [startup flow](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html). Sau bootloader, runtime chuẩn bị bộ nhớ và gọi `app_main()` trong ngữ cảnh main task; `app_main()` không phải reset vector. Sau khi build, `idf.py size` và file `.map` cho biết code/data thực tế được đặt ở đâu.

## 3. Cách đọc một thanh ghi từ tài liệu gốc

Với **mỗi** thanh ghi, lập đủ 9 cột: tên; chip/revision; base; offset/địa chỉ tuyệt đối; độ rộng; quyền/kiểu ghi (`RO`, `RW`, `W1TS`, `W1TC`...); trường bit và giá trị; trạng thái sau reset; hiệu ứng phụ/điều kiện trước khi dùng. Cuối cùng ghi **nguồn và phiên bản** tài liệu.

Ví dụ trên ESP32 gốc:

```text
DR_REG_GPIO_BASE       = 0x3FF44000
GPIO_OUT_W1TS_REG      = DR_REG_GPIO_BASE + 0x0008 = 0x3FF44008
GPIO18_MASK            = 1 << 18 = 0x00040000
ghi GPIO18_MASK vào GPIO_OUT_W1TS_REG → đặt bit 18 của output latch thành 1
ghi GPIO18_MASK vào GPIO_OUT_W1TC_REG → xóa bit 18 của output latch thành 0
```

`W1TS` nghĩa là **write one to set**, `W1TC` là **write one to clear**: bit ghi `1` tác động, bit ghi `0` không đổi. Đây là thanh ghi tác động theo *lệnh ghi*, không phải nơi lưu trạng thái để dùng `|=`. Đọc lại `GPIO_OUT_REG` mới là đọc latch. Một thao tác W1TS/W1TC tránh read–modify–write đối với các bit output khác, nhưng không tự thay thế mọi đồng bộ giữa task, core và ISR.

## 4. Bản đồ thanh ghi tối thiểu cho LED GPIO18

Các hằng số được kiểm từ `components/soc/esp32/register/soc/{reg_base,gpio_reg,io_mux_reg}.h` và `components/soc/esp32/include/soc/gpio_sig_map.h` của ESP-IDF **v6.1**. Tra TRM để hiểu phần cứng; header xác nhận tên/offset cho phiên bản SDK đang dùng. Tất cả thanh ghi bên dưới rộng 32 bit theo nhóm GPIO/IO MUX này.

| Thanh ghi | Offset → địa chỉ | Trường cần biết | Khi nào dùng |
| --- | --- | --- | --- |
| `GPIO_OUT_REG` | `0x0004` → `0x3FF44004` | bit 18 là output latch; đọc trạng thái latch | Kiểm tra giá trị điều khiển đã đặt, **không** thay phép đo mức pad. |
| `GPIO_OUT_W1TS_REG` | `0x0008` → `0x3FF44008` | ghi `0x00040000` để đặt latch GPIO18 = 1 | LED mức cao, sau khi pad đã được cấu hình đúng. |
| `GPIO_OUT_W1TC_REG` | `0x000C` → `0x3FF4400C` | ghi `0x00040000` để xóa latch GPIO18 = 0 | LED mức thấp. |
| `GPIO_ENABLE_REG` | `0x0020` → `0x3FF44020` | bit 18: `1` cho phép output | Đọc xác nhận output enable. |
| `GPIO_ENABLE_W1TS_REG` | `0x0024` → `0x3FF44024` | ghi mask để bật output driver | Làm **cuối** sau khi chọn mux và đặt mức latch ban đầu. |
| `GPIO_ENABLE_W1TC_REG` | `0x0028` → `0x3FF44028` | ghi mask để tắt output driver | Dùng đầu tiên để tránh glitch trong lúc cấu hình. |
| `GPIO_IN_REG` | `0x003C` → `0x3FF4403C` | bit 18 là đường input mẫu (read-only) | Chỉ diễn giải khi input path được cấu hình phù hợp; không xem là thiết bị đo điện áp độc lập. |
| `GPIO_PIN18_REG` | `0x00D0` → `0x3FF440D0` | `PAD_DRIVER` bit 2: `0` push-pull, `1` open-drain; `INT_TYPE` bits 9:7; `INT_ENA` bits 17:13 | LED push-pull cần bit 2 = 0; phần interrupt học ở bài riêng. |
| `GPIO_FUNC18_OUT_SEL_CFG_REG` | `0x0578` → `0x3FF44578` | `OUT_SEL` bits 8:0 = **256** (`SIG_GPIO_OUT_IDX`) để lấy GPIO software output; `OEN_SEL` bit 10 = 1 để output enable do GPIO điều khiển | Cấu hình GPIO matrix. |
| `IO_MUX_GPIO18_REG` | IO MUX `0x0070` → `0x3FF49070` | `MCU_SEL` bits 14:12 = **2** để chọn GPIO function; `FUN_IE` bit 9; `FUN_PU` bit 8; `FUN_PD` bit 7 | Đưa tín hiệu GPIO ra đúng chân; pull-up/down không cần cho LED output. |

`DR_REG_IO_MUX_BASE = 0x3FF49000`. Offset IO MUX **không** thể suy ra bằng công thức `pin × 4` cho mọi chân; hãy tra header/TRM từng chân. Một số chân chỉ input (GPIO34–39), một số bị ràng buộc bởi boot/Flash/JTAG/board. GPIO18 phù hợp cho lab LED này nhưng vẫn kiểm tra sơ đồ board đang dùng.

Header sinh tự động có thể gắn nhãn truy cập tổng quát `R/W` cho thanh ghi `W1TS/W1TC` hoặc để reset value là `x`; **ngữ nghĩa phần cứng và reset value phải đối chiếu TRM**, không tự coi `x` là 0. Không ghi tràn cả word `GPIO_PIN18_REG`/IO MUX vì có thể làm đổi trường không liên quan: sau này driver chỉ sửa những bit đã xác định.

### Đường đi và thứ tự cấu hình

```text
GPIO_OUT bit18 (latch) ── GPIO matrix OUT_SEL=256 ── IO MUX MCU_SEL=2
             │                         │                        │
GPIO_ENABLE bit18 ─────────────── chọn output enable ───────── pad GPIO18
```

1. Ghi mask vào `GPIO_ENABLE_W1TC_REG` để pad chưa lái trong lúc đổi cấu hình.
2. Chọn software GPIO output qua `GPIO_FUNC18_OUT_SEL_CFG_REG`; đặt `OEN_SEL` cho GPIO điều khiển output enable.
3. Chọn push-pull (`PAD_DRIVER=0`) và chức năng GPIO (`MCU_SEL=2`); xử lý pull-up/down/input enable theo yêu cầu mạch.
4. Đặt latch ban đầu bằng `GPIO_OUT_W1TC_REG` hoặc `GPIO_OUT_W1TS_REG` **trước** khi bật output, tránh nháy ngoài ý muốn.
5. Ghi `GPIO_ENABLE_W1TS_REG`; đọc `GPIO_OUT_REG` và `GPIO_ENABLE_REG` để xác nhận cấu hình logic, rồi quan sát LED/pad.

Đây là chuỗi khái niệm cho ESP32 gốc. Lab thực hành có source cụ thể ở [driver MMIO GPIO18](../register_gpio/main/mmio_gpio.c); đối chiếu nó với [GPIO API chính thức](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html) và source `gpio_ll.h` trong **đúng checkout v6.1**. LL/HAL nội bộ của IDF là tài liệu tham khảo triển khai, không mặc nhiên là public API ổn định cho ứng dụng.

## 5. Những thanh ghi sẽ học ngay sau, nhưng chưa cần ghi trong lab đầu

- `GPIO_STATUS_REG` (`0x3FF44044`) và `GPIO_STATUS_W1TC_REG` (`0x3FF4404C`) chứa/clear trạng thái interrupt GPIO. Cùng với `INT_TYPE`/`INT_ENA` của `GPIO_PIN18_REG`, chúng thuộc bài ngắt; đừng bật interrupt khi chưa có ISR, routing và quy trình clear hợp lệ.
- Clock/reset của nhiều peripheral được điều khiển bởi `DPORT_PERIP_CLK_EN_REG` (`0x3FF000C0`) và `DPORT_PERIP_RST_EN_REG` (`0x3FF000C4`). **Không** tự ghi hai thanh ghi này để bật LED GPIO18: đây không phải một “GPIO clock bit” chung. Học clock tree/reset cho UART/timer/SPI ở bài sau và kiểm từng bit theo TRM.
- Thanh ghi UART/timer, interrupt matrix, DMA, watchdog, RTC có bank riêng. Học theo thứ tự: chức năng → clock/reset → pin/route → dữ liệu/interrupt → lỗi/timeout; đừng học thuộc địa chỉ rời rạc.

Interrupt phát sinh từ sự kiện phần cứng; exception liên quan trap/lỗi CPU; task là thực thể scheduler. ISR không phải “task ưu tiên cao” và phải dùng API an toàn cho interrupt. Trên ESP32 hai core, cùng một peripheral có thể được task/core/ISR khác truy cập; thiết kế driver cần chỉ rõ ai sở hữu thanh ghi và cách đồng bộ.

## 6. Sổ tay thanh ghi mẫu và cách tự kiểm

| Mục | Ghi cho `GPIO_OUT_W1TS_REG` |
| --- | --- |
| Chip / SDK | ESP32 gốc / ESP-IDF v6.1 |
| Base + offset | `0x3FF44000 + 0x0008` |
| Địa chỉ / độ rộng | `0x3FF44008` / 32 bit |
| Bit tác động | GPIO18: bit 18, mask `0x00040000` |
| Ngữ nghĩa | Ghi `1` đặt output latch; ghi `0` không đổi; không dùng read–modify–write trên thanh ghi này |
| Điều kiện trước | GPIO matrix + IO MUX đúng, mức latch chọn trước khi bật output |
| Kiểm chứng | Đọc `GPIO_OUT_REG` bit 18; quan sát LED sau khi bật `GPIO_ENABLE` |
| Nguồn | TRM GPIO, ESP-IDF v6.1 `reg_base.h` + `gpio_reg.h`; đối chiếu driver/LL hãng |

Tự điền bảng tương tự cho **9 thanh ghi còn lại** ở mục 4 (có thể gộp cặp W1TS/W1TC nếu mô tả đủ hai địa chỉ). Khi tìm một chip khác, **làm lại từ tài liệu của chip đó**; chỉ chuyển phương pháp, không chuyển địa chỉ hay giá trị `MCU_SEL`.

## Đọc tiếp theo thứ tự

1. Đọc lại datasheet/TRM và vẽ block diagram từ trí nhớ; chỉ ra Flash, RAM, bus, GPIO, matrix, IO MUX, pad và clock/reset.
2. Hoàn thành các bài lý thuyết còn lại trong [mục Kiến trúc và Boot](../README.md): memory map, startup, clock/reset, interrupt, peripheral/bus. Sau **toàn bộ** lý thuyết mới làm trắc nghiệm.
3. Sau trắc nghiệm, làm lab thư viện GPIO bằng MMIO rồi đối chiếu với public API; so sánh source với [thư viện/SDK chính hãng và bài đọc source](../../10_MCU_Families_and_Portability/11_THU_VIEN_SDK_MA_MAU.md); lưu requirement, thiết kế, test case và kết quả theo V-model.

**Nguồn gốc cần giữ khi tra cứu:** [ESP32 datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf), [ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf), [ESP-IDF source](https://github.com/espressif/esp-idf), [ESP-IDF Memory Types](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/memory-types.html), [ESP-IDF startup](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html).

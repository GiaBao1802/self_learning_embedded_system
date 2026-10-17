# Lab — Viết thư viện GPIO qua thanh ghi ESP32

**Làm lab này sau khi hoàn thành lý thuyết 01–06 và trắc nghiệm.** Chỉ dùng ESP32 gốc và các chân GPIO18/19/21/22/23. Demo hiện tại dùng GPIO18 → 1 kΩ → LED → GND, đúng với `diagram.json`. ESP-IDF phụ trách boot, FreeRTOS và build; phần GPIO do code của bạn ghi trực tiếp MMIO, không gọi `driver/gpio.h`.

## Contract thư viện

| API | Tác dụng | Register chính |
|---|---|---|
| `mmio_gpio_config_output(pin, initial_high)` | tắt output trước khi đổi mux/matrix, chọn push-pull, không pull; đặt latch trước khi enable pad | `GPIO_ENABLE_W1TC`, `GPIO_FUNCn_OUT_SEL_CFG`, `IO_MUX_GPIOx`, `GPIO_PINn`, `GPIO_OUT_W1TS/W1TC`, `GPIO_ENABLE_W1TS` |
| `mmio_gpio_config_input(pin, pull)` | tắt output, chọn GPIO function, bật input, chọn pull | `GPIO_ENABLE_W1TC`, `IO_MUX_GPIOx` |
| `mmio_gpio_write(pin, high)` | set hoặc clear riêng một bit | `GPIO_OUT_W1TS/W1TC` |
| `mmio_gpio_read(pin, *high)` | đọc mức pad/input | `GPIO_IN` |
| `mmio_gpio_deinit(pin)` | tắt output, đưa latch về 0, tắt pull/input | `GPIO_ENABLE_W1TC`, `GPIO_OUT_W1TC`, `IO_MUX_GPIOx` |

Pin ngoài tập cho phép trả `MMIO_GPIO_INVALID_PIN` **trước khi ghi register**; input null hoặc pull sai trả `MMIO_GPIO_INVALID_ARGUMENT`. `write/read` yêu cầu caller đã cấu hình mode phù hợp và nắm quyền sở hữu pin. Không có lock nội bộ cho IO MUX RMW: một owner cấu hình pin và serialize reconfiguration. `deinit` không phục hồi toàn bộ trạng thái trước init.

## Địa chỉ và bit cần tự kiểm tra

| Register | Địa chỉ / công thức | Lý do |
|---|---|---|
| `GPIO_OUT_W1TS` / `W1TC` | `0x3FF44008` / `0x3FF4400C` | set/clear GPIO0–31 không RMW |
| `GPIO_ENABLE_W1TS` / `W1TC` | `0x3FF44024` / `0x3FF44028` | bật/tắt output driver |
| `GPIO_IN` | `0x3FF4403C` | đọc pad/input GPIO0–31 |
| `GPIO_PINn` | `0x3FF44088 + 4*n` | bit 2 là PAD_DRIVER (0 = push-pull) |
| `GPIO_FUNCn_OUT_SEL_CFG` | `0x3FF44530 + 4*n` | `OUT_SEL=256` software GPIO, `OEN_SEL=1` dùng GPIO_ENABLE |
| `IO_MUX_GPIO18` | `0x3FF49070` | `MCU_SEL[14:12]=2`, `FUN_IE=9`, `FUN_PU=8`, `FUN_PD=7` |

Đối chiếu [ESP32 TRM Chapter 6](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf) và header ESP-IDF 6.1 `components/soc/esp32/register/soc/`. Chú ý khác biệt giữa **địa chỉ tuyệt đối** và **offset**.

## Chạy trên máy

Mở thư mục `register_gpio/` riêng trong VS Code. Trong terminal **IDF PowerShell Environment** đã cài:

```powershell
idf.py set-target esp32
idf.py build
```

Sau build, mở `diagram.json` và chạy **Wokwi: Start Simulator**; quan sát LED và log `GPIO18 level=...`. Wokwi cần extension/license đã kích hoạt. Khi chuyển máy hoặc di chuyển folder sau khi build, xóa riêng cache build bằng `idf.py fullclean` rồi build lại.

## Tự test trước Wokwi

`tests/test_mmio_gpio.c` thay lớp truy cập MMIO bằng fake register bank. Test kiểm tra pin sai không tạo ghi, IO MUX giữ bit không liên quan, output selector, pull/input, địa chỉ/mask và thứ tự đặt latch trước khi enable. Biên dịch bằng compiler C **native cho Windows** (ví dụ Clang đã cấu hình host toolchain, hoặc GCC từ MSYS2):

```powershell
clang -std=c11 -Wall -Wextra -Werror -I main main/mmio_gpio.c tests/test_mmio_gpio.c -o mmio_gpio_test.exe
./mmio_gpio_test.exe
```

Nếu dùng MSYS2 GCC, thay `clang` bằng `gcc`. Compiler `xtensa-esp32-elf-gcc` hoặc `esp-clang` mặc định của ESP-IDF tạo chương trình cho chip, **không chạy được** file test trên Windows.

`mmio_gpio_test.exe` là artifact local; không commit. Đây là unit test logic giao dịch register. Wokwi là integration test với mô hình chip. Phép đo glitch/điện/timing phải dùng board ESP32 thật, logic analyzer/oscilloscope theo requirement. Xem [hồ sơ V-model](docs/VMODEL.md) để biết test nào chứng minh requirement nào.

## Chỗ cần đọc trong source

1. `main/mmio_gpio.c`: địa chỉ, mask, sequence init và validate.
2. `main/mmio_port_esp32.c`: `volatile` 32-bit MMIO.
3. `tests/test_mmio_gpio.c`: cách giả lập MMIO và kiểm tra read/write.
4. `main/main.c`: ứng dụng dùng driver mà không biết từng địa chỉ.

Sau khi chạy đúng, làm [`09_BAI_TAP.md`](../theory/09_BAI_TAP.md) theo thứ tự. Đừng mở bài API cũ `Week_01/esp32_gpio` trước khi đã giải thích được từng register trong lab này.

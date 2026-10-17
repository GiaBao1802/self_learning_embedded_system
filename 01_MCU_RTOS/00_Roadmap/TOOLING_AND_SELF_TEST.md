# Tool để tự học, tự test và lưu bằng chứng MCU/RTOS

Chọn tool theo câu hỏi cần trả lời. **Build pass** chỉ chứng minh compile/link; **host unit** kiểm tra logic; **simulator** kiểm tra firmware trên mô hình; **board + thiết bị đo** xác nhận điện và timing vật lý.

| Mục tiêu | Tool | Dùng ngay / sau này | Bằng chứng nên lưu |
|---|---|---|---|
| Build firmware ESP32 | ESP-IDF 6.1, `idf.py build`, CMake/Ninja | Đã cài trên máy này | Build log, ELF/BIN, version, commit |
| Size/memory budget | `idf.py size`, `size-components`, map file, `readelf` | Dùng ngay | Bảng RAM/Flash trước–sau |
| Mô phỏng logic GPIO/serial | Wokwi VS Code | Đã cài; license do người dùng kích hoạt | Video/log/screenshot + scenario |
| Host unit test MMIO | **Native** Clang/GCC/MSVC, fake MMIO trong `register_gpio/tests` | Cần cài compiler native Windows; `esp-clang` kèm ESP-IDF mặc định target RISC-V/ESP, không chạy `.exe` trên Windows | Output PASS/FAIL, warning log |
| Test trên chip | ESP-IDF Unity + `pytest-embedded` | Khi có board ESP32 | Test report, serial log, board revision |
| Tự động mô phỏng trong CI | Wokwi CLI + scenario, GitHub Actions | Sau khi đã có scenario/token CI | CI job và scenario result |
| Debug target | `idf.py monitor`, GDB + OpenOCD/JTAG | Monitor dùng ngay; JTAG cần adapter/board | Backtrace, register dump, root cause |
| Phân tích crash | ESP-IDF Core Dump | Khi có fault handling | Core dump, decoded stack, regression test |
| Static checks | Compiler `-Wall -Wextra -Werror`, clang-tidy/cppcheck | Thêm theo module | Finding, disposition và suppressions có lý do |
| Đo timing/điện | Logic analyzer, oscilloscope, multimeter, bench PSU | Cần board thật | Waveform CSV, min/mean/max, setup đo |
| Mô phỏng CPU/firmware khác | Espressif QEMU; Zephyr native_sim/Twister, Renode tùy board | Module nâng cao | Test log và danh sách peripheral mô phỏng |
| Traceability/V-model | Markdown requirement IDs + test matrix, Git; về sau issue/CI | Dùng ngay | Requirements ↔ design ↔ code ↔ test ↔ result |

## Quy trình tự test bài `register_gpio`

1. Đọc requirement và test matrix trong [`VMODEL.md`](../01_MCU_Architecture_and_Boot/register_gpio/docs/VMODEL.md); tự dự đoán test pass/fail.
2. Cài compiler **native** cho Windows nếu muốn chạy host test. Ví dụ [MSYS2 UCRT64 toolchain](https://www.msys2.org/) hoặc [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/); mở terminal đúng môi trường và kiểm tra `gcc --version`/`clang --version` cùng target trước khi compile. Lệnh test có trong [README lab](../01_MCU_Architecture_and_Boot/register_gpio/README.md).
3. Chạy `idf.py build` từ project, sau đó Wokwi VS Code. Quan sát LED/log; đổi một yêu cầu thì thêm test tương ứng.
4. Khi có board thật, đo trạng thái GPIO18 lúc cấp nguồn/reset và cạnh LED; lưu tool setup, supply voltage, firmware hash, nhiệt độ nếu timing/điện quan trọng.
5. Với bug, tạo test tái hiện trước khi sửa; sau sửa ghi root cause và regression result.

## Nguồn chính thức

- [ESP-IDF build project](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/start-project.html) và [phân tích kích thước](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/performance/size.html).
- [ESP-IDF tests với pytest](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/contribute/esp-idf-tests-with-pytest.html) và [JTAG/GDB](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/jtag-debugging/index.html).
- [Wokwi CLI](https://docs.wokwi.com/wokwi-ci/cli-installation), [automation scenarios](https://docs.wokwi.com/wokwi-ci/automation-scenarios), [GitHub Actions](https://docs.wokwi.com/wokwi-ci/github-actions).
- [Espressif QEMU](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/qemu.html), [Zephyr Twister](https://docs.zephyrproject.org/latest/develop/test/twister.html).

Không cần mua mọi tool trước. Điểm bắt đầu đủ dùng là ESP-IDF + Wokwi + compiler native cho host test; board/logic analyzer chỉ cần khi đến yêu cầu có bằng chứng điện/timing.

Khi chuyển họ chip, dùng [bảng chip/board/tool riêng](../10_MCU_Families_and_Portability/README.md). STM32CubeIDE/ST-LINK, NXP S32 Design Studio/debugger, SPC5Studio/PLS, AURIX Development Studio/miniWiggler là đường khởi đầu theo từng kit; TRACE32 là tùy chọn chuyên sâu khi có probe/license phù hợp. Tool host/CI/Doxygen hỗ trợ chung nhưng **không** thay thế target test.

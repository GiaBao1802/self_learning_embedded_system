# Tool để tự học, tự test và lưu bằng chứng MCU/RTOS

Chọn tool theo câu hỏi cần trả lời. **Build pass** chỉ chứng minh compile/link; **host unit** kiểm tra logic; **simulator** kiểm tra firmware trên mô hình; **board + thiết bị đo** xác nhận điện và timing vật lý.

| Mục tiêu | Tool | Dùng ngay / sau này | Bằng chứng nên lưu |
|---|---|---|---|
| Build firmware ESP32 | ESP-IDF 6.1, `idf.py build`, CMake/Ninja | Đã cài trên máy này | Build log, ELF/BIN, version, commit |
| Size/memory budget | `idf.py size`, `size-components`, map file, `readelf` | Dùng ngay | Bảng RAM/Flash trước–sau |
| Mô phỏng GPIO/serial ESP32 | Wokwi browser hoặc VS Code | Đã chạy được trên máy; Community cho project công khai, VS Code cần license (miễn phí cho dự án open-source theo Wokwi) | Video/log/screenshot + scenario |
| Host unit test MMIO | **Native** GCC/Clang/MSVC + fake MMIO trong `register_gpio/tests`; Unity/Ceedling khi test lớn hơn | Compiler native Windows cần có trong terminal; `esp-clang`/`xtensa-esp32-elf-gcc` target chip không tạo `.exe` Windows | Output PASS/FAIL, warning log, test ID |
| Test trên chip | ESP-IDF Unity + `pytest-embedded` | Khi có board ESP32 | Test report, serial log, board revision |
| Tự động mô phỏng trong CI | Wokwi CLI + scenario, GitHub Actions | Giai đoạn nâng cao; kiểm gói sử dụng/phút CI và license hiện hành trước khi bật | CI job và scenario result |
| Debug target | `idf.py monitor`, GDB + OpenOCD/JTAG | Monitor dùng ngay; JTAG cần adapter/board | Backtrace, register dump, root cause |
| Phân tích crash | ESP-IDF Core Dump | Khi có fault handling | Core dump, decoded stack, regression test |
| Static checks | Compiler `-Wall -Wextra -Werror`, Cppcheck/clang-tidy | Dùng khi bắt đầu viết driver; công cụ mã nguồn mở, nhưng không chứng minh code đúng yêu cầu | Finding, disposition và suppressions có lý do |
| Đo timing/điện | Logic analyzer, oscilloscope, multimeter, bench PSU | Cần board thật | Waveform CSV, min/mean/max, setup đo |
| Mô phỏng CPU/firmware khác | Espressif QEMU cho ESP32; Renode cho Nucleo-H753ZI; Zephyr native_sim/Twister cho bài Zephyr | Tùy chọn sau lab Wokwi; phải kiểm peripheral được mô hình hóa đúng bài | Test log, phiên bản mô hình và danh sách peripheral mô phỏng |
| Traceability/V-model | Markdown requirement IDs + test matrix, Git; về sau issue/CI | Dùng ngay | Requirements ↔ design ↔ code ↔ test ↔ result |

## Bộ tool miễn phí nên dùng theo thứ tự học

| Mốc giáo án | Việc làm cụ thể | Tool tối thiểu | Đầu ra/gate |
|---|---|---|---|
| Tuần 1–5: kiến trúc, memory map, thanh ghi, clock, ngắt | Đọc TRM/header, tự tính base + offset + bit mask; review requirement và test case **trước code** | Datasheet/TRM + ESP-IDF source + Git; `idf.py size` sau khi có build | Sơ đồ kiến trúc, register notebook, quiz; chưa ghi PASS cho phần cứng chỉ vì đã đọc tài liệu |
| Tuần 6: driver GPIO MMIO | Chạy host fake-MMIO test cho `UT-01`…`UT-05`; build firmware; chạy `IT-01` LED trên Wokwi | Compiler C native + test hiện có; ESP-IDF + Wokwi | Log unit PASS/FAIL, build log, ảnh/video LED, commit và bảng `SW-GPIO-* → UT/IT` |
| Tuần 7–24: ngoại vi và RTOS | Thêm test biên/lỗi cho debounce, parser, queue-full, timeout; mô phỏng tích hợp phần được hỗ trợ | Fake MMIO/Unity (Ceedling tùy chọn), Wokwi | Test từng requirement; timing trên simulator chỉ là quan sát mô hình, **không** là worst-case silicon |
| Tuần 29–34: verification chuyên sâu | Chạy static analysis, regression, mô phỏng CPU/debug khi cần và tổng hợp report | Cppcheck/clang-tidy; ESP32 QEMU tùy chọn; CI host test | Finding được phân loại, bug có test tái hiện, log GDB/QEMU ghi rõ giới hạn mô hình |
| Khi học STM32H753 | Chạy ví dụ phù hợp trên Renode **sau khi** đã kiểm Nucleo-H753ZI và peripheral cần dùng được mô hình hóa | Renode mã nguồn mở; host unit test vẫn là lớp chính | Log mô phỏng, danh sách tính năng chưa hỗ trợ; bài điện/timing chuyển sang board thật |
| Khi học S32K344, SPC584B, TC375 | Giữ host fake-register/component test và build bằng toolchain hãng; xác nhận đúng *part + board + peripheral* trước khi tin simulator bên thứ ba | Compiler/test host; SDK/IDE hãng theo quyền truy cập | Mục chưa kiểm được trên chip ghi **PENDING/BLOCKED**, không thay bằng kết quả simulator chip khác |

**Chọn nhanh cho bài hiện tại:** không cần cài thêm simulator. Làm lý thuyết/quiz xong → chạy test `register_gpio` trên host → build ESP-IDF → chạy Wokwi bạn đã kích hoạt. ESP32 QEMU hữu ích khi muốn xem CPU, boot, UART hoặc dùng GDB, nhưng [Espressif chỉ mô phỏng CPU, memory và một số peripheral](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/qemu.html); không dùng QEMU để kết luận mạch LED hay điện áp đúng.

Wokwi [Community miễn phí cho project công khai](https://wokwi.com/pricing); [license VS Code miễn phí cho dự án open-source](https://wokwi.com/license) nhưng vẫn phải kích hoạt/kiểm điều khoản hiện hành. Wokwi browser có thể nạp firmware đã build; VS Code dùng [artifact ESP-IDF sau `idf.py build`](https://docs.wokwi.com/vscode/esp-idf). Không coi Wokwi CLI/CI hoặc tính năng project riêng tư mặc nhiên miễn phí. [Renode có mô hình Nucleo-H753ZI](https://github.com/renode/renode/releases), nhưng mô hình có giới hạn; một vấn đề [PWR/backup SRAM](https://github.com/renode/renode/issues/857) từng được ghi nhận, nên chỉ dùng cho test đã xác minh tương thích.

## Quy trình tự test bài `register_gpio`

1. Đọc requirement và test matrix trong [`VMODEL.md`](../01_MCU_Architecture_and_Boot/register_gpio/docs/VMODEL.md); tự dự đoán test pass/fail.
2. Cài compiler **native** cho Windows nếu muốn chạy host test. Ví dụ [MSYS2 UCRT64 toolchain](https://www.msys2.org/) hoặc [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/); mở terminal đúng môi trường và kiểm tra `gcc --version`/`clang --version` cùng target trước khi compile. Lệnh test có trong [README lab](../01_MCU_Architecture_and_Boot/register_gpio/README.md).
3. Từ `register_gpio/`, compile/chạy `tests/test_mmio_gpio.c` bằng compiler native theo README lab; lưu output với các ID `UT-01`…`UT-05`. Đừng dùng compiler Xtensa rồi cố chạy binary trên Windows.
4. Chạy `idf.py build` từ project, sau đó Wokwi VS Code. Quan sát LED/log; đổi một yêu cầu thì thêm test tương ứng.
5. Khi có board thật, đo trạng thái GPIO18 lúc cấp nguồn/reset và cạnh LED; lưu tool setup, supply voltage, firmware hash, nhiệt độ nếu timing/điện quan trọng.
6. Với bug, tạo test tái hiện trước khi sửa; sau sửa ghi root cause và regression result.

## Nguồn chính thức

- [ESP-IDF build project](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/start-project.html) và [phân tích kích thước](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/performance/size.html).
- [ESP-IDF tests với pytest](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/contribute/esp-idf-tests-with-pytest.html) và [JTAG/GDB](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/jtag-debugging/index.html).
- [Wokwi CLI](https://docs.wokwi.com/wokwi-ci/cli-installation), [automation scenarios](https://docs.wokwi.com/wokwi-ci/automation-scenarios), [GitHub Actions](https://docs.wokwi.com/wokwi-ci/github-actions).
- [Espressif QEMU](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/qemu.html), [Zephyr Twister](https://docs.zephyrproject.org/latest/develop/test/twister.html).
- [Unity](https://github.com/ThrowTheSwitch/Unity), [Ceedling](https://github.com/ThrowTheSwitch/Ceedling), [Cppcheck](https://github.com/cppcheck-opensource/cppcheck) và [Renode](https://github.com/renode/renode).

Không cần mua mọi tool trước. Điểm bắt đầu đủ dùng là ESP-IDF + Wokwi + compiler native cho host test; board/logic analyzer chỉ cần khi đến yêu cầu có bằng chứng điện/timing.

Khi chuyển họ chip, dùng [bảng chip/board/tool riêng](../10_MCU_Families_and_Portability/README.md). STM32CubeIDE/ST-LINK, NXP S32 Design Studio/debugger, SPC5Studio/PLS, AURIX Development Studio/miniWiggler là đường khởi đầu theo từng kit; TRACE32 là tùy chọn chuyên sâu khi có probe/license phù hợp. Tool host/CI/Doxygen hỗ trợ chung nhưng **không** thay thế target test.

[Bản đồ thư viện/SDK chính hãng](../10_MCU_Families_and_Portability/11_THU_VIEN_SDK_MA_MAU.md) chỉ rõ source nào nên đọc cho ESP-IDF, STM32CubeH7, S32K3 RTD, SPC5Studio và AURIX iLLD; mỗi nguồn có bài tập lần API → register và bằng chứng V-model.

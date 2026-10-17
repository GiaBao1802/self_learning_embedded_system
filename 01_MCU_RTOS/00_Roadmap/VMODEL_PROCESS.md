# Quy trình V-model áp dụng cho mọi project MCU

V-model là cách nối từng quyết định thiết kế với loại kiểm thử tương ứng. Thứ tự làm việc: xác định yêu cầu và test có thể chứng minh chúng → thiết kế hệ thống/driver → cài đặt → chạy unit/component/integration/system/acceptance test → xử lý sai lệch và bàn giao. Review diễn ra ở mỗi bước; không chờ đến tuần cuối.

## Các bước và artifact phải có

| Bước | Artifact tối thiểu | Gate trước khi đi tiếp | Verification tương ứng |
|---|---|---|---|
| 1. Nhu cầu hệ thống | `system_requirements.md`: ID, use case, môi trường, giới hạn an toàn/điện/timing | Có acceptance criteria đo được, owner đồng ý | System/acceptance test trên board đích |
| 2. Yêu cầu phần mềm | `software_requirements.md`: input/output, normal/error/recovery, timing/memory | Mỗi yêu cầu liên kết system ID và ít nhất một test ID | Integration test, fault injection |
| 3. Kiến trúc | `architecture.md`: component, data flow, concurrency, interfaces, ownership, ADR | Review trade-off, pin/clock/IRQ/DMA/memory budget | Component/interface test |
| 4. Thiết kế chi tiết | `detailed_design.md`: state machine, register sequence, address/bit/source, error flow | Review TRM, reserved bits, race, reset states và side effects | Unit test/fake MMIO, static analysis |
| 5. Implementation | Source, build config, coding policy, review record | Warning/build gate, code review, version pin | Build và test theo tầng |
| 6. Bàn giao | `traceability.md`, `test_report.md`, known limits/deviation, release note | Không có requirement thiếu bằng chứng hoặc deviation chưa xử lý | Chạy regression trên phiên bản bàn giao |

## Mẫu requirement có thể kiểm tra

```text
ID: SW-GPIO-03
Parent: SYS-GPIO-01
Given: pin GPIO18 đã init output, chip ESP32 gốc
When: ứng dụng yêu cầu high
Then: driver ghi đúng mask bit 18 vào GPIO_OUT_W1TS, không đổi bit pin khác
Error/recovery: pin không hỗ trợ phải trả lỗi và không ghi MMIO
Verification: UT-03 fake MMIO; IT-01 Wokwi LED; HT-01 board thật
Evidence: commit, tool/version, test log, board revision, ngày đo
```

## Mẫu test case

```text
Test ID: UT-03
Requirement: SW-GPIO-03
Setup: fake MMIO reset; pin 18 hợp lệ
Stimulus: mmio_gpio_write(18, true)
Expected: 1 lần ghi tại 0x3FF44008, data = 0x00040000
Actual: [điền kết quả]
Verdict: PASS/FAIL/BLOCKED
Environment: compiler/OS/commit
```

## Traceability và thay đổi

Tạo bảng `Requirement ID | Design section | Source function | Test ID | Result | Deviation`. Khi code hoặc TRM/chip revision thay đổi, xác định các hàng bị ảnh hưởng, cập nhật test và chạy regression. Việc chấp nhận một hạn chế phải có ID, lý do, phạm vi, người review, biện pháp giảm rủi ro và điều kiện đóng. Một test bị chặn vì thiếu board phải ghi **BLOCKED**, không ghi PASS nhờ Wokwi.

## Gates kỹ thuật theo mức độ bằng chứng

1. **Unit/host:** logic, mask, address, state machine, error path; dùng fake/mock, warning-as-error và sanitizer nếu compiler native hỗ trợ.
2. **Component/target:** driver với RTOS/IDF và peripheral model; kiểm tra resource ownership và init/deinit.
3. **Integration:** kết hợp sensor/queue/task/driver; fault, timeout, backpressure; Wokwi/QEMU hỗ trợ phần được mô phỏng.
4. **System/hardware:** trên board đúng revision, đo điện, clock, boot, interrupt latency, CPU/RAM/Flash, power và fault recovery.
5. **Acceptance:** đối chiếu toàn bộ system requirement dưới điều kiện sử dụng thực; lưu artifact và chữ ký/review record của người chịu trách nhiệm dự án.

## Áp dụng ngay

Xem hồ sơ đã điền cho [GPIO MMIO](../01_MCU_Architecture_and_Boot/register_gpio/docs/VMODEL.md). Với bài tập mới, copy cấu trúc ID và bảng trên, rồi thay bằng yêu cầu đo được của chính bài đó. [Lộ trình 40 tuần](SENIOR_MCU_VMODEL_40_WEEKS.md) ghi thời điểm học từng kỹ thuật; V-model này dùng ở **mọi tuần**, không chỉ capstone.

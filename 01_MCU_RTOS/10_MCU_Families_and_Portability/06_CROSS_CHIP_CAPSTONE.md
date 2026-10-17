# Capstone — Một đặc tả, bốn implementation MCU

Làm sau khi đã đi đủ ít nhất **hai** họ chip ngoài ESP32; hoàn thiện bốn bản khi theo hết lộ trình. Không đòi cùng một binary/source driver chạy trên mọi chip. Chỉ cố định yêu cầu cấp hệ thống, API của application và bộ test intent; phần BSP/register/clock/pin/interrupt phải viết và chứng minh riêng.

## Đề bài: node giám sát tín hiệu có fault response

Một button/sensor input giả lập tạo event. Firmware định kỳ đọc/xử lý, điều khiển LED trạng thái, ghi UART, và trên board automotive có thể phát CAN status. Mode gồm `INIT`, `IDLE`, `RUN`, `FAULT`, `RECOVER`. Host test dùng injected time/input; target test dùng pin/timer/bus thật. Thông số như period/deadline/queue depth phải do bạn **điền sau khi đo/đọc chip**, không copy con số giữa board.

| ID | Requirement cần cụ thể hóa | Test tương ứng |
|---|---|---|
| SYS-01 | Sau reset output safe, log reset reason và chỉ vào `RUN` khi init thành công | IT reset/power-cycle, pad measurement |
| SYS-02 | Nhận input event, debounce theo chính sách, không bỏ event âm thầm | UT state transitions; IT button/bounce |
| SYS-03 | Periodic sampling và response deadline trong workload định nghĩa | IT timing min/max/jitter, overload case |
| SYS-04 | UART status gồm mode/error; buffer hữu hạn và xử lý overflow | UT ring buffer; IT serial/flood |
| SYS-05 | Nếu có CAN: status frame ID/payload/rate theo spec, timeout/bus error có recovery | IT hai node/CAN analyzer; fault injection |
| SYS-06 | Watchdog/fault không gây output nguy hiểm, recovery hoặc reset có bằng chứng | IT fault/reset/no-debug run |
| SW-01 | Mỗi peripheral thuộc một owner; ISR ngắn, task/service xử lý | Code review + race/ISR test |
| SW-02 | Driver từ chối input sai, MMIO không ghi sai address/bit | Host fake-MMIO tests + target readback |

## Cấu trúc portfolio cho **mỗi** target

```text
target-name/
  TARGET.md             # part, board, silicon, errata, compiler, probe
  requirements.md       # ID + acceptance criteria + hazards/assumptions
  architecture.md       # block/state/sequence, ownership, memory/timing budget
  register-notebook.md  # địa chỉ/bit/access/reset/source/version
  test-plan.md          # unit/component/integration/system/acceptance
  traceability.md       # requirement -> design -> code -> test -> result
  evidence/             # log, waveform, CAN trace, map/size, screenshots
  source/               # BSP/driver/service/app
```

Đây là **mẫu cấu trúc**, không tạo thư mục rỗng cho từng chip trước khi có board/source. Với NXP và Infineon, tài liệu hoặc SDK có thể có điều khoản truy cập; **không đưa SDK, PDF giới hạn quyền, license key hay binary bên thứ ba vào GitHub**. Chỉ commit ghi chú tự viết, source được phép chia sẻ, link hãng và bằng chứng không chứa bí mật.

## Review so sánh khi hoàn tất

| Trục so sánh | Phải giải thích bằng bằng chứng |
|---|---|
| CPU/ISA và startup | Xtensa vs Cortex-M7 vs e200z4 vs TriCore, vector/exception/context, linker |
| Register/clock/pin | Mapping nào hoàn toàn khác, field nào có side effect, errata nào ảnh hưởng |
| Concurrency | ISR/task/core ownership, RMW/atomic, DMA/cache coherency, watchdog |
| Tool/debug | IDE/compiler/probe, mức quan sát, debugger intrusion, trace availability |
| Verification | Test host tái sử dụng được gì, test target phải viết lại gì, thiếu hardware evidence gì |
| Safety/reliability | Fault detection/recovery và safe-state có chứng minh được không; không tự xưng chứng nhận ASIL |

**Gate cuối:** chọn bất kỳ requirement ID, lần ngược được từ kết quả test → test case → code → design → requirement; và với một bug/fault, chỉ ra reproduction, root cause, correction, regression result trên từng target có liên quan. Nếu chỉ build được ví dụ LED mà chưa có chuỗi bằng chứng này, chặng đó chưa hoàn tất.

# Roadmap Automotive/Embedded Linux C++ — 48 tuần

## Mục tiêu

Mục tiêu không phải chỉ tạo chương trình chạy được. Mỗi sản phẩm phải đi qua chuỗi:

`Requirement → Architecture → Detailed Design → Code → Static Analysis → Test → Traceability → Review`

Sau lộ trình, bạn có thể xây một ứng dụng Linux C++ theo tư duy safety-critical: thiết kế có lý do, code theo profile đã chọn, có bằng chứng kiểm tra và quản lý deviation.

## Thời lượng

- Chuẩn: **48 tuần × 10–12 giờ/tuần**, khoảng 12 tháng.
- Nhẹ: 6 giờ/tuần, khoảng 18 tháng.
- Tăng tốc: 18–20 giờ/tuần, khoảng 7–8 tháng; không bỏ design review và project.

Mỗi tuần dành khoảng 20% cho lý thuyết, 45% cho code, 20% cho test/analysis và 15% cho tài liệu/review.

## Cấu trúc mới

| Folder | Nội dung | Tuần |
|---|---|---:|
| `01_CPP_Language_and_RAII` | C++14/17, ownership, RAII, STL có kiểm soát | 1–6 |
| `02_Linux_System_Programming` | Linux, process, file descriptor, signal, POSIX | 7–10 |
| `03_Requirements_and_Software_Design` | Requirement, UML/C4, SOLID, patterns, review | 11–15 |
| `04_Safety_Critical_CPP_MISRA_AUTOSAR` | MISRA C++:2023, AUTOSAR C++14, compliance | 16–21 |
| `05_Concurrency_and_Realtime` | Thread, synchronization, timing, determinism | 22–25 |
| `06_IPC_and_Networking` | IPC, TCP/UDP, epoll, protocol design | 26–29 |
| `07_Build_Test_and_Quality_Gates` | CMake, test, analysis, CI, cross-compile | 30–34 |
| `08_Debugging_Performance_and_Safety` | Debug, tracing, profiling, defensive design | 35–38 |
| `09_Embedded_Linux_and_AUTOSAR` | Buildroot/QEMU, AUTOSAR concepts, lifecycle | 39–43 |
| `10_Architecture_Projects` | Capstone có evidence package | 44–48 |

Các tiêu chuẩn được áp dụng từ tuần đầu; tuần 16–21 là lúc học có hệ thống và lập hồ sơ compliance.

## Roadmap theo chặng

### Tuần 1–6 — C++ và RAII

- C++14 là compatibility profile; C++17 là MISRA-learning profile.
- Type, scope, reference, pointer, const, object lifetime.
- Class invariants, Rule of Zero/Five, RAII, smart pointer.
- Copy/move semantics, error handling, STL và algorithm.
- Viết interface nhỏ, ownership rõ, không dùng raw owning pointer.

Sản phẩm: thư viện `FixedCapacityQueue` hoặc `Result/Error` nhỏ, kèm unit test, API contract và compiler warnings sạch.

### Tuần 7–10 — Linux system programming

- Shell, permission, process, `fork/exec/wait`.
- File descriptor, signal, `/proc`, system call và POSIX API.
- Resource wrapper bằng RAII cho file descriptor.
- Error path, cleanup, timeout và graceful shutdown.

Sản phẩm: process supervisor nhỏ có state machine và signal handling.

### Tuần 11–15 — Requirements, architecture và design patterns

- Requirement phải atomic, verifiable và có ID.
- Quality attributes: safety, reliability, latency, memory, testability.
- Context/container/component diagram; sequence và state-machine diagram.
- SOLID, dependency inversion, composition over inheritance.
- Patterns bắt buộc thực hành: Strategy, State, Observer, Factory, Adapter.
- Patterns cho embedded/concurrent: Active Object, Reactor, Producer–Consumer.
- Nhận biết anti-pattern: God object, singleton toàn cục, pattern overengineering.

Sản phẩm: thiết kế telemetry service trước khi code, gồm `SRS`, `SAD`, interface contract, ADR và design review checklist.

### Tuần 16–21 — MISRA C++ và AUTOSAR C++

- Dùng **MISRA C++:2023** làm baseline chính cho code C++17.
- Học **AUTOSAR C++14** để hiểu codebase automotive hiện có và mapping lịch sử.
- Phân biệt coding style với coding guideline.
- Guideline categories, decidability, rule scope và false positive.
- Guideline Re-categorization Plan, deviation record, compliance summary.
- Undefined/unspecified/implementation-defined behavior.
- Type safety, conversions, initialization, expressions, control flow.
- Dynamic memory, exceptions, recursion, RTTI và STL theo policy dự án.

Sản phẩm: refactor một module chưa an toàn, chạy analyzer, phân loại finding và tạo deviation record hợp lệ cho trường hợp không thể sửa.

Lưu ý: muốn tuyên bố compliance thực sự cần tài liệu MISRA có license và analyzer đủ năng lực. `clang-tidy`/`cppcheck` hữu ích để học và làm gate bổ sung nhưng không tự tạo ra chứng nhận MISRA.

### Tuần 22–25 — Concurrency và realtime

- Thread, mutex, condition variable, future, atomic.
- Race, deadlock, priority inversion, bounded blocking.
- WCET/latency concepts, monotonic clock, deadline và watchdog.
- Thiết kế ownership giữa thread; không chia sẻ mutable state nếu không cần.

Sản phẩm: bounded queue + worker service; có concurrency design, shutdown protocol, TSan và timing test.

### Tuần 26–29 — IPC và networking

- Pipe, shared memory, Unix domain socket.
- TCP/UDP, framing, byte order, partial I/O, timeout và reconnect.
- Reactor với `epoll`; protocol state machine.
- Defensive parsing, validation và resource limits.

Sản phẩm: multi-client telemetry server có protocol specification, malformed-input tests và graceful degradation.

### Tuần 30–34 — Build, test và quality gates

- Compiler/linker, static/shared library và ABI.
- Target-based CMake, preset, toolchain file và cross-compile.
- Unit/component/integration/system test; GoogleTest/Mock.
- clang-format, clang-tidy, cppcheck, ASan, UBSan, TSan và coverage.
- CI pipeline, reproducible build và artifact retention.

Sản phẩm: pipeline chặn merge khi build, test, warning, sanitizer hoặc policy check thất bại.

### Tuần 35–38 — Debugging, performance và safety mechanisms

- GDB, core dump, `strace`, `lsof`, `perf` và flame graph.
- Fault injection, fail-safe state, watchdog, alive/deadline supervision.
- Logging có cấu trúc; không làm thay đổi timing critical path ngoài kiểm soát.
- Benchmark có baseline, budget và báo cáo regression.

Sản phẩm: failure-analysis report gồm cách tái hiện, root cause, corrective action và regression test.

### Tuần 39–43 — Embedded Linux và AUTOSAR concepts

- Boot flow, BusyBox, systemd, Buildroot và QEMU.
- Cross-deploy, read-only rootfs concepts và application lifecycle.
- AUTOSAR Classic vs Adaptive; service-oriented communication.
- Adaptive concepts: Execution Management, Communication, Persistency, Log/Trace, Health Management.
- SOME/IP, E2E protection và ara API ở mức kiến trúc; không giả vờ triển khai full AUTOSAR stack.

Sản phẩm: image Buildroot/QEMU tự khởi động service C++, health monitor và controlled shutdown.

### Tuần 44–48 — Capstone

Đề xuất: **Automotive Telemetry & Health Monitoring Service**.

Project phải có:

- Requirement IDs và traceability matrix.
- Architecture, state machine, sequence diagram và ADR.
- Ít nhất ba pattern được dùng có lý do; ghi rõ phương án đã loại.
- MISRA/AUTOSAR profile, analysis report và deviation log.
- Unit/component/integration tests, sanitizer, coverage và CI.
- Fault injection, performance budget, benchmark và release checklist.
- Build native + cross-build; chạy trên QEMU hoặc board.

## Gate để qua mỗi chặng

Không đánh dấu hoàn thành nếu chỉ “build được”. Một chặng chỉ hoàn tất khi:

1. Requirement có ID và acceptance criteria.
2. Design được ghi trước phần implementation chính.
3. Code tuân theo `CODING_STANDARD.md`.
4. Build sạch warning với GCC và Clang.
5. Test pass; sanitizer/static analysis không còn issue chưa phân loại.
6. Mọi deviation có lý do, phạm vi, rủi ro và biện pháp giảm thiểu.
7. README cho phép người khác build/test lại từ folder sạch.

Chi tiết xem `DEFINITION_OF_DONE.md` và `STANDARDS_STRATEGY.md`.


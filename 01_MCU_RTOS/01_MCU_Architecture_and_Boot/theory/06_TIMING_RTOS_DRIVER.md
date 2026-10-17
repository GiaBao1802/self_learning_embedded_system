# 06 — Timing, RTOS và thiết kế driver

## 1. Delay, period, deadline

`vTaskDelay(500 ms)` chặn task ít nhất gần khoảng tick yêu cầu; không cam kết cạnh LED đổi đúng 500 ms. **Period** là khoảng giữa hai lần kích hoạt; **execution time** là thời gian xử lý; **deadline** là thời điểm phải hoàn thành; **jitter** là độ dao động. Với periodic task, dùng primitive lập lịch theo mốc (ví dụ `vTaskDelayUntil`) rồi đo, không chỉ delay sau khi xử lý. Với tín hiệu cần độ chính xác cao hơn, dùng hardware timer/PWM phù hợp.

## 2. Task, scheduler và data flow

Task có state ready/running/blocked/suspended. Scheduler chọn task chạy dựa vào priority/affinity/điều kiện. Nếu task mức cao spin mà không block/yield, task khác có thể đói CPU. ESP32 hai core tăng khả năng song song nhưng cũng tăng race. Giao tiếp qua queue/notification giúp ghi rõ owner dữ liệu; shared mutable state cần policy đồng bộ.

## 3. Contract của một driver cấp thấp

Mỗi API phải mô tả:

- **Precondition:** chip/board, pin hợp lệ, ai đang sở hữu peripheral, clock/reset state.
- **Postcondition:** register nào đổi; mức đầu ra sau init/deinit.
- **Error:** pin không hỗ trợ, mode/pull sai, null pointer; không ghi register khi tham số sai.
- **Concurrency:** nhiều task/core gọi có an toàn không; RMW register cần bảo vệ thế nào.
- **Timing:** thao tác có thể block không; khoảng thời gian nào chỉ đo được trên board thật.
- **Test seam:** tách hàm read32/write32 để host test quan sát giao dịch MMIO.

Driver lab `register_gpio/` cố ý giới hạn GPIO18/19/21/22/23. Giá trị đầu ra được set **trước** khi bật output enable để giảm glitch lúc init. Thư viện sử dụng W1TS/W1TC cho thao tác trên bit của nhiều pin. Register IO MUX vẫn là read-modify-write: caller phải giữ quyền sở hữu pin và serialize reconfiguration. Không gọi đồng thời ESP-IDF GPIO driver trên cùng pin.

## 4. Tư duy senior và V-model

Senior MCU engineer không chỉ biết viết register driver: phải giải thích vì sao chọn register/bit, xác định requirement có thể test, review race/timing/fault, định lượng memory/CPU, giữ traceability và biết giới hạn của simulator. Với mỗi tính năng, đi theo cặp V-model:

| Bên thiết kế | Bằng chứng xác nhận tương ứng |
|---|---|
| Stakeholder/system requirement | System acceptance test trên board thật |
| Software requirement | Integration test trên target/mô phỏng và fault cases |
| Architecture/interface | Component/interface test |
| Detailed driver design | Unit test host mock MMIO + code review |
| Implementation | Build, static analysis, test log |

Xem hồ sơ thực tế tại [`register_gpio/docs/VMODEL.md`](../register_gpio/docs/VMODEL.md) và lộ trình dài hạn ở [`SENIOR_MCU_VMODEL_40_WEEKS.md`](../../00_Roadmap/SENIOR_MCU_VMODEL_40_WEEKS.md).

**Nguồn:** [ESP-IDF FreeRTOS](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html), [ESP-IDF timer](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/esp_timer.html), [ESP-IDF testing](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/contribute/esp-idf-tests-with-pytest.html).

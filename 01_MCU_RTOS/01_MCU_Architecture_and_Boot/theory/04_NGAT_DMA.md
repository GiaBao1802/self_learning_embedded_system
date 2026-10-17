# 04 — Interrupt, ISR và DMA

## 1. Đường đi của interrupt

Peripheral tạo event → status/enable bit của peripheral → interrupt matrix/controller → interrupt input của một CPU core → CPU lưu context cần thiết → ISR → xác nhận/xóa nguyên nhân → khôi phục context. ESP32 có interrupt matrix để nối nguồn interrupt với input CPU; không suy rằng “mỗi peripheral có một IRQ cố định”. Nguồn interrupt, mức ưu tiên, core affinity và sharing ảnh hưởng thiết kế.

Đọc **ESP32 TRM Chapter 8** và [ESP-IDF Interrupt Allocation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/intr_alloc.html). Luồng event nút GPIO đi qua GPIO status → interrupt matrix → ISR → task notification → task xử lý debounce.

## 2. ISR contract

ISR nên nhanh, có thời gian chặn có giới hạn, xóa đúng cờ interrupt và chuyển việc dài sang task. Chỉ gọi API có biến thể dùng trong ISR khi framework yêu cầu; không chờ mutex/queue blocking trong ISR. Nếu callback phải chạy khi Flash cache tắt, code và dữ liệu liên quan phải đặt trong vùng bộ nhớ phù hợp theo ESP-IDF.

Lỗi thường gặp: xóa cờ trước khi đọc nguyên nhân; quên xóa cờ tạo interrupt storm; log quá nhiều làm tăng latency; truy cập buffer đang do DMA sở hữu; dùng một biến `volatile` như thể nó là queue an toàn giữa ISR/task.

## 3. Latency, jitter và priority

**Interrupt latency** là thời gian từ event đến khi ISR bắt đầu; **response time** có thể gồm ISR + task xử lý; **jitter** là biến thiên thời điểm thực thi. Đo bằng GPIO toggle/logic analyzer trên board thật khi cần chứng cứ vật lý. Wokwi chứng minh logic và trình tự sự kiện, không thay thế số đo worst-case latency trên silicon. Khi chia interrupt, phải cân nhắc nguồn nào được phép chạy cùng priority và cách xác định nguồn gây ngắt.

## 4. DMA và ownership

DMA chuyển dữ liệu giữa peripheral và RAM mà CPU không copy từng byte. CPU vẫn cần cấu hình descriptor/buffer, khởi chạy, kiểm tra completion/error và trao quyền sở hữu buffer. Với cache, phải hiểu coherency: dữ liệu CPU thấy có thể chưa phản ánh RAM mà DMA ghi hoặc ngược lại. Alignment, vùng nhớ có khả năng DMA và lifetime của buffer là một phần của driver contract.

Ví dụ UART RX DMA: buffer `FREE → DMA_OWNED → READY → APP_OWNED → FREE`. Không sửa buffer ở trạng thái `DMA_OWNED`. Khi queue đầy, phải có chính sách drop/overwrite/backpressure đã ghi trong requirement.

## 5. Phân biệt cơ chế đồng bộ

- Critical section: bảo vệ đoạn rất ngắn, tránh race với context tương ứng; thời gian khóa có giới hạn.
- Mutex: bảo vệ tài nguyên giữa task, có thể xử lý priority inheritance tùy RTOS.
- Semaphore/notification: báo event; không tự bảo vệ dữ liệu phức tạp.
- Queue: truyền message và quy định ownership/lifetime.

Đọc tiếp [`05_Interrupts_DMA_and_Concurrency`](../../05_Interrupts_DMA_and_Concurrency/README.md) sau module này để thực hành ISR và DMA.

## Điểm cần nắm

ISR là interrupt context, không phải task; cờ không được xử lý đúng có thể gây interrupt storm. Buffer DMA phải có owner và trạng thái bàn giao rõ. Một cờ `volatile bool ready` không đủ mô tả payload, lifetime hoặc đồng bộ frame UART.

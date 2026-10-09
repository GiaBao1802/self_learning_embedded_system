# Design Review Checklist

- [ ] Component có một trách nhiệm chính và boundary rõ.
- [ ] Dependency hướng vào abstraction phù hợp; không có vòng phụ thuộc.
- [ ] Ownership và lifetime của từng resource rõ ràng.
- [ ] Interface nhỏ, cohesive, không làm lộ chi tiết triển khai.
- [ ] State transition hợp lệ và invalid transition được xử lý.
- [ ] Error/timeout/retry/fallback có giới hạn và trạng thái an toàn.
- [ ] Memory, CPU, latency, queue và thread budget được định lượng.
- [ ] Shared state được giảm thiểu; locking order được ghi lại.
- [ ] Input ngoài trust boundary được validate.
- [ ] Design có test seam và quan sát được khi lỗi.
- [ ] Pattern dùng có vấn đề cụ thể cần giải quyết.
- [ ] Có phương án đơn giản hơn đã được cân nhắc.
- [ ] Assumption và implementation-defined behavior đã ghi lại.
- [ ] Requirement quan trọng truy vết được tới component/test.


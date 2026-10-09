# Lộ trình học Embedded Software Architect

Mục tiêu của workspace này là giúp bạn phát triển từ kỹ sư embedded software thành Software Architect (SA), bắt đầu với `01_MCU_RTOS`, sau đó đến `02_Linux_CPP`, `03_AUTOSAR` và `04_Edge_AI`.

## Thứ tự học đề xuất

1. Bắt đầu với `01_MCU_RTOS` để học kiến trúc MCU, peripheral driver, interrupt và RTOS (bỏ qua phần ôn tập C).
2. Học `02_Linux_CPP` để mở rộng sang Modern C++, Linux, concurrency, testing và debugging.
3. Học `00_SA_Core` song song từ sớm; áp dụng từng chủ đề vào lab và project MCU.
4. Đi sâu vào `03_AUTOSAR` nếu mục tiêu chính là automotive/ECU architecture.
5. Học `04_Edge_AI` như một nhánh chuyên sâu cho intelligent edge systems.
6. Sau mỗi cụm kiến thức, tạo một sản phẩm trong `00_SA_Core/09_Portfolio_and_Case_Studies`: architecture diagram, ADR, interface specification, trade-off analysis hoặc prototype.

## Cách sử dụng mỗi thư mục

- `01_...`, `02_...`: thứ tự ưu tiên học.
- `notes`: ghi chú bằng ngôn ngữ của bạn.
- `references`: datasheet, standard, link và sách cần đọc.
- `labs`: bài tập nhỏ để kiểm chứng kiến thức.
- `projects`: bài tích hợp có yêu cầu, thiết kế, code, test và kết luận.
- `architecture`: diagram, ADR, interface, quality attributes và trade-off.

## Đầu ra cần có của một Embedded SA

- Biến business/system requirements thành software requirements.
- Chia component và định nghĩa interface rõ ràng.
- Phân tích trade-off về latency, memory, CPU, power, safety, security và cost.
- Mô tả kiến trúc bằng C4, UML/SysML, sequence/state/deployment diagram.
- Tạo ADR và truy vết requirement → design → implementation → test.
- Có khả năng review code, debug system, đo performance và dẫn dắt technical decision.

## Mốc tự đánh giá

- Foundation: giải thích được concept và làm lab riêng lẻ.
- Practitioner: tích hợp được một subsystem và đo được quality attributes.
- Architect: đề xuất nhiều phương án, bảo vệ trade-off, quản lý interface/risk và tạo bộ tài liệu kiến trúc hoàn chỉnh.

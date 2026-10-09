# 09 — Project tích hợp

Project là nơi biến kiến thức theo chủ đề thành bằng chứng năng lực kiến trúc. Không chỉ nộp source: cần requirement, interface, sơ đồ, kiểm thử và ghi rõ giới hạn mô phỏng.

## Project 1 — GPIO bring-up (tuần 1–2)

Code và mạch nằm tại [`01_MCU_Architecture_and_Boot/Week_01/esp32_gpio`](../01_MCU_Architecture_and_Boot/Week_01/esp32_gpio/). Tuần 1 hoàn thành theo [`Week_01/README.md`](../01_MCU_Architecture_and_Boot/Week_01/README.md): hiểu boot-to-app, cấu hình GPIO, build ESP-IDF và chạy Wokwi.

## Capstone — Sensor monitor có RTOS

Ứng dụng dự kiến gồm `sensor_task`, `processing_task`, `logger_task` và `command_task`; có queue, state `FAULT`, watchdog, timeout và budget timing/RAM.

## Hồ sơ bàn giao bắt buộc

- Requirements có ID và acceptance criteria.
- Context/component diagram, sequence/state diagram và interface specification.
- Ít nhất 3 ADR có phương án thay thế và trade-off.
- Source, build instructions, unit/integration tests và log/demo.
- Timing/RAM budget, fault matrix, known limitations và retrospective.

# 02 — Bare-metal và cross-compilation

Học cách build firmware cho kiến trúc đích, đọc toolchain/linker output, hiểu startup và phân biệt phần do ứng dụng, runtime và framework đảm nhiệm. Bạn đã master C nên phần này tập trung vào execution model, ABI, linker script và artifact, không ôn cú pháp C.

## Lộ trình và đầu ra

- Tuần 1: dùng project ESP-IDF ở [`01_MCU_Architecture_and_Boot/Week_01`](../01_MCU_Architecture_and_Boot/Week_01/README.md) để hiểu build/ELF/firmware.
- Tuần 8: quay lại linker, section, map file, stack/heap và vùng nhớ.
- Lab: build ESP32, kiểm tra ELF bằng `idf.py size`/`idf.py size-components`, liên hệ section trong map file với memory map của chip.
- Đầu ra: một sơ đồ build pipeline và ghi chú giải thích ELF, bootloader, partition table, app image.

## Tài liệu đọc

- [ESP-IDF: Build the Project](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/start-project.html)
- [ESP-IDF: Application Startup Flow](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/startup.html)
- [ESP-IDF: Linker Script Generation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/linker-script-generation.html)
- [GNU Binutils: `objdump`](https://sourceware.org/binutils/docs/binutils/objdump.html) và [`readelf`](https://sourceware.org/binutils/docs/binutils/readelf.html)

## Câu hỏi tự kiểm tra

Bạn có thể chỉ ra ELF nào là đầu vào debugger, file nào nạp vào flash, và linker quyết định địa chỉ của section bằng cách nào không?

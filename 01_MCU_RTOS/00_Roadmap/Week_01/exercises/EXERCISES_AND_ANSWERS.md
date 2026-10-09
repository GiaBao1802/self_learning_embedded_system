# Bai tap va quiz — Tuan 1 MCU/GPIO

Hay lam phan bai tap truoc khi doc dap an. Voi moi thay doi code, ghi expected result, sau do mo phong va so sanh actual result.

## Bai tap thuc hanh

### Bai 1 — Thay doi GPIO

Doi `LED_GPIO` tu GPIO18 sang GPIO19 va cap nhat `diagram.json`. Build lai, chay simulator. Ghi ro thay doi nao can dong bo giua source va wiring.

**Dap an:** doi macro `LED_GPIO` trong `main.c`, doi endpoint `esp:18` thanh `esp:19` trong connection cua LED, build lai va restart/reload simulator. Neu chi sua source hoac chi sua wiring, LED se khong hoat dong nhu mong doi.

### Bai 2 — Thay doi blink pattern

Sua code de LED sang 200 ms, tat 800 ms. Khong thay driver configuration.

**Dap an tham khao:** thay vong lap thanh hai lan `gpio_set_level(LED_GPIO, 1/0)` va delay rieng 200/800 ms. Ghi chu: day la relative delay; log va overhead lam chu ky thuc khong chinh xac tuyet doi.

### Bai 3 — Hai GPIO output

Them LED xanh tren GPIO19. Tao cau hinh rieng cho GPIO19 hoac gom ca hai pin vao `pin_bit_mask`; tao pattern luan phien: LED do sang khi LED xanh tat, roi doi trang thai.

**Dap an:** moi pin phai duoc ket noi rieng va cau hinh la output. Co the dung mot `gpio_config_t` co mask `(1ULL << GPIO_NUM_18) | (1ULL << GPIO_NUM_19)` neu mode/pull/interrupt giong nhau. Ghi muc logic tung LED de tranh hai LED cung sang.

### Bai 4 — Pull resistor va input

Them button active-low tren GPIO4: mot dau button noi GPIO4, dau kia GND. Cau hinh GPIO4 input + pull-up; in serial khi button duoc nhan.

**Dap an:** khi nha button, pull-up dua input len logic 1; khi nhan, button noi pin xuong GND nen input doc 0. Active-low nghia la muc 0 la asserted/pressed. Khong bat pull-down dong thoi voi pull-up.

### Bai 5 — Loi init

Bo `ESP_ERROR_CHECK` va bo qua gia tri tra ve cua `gpio_config()`. Vi sao day la design xau? Them error path co log va dung firmware neu init that bai.

**Dap an:** neu init that bai, loop tiep tuc dieu khien pin chua duoc config, trong khi log co the lam nguoi debug tuong init thanh cong. Check `esp_err_t`; log GPIO/error code va dung hoac chuyen sang state fault ro rang. `ESP_ERROR_CHECK` la fail-fast shorthand.

### Bai 6 — Nhin qua GPIO driver

Lan theo declarations cua `gpio_config_t`, `gpio_config()` va `gpio_set_level()` trong ESP-IDF API. Ghi lai: field nao cau hinh pin, field nao chon mode, API nao dat output level, error type la gi.

**Dap an:** `pin_bit_mask` chon nhom GPIO; `mode` chon input/output; `pull_up_en`/`pull_down_en` bat pull; `intr_type` chon edge/level interrupt; `gpio_config()` apply configuration; `gpio_set_level()` set output 0/1; API tra `esp_err_t`.

## Cau hoi ly thuyet va dap an

### 1. MCU khac board development kit the nao?

**Dap an:** MCU la chip/SoC; development board la bo mach gom MCU, nguon, clock/reset circuitry, USB/debug interface va cac dau pin/linh kien phu.

### 2. Peripheral la gi? Cho ba vi du.

**Dap an:** Khoi phan cung ben trong MCU thuc hien chuc nang ngoai CPU, vi du GPIO, timer, UART, SPI, I2C, ADC, DMA.

### 3. Memory map la gi?

**Dap an:** Bang phan bo cac dia chi CPU co the truy cap cho Flash, SRAM va cac peripheral/register region. Firmware/linker va chip manual phai thong nhat voi memory map.

### 4. Pin pad co phai luon la GPIO khong?

**Dap an:** Khong. Mot pad co the duoc mux sang GPIO hoac alternate function nhu UART/SPI/PWM/ADC. Phai chon dung pin mux va kiem tra board routing.

### 5. GPIO mode output nghia la gi?

**Dap an:** Pad output driver duoc dieu khien boi output latch/peripheral; code co the dat muc logic. No khong co nghia pin cap du dong cho bat ky tai nao.

### 6. Tai sao LED can dien tro?

**Dap an:** LED can current limit; dien tro han dong va bao ve LED/GPIO. Tinh gia tri theo voltage supply, LED forward voltage, va dong cho phep; datasheet board/chip la tham chieu.

### 7. `pin_bit_mask` bieu dien gi?

**Dap an:** Moi bit ung voi mot GPIO number; bit 1 trong mask chon pin duoc config. Vi field la 64-bit, sample dung `1ULL << GPIO_NUM_18`.

### 8. `gpio_config()` khac `gpio_set_level()` the nao?

**Dap an:** `gpio_config()` setup mode/pull/interrupt cho pin; `gpio_set_level()` dat output level sau khi pin da duoc cau hinh output.

### 9. `ESP_ERROR_CHECK()` xu ly dieu gi?

**Dap an:** Kiem tra gia tri `esp_err_t`; neu loi, ghi log va abort thay vi am tham chay tiep voi hardware state khong ro rang.

### 10. Wokwi co xac nhan dong dien/analog/EMI cua board that khong?

**Dap an:** Khong. Simulator tot cho logic firmware, wiring mo phong va quan sat output. Electrical limits, analog accuracy, EMI, silicon errata va timing vat ly can datasheet/measurement va hardware validation.

### 11. Vi sao `vTaskDelay(500 ms)` khong dam bao LED toggle moi chinh xac 500 ms?

**Dap an:** Delay yield task theo tick; thoi gian code truoc delay, scheduler va tick resolution lam chu ky quan sat khac 500 ms. Dung hardware timer hoac periodic scheduling API va measurement de co timing yeu cau ro rang.

### 12. `app_main()` co phai la hardware reset vector khong?

**Dap an:** Khong. Boot ROM/bootloader/startup framework xu ly reset va runtime initialization; ESP-IDF goi `app_main()` sau do. `app_main()` chay nhu mot task trong ESP-IDF FreeRTOS integration.

### 13. Tai sao sample dung GPIO driver thay vi ghi dia chi register truc tiep?

**Dap an:** Driver dong goi register fields, pin configuration va chip-specific details; app code de doc hon va it phu thuoc register encoding. Register-level learning van can de hieu hardware, nhung ghi tay can dung reference manual, barriers/access semantics va errata.

### 14. Diem giua software simulation va hardware test?

**Dap an:** Simulation co the xac nhan logic, state transition, driver interaction duoc model va regression nhanh. Hardware test xac nhan silicon, electrical conditions, board wiring, timing vat ly, power va peripheral behavior thuc.

## Quiz tu cham

Khong xem dap an: ve boot flow; ve mach GPIO; mo ta 4 field cua `gpio_config_t`; giai thich active-high/active-low; noi ro gioi han Wokwi. Dat 8/10 equivalent (khong bo trong muc nao) truoc khi sang tuan 2.

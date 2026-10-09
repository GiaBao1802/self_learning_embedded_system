# Tuan 1 — MCU execution flow va GPIO output

Tuan nay vao thang MCU/peripheral, khong on lai C. Ket qua cuoi tuan la ban build duoc ESP-IDF firmware, mo phong ESP32 DevKitC V4 trong Wokwi, dieu khien LED ngoai qua GPIO, va giai thich duoc duong di tu `app_main()` den chan GPIO.

## Board va project ao

- Board: **ESP32 DevKitC V4 / ESP32**.
- Simulator: Wokwi.
- Source/build: ESP-IDF project nam trong `esp32_gpio/`; Wokwi nap artifact da build tu `build/flasher_args.json`.
- LED: GPIO18 -> dien tro 1 kOhm -> anode LED; cathode LED -> GND.

Wokwi browser editor cho phep mo project ESP32 moi: https://wokwi.com/projects/new/esp32. De chay dung source ESP-IDF ben duoi, cai ESP-IDF va Wokwi for VS Code, build local roi chon **Wokwi: Start Simulator**. Wokwi docs yeu cau build truoc va firmware path ESP-IDF tro den `build/flasher_args.json`; board trong diagram phai khop target chip.

## Ly thuyet

### 1. MCU la mot he thong tren chip

MCU gom CPU core, Flash, SRAM, clock/reset logic, interrupt controller va peripheral. CPU chay instruction; peripheral thuc hien I/O/timing/giao tiep; memory map gan dia chi cho cac vung RAM, Flash va peripheral registers. Moi dong chip co memory map, clock tree va pin mux rieng: tra datasheet/reference manual thay vi doan theo ten pin.

### 2. Pin khong tu dong la GPIO

Mot pad co the co nhieu chuc nang: GPIO, UART, SPI, I2C, PWM, ADC... GPIO driver/pin mux chon chuc nang. GPIO output co data latch/direction/enable logic va pad driver. GPIO input doc muc logic cua pad. Board pin label, SoC GPIO number, va function signal la ba khai niem co the khac nhau.

### 3. Logic 0/1 va electrical limits

GPIO la muc logic, khong phai nguon cap tai tuy y. GPIO co gioi han voltage/dong; LED can dien tro han dong. Wokwi mo phong logic va mot so hanh vi linh kien, nhung khong thay the datasheet hay do dong that. Trong project nay LED duoc noi qua dien tro 1 kOhm de mach co current limit.

### 4. Driver/HAL va memory-mapped register

Trong code project, `gpio_config()` cau hinh mode/pull/interrupt va `gpio_set_level()` doi muc logic. Day la API driver; driver phia duoi thao tac hardware registers va pin mux. Hoc theo hai lop: hieu register-level idea, roi dung API driver cua chip de code app. Khong viet dia chi register ESP32 bang phan doan.

### 5. Reset den `app_main()`

Sau reset, chip chay boot ROM/bootloader, nap app image tu Flash, startup code khoi tao runtime va goi entry point cua framework. ESP-IDF sau do goi `app_main()`. Trong ESP-IDF, `app_main()` duoc chay nhu mot FreeRTOS task; tuan nay ta chua thiet ke task/scheduler, chi dung task hien tai de thuc hien GPIO blink.

### 6. Config object va error handling

`gpio_config_t` mo ta nhom pin, mode, pull va interrupt. `ESP_ERROR_CHECK()` dung de fail-fast neu API tra error: khi init hardware that bai, tiep tuc chay voi trang thai khong ro rang se nguy hiem hon dung lai co log.

Trong sample, `vTaskDelay()` yield task hien tai trong mot so tick. Khoang thoi gian giua hai lan doi LED con bao gom thoi gian chay code/log; day la bai blink de hoc GPIO, chua phai periodic timing chinh xac. Tuan 3 se hoc hardware timer va jitter.

## Ke hoach hoc 6 buoi (90 phut/buoi)

### Buoi 1 — MCU architecture va datasheet (90 phut)

- 25 phut: hoc muc 1 va ve CPU/Flash/RAM/peripheral/clock/reset.
- 25 phut: mo Wokwi ESP32 board, tim GPIO18, GND, 3V3; phan biet SoC pin voi board pin label.
- 25 phut: doc README project va lan theo `app_main()` -> `gpio_config()` -> `gpio_set_level()`.
- 15 phut: tu ve boot flow tu reset den app.

**Bai nop:** mot so do khoi MCU va 5 dong mo ta boot flow.

### Buoi 2 — GPIO, pin mux va mach LED (90 phut)

- 25 phut: hoc muc 2-4; ve duong GPIO output latch -> pad -> resistor -> LED -> GND.
- 20 phut: doc `diagram.json`; xac nhan GPIO18, anode/cathode va resistor.
- 25 phut: du doan LED sang/tat theo muc GPIO va chu ky.
- 20 phut: tra datasheet thong so current/voltage cua GPIO va LED (khong lay Wokwi lam thong so dien).

**Bai nop:** pin map va ghi chu electrical limit can kiem tra khi chuyen sang board that.

### Buoi 3 — ESP-IDF build va Wokwi (90 phut)

- 20 phut: cai ESP-IDF theo official setup neu chua co.
- 20 phut: mo project folder `esp32_gpio` trong VS Code co ESP-IDF extension.
- 25 phut: build `idf.py build`; doc summary va artifact duoc tao.
- 25 phut: mo Wokwi simulator va xem serial log/LED.

Lenh chinh:

```powershell
idf.py set-target esp32
idf.py build
```

Trong VS Code: **F1 -> Wokwi: Start Simulator**. Project da co `wokwi.toml` va `diagram.json`.

**Bai nop:** log build thanh cong va mo ta artifact ELF/flash images.

### Buoi 4 — Sua firmware co chu dich (90 phut)

- Doi `LED_GPIO` tu 18 sang mot GPIO khac duoc board va simulator ho tro; cap nhat mach.
- Doi chu ky 500 ms thanh 100 ms, sau do thanh 1 s; ghi nhan thay doi quan sat.
- Doi output pattern thanh 3 blink nhanh + 1 blink dai.
- Neu `gpio_config()` loi, xac dinh `ESP_ERROR_CHECK` lam gi va log nao can giu lai.

**Bai nop:** 3 thay doi nho, moi thay doi co mot ghi chu expected vs observed.

### Buoi 5 — Hien tuong va gioi han mo phong (90 phut)

- So sanh LED output voi UART log: co the serial log lam thay doi thoi diem quan sat.
- Tim pin co chuc nang alternate; doc pin mux va peripheral ownership trong tai lieu chip.
- Liet ke nhung thu simulator khong xac nhan: dien ap/dong that, EMI, silicon errata, timing vat ly, hardware wiring.
- Them mot assertion/validation cho GPIO pin configuration truoc khi goi driver.

**Bai nop:** bang 2 cot: simulator co the xac nhan / phai xac nhan tren board that.

### Buoi 6 — Review va quiz (90 phut)

- 30 phut: lam bai tap trong `exercises/EXERCISES_AND_ANSWERS.md` ma chua xem dap an.
- 20 phut: giai thich lai GPIO config struct tung field.
- 20 phut: demo firmware va trinh bay boot-to-GPIO flow trong 5 phut.
- 20 phut: lam quiz, danh dau chu de can hoc lai.

## Code project

Mo file `esp32_gpio/main/main.c`. Firmware cau hinh GPIO18 output, doi muc logic moi 500 ms, va ghi log. `app_main()` la entry cua ung dung ESP-IDF; driver GPIO cau hinh hardware va bao loi qua `esp_err_t`. Tuan nay muc tieu la MCU/peripheral flow, khong phai hoc lai syntax C.

## Tieu chi hoan thanh

- Build project ESP-IDF thanh cong va mo phong LED toggle trong Wokwi.
- Chi ra duoc MCU, board pin, SoC GPIO, LED, resistor, GND tren diagram.
- Giai thich duoc mode/pull/intr_type/pin_bit_mask va `gpio_set_level()`.
- Ve duoc boot flow va noi ro `app_main()` la FreeRTOS task trong ESP-IDF.
- Tra loi dung it nhat 8/10 cau quiz; doc lai phan sai truoc khi sang tuan 2.

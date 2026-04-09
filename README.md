# TOBB ETÜ ELE362 - AVR Tabanlı Oda Ortam İzleme Sistemi

ATmega328P mikrodenetleyicisi kullanılarak geliştirilmiş çok sensörlü ortam izleme sistemi. Sıcaklık, nem, basınç, ışık ve hava kalitesi verilerini ölçer; LCD'de gösterir ve ThingSpeak'e iletir.


## İçindekiler

- [Proje Hakkında](#proje-hakkında)
- [Özellikler](#özellikler)
- [Donanım](#donanım)
- [Depo Yapısı](#depo-yapısı)
- [Kurulum](#kurulum)
- [Kullanım](#kullanım)
- [Ekran Görüntüleri](#ekran-görüntüleri)
- [Kaynaklar](#kaynaklar)

---

## Proje Hakkında

Bu proje, TOBB ETÜ ELE362/BIL362 Mikrodenetleyiciler dersi kapsamında geliştirilmiştir. Atmel Studio 7 kullanılarak register düzeyinde C ile yazılmıştır. Arduino kütüphanesi kullanılmamıştır.

---

## Özellikler

- BME280 ile sıcaklık, nem ve basınç ölçümü
- LDR ile ışık seviyesi ölçümü (ADC)
- MQ135 ile hava kalitesi ölçümü (ADC)
- DS1307 RTC ile gerçek zamanlı saat ve tarih
- 16x2 I2C LCD ekran üzerinde sensör verisi gösterimi
- Kırmızı/Sarı/Yeşil LED ile ortam durum göstergesi
- Eşik aşımında piezo buzzer ile sesli uyarı
- ESP8266 ile WiFi bağlantısı ve ThingSpeak'e veri gönderimi
- Tüm sürücüler (I2C, UART, ADC) sıfırdan register düzeyinde yazılmıştır

## Donanım

### Bileşenler

| Bileşen | Açıklama | Bağlantı |
|---|---|---|
| ATmega328P (Arduino Nano) | Ana mikrodenetleyici | - |
| BME280 | Sıcaklık / Nem / Basınç sensörü | I2C - A4/A5 (0x76) |
| DS1307 RTC Modülü | Gerçek zamanlı saat | I2C - A4/A5 (0x68) |
| 16x2 LCD (I2C) | Ekran | I2C - A4/A5 (0x27) |
| LDR | Işık sensörü | ADC - A0 |
| MQ135 | Hava kalitesi sensörü | ADC - A1 |
| LED (K/S/Y) | Durum göstergesi | GPIO - D5/D6/D7 |
| Piezo Buzzer | Sesli uyarı | GPIO - PB1 |
| ESP8266 | WiFi modülü | UART - D0/D1 |
| AMS1117-3.3 | ESP8266 güç regülatörü | 5V → 3.3V/800mA |

### Gerekli Ekstra Bileşenler

- CR2032 pil (DS1307 yedek güç)
- 4.7kΩ pull-up dirençler (I2C hattı)
- 1kΩ + 2kΩ direnç (ESP8266 RX gerilim bölücüsü)
- 100µF elektrolitik kondansatör (ESP8266 güç hattı)

---

## Depo Yapısı

```
room-monitor/
├── main.c          # Ana döngü, ESP8266 yönetimi
├── config.h        # WiFi, ThingSpeak ayarları, eşik değerleri
├── i2c.c / i2c.h   # TWI register tabanlı I2C sürücüsü
├── bme280.c / .h   # BME280 sürücüsü ve kalibrasyon
├── ds1307.c / .h   # DS1307 RTC sürücüsü
├── lcd.c / lcd.h   # I2C LCD sürücüsü (PCF8574)
├── adc.c / adc.h   # ADC sürücüsü
├── uart.c / uart.h # UART sürücüsü
└── gpio.c / gpio.h # LED ve buzzer kontrolü
```
---

## Kurulum

### Gereksinimler

- Atmel Studio 7
- Arduino Nano (ATmega328P)
- Arduino as ISP programlayıcı (ikinci Arduino Nano)

### Adımlar

1. Repoyu klonla:
```bash
   git clone https://github.com/inaneda/ELE362-room-monitor
```
2. `config.h` dosyasını aç, WiFi SSID/şifre ve ThingSpeak API anahtarını gir
3. Atmel Studio'da projeyi aç
4. Arduino as ISP bağlantısını kur (SCK/MISO/MOSI/SS)
5. Derle ve yükle

---

## Kullanım

Sistem açıldığında:

1. BME280 başlatma kontrolü yapılır
2. ESP8266 WiFi ağına bağlanır, LCD'de bağlantı durumu gösterilir
3. Tarih ve saat 3 saniye ekranda gösterilir
4. Ana döngü başlar: sensörler okunur, LCD güncellenir, LED/buzzer durumu ayarlanır
5. Her 15 saniyede bir ThingSpeak'e veri gönderilir

**Eşik Değerleri** (`config.h`):

| Parametre | Min | Max |
|---|---|---|
| Sıcaklık | 15°C | 30°C |
| Nem | %30 | %70 |
| MQ135 (ADC) | - | 400 |

---

## Ekran Görüntüleri

---

## Kaynaklar

| Kaynak | Bağlantı |
|---|---|
| BME280 Veri Sayfası | https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf |
| DS1307 Veri Sayfası | https://datasheets.maximintegrated.com/en/ds/DS1307.pdf |
| ESP8266 Veri Sayfası | https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf |
| ATmega328P Veri Sayfası | https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf |
| ThingSpeak Dokümantasyonu | https://www.mathworks.com/help/thingspeak/ |
| Seeed Studio ESP8266 Wiki | https://wiki.seeedstudio.com/WiFi_Serial_Transceiver_Module/ |

---

**Eda İnan - TOBB ETÜ Elektrik-Elektronik Mühendisliği - Bahar, 2026**


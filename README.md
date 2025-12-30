
# PPN ESP32 Stacja Pogodowa

Projekt stacji pogodowej na mikrokontrolerze ESP32 z obsługą WiFi i serwera WWW.

## Setup

1. **Zainstaluj ESP-IDF**
    - Pobierz [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
    - Ustaw zmienne środowiskowe

2. **Sklonuj repozytorium**
    ```bash
    git clone <repo-url>
    cd final-project-Zoober97/PPN_ESP32_StacjaPogodowa_Marchewka_181257
    ```

3. **Konfiguracja**
    ```bash
    idf.py menuconfig
    ```

## Build

```bash
idf.py build
```

## Uruchomienie

1. **Podłącz urządzenie** przez USB do komputera

2. **Wgraj firmware**
    ```bash
    idf.py -p PORT flash
    ```
    (zamień PORT na port szeregowy, np. COM3 lub /dev/ttyUSB0)

3. **Monitor**
    ```bash
    idf.py -p PORT monitor
    ```

4. **Połącz się z WiFi** i otwórz serwer WWW na adresie IP urządzenia


# ESP32 Weather Station - System Design

## 1. Cel Systemu

Stacja pogodowa oparta na mikrokontrolerze ESP32 umożliwia:
- Pomiar temperatury
- Zdalny dostęp do danych poprzez interfejs webowy
- Sterowanie stacją via CLI (UART)
- Autonomiczną pracę z automatycznym odświeżaniem pomiarów

## 2. Architektura (Diagram Blokowy)

```
┌─────────────────────────────────────────┐
│          ESP32 Microcontroller          │
├─────────────────────────────────────────┤
│                                         │
│  ┌──────────────┐   ┌──────────────┐    │
│  │     FSM      │   │     CLI      │    │
│  │  (fsm.c)     │   │   (cli.c)    │    │
│  └──────┬───────┘   └──────┬───────┘    │
│         │                  │            │
│  ┌──────▼──────────────────▼─────┐      │
│  │     WiFi AP Manager           │      │
│  │     (wifi_ap.c)               │      │
│  └──────┬──────────────────────┬─┘      │
│         │                      │        │
│  ┌──────▼──┐          ┌────────▼───┐    │
│  │ Sensors │          │ Web Server │    │ 
│  │(DS18B20,│          │(web_server)│    │
│  │ AHT,BME)│          │   :80      │    │
│  └─────────┘          └────────────┘    │
│         │                               │
└─────────┼───────────────────────────────┘
          │
    ┌─────▼─────┐
    │   UART    │
    │   (CLI)   │
    └───────────┘
```

## 3. Maszyna Stanów (FSM)

```
┌─────────┐
│  INIT   │─────────────────────┐
└────┬────┘                     │
     │ WiFi gotowy              │ Błąd
     ▼                          │
┌──────────────┐                │
│ IDLE/READY   │◄──────┐        │
└────┬─────────┘       │        │
     │ Pomiar          │ Done   │
     ▼                 │        │
┌──────────────┐       │        │
│  MEASURING   │───────┘        │
└────┬─────────┘                │
     │                          │
     └──────────────────────────►
          ERROR state
```

## 4. Algorytm Sterowania

1. **Inicjalizacja**: Konfiguracja GPIO, I2C, WiFi AP
2. **Pętla główna**: 
   - Odczyt sensorów co X sekund
   - Aktualizacja bufora danych
   - Obsługa zdarzeń CLI
   - Serwowanie żądań HTTP
3. **Przejścia stanów**: Zarządzane przez FSM

## 5. Interfejsy i Protokoły

### 5.1 CLI (UART)
- **Port**: UART0 (GPIO 1, 3)
- **Baudrate**: 115200 bps
- **Komendy**:
  - `GET TEMP` - Pobranie temperatury
  - `GET AVG` - Pobranie średniej temperatury
  - `SET PERIOD` - Ustawienie interwału odświeżania danych
  - `STAT` - Stan systemu
  - `RESET` - Reset stanu systemu

### 5.2 HTTP (Web UI)
- **Port**: 80
- **Endpointy**:
  - `GET /` - strona główna
  - `GET /temp` - obecna temperatura
  - `GET /avg_temp` - średnia temperatura


# Hum Temp Press Sensor (Modbus)

Firmware for humidity, pressure & temperature sensor.

### Compilader
* **MPLAB XC8:** v2.41

### Device Packs
* **Device Family Pack (DFP):** v1.15.191

### IDE Recomendado
* **MPLAB X IDE:** v5.x to v6.20.

## 📦 Install & compile

1.  Clone this repository.
2.  Open with MPLAB IDE.
3.  Check on (`Project Properties`) **Compiler Toolchain** == **XC8 v2.41**.
4.  Check on "Packs", **1.15.191**.
5.  Clean & Buld.

## ⚙️(UART/Modbus)

* **BaudRate:** 9600
* **Data Bits:** 8
* **Stop Bits:** 1
* **Parity:** None (Ninguna)
* **DTR (Data Terminal Ready):** False
* **RTS (Request To Send):** False

##  🆔 Modbus Register Map

### Holding Registers 

| Address | Name          | Type   | Unit       | Description                                      | Range        | Default |
|--------|--------------|--------|------------|--------------------------------------------------|-------------|--------|
| 0      | Slave Address | uint16 | -          | Modbus device address                           | 1 – 255     | 3      |
| 1      | Baudrate      | uint16 | bps        | Serial communication speed                      | 1200–115200 | 9600   |

---

## Input Registers 

| Address | Name          | Type   | Unit     | Description                 | Default |
|--------|--------------|--------|----------|-----------------------------|--------|
| 0      | Sensor Type   | uint16 | -        | Sensor type identifier      | 200      |
| 1      | Serial Number | uint16 | -        | Device serial number        | 0   |
| 2      | Termistor Value  | uint16 | value   | Measured termistor value by ADC   | 0   |
| 3      | SHTC3 Value  | uint16 | value   | Measured humidity value      | 0   |
| 4      | BMP Pressure High Value  | uint16 | value   | Measured High Pressure value      | 0   |
| 5      | BMP Pressure Low Value  | uint16 | value   | Measured High Pressure value      | 0   |
| 6      | BMP Temperature High Value  | uint16 | value   | Measured High Temperature value      | 0   |
| 7      | BMP Temperature Low Value  | uint16 | value   | Measured High Temperature value      | 0   |
| 8 - 20     | BMP Coeficients Value's | uint16 | values   | Measured BMP coeficients value's      | 0   |
---

## Coils 

| Address | Name         | Type | Description                    |
|--------|-------------|------|--------------------------------|
| 0      | Trigger Read | bool | Start a sensor measurement     |

---

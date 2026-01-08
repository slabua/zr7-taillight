# ZR-7 ATtiny85 Tail Light

![License](https://img.shields.io/badge/License-MIT-green)
![MCU](https://img.shields.io/badge/MCU-ATtiny85-blue)
![Platform](https://img.shields.io/badge/Platform-AVR-lightgrey)
![Status](https://img.shields.io/badge/Status-Active-brightgreen)
![Sponsor](https://img.shields.io/badge/Sponsored%20by-PCBWay-darkgreen)

###### tags: `SLBLabs` `SLB Labs` `ZR-7` `ZR7` `ZR750` `Kawasaki` `KawasakiZR7` `TailLight` `TailLights` `ATtiny` `ATtiny85` `Arduino` `C/C++`

## A custom motorcycle tail light based on ATtiny85
> Hardware • Firmware • Animations • Automotive-grade considerations

![Front](https://github.com/user-attachments/assets/b4b53afb-a872-4cca-82cc-ddf7f4c7c053)

---

## ✦ Overview

The **ZR-7 Tail Light** is a custom-designed motorcycle tail light built around the **ATtiny85** microcontroller.  
It spans the complete stack, from schematic and PCB design to low-level firmware and animation logic, with a focus on robustness, simplicity, and visual clarity.

The goal of this project is not only to function as a tail light, but to serve as a well-engineered embedded system suitable for real automotive environments.

---

## ✦ Features

- ATtiny85-based control logic
- Addressable RGB LED layout
- Distinct operating modes:
  - Tail / running light
  - Brake light
  - Left / right indicators
- Deterministic, non-blocking firmware
- Designed for 12 V motorcycle electrical systems
- Compact, purpose-built PCB

---

## ✦ Hardware

### Core Components

- **MCU:** ATtiny85
- **LEDs:** Addressable RGB LEDs
- **Power:** 12 V input with on-board regulation
- **Inputs:** Brake, left indicator, right indicator

### Design Notes

Several constraints influenced the hardware design:

- **Automotive signal noise**
  Input conditioning and pull-down strategies are used to avoid floating or unstable states.

- **Power integrity**
  The circuit is designed to tolerate voltage fluctuations typical of motorcycle electrical systems.

- **Mechanical constraints**
  PCB shape, connector placement, and LED layout are optimized for the tail light enclosure.

Schematics, PCB layouts, and manufacturing files are included in the `pcb/` directory.

---

## ✦ Firmware

Firmware is written in **C**, targeting the ATtiny85 directly, with no operating system or dynamic memory usage.

### Architecture Highlights

- **State-based rendering**
  Tail, brake, and indicator logic are resolved into a single rendering pipeline.

- **Time-driven animations**
  All animations are non-blocking and tick-based.

- **Config-ready architecture**
  The firmware structure allows future addition of persistent configuration if needed.

Firmware source is located in the `firmware/` directory.

---

## ✦ Build & Flashing

The firmware is developed and flashed using the **Arduino IDE** with an external ISP programmer.

---

### Requirements

- Arduino IDE
- ATtiny board core (e.g. **ATTinyCore** by Spence Konde)
- Adafruit_NeoPixel library
- ISP programmer:
  - USBtinyISP
  - USBasp

---

### Arduino IDE Setup

1. Install the ATtiny board package via the Boards Manager.
2. Install the Adafruit_NeoPixel library via the Library Manager.
2. Connect the ISP programmer to the ATtiny85 ISP header.
3. In the Arduino IDE, configure:
   - **Board:** ATtiny85
   - **Clock:** 16.5 MHz
   - **B.O.D Level:** 4.3v
   - **Programmer:** USBtinyISP or USBasp

---

### Flashing

Use:

``Sketch → Upload Using Programmer``

This compiles the firmware and flashes it directly to the ATtiny85.  
No bootloader is used.

---

## ✦ Media

> Visual material will be added here.

Planned additions:
- PCB and assembly photos
- Installed tail light images
- Short animation demo videos

---

## ✦ Repository Layout

```
zr7-taillight/
├── firmware/        # ATtiny85 firmware source
├── hardware/        # Schematics and PCB design
├── docs/            # Images and additional documentation
└── README.md
```

---

## ✦ Safety & Disclaimer

⚠️ **Important Notice**

This project is provided **as-is**, for educational and experimental purposes.

- The author makes **no guarantees** regarding road legality, safety compliance, or fitness for any particular vehicle.
- Modifying vehicle lighting may be regulated or restricted depending on jurisdiction.
- You are solely responsible for ensuring that any implementation complies with **local laws**, **vehicle regulations**, and **safety requirements**.

Use at your own risk.

---

## ✦ Sponsorship

This project is sponsored by **PCBWay**.

PCBWay provided support for PCB manufacturing during the prototyping and validation stages of this project.

It was my first time trying their services and I am pleased with the results.  
The order flow was a little different than what I was used to, but I have been diligently followed thorough the whole process.  
Other than PCB manufacturing, PCBWay also offers services such as CNC and 3D printing. Be sure to check them out at https://www.pcbway.com/

---

## ✦ License

Released under the **MIT License**.
See the `LICENSE` file for details.

---

## ✦ Project Status

The project is **functional and actively evolving**.
Both hardware and firmware are stable, with ongoing refinements and documentation improvements planned.

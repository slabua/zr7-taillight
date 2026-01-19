# ZR-7 ATtiny85 Tail Light

![Status](https://img.shields.io/badge/Status-Active-brightgreen)
![MCU](https://img.shields.io/badge/MCU-ATtiny85-blue)
![Platform](https://img.shields.io/badge/Platform-AVR-lightgrey)
![Sponsor](https://img.shields.io/badge/Sponsored%20by-PCBWay-darkgreen)
![License](https://img.shields.io/badge/License-MIT-green)

###### tags: `SLBLabs` `SLB Labs` `ZR-7` `ZR7` `ZR750` `Kawasaki` `KawasakiZR7` `TailLight` `TailLights` `ATtiny` `ATtiny85` `Arduino` `C/C++`

## A custom motorcycle tail light based on ATtiny85
> Hardware • Firmware • Animations • Automotive-grade considerations

![Front](https://lh3.googleusercontent.com/pw/AP1GczPcQGxCfUZAwsfYwrjawJziC8Ts_PzSu_Ykivu9friekENxc6mTAWhSCQKqfeNy27R3Dzpx0y0I0km117YR619BW6nFuORxewza40VULuJ0Tds7sBByco_MM5tdd6fSmaa7ZrmyeIM8mrPyxQaEvrWt3w=w800)

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

![Front-PCB3D](https://lh3.googleusercontent.com/pw/AP1GczN5KGg9Ho-JnIh3h1fnWg4-O0gHjBBb7uz8jC2eObNq4i56pB39m6ivb-ZZthR80GrJmakMjOK81XY7n-SXsZHMQmZW7eI_xxhKxkVw7WxX4tRmijJMDPrFLsQhBhjRwKpwPz5tRacYwXufTO80ZyU5_A=w800)

![Front-PCB](https://lh3.googleusercontent.com/pw/AP1GczMJTML8wlEuSRibU8EQbbRFBFLXe1Q2A6dYsYtwLaMZVqKy9t3xnTSOWS1deyr4vOzW1fSDwdhgVVqTpdU7R0sroFQAo0qNH-DWFzJbBUfI26OPDezkY_rc57Kn01DNvZfhME1yRvNAP9eX0JbYQlCw-w=w800)

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

Schematics, PCB layouts, and manufacturing files are included in the `hardware/` directory.

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

### Requirements

- Arduino IDE
- ATtiny board core (e.g. **ATTinyCore** by Spence Konde)
- Adafruit_NeoPixel library
- ISP programmer:
  - USBtinyISP
  - USBasp

### Arduino IDE Setup

1. Install the ATtiny board package via the Boards Manager.
2. Install the Adafruit_NeoPixel library via the Library Manager.
2. Connect the ISP programmer to the ATtiny85 ISP header.
3. In the Arduino IDE, configure:
   - **Board:** ATtiny85
   - **Clock:** 16.5 MHz
   - **B.O.D Level:** 4.3v
   - **Programmer:** USBtinyISP or USBasp

### Flashing

Use:

``Sketch → Upload Using Programmer``

This compiles the firmware and flashes it directly to the ATtiny85.  
No bootloader is used.

---

## ✦ Media

### PCB details
![](https://lh3.googleusercontent.com/pw/AP1GczPFNp3BiLHL9ANtcv_BErcwlV7I2kLZODztr1_LbhClzMRUxLuRXSd34Jgd-NtKujqgwgUcPDabAtg2bCTwV_3BlHtAgWgcPjlDMSq-4ych3D222EBIR6UqJ1nx2GgZ_SrT--N_Nd689MknwCYJ-bzP3Q=w800)

<details>
  <summary>[More]</summary>
  
  ![](https://lh3.googleusercontent.com/pw/AP1GczN5qVahsUoubwwKXgMT9H9qP-lxrKxA6Uz4MyAxFfWnwbB6uKf4fLl120Uw7yXENnp3Dctk6xQ149svxyS85tu_NEmm6j2BIO8MWcyOx0GkI0uzB_hrGXcUo5iskEThkZ99jeaDvljBX0nC_pMcvMZxrw=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczPvxRdZKBnOmEgsDN5ky-lyao_-msAf6u6RclNbvQg9x8AnMCfo1x2VxNIbYjWeFLPuZ7105XahBKOCZHtAfEKvdcYL6MlnApfmit3KFN4rWSvsrpL0IH6yKrxYCymdSctc6DtARZ3IK2OoCz8BHEpVYQ=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczMy3Hz2OSvrXkQ9lvN-TBUcajhrmlbae0IxblqDJeSU4TuXSmUXE3TUPpIXe4G37Ik_0SjB4b15h0YBt-4RB1GLwSlxWcHav3symX6_J6Ifoer78a_kHa8JH9h1KdK2B-NKq3xojhZO-Tb08yENiIMIOA=w800)
  
</details>

---

### PCB assembly
![](https://lh3.googleusercontent.com/pw/AP1GczMCgYOm2Y5SLg9HUnLRQoBCbmA2efFoKbqAtwnOPQZr6Kyj9yddCsei-p9wVmXCl1HHDYVXAjOKxx3FSDjgm6PiKM7tKDPZ2UbLgj5SqNexSVfijuhASzdvorjjTq9QUHoxx-jCM02VcALmO9q0o7gucg=w800)

<details>
  <summary>[More]</summary>
  
  ![](https://lh3.googleusercontent.com/pw/AP1GczO07dx4XhIIKnDCl8cNuho4B5vGLqp_G-40_JN3-40kjYsAsqIElL-hcyQ0otFPo597J8IlFaTIzirtVbeF0LYC2YBXQmddekyKDrNt1hnQ_hgbQN8X_lrqWkktRN1_anY2PZHUaoiEZVHHMjP1y5NCjg=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczPSSLAMXZsuSL_EH_9LSeTFOb42-j8KSHyz3KFpyatDCM9CilzB28s7zqrolDcj0CZcwBIKWkPrYFwMZpy6AsMeEiN2O4yZcqHATMquSz77iWAU-4JYXizwOwYZw8dz4eIr70fyvqRoHBP1CCd7CulPgw=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczNZv1NvUvTarLuRiCgrPUBU-0EZsz3bD7TB45ztpoSmwOWCz_43uuzK31d4ErqJUYVlErKZZllXamzBxI6egtS93WavpUtofxnkMu886Qtz34r2INvvv5jgN5EK0FxeGBBCJKyqaGI7YVXFoZfPkAsMXQ=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczNZ2JtgMGtT5p7cis8g-_FFwKE01C7NRtReflz92n299qja9ku9Mxd0oBlaGxRaFWYk3J8EWHP7K4jet1rwGvW543BKfqyJLXMx01J6yNgpLt12rBP0K6fS_JBf3XbqoMJ3z7VzcgPFTsAWxdOIIwmfUA=w800)
  
</details>

---

### Diffuser
![](https://lh3.googleusercontent.com/pw/AP1GczMwQMm-WD50Sve8gNhy9e6oSqkowAisFYjIyjmDcKhZWFWbAS1uErCnoe925YWuyHPUmjgC_dBRWgtINm40z1Sl_q24rQXYiLMypBOXibmshpchovIGzR1X-qbd0L5DKTAxNF9ZjeFhEiNT5_9c9DIhAw=w800)
  
<details>
  <summary>[More]</summary>

  ![](https://lh3.googleusercontent.com/pw/AP1GczOEwnY6p-bO1ZNT7XdJTO0avoZpeaeT0XBrAThpfyMKgIoVpiMAdrl6P5t8TZMqruWbcOD7rerbcr5cH7bDJ82kchki6_FJ4BGGvtzOC4QAmhyybcGHY1mWFi3xYYOgrdj1gbyo2hoDT4rOiPCW3LnizQ=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczPOHxZvDetpvY_gtCAiK0V8dATaXEfqqbanUAJQfk4v_0Ab3awrhQ6X_PXREAnkJtaKdNiX_U_YKvtCWMj_GN5E60ryHwlaS3q-rRqBB92vw6ez3YqMO84t_bbZwsNba89-n3XeRUwVUqYnRiNXc6Fz1w=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczPsnyKZ1bvFCTzrbHtdoDA00lYB1wi5nZs1CPvN8huBKLir0IpzlZ0zIITNP8Pl3gQ8NYsB4_JgVCXcyQy4Yd7ZO6fDhY_EDhE350kZnHz-Glxuye2ZOR7kHYGGJwMNbPdoPEGTBq9dcQljnYn9Tfy6Pw=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczN65gjKsop9bZl7fM-PTdDDuNb-p6kshQikXU5CC6t6ZVC79hLXIbeLh2yWoxyhopcnV0ycRGBrTcpQn_pGj8pnzjX1u6mYRRKLUZhG6LbWufw2f-CZtN22O6_stAKXbESkH4ODfjjVSiTwy2F04BXBRQ=w800)
  
</details>

---

### Relay
![](https://lh3.googleusercontent.com/pw/AP1GczPeLyVRGk_yuKah9XnrqGgZa_J2txg9U7z5hG6HLvXJ6va8FxJ81QCcXP9AmNd6E5qp7fUdb9P566-0_wJW_cKQdhGulJPrh1lyj4ielq--x-H7g6TOjGBrhdmC4Cy7i1xPexZKhd6P9MNb1FwsJLtqPA=w800)
  
<details>
  <summary>[More]</summary>

  ![](https://lh3.googleusercontent.com/pw/AP1GczP-5wlhSufNMeguDrVrzgTMAC51bXMJBx-mgRjUAle-tZ9lpAKRNg2Vmh7_Tye8paUWRXpnVABmQikY18893BWnFT-d7fmCi8j97hwO9FbB0qOP8tRCwLpT1T8G7tidd2YTIP5ZTpzHDCWBRyjrOdnnZw=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczP9BlrL8dz0SnBdaN9itqWeFJFBaz6AQ1ZRZl1snnDEdJyI_5_ZYtqyjJU9H-30GHiCk6mggM7TBVHnK14bfqG0hm05-d9mfzUM7CTtPY7DlKfsOCUTKCmeZblBPbHHi-VzfpqD1DedTwbIqwkevYWO7A=w800)
  ![](https://lh3.googleusercontent.com/pw/AP1GczOXQYyKUd9VvQMMuxmh2Rz_9rUcCMPjUMlxbuznMefY1q6xxvbxbhIbALQ9i1lioAtwPcu6u0FxBL7AZTJhhJOVEzi2X6ciobPFbPpWv_hg_QHOkwUm2nuSVxbJI6Xa30US1y1jLegRC6elU6QeJ3S8ew=w800)
  
</details>

---

- [TODO] Installed tail light images
- [TODO] Short animation demo videos

※ Attached photographs of the assembled PCB are representative of Rev 1.0.

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

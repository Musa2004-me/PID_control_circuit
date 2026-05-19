# 🎛️ Analog PID Control Circuit — MCT232s Project

> **Electronics for Instrumentation | Ain Shams University — Faculty of Engineering**
> Team 27 | Section MCT-1

---

## 👥 Team Members

| Name | ID |
|---|---|
| Musa Gamal El-Shenawy | 2100614 |
| Fares Mohamed Mostafa | 2100409 |
| Youssef Kamel Ahmed | 2101081 |

---

## 📌 Overview

This project implements a **full analog PID controller** using op-amps to control a **DC motor's position angle**. The analog PID output is interfaced with an Arduino UNO and MATLAB Simulink for real-time testing and feedback via a DAC (MCP4725).

---

## 🎯 Objective

Design an analog PID circuit using op-amps that:
- Accepts a **voltage setpoint** (potentiometer input)
- Computes the **P, I, and D** terms in analog hardware
- Drives a motor to the desired angular position with **minimal error and fast response**

---

## 📐 System Block Diagram

> *Insert image: block diagram from page 7 (Set point → PRO/INT/DIF → Adder → AMP → Motor → Feedback encoder)*

```
Setpoint ──► [Subtractor] ──► [P] ─┐
                   ▲          [I] ──► [Adder] ──► [Motor] ──► [Encoder]
                   │          [D] ─┘                               │
                   └───────────────────────── Feedback ────────────┘
```

---

## 🔧 Circuit Architecture

The full Proteus schematic consists of the following stages in order:

> *Insert image: full circuit screenshot from page 8 (showing Buffer → Subtractor → LPF → P/I/D → Adder pipeline)*

### Signal Chain

```
Potentiometer (Setpoint)
    → Buffer
    → Inverting Amp (×-1)
    → LPF 1 (H(s) = 1/(s+1))
    → Subtractor (error = setpoint - feedback)
    → LPF 2 (H(s) = 1/(s/10+1))
    → P / I / D (parallel paths)
    → Adder
    → Inverting Amp (gain = -5/12, maps to Arduino range)
    → Arduino UNO (analog input A0)
    → DAC MCP4725 (feedback output)
```

---

## 🧩 Circuit Blocks

### 1. Setpoint — Potentiometer
> *Insert image: potentiometer circuit from page 9*

A 50kΩ pot with a 10kΩ series resistor limits the output range:
- **V(max) = 5 × (50/60) = 4.167 V**
- **V(min) = 0 V**

---

### 2. Buffer
> *Insert image: buffer op-amp circuit from page 10*

Unity-gain voltage follower (A = 1) used to isolate the setpoint source from the downstream load.

---

### 3. Inverting Amplifier (×−1)
> *Insert image: inverting amp circuit from page 10*

Restores signal polarity with unity gain (Av = −1), using matched 10kΩ resistors.

---

### 4. Low-Pass Filter 1 (feedback path)
> *Insert image: LPF 1 circuit and Bode plot from page 16*

Eliminates oscillations from the feedback signal.

$$H(s) = \frac{1}{s + 1}, \quad \omega_c = 1 \text{ rad/s}$$

- R = 10 kΩ, C = 100 µF → RC = 1

---

### 5. Subtractor (Error Comparator)
> *Insert image: subtractor circuit from page 11 / page 17*

Computes the error signal: `e(t) = V_setpoint − V_feedback`

$$A_v = \frac{R_{12}}{R_{13}} = 1$$

---

### 6. Low-Pass Filter 2 (post-subtractor)
> *Insert image: LPF 2 circuit and Bode plot from page 17*

Removes high-frequency noise introduced by the feedback path.

$$H(s) = \frac{1}{\frac{s}{10} + 1}, \quad \omega_c = 10 \text{ rad/s} \ (f \approx 1.59 \text{ Hz})$$

- R = 1 kΩ, C = 100 µF → RC = 0.1

---

### 7. PID Block
> *Insert image: P/I/D combined circuit from page 18*

All three terms use tunable potentiometers so gains can be varied in real time.

#### Proportional (P)
$$A_v = -\frac{R_{V4}}{R_1}, \quad K_p \in [0, 100]$$
- Pot: 100 kΩ, R_const: 1 kΩ

#### Integral (I)
$$A_v = \frac{-1}{C \cdot s \cdot (R_7 + R_{V5})}, \quad K_i \in [55, 666]$$
- C = 0.3 µF (3 × 0.1 µF), R₇ = 5 kΩ, Pot = 50 kΩ

#### Derivative (D)
$$A_v = -C \cdot s \cdot R_{V6}, \quad K_d \in [0, 88]$$
- C = 440 µF (2 × 220 µF), Pot = 200 kΩ

> *Insert image: individual P amplifier circuit from page 12*

> *Insert image: integrator circuit from page 12*

> *Insert image: differentiator circuit from page 13*

---

### 8. Adder
> *Insert image: adder circuit from page 13*

Sums the three PID terms:

$$V_{out} = -(V_P + V_I + V_D)$$

---

### 9. Arduino Interface & DAC
> *Insert image: Arduino + DAC block from page 14*

- **Arduino UNO** reads the scaled PID output via analog pin A0 and sends it over I²C
- **MCP4725 DAC** converts the digital value back to an analog feedback voltage

---

## 📊 Simulations & Test Results

### Individual Block Tests

| Block | Input | Expected Output | Actual Output |
|---|---|---|---|
| P (Kp = 50) | 0.1 V DC | −5 V | −5 V ✅ |
| I (Ki = 100) | 0.1 V DC | −∞ (saturates) | −11 V (rail) ✅ |
| D (Kd = 10) | 0.1 V DC | 0 V (DC → no derivative) | ~0.01 mV ✅ |

> *Insert image: P test oscilloscope screenshot from page 20*

> *Insert image: I test oscilloscope screenshot from page 20*

> *Insert image: D test oscilloscope screenshot from page 21*

---

### PID Combined — Simulink
> *Insert image: Simulink block diagram and response plot from page 22*

With Vin = 0.1 V DC, the output saturates at **+12 V** (op-amp rail) as expected — the integrator drives to maximum with a constant non-zero error.

---

### PID Combined — Proteus
> *Insert image: Proteus simulation schematic and oscilloscope from page 23*

Proteus result: **Vout = 11.00 V** (op-amp rail), consistent with Simulink.

---

### Full Closed-Loop System — Simulink
> *Insert image: full Simulink block diagram from page 25*

$$\text{Plant: } G(s) = \frac{1}{s^2 + 10s + 20}$$

With setpoint = 0.1 V, the system settles in approximately **4 seconds** with no overshoot.

> *Insert image: closed-loop step response plot from page 25*

---

### Hardware-in-the-Loop Test
> *Insert image: hardware photo with multimeter from page 26*

With setpoint = **2.51 V** and the full loop running (Arduino + DAC + Simulink plant):

> *Insert image: TF response plot varying around 2.5 V from page 27*

The output oscillates around **2.5 V** — confirms the controller is actively regulating.

---

## 🖨️ PCB Design

> *Insert image: PCB layout (top copper + schematic view) from page 24*

> *Insert image: physical PCB photo from page 24*

- IC used: **LM348N** (quad op-amp in single package — chosen for compact PCB footprint)
- Potentiometers for P, I, D are panel-mounted for in-circuit tuning

---

## ⚙️ Tools Used

| Tool | Purpose |
|---|---|
| Proteus | Circuit simulation & Bode plots |
| MATLAB Simulink | System-level simulation & HIL testing |
| KiCad / EasyEDA | PCB layout |
| Arduino UNO | ADC interface |
| MCP4725 | DAC feedback |
| LM348N / µA741 | Op-amp implementation |

---

## ✅ Conclusion

- All individual blocks (P, I, D, LPF, Subtractor, Adder) were verified in Proteus and match theoretical calculations
- The full closed-loop system was simulated successfully in Simulink
- Hardware-in-the-loop test shows the output tracking the 2.51 V setpoint at ~2.5 V
- Remaining discrepancies are attributed to **Simulink–Arduino–DAC communication latency**, not the analog circuit itself

---

## 📁 Repository Structure

```
📦 PID-Control-Circuit
 ┣ 📂 proteus/          # Proteus simulation files (.pdsprj)
 ┣ 📂 simulink/         # MATLAB Simulink models (.slx)
 ┣ 📂 pcb/              # PCB layout files
 ┣ 📂 arduino/          # Arduino sketch for ADC/I2C interface
 ┣ 📂 report/           # Project final report (PDF)
 ┗ 📜 README.md
```

---

> *MCT232s — Electronics for Instrumentation | Ain Shams University 2024*

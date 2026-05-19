# Analog PID Control Circuit

**Electronics for Instrumentation — MCT232s | Ain Shams University**
Team 27 — Musa Gamal, Fares Mohamed, Youssef Kamel

---

## What is this project?

This project is a fully analog PID controller built using op-amps. It controls the position angle of a DC motor by continuously comparing the desired setpoint (set by a potentiometer) against the actual motor position (read by an encoder), and adjusting the motor drive signal to minimize the error.

The analog circuit is connected to an Arduino UNO which interfaces with MATLAB Simulink for real-time monitoring and a DAC (MCP4725) to close the feedback loop.

---

## How it works

A potentiometer sets the desired angle as a voltage. A subtractor op-amp computes the error between the setpoint and the feedback. That error is then processed by three parallel op-amp stages — Proportional, Integral, and Derivative — whose outputs are summed and sent to the motor driver.

![System Block Diagram](images/block_diagram.png)

Each of the three gains (Kp, Ki, Kd) is adjustable in real time using potentiometers on the PCB.

---

## The Circuit

The full circuit was designed and simulated in Proteus. It includes input buffering, two low-pass filters to remove noise, a subtractor, the PID block, an adder, and output scaling amplifiers to match the Arduino's 5V ADC range.

![Full Circuit Schematic](images/full_circuit.png)

---

## PID Gains

| Term | Gain Range | Components |
|---|---|---|
| Proportional (Kp) | 0 → 100 | 100kΩ pot, 1kΩ resistor |
| Integral (Ki) | 55 → 666 | 50kΩ pot, 5kΩ resistor, 0.3µF cap |
| Derivative (Kd) | 0 → 88 | 200kΩ pot, 440µF cap |

---

## Simulation Results

The full closed-loop system was simulated in Simulink with the motor modeled as:

$$G(s) = \frac{1}{s^2 + 10s + 20}$$

With a step input the system settles in around 4 seconds with no overshoot.

![Simulink Response](images/simulink_response.png)

Setting the setpoint to 2.51V in the hardware-in-the-loop test, the output tracked it closely at ~2.5V.

![TF Response](images/tf_response.png)

---

## PCB

The board was designed using the LM348N quad op-amp IC to keep it as compact as possible. The three potentiometers for Kp, Ki, Kd are accessible on the top of the board.

![PCB Layout](images/pcb_layout.png)

![Physical Board](images/pcb_photo.png)

---

## Tools

- **Proteus** — circuit simulation
- **MATLAB Simulink** — system simulation and hardware-in-the-loop testing
- **Arduino UNO** — analog interface
- **MCP4725** — DAC for feedback signal
- **LM348N / µA741** — op-amps

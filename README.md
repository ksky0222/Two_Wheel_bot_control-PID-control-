# Two_Wheel_bot_control-with_pid_control

## What is PID control?

PID control is an abbreviation for Proportional-Integral-Derivative control, which is one of the most commoly used control algorithms in contorl systems.
PID control compares the feedback signal from sensors with a desired setpoint to generate a control input signal and reduce the system's error.
This algorithm is  widely applide in various industries, including robotics, automotive control, and household appliances, to effectivey regulate and control system behavior.

PID contorl consists of three main components:

1. Proportional Control
Generates a control input signal proportional to the magnitude of the current error.
The control input varies in proportion to the error, where larger errors result in larger control inputs.
Proportional control helps reduce the error, but it cannot eliminate it completely.
The control input is adjusted using a proportional constant(Kp) that reflects the proportionality between the error and the control input.

2. Integral Control
Calculates the accumulated value of the error over time and adds it to the control input.
Integral control imprved system accuracy and stability by considering the temporal accumulation of the error.
The Control input is adjusted using an integral constant(Ki) that represents the weighting of the accumulated error.

3. Derivative Control
Adjusts the control input based on the rate change of the error.
Derivative control tracks the rate of change of the error, providing fast response and stability.
The control input is adjusted using a derivative constant(Kd) that represents the weighting of the error's rate of change.


![image](https://github.com/ksky0222/Two_Wheel_bot_control-PID-control-/assets/109937431/2e23ce90-bcca-49e9-9796-ed5beeaad40c)


https://github.com/ksky0222/Two_Wheel_bot_control-PID-control-/assets/109937431/6b15ef73-04bc-4cee-a3d3-a4c1ba0a27d9


-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

I tried controlling a two-wheel robot by using the PID control mentioned above, and attempted basic motion control.

- Straight driving

https://github.com/ksky0222/Two_Wheel_bot_control-PID-control-/assets/109937431/8267d70e-1d53-4ab5-afc8-be2e1756cb40


- Pivot turning driving

https://github.com/ksky0222/Two_Wheel_bot_control-PID-control-/assets/109937431/d06f76fe-a885-4e41-982c-20bc31c467da



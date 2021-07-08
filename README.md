
# Introduction

Self-balancing robot is a two-wheeled robot which balances itself so that it prevents itself
from falling. This concept is somewhat similar to the operation of a unicycle, the rider of the
unicycle balances by moving himself in the same direction of the inclination so that he stays
vertical, similarly the self-balancing robot balances by moving in the same direction of the
inclination.
A more complete model of this problem is represented by the theory of the Inverted
Pendulum.The inverted pendulum is a common problem to solve in control theory. The
pendulum is usually mounted on a cart and is balanced by controlling the movement of the
cart. This setup can be varied in many ways to make the system more complex and it can be
controlled in many ways. For example the company Segway introduced their solution of a
transport vehicle in 2001, the Segway Personal Transporter, which is based on the same
dynamics as the inverted pendulum. It has two wheels with a platform between them where
the passenger stands. The passenger then acts as the pendulum to be balanced and the
passenger runs it by leaning forward or backward. Since Segway introduced it’s solution the
area has gained more momentum both through public interest and in the transportation field.

# Purpose and Goals

The main purpose was to design and construct a robot which will balance on two wheels and
be able to work on its own without any supervisor. The goals achieved for the robot
are as follows:
* Be able to balance on two wheels
* Be resistant to small bumps or pushes
* Be able to balance on a 5% surface slope
* Should also be controlled by a wired joystick that make him move forward and
backward

# Component List
* STM32 Nucleo-F401RE
* GY-521 MPU6050  
* DC 12V Motor
* L298N H-bridge
* Joystick

# Solution
For the realization of the Self Balancing Robot there are several software modules that
manage: two DC motors through L298N, MPU6050 and a joystick.
The two motors are controlled by the values taken from MPU6050 which take the
acceleration and rotation values with respect to the plane (x, y, z).
The robot works in two modes and uses a PID for the first mode and a PI controller for the
second mode.
1. Self - Balancing mode → from MPU6050 the current angle is calculated and is
used to maintain the balance position.
2. Forward/Backward movement →using a joystick the robot can move forward and
backward.

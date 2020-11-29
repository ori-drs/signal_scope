# Signal Scope

signal-scope is a live plotting tool for ROS messages. 

## Main Features

* Can plot dozens of signals a 1kHz. It was heavily used to develop walking robot locomotion control (BD Atlas, ANYbotics ANYmal)
* Can 'pause' the scope to zoom in on event of interest for in-situ debugging.
* Represents time in seconds to a starting point (many other tools just use unique time)
* Can dynamically rescale duration and magnitude
* Python macros allow simple conversions - quaternion to Euler angles, 3D norm etc

## Build Instructions

* catkin build signal_scope


## Running It

A configuration is specified in an simple python script. See the 'examples' directory for scripts demonstrating the basic usage of signal-scope.

* roscd signal_scope/examples
* rosrun signal_scope signal_scope example_anymal.py


![An old rock in the desert](signal-scope.jpg)


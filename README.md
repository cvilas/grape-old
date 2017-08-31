grape
=====

GRAPE stands for Graphical Real-time Application Prototyping Environment. This is forever work-in-progress as I tend to
use this repository to collect useful bits of code I have written over time  

Goals
-----
- A core set of software building blocks for real-time control 
- Supports Linux and Android
- Lightweight: can be installed on embedded platforms such as Arm
- Simple: able to develop a realtime control loop very easily
- Leverages existing high-quality and stable libraries such as Qt.
- Open source and unrestrictive licensing

Design
------
C++11 has features that make it much easier to build real-time software, especially the _soft_ variety. Given careful
usage of the language features, it also benefits design of hard-real-time software that must run at several kHz. My
inspiration for this project is QMotor - an excellent QNX-based hard-realtime control system developed at Clemson 
University during my grad school days. What I am trying to do here is bring some of the features of QMotor, developed 
in the c++98 era, to todays standards. My design guides are (see docs folder):
- QMotor 3.0 and the QMotor Robotic Toolkit: A PC-Based Control Platform
- Design and Implementation of the Robotic Platform
- NSTX-U Advances in Real-time C++11 on Linux
